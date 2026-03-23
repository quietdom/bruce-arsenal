#include "AppStoreMenu.h"

#if !defined(LITE_VERSION) && !defined(DISABLE_INTERPRETER)

#include "core/display.h"
#include "core/sd_functions.h"
#include "core/mykeyboard.h"
#include "core/wifi/wifi_common.h"
#include "core/utils.h"
#include <globals.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>

#define AS_HOST "http://ghp.iceis.co.uk"
#define AS_INST "/BruceAppStore/installed.json"
#define AS_CACHE "/BruceAppStore/cache/"

static int _destFs = 0;
static FS *_afs = nullptr;

static void _initFs() {
    setupSdCard();
    _afs = (_destFs == 1 && sdcardMounted) ? (FS *)&SD : (FS *)&LittleFS;
    const char *dirs[] = {"/BruceAppStore", AS_CACHE};
    for (auto d : dirs)
        if (!_afs->exists(d)) _afs->mkdir(d);
}

static bool _needWifi() {
    if (WiFi.status() == WL_CONNECTED) return true;
    wifiConnectMenu(WIFI_STA);
    if (WiFi.status() != WL_CONNECTED) {
        displayError("WiFi not connected", true);
        return false;
    }
    return true;
}

static void _mkDirRecursive(String path) {
    int start = 0;
    while (true) {
        int idx = path.indexOf('/', start + 1);
        if (idx < 0) break;
        String p = path.substring(0, idx);
        if (!_afs->exists(p)) _afs->mkdir(p);
        start = idx;
    }
}

static String _httpGet(const String &url) {
    HTTPClient http;
    http.begin(url);
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    http.setTimeout(10000);
    String r;
    if (http.GET() == 200) r = http.getString();
    http.end();
    return r;
}

static bool _httpSave(const String &url, const String &path) {
    HTTPClient http;
    http.begin(url);
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    http.setTimeout(15000);
    if (http.GET() != 200) {
        http.end();
        return false;
    }
    int sl = path.lastIndexOf('/');
    if (sl > 0) {
        String d = path.substring(0, sl);
        _mkDirRecursive(d + "/");
    }
    File f = _afs->open(path, FILE_WRITE);
    if (!f) {
        http.end();
        return false;
    }
    http.writeToStream(&f);
    f.close();
    http.end();
    return true;
}

static String _instVer(const String &slug) {
    File f = _afs->open(AS_INST, FILE_READ);
    if (!f) return "";
    JsonDocument doc;
    deserializeJson(doc, f);
    f.close();
    if (doc[slug].is<JsonObject>()) return doc[slug]["version"].as<String>();
    return "";
}

static void _doInstall(String slug, String appName) {
    displayTextLine("Fetching metadata...", false);
    String url = String(AS_HOST) + "/service/main/repositories/" + slug + "/metadata.json";
    url.replace(" ", "%20");
    String raw = _httpGet(url);
    if (raw.isEmpty()) {
        displayError("Fetch failed", true);
        return;
    }

    JsonDocument meta;
    if (deserializeJson(meta, raw)) {
        displayError("Parse error", true);
        return;
    }

    JsonArray files = meta["files"].as<JsonArray>();
    String cat = meta["category"].as<String>();
    String bp = meta["path"].as<String>();
    String owner = meta["owner"].as<String>();
    String repo = meta["repo"].as<String>();
    String commit = meta["commit"].as<String>();
    String ver = meta["version"].as<String>();
    String dest = (cat == "Themes") ? "/Themes/" + appName + "/" : "/" + cat + "/";
    int total = files.size(), ok = 0;

    for (int i = 0; i < total; i++) {
        displayTextLine("Downloading " + String(i + 1) + "/" + String(total), false);
        String fn, src;
        if (files[i].is<JsonObject>()) {
            fn = files[i]["destination"].as<String>();
            src = files[i]["source"].as<String>();
        } else {
            fn = files[i].as<String>();
            src = fn;
        }
        while (fn.startsWith("/")) fn = fn.substring(1);
        while (src.startsWith("/")) src = src.substring(1);
        String sp = bp + src;
        while (sp.startsWith("/")) sp = sp.substring(1);
        String dl = String(AS_HOST) + "/service/manual/" + owner + "/" + repo + "/" + commit + "/" + sp;
        dl.replace(" ", "%20");
        if (_httpSave(dl, dest + fn)) ok++;
    }

    if (ok == total) {
        File f = _afs->open(AS_INST, FILE_READ);
        JsonDocument inst;
        if (f) {
            deserializeJson(inst, f);
            f.close();
        }
        inst[slug]["version"] = ver;
        inst[slug]["commit"] = commit;
        File w = _afs->open(AS_INST, FILE_WRITE);
        if (w) {
            serializeJson(inst, w);
            w.close();
        }
        displaySuccess("Installed!", true);
    } else {
        displayError(String(ok) + "/" + String(total) + " files ok", true);
    }
}

static void _doDelete(String slug, String appName) {
    displayTextLine("Deleting...", false);
    String url = String(AS_HOST) + "/service/main/repositories/" + slug + "/metadata.json";
    url.replace(" ", "%20");
    String raw = _httpGet(url);
    if (raw.isEmpty()) {
        displayError("Fetch failed", true);
        return;
    }

    JsonDocument meta;
    if (deserializeJson(meta, raw)) {
        displayError("Parse error", true);
        return;
    }

    JsonArray files = meta["files"].as<JsonArray>();
    String cat = meta["category"].as<String>();
    String dest = (cat == "Themes") ? "/Themes/" + appName + "/" : "/" + cat + "/";

    for (int i = 0; i < (int)files.size(); i++) {
        String fn;
        if (files[i].is<JsonObject>()) fn = files[i]["destination"].as<String>();
        else fn = files[i].as<String>();
        while (fn.startsWith("/")) fn = fn.substring(1);
        _afs->remove(dest + fn);
    }

    File f = _afs->open(AS_INST, FILE_READ);
    JsonDocument inst;
    if (f) {
        deserializeJson(inst, f);
        f.close();
    }
    inst.remove(slug);
    File w = _afs->open(AS_INST, FILE_WRITE);
    if (w) {
        serializeJson(inst, w);
        w.close();
    }
    displaySuccess("Deleted!", true);
}

static void _appMenu(String name, String slug, String ver) {
    String iv = _instVer(slug);
    options.clear();
    if (iv.isEmpty()) {
        options.push_back({"Install", [slug, name]() { _doInstall(slug, name); }});
    } else {
        if (iv != ver) options.push_back({"Update", [slug, name]() { _doInstall(slug, name); }});
        options.push_back({"Reinstall", [slug, name]() { _doInstall(slug, name); }});
        options.push_back({"Delete", [slug, name]() { _doDelete(slug, name); }});
    }
    addOptionToMainMenu();
    loopOptions(options, MENU_TYPE_SUBMENU, name.c_str());
}

static void _catMenu(String slug, String catName) {
    displayTextLine("Loading " + catName + "...", false);
    String cp = String(AS_CACHE) + "category-" + slug + ".json";
    String raw;
    File cf = _afs->open(cp, FILE_READ);
    if (cf) {
        raw = cf.readString();
        cf.close();
    }
    if (raw.isEmpty()) {
        if (!_needWifi()) return;
        raw = _httpGet(String(AS_HOST) + "/service/main/releases/category-" + slug + ".min.json");
        if (raw.isEmpty()) {
            displayError("Load failed", true);
            return;
        }
        File wf = _afs->open(cp, FILE_WRITE);
        if (wf) {
            wf.print(raw);
            wf.close();
        }
    }

    JsonDocument doc;
    if (deserializeJson(doc, raw)) {
        displayError("Parse error", true);
        return;
    }
    JsonArray apps = doc["apps"].as<JsonArray>();
    if (!apps.size()) {
        displayInfo("No apps in this category", true);
        return;
    }

    struct AppEntry {
        String n, s, v;
    };
    std::vector<AppEntry> list;
    for (int i = 0; i < (int)apps.size(); i++)
        list.push_back({apps[i]["n"].as<String>(), apps[i]["s"].as<String>(), apps[i]["v"].as<String>()});

    options.clear();
    for (auto &a : list) {
        String iv = _instVer(a.s);
        String lbl = a.n;
        if (!iv.isEmpty()) lbl += (iv != a.v) ? " [UPD]" : " [OK]";
        String n = a.n, s = a.s, v = a.v;
        options.push_back({lbl, [n, s, v]() { _appMenu(n, s, v); }});
    }
    addOptionToMainMenu();
    loopOptions(options, MENU_TYPE_SUBMENU, catName.c_str());
}

void AppStoreMenu::optionsMenu() {
    _initFs();
    if (!_needWifi()) return;

    displayTextLine("Loading App Store...", false);
    String raw = _httpGet(String(AS_HOST) + "/service/main/releases/categories.json");
    if (raw.isEmpty()) {
        displayError("Failed to load categories", true);
        return;
    }

    JsonDocument doc;
    if (deserializeJson(doc, raw)) {
        displayError("Parse error", true);
        return;
    }
    JsonArray cats = doc["categories"].as<JsonArray>();
    if (!cats.size()) {
        displayInfo("No categories available", true);
        return;
    }

    struct CatEntry {
        String name, slug;
        int count;
    };
    std::vector<CatEntry> list;
    for (int i = 0; i < (int)cats.size(); i++) {
        String s = cats[i]["slug"].as<String>();
        if (s == "updates") continue;
        list.push_back({cats[i]["name"].as<String>(), s, cats[i]["count"].as<int>()});
    }

    options.clear();
    for (auto &c : list) {
        String lbl = c.name + " (" + String(c.count) + ")";
        String slug = c.slug, name = c.name;
        options.push_back({lbl, [slug, name]() { _catMenu(slug, name); }});
    }

    options.push_back({"Config", []() {
        bool inConfig = true;
        while(inConfig && !returnToMenu) {
            std::vector<Option> cfgOpts;
            if (sdcardMounted) {
                cfgOpts.push_back({String("Dest: ") + (_destFs == 1 ? "SD Card" : "LittleFS"), []() {
                    _destFs = (_destFs == 0) ? 1 : 0;
                    _afs = nullptr;
                }});
            } else {
                cfgOpts.push_back({"Dest: LittleFS (No SD)", []() {}});
            }
            cfgOpts.push_back({"Back", [&inConfig]() { inConfig = false; }});
            loopOptions(cfgOpts, MENU_TYPE_SUBMENU, "App Store Config");
        }
    }});

    addOptionToMainMenu();
    loopOptions(options, MENU_TYPE_SUBMENU, "App Store");
}

void AppStoreMenu::drawIcon(float scale) {
    clearIconArea();

    int iconW = scale * 60;
    int iconH = scale * 60;
    if (iconW % 2 != 0) iconW++;
    if (iconH % 2 != 0) iconH++;

    int cx = iconCenterX;
    int cy = iconCenterY;
    int half = iconW / 2;

    tft.drawRoundRect(cx - half, cy - half, iconW, iconH, half / 4, bruceConfig.priColor);

    int aw = iconW / 5;
    int top = cy - half / 2;
    int bot = cy + half / 6;
    tft.fillRect(cx - aw / 2, top, aw, bot - top, bruceConfig.priColor);

    int tw = half / 2 + 2;
    tft.fillTriangle(cx - tw, bot, cx + tw, bot, cx, cy + half / 2, bruceConfig.priColor);

    int ly = cy + half - half / 5;
    tft.drawLine(cx - half / 2, ly, cx + half / 2, ly, bruceConfig.priColor);
}

#endif
