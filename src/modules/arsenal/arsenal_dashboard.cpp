#include "arsenal_dashboard.h"
#include "arsenal.h"
#include "arsenal_config.h"
#include "core/display.h"
#include "core/mykeyboard.h"
#include "core/sd_functions.h"
#include "core/wifi/wifi_common.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SD.h>
#include <Update.h>
#include <WiFi.h>
#include <globals.h>

static AsyncWebServer *arsenalServer = nullptr;
static bool dashboardActive = false;
static char AP_SSID[33] = "ArsenalNet";
static char AP_PASS[64] = "arsenal32";
static String uploadPath = "/arsenal";


static const char ARSENAL_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no">
<title>Arsenal Dashboard</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
body{font-family:-apple-system,BlinkMacSystemFont,sans-serif;background:#0a0a0f;color:#e0e0e0;min-height:100vh;padding:12px}
.header{text-align:center;padding:16px 0;border-bottom:1px solid #1a1a2e}
.header h1{font-size:1.4rem;color:#00ff88}
.header .status{font-size:0.8rem;margin-top:4px;color:#888}
.opsec{display:inline-block;padding:4px 12px;border-radius:12px;font-size:0.75rem;font-weight:bold;margin-top:8px}
.opsec.green{background:#0a2e0a;color:#00ff88;border:1px solid #00ff88}
.section{margin-top:16px}
.section h2{font-size:0.9rem;color:#888;text-transform:uppercase;letter-spacing:1px;margin-bottom:8px;padding-left:4px}
.card{background:#12121a;border:1px solid #1a1a2e;border-radius:8px;padding:12px 16px;margin-bottom:8px;display:flex;justify-content:space-between;align-items:center;cursor:pointer;transition:all .2s}
.card:active{background:#1a1a2e;transform:scale(0.98)}
.card .name{font-size:0.95rem}
.card .badge{font-size:0.7rem;padding:3px 8px;border-radius:4px;background:#1a2e1a;color:#00ff88}
.card .badge.off{background:#1a1a2e;color:#666}
.card .badge.running{background:#2e1a1a;color:#ff4444;animation:pulse 1s infinite}
@keyframes pulse{0%,100%{opacity:1}50%{opacity:0.5}}
.btn{display:block;width:100%;padding:14px;border:none;border-radius:8px;font-size:1rem;font-weight:bold;cursor:pointer;margin-top:8px;transition:all .2s}
.btn:active{transform:scale(0.97)}
.btn-danger{background:#ff4444;color:white}
.btn-primary{background:#00ff88;color:#0a0a0f}
.btn-secondary{background:#1a1a2e;color:#e0e0e0;border:1px solid #333}
.btn-small{display:inline-block;width:auto;padding:6px 12px;font-size:0.8rem;margin:4px}
.upload-area{border:2px dashed #333;border-radius:8px;padding:24px;text-align:center;margin-top:8px;transition:all .2s}
.upload-area.dragover{border-color:#00ff88;background:#0a2e0a}
.upload-area p{color:#888;font-size:0.85rem}
.progress{width:100%;height:4px;background:#1a1a2e;border-radius:2px;margin-top:12px;overflow:hidden;display:none}
.progress .bar{height:100%;background:#00ff88;width:0%;transition:width .3s}
.footer{text-align:center;padding:16px 0;color:#444;font-size:0.75rem;margin-top:24px}
.nav{display:flex;gap:4px;margin-top:12px;flex-wrap:wrap}
.nav .tab{flex:1;padding:8px;text-align:center;background:#12121a;border:1px solid #1a1a2e;border-radius:6px;font-size:0.75rem;color:#888;cursor:pointer;min-width:50px}
.nav .tab.active{background:#1a2e1a;color:#00ff88;border-color:#00ff88}
.hidden{display:none}
.file-list{list-style:none;max-height:60vh;overflow-y:auto}
.file-item{display:flex;justify-content:space-between;align-items:center;padding:10px 12px;border-bottom:1px solid #1a1a2e;cursor:pointer}
.file-item:active{background:#1a1a2e}
.file-item .fname{font-size:0.9rem;overflow:hidden;text-overflow:ellipsis;white-space:nowrap;max-width:60%}
.file-item .finfo{font-size:0.7rem;color:#666}
.file-item.folder .fname::before{content:'📁 '}
.file-item.file .fname::before{content:'📄 '}
.breadcrumb{font-size:0.8rem;color:#888;padding:8px 0;word-break:break-all}
.breadcrumb span{color:#00ff88;cursor:pointer}
.modal{position:fixed;top:0;left:0;right:0;bottom:0;background:rgba(0,0,0,0.8);display:flex;justify-content:center;align-items:center;z-index:100}
.modal-box{background:#12121a;border:1px solid #333;border-radius:12px;padding:24px;width:90%;max-width:360px}
.modal-box h3{margin-bottom:12px;color:#00ff88}
.modal-box input{width:100%;padding:10px;background:#0a0a0f;border:1px solid #333;border-radius:6px;color:#fff;margin-bottom:12px}
</style>
</head>
<body>
<div class="header">
<h1>&#127919; Arsenal</h1>
<div class="status">Connected | Heap: <span id="heap">--</span>KB | SD: <span id="sd">--</span></div>
<div class="opsec green" id="opsec">OPSEC: CLEAR</div>
</div>

<div class="nav">
<div class="tab active" onclick="showSection('tools')">Tools</div>
<div class="tab" onclick="showSection('files')">Files</div>
<div class="tab" onclick="showSection('upload')">Upload</div>
<div class="tab" onclick="showSection('system')">System</div>
</div>

<!-- TOOLS SECTION -->
<div class="section" id="sec-tools">
<h2>Arsenal Tools</h2>
<div class="card" onclick="toggle('network_scanner')"><span class="name">Network Scanner</span><span class="badge off" id="s-network_scanner">OFF</span></div>
<div class="card" onclick="toggle('dhcp_starvation')"><span class="name">DHCP Starvation</span><span class="badge off" id="s-dhcp_starvation">OFF</span></div>
<div class="card" onclick="toggle('karma_attack')"><span class="name">Karma Attack</span><span class="badge off" id="s-karma_attack">OFF</span></div>
<div class="card" onclick="toggle('dns_spoofer')"><span class="name">DNS Spoofer</span><span class="badge off" id="s-dns_spoofer">OFF</span></div>
<div class="card" onclick="toggle('ble_tracker')"><span class="name">BLE Tracker</span><span class="badge off" id="s-ble_tracker">OFF</span></div>
<div class="card" onclick="toggle('bt_spammer')"><span class="name">Name Spammer</span><span class="badge off" id="s-bt_spammer">OFF</span></div>
<div class="card" onclick="toggle('airtag_spoofer')"><span class="name">AirTag Spoofer</span><span class="badge off" id="s-airtag_spoofer">OFF</span></div>
<div class="card" onclick="toggle('opsec_monitor')"><span class="name">OPSEC Monitor</span><span class="badge off" id="s-opsec_monitor">OFF</span></div>
<div class="card" onclick="toggle('mac_rotator')"><span class="name">MAC Rotator</span><span class="badge off" id="s-mac_rotator">OFF</span></div>
<button class="btn btn-danger" onclick="toggle('jam_all')">&#9888;&#65039; JAM ALL</button>
</div>

<!-- FILES SECTION -->
<div class="section hidden" id="sec-files">
<h2>Arsenal File Browser</h2>
<div class="breadcrumb" id="breadcrumb"><span onclick="browse('/arsenal')">/arsenal</span></div>
<ul class="file-list" id="filelist"></ul>
<button class="btn btn-secondary" onclick="newFolder()">+ New Folder</button>
</div>

<!-- UPLOAD SECTION -->
<div class="section hidden" id="sec-upload">
<h2>Upload Scripts to SD</h2>
<p style="color:#888;font-size:0.85rem;margin-bottom:8px">Upload scripts, signals, payloads directly to /arsenal on SD card.</p>
<select id="uploadDest" style="width:100%;padding:10px;background:#12121a;border:1px solid #333;border-radius:6px;color:#fff;margin-bottom:8px">
<option value="/arsenal/badusb">BadUSB / DuckyScript</option>
<option value="/arsenal/subghz">Sub-GHz Signals</option>
<option value="/arsenal/ir">IR Remotes</option>
<option value="/arsenal/portals">Evil Portals (HTML)</option>
<option value="/arsenal/nfc">NFC Dumps</option>
<option value="/arsenal/rfid">RFID Dumps</option>
<option value="/arsenal/ibutton">iButton Keys</option>
<option value="/arsenal/scripts">JS Scripts</option>
<option value="/arsenal">Arsenal Root</option>
</select>
<div class="upload-area" id="dropzone">
<p>&#128229; Drag files here or tap to select</p>
<p style="margin-top:8px;color:#555">Supports: .txt .sub .ir .html .nfc .rfid .js .ibutton</p>
<input type="file" id="fileInput" multiple style="display:none">
</div>
<div class="progress" id="uploadProgress"><div class="bar" id="uploadBar"></div></div>
<div id="uploadStatus" style="margin-top:8px;font-size:0.8rem;color:#888"></div>
<button class="btn btn-primary" onclick="document.getElementById('fileInput').click()">Select Files</button>

<h2 style="margin-top:24px">Upload Firmware (.bin)</h2>
<div class="upload-area" id="fwdrop">
<p>&#128229; Drag .bin firmware here</p>
<input type="file" id="fwfile" accept=".bin" style="display:none">
</div>
<div class="progress" id="fwProgress"><div class="bar" id="fwBar"></div></div>
<button class="btn btn-secondary" onclick="document.getElementById('fwfile').click()">Select Firmware</button>
</div>

<!-- SYSTEM SECTION -->
<div class="section hidden" id="sec-system">
<h2>System</h2>
<div class="card"><span class="name">Free Heap</span><span class="badge" id="sysHeap">--</span></div>
<div class="card"><span class="name">SD Total</span><span class="badge" id="sysSD">--</span></div>
<div class="card"><span class="name">SD Used</span><span class="badge" id="sysSDUsed">--</span></div>
<div class="card"><span class="name">Uptime</span><span class="badge" id="sysUptime">--</span></div>
<button class="btn btn-danger" onclick="if(confirm('Reboot?'))fetch('/api/reboot',{method:'POST'})">Reboot Device</button>
</div>

<div class="footer">Arsenal v1.0 | T-Embed CC1101</div>

<script>
const sections=['tools','files','upload','system'];
let currentPath='/arsenal';

function showSection(s){
  sections.forEach(x=>document.getElementById('sec-'+x).classList.toggle('hidden',x!==s));
  document.querySelectorAll('.nav .tab').forEach((t,i)=>t.classList.toggle('active',sections[i]===s));
  if(s==='files')browse(currentPath);
}

function toggle(f){
  fetch('/api/arsenal/toggle',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({feature:f})})
  .then(r=>r.json()).then(d=>{
    let el=document.getElementById('s-'+f);
    if(el){el.textContent=d.state;el.className='badge '+(d.state==='ON'?'running':'off');}
  }).catch(()=>{});
}

// ─── File Browser ───────────────────────────────────────
function browse(path){
  currentPath=path;
  fetch('/api/files/list?path='+encodeURIComponent(path))
  .then(r=>r.json()).then(data=>{
    // Update breadcrumb
    let parts=path.split('/').filter(Boolean);
    let bc='';
    let cumulative='';
    parts.forEach((p,i)=>{
      cumulative+='/'+p;
      let cp=cumulative;
      bc+='/<span onclick="browse(\''+cp+'\')">'+p+'</span>';
    });
    document.getElementById('breadcrumb').innerHTML=bc;

    // Build file list
    let list=document.getElementById('filelist');
    list.innerHTML='';

    // Back button if not at root
    if(path!=='/arsenal'){
      let parentPath=path.substring(0,path.lastIndexOf('/'));
      if(!parentPath)parentPath='/arsenal';
      let li=document.createElement('li');
      li.className='file-item folder';
      li.innerHTML='<span class="fname">⬆️ ..</span><span class="finfo">Back</span>';
      li.onclick=()=>browse(parentPath);
      list.appendChild(li);
    }

    // Sort: folders first
    data.sort((a,b)=>{if(a.isDir!==b.isDir)return a.isDir?-1:1;return a.name.localeCompare(b.name);});

    data.forEach(item=>{
      let li=document.createElement('li');
      li.className='file-item '+(item.isDir?'folder':'file');
      li.innerHTML='<span class="fname">'+item.name+'</span><span class="finfo">'+(item.isDir?'':item.size)+'</span>';
      if(item.isDir){
        li.onclick=()=>browse(path+'/'+item.name);
      } else {
        li.onclick=()=>fileAction(path+'/'+item.name,item.name);
      }
      list.appendChild(li);
    });

    if(data.length===0){
      list.innerHTML='<li class="file-item"><span class="fname" style="color:#666">Empty folder</span></li>';
    }
  }).catch(e=>console.error(e));
}

function fileAction(filepath,name){
  let action=prompt('File: '+name+'\n\nType:\n  download - download file\n  delete - delete file\n  cancel - cancel','download');
  if(!action||action==='cancel')return;
  if(action==='delete'){
    if(!confirm('Delete '+name+'?'))return;
    fetch('/api/files/delete',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({path:filepath})})
    .then(()=>browse(currentPath));
  } else if(action==='download'){
    window.open('/api/files/download?path='+encodeURIComponent(filepath));
  }
}

function newFolder(){
  let name=prompt('New folder name:');
  if(!name)return;
  fetch('/api/files/mkdir',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({path:currentPath+'/'+name})})
  .then(()=>browse(currentPath));
}

// ─── File Upload ────────────────────────────────────────
const dropzone=document.getElementById('dropzone');
const fileInput=document.getElementById('fileInput');
const uploadProgress=document.getElementById('uploadProgress');
const uploadBar=document.getElementById('uploadBar');
const uploadStatus=document.getElementById('uploadStatus');

dropzone.onclick=()=>fileInput.click();
dropzone.ondragover=e=>{e.preventDefault();dropzone.classList.add('dragover');};
dropzone.ondragleave=()=>dropzone.classList.remove('dragover');
dropzone.ondrop=e=>{e.preventDefault();dropzone.classList.remove('dragover');uploadFiles(e.dataTransfer.files);};
fileInput.onchange=e=>uploadFiles(e.target.files);

function uploadFiles(files){
  let dest=document.getElementById('uploadDest').value;
  let total=files.length;
  let done=0;
  uploadProgress.style.display='block';
  uploadStatus.textContent='Uploading 0/'+total+'...';

  Array.from(files).forEach(file=>{
    let form=new FormData();
    form.append('file',file);
    form.append('path',dest);
    let xhr=new XMLHttpRequest();
    xhr.open('POST','/api/files/upload');
    xhr.upload.onprogress=e=>{
      if(e.lengthComputable){
        let pct=Math.round(((done/total)+((e.loaded/e.total)/total))*100);
        uploadBar.style.width=pct+'%';
      }
    };
    xhr.onload=()=>{
      done++;
      uploadStatus.textContent='Uploaded '+done+'/'+total;
      uploadBar.style.width=Math.round(done/total*100)+'%';
      if(done===total){uploadStatus.textContent='Done! '+total+' files uploaded to '+dest;}
    };
    xhr.onerror=()=>{uploadStatus.textContent='Error uploading '+file.name;};
    xhr.send(form);
  });
}

// ─── Firmware OTA ───────────────────────────────────────
const fwdrop=document.getElementById('fwdrop');
const fwfile=document.getElementById('fwfile');
const fwProgress=document.getElementById('fwProgress');
const fwBar=document.getElementById('fwBar');
fwdrop.onclick=()=>fwfile.click();
fwdrop.ondragover=e=>{e.preventDefault();fwdrop.classList.add('dragover');};
fwdrop.ondragleave=()=>fwdrop.classList.remove('dragover');
fwdrop.ondrop=e=>{e.preventDefault();fwdrop.classList.remove('dragover');flashFW(e.dataTransfer.files[0]);};
fwfile.onchange=e=>flashFW(e.target.files[0]);

function flashFW(file){
  if(!file||!file.name.endsWith('.bin')){alert('Select a .bin file');return;}
  if(!confirm('Flash '+file.name+' ('+Math.round(file.size/1024)+'KB)?\nDevice will reboot!'))return;
  fwProgress.style.display='block';fwBar.style.width='0%';
  let xhr=new XMLHttpRequest();
  xhr.open('POST','/api/ota');
  xhr.upload.onprogress=e=>{if(e.lengthComputable)fwBar.style.width=Math.round(e.loaded/e.total*100)+'%';};
  xhr.onload=()=>{fwBar.style.width='100%';alert('Flash complete! Rebooting...');};
  xhr.onerror=()=>alert('Upload failed');
  let form=new FormData();form.append('firmware',file);
  xhr.send(form);
}

// ─── Status Polling ─────────────────────────────────────
setInterval(()=>{
  fetch('/api/arsenal/status').then(r=>r.json()).then(d=>{
    document.getElementById('heap').textContent=Math.round(d.freeHeap/1024);
    document.getElementById('sd').textContent=d.sdFree||'--';
    if(d.uptime)document.getElementById('sysUptime').textContent=Math.round(d.uptime/1000)+'s';
    if(d.freeHeap)document.getElementById('sysHeap').textContent=Math.round(d.freeHeap/1024)+'KB';
    if(d.sdTotal)document.getElementById('sysSD').textContent=d.sdTotal;
    if(d.sdUsed)document.getElementById('sysSDUsed').textContent=d.sdUsed;
  }).catch(()=>{});
},5000);
</script>
</body>
</html>
)rawliteral";


static String hrSize(uint64_t bytes) {
    if (bytes < 1024) return String(bytes) + "B";
    if (bytes < 1024 * 1024) return String(bytes / 1024) + "KB";
    if (bytes < 1024ULL * 1024 * 1024) return String(bytes / 1024 / 1024) + "MB";
    return String(bytes / 1024 / 1024 / 1024) + "GB";
}


static void mkdirRecursive(String path) {
    String current = "";
    int start = 0;
    while (start < (int)path.length()) {
        int end = path.indexOf('/', start);
        if (end == -1) end = path.length();
        current += path.substring(start, end);
        if (current.length() > 0 && !SD.exists(current)) {
            SD.mkdir(current);
        }
        if (end < (int)path.length()) current += "/";
        start = end + 1;
    }
}


static void setupArsenalRoutes() {

    arsenalServer->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", ARSENAL_HTML);
    });


    arsenalServer->on("/api/arsenal/status", HTTP_GET, [](AsyncWebServerRequest *request) {
        char json[512];
        uint64_t sdTotal = SD.totalBytes();
        uint64_t sdUsed = SD.usedBytes();
        snprintf(json, sizeof(json),
                 "{\"freeHeap\":%lu,\"uptime\":%lu,\"opsec\":\"green\",\"sdTotal\":\"%s\",\"sdUsed\":\"%s\",\"sdFree\":\"%s\"}",
                 (unsigned long)ESP.getFreeHeap(),
                 (unsigned long)millis(),
                 hrSize(sdTotal).c_str(),
                 hrSize(sdUsed).c_str(),
                 hrSize(sdTotal - sdUsed).c_str());
        request->send(200, "application/json", json);
    });


    arsenalServer->on("/api/arsenal/toggle", HTTP_POST,
        [](AsyncWebServerRequest *request) {},
        NULL,
        [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
            String body = String((char *)data).substring(0, len);
            int valStart = body.indexOf("\"feature\"");
            if (valStart < 0) { request->send(400, "application/json", "{\"error\":\"bad\"}"); return; }
            valStart = body.indexOf("\"", valStart + 10) + 1;
            int valEnd = body.indexOf("\"", valStart);
            String feature = body.substring(valStart, valEnd);
            request->send(200, "application/json", "{\"feature\":\"" + feature + "\",\"state\":\"ON\"}");
        });


    arsenalServer->on("/api/files/list", HTTP_GET, [](AsyncWebServerRequest *request) {
        String path = "/arsenal";
        if (request->hasArg("path")) path = request->arg("path");

        if (!setupSdCard()) {
            request->send(500, "application/json", "[]");
            return;
        }

        File dir = SD.open(path);
        if (!dir || !dir.isDirectory()) {
            request->send(404, "application/json", "[]");
            return;
        }

        String json = "[";
        bool first = true;
        while (true) {
            File entry = dir.openNextFile();
            if (!entry) break;

            String name = String(entry.name());
            name = name.substring(name.lastIndexOf('/') + 1);

            if (!first) json += ",";
            json += "{\"name\":\"" + name + "\",\"isDir\":" + (entry.isDirectory() ? "true" : "false");
            if (!entry.isDirectory()) {
                json += ",\"size\":\"" + hrSize(entry.size()) + "\"";
            }
            json += "}";
            first = false;
            entry.close();

            esp_task_wdt_reset();
            if (json.length() > 8000) break;
        }
        json += "]";
        dir.close();
        request->send(200, "application/json", json);
    });


    arsenalServer->on("/api/files/download", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (!request->hasArg("path")) { request->send(400); return; }
        String path = request->arg("path");
        if (!setupSdCard() || !SD.exists(path)) { request->send(404); return; }
        request->send(SD, path, "application/octet-stream", true);
    });


    arsenalServer->on("/api/files/delete", HTTP_POST,
        [](AsyncWebServerRequest *request) {},
        NULL,
        [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
            String body = String((char *)data).substring(0, len);
            int s = body.indexOf("\"path\"");
            if (s < 0) { request->send(400); return; }
            s = body.indexOf("\"", s + 7) + 1;
            int e = body.indexOf("\"", s);
            String path = body.substring(s, e);

            if (setupSdCard() && SD.exists(path)) {
                File f = SD.open(path);
                if (f.isDirectory()) {
                    SD.rmdir(path);
                } else {
                    SD.remove(path);
                }
                f.close();
            }
            request->send(200, "application/json", "{\"ok\":true}");
        });


    arsenalServer->on("/api/files/mkdir", HTTP_POST,
        [](AsyncWebServerRequest *request) {},
        NULL,
        [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
            String body = String((char *)data).substring(0, len);
            int s = body.indexOf("\"path\"");
            if (s < 0) { request->send(400); return; }
            s = body.indexOf("\"", s + 7) + 1;
            int e = body.indexOf("\"", s);
            String path = body.substring(s, e);

            if (setupSdCard()) {
                mkdirRecursive(path);
            }
            request->send(200, "application/json", "{\"ok\":true}");
        });


    arsenalServer->on("/api/files/upload", HTTP_POST,
        [](AsyncWebServerRequest *request) {
            request->send(200, "text/plain", "OK");
        },
        [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
            static File uploadFile;
            String dest = "/arsenal";
            if (request->hasArg("path")) dest = request->arg("path");

            if (!index) {
                if (!setupSdCard()) return;
                mkdirRecursive(dest);
                String fullPath = dest + "/" + filename;
                uploadFile = SD.open(fullPath, FILE_WRITE);
                Serial.println("[Arsenal] Upload: " + fullPath);
            }

            if (uploadFile && len) {
                uploadFile.write(data, len);
            }

            if (final && uploadFile) {
                uploadFile.close();
                Serial.printf("[Arsenal] Upload done: %u bytes\n", index + len);
            }
        });


    arsenalServer->on("/api/ota", HTTP_POST,
        [](AsyncWebServerRequest *request) {
            bool success = !Update.hasError();
            request->send(success ? 200 : 500, "text/plain", success ? "OK" : "FAIL");
            if (success) { delay(500); ESP.restart(); }
        },
        [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
            if (!index) {
                Serial.println("[Arsenal OTA] Start: " + filename);
                if (len > 0 && data[0] != 0xE9) {
                    Serial.println("[Arsenal OTA] Invalid header!");
                    return;
                }
                if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
                    Update.printError(Serial);
                }
            }
            if (Update.isRunning()) {
                if (Update.write(data, len) != len) {
                    Update.printError(Serial);
                }
            }
            if (final) {
                if (Update.end(true)) {
                    Serial.printf("[Arsenal OTA] OK! %u bytes\n", index + len);
                } else {
                    Update.printError(Serial);
                }
            }
        });


    arsenalServer->on("/api/reboot", HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(200); delay(500); ESP.restart();
    });
}


void arsenal_dashboard_start(void) {
    if (dashboardActive) return;

    arsenal_config_load();
    if (strlen(arsenal_config().apSsid) > 0) {
        strncpy(AP_SSID, arsenal_config().apSsid, sizeof(AP_SSID) - 1);
        AP_SSID[sizeof(AP_SSID) - 1] = '\0';
    }
    if (strlen(arsenal_config().apPass) >= 8) {
        strncpy(AP_PASS, arsenal_config().apPass, sizeof(AP_PASS) - 1);
        AP_PASS[sizeof(AP_PASS) - 1] = '\0';
    }

    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, AP_PASS);
    delay(100);


    if (setupSdCard()) {
        if (!SD.exists("/arsenal")) SD.mkdir("/arsenal");
        if (!SD.exists("/arsenal/badusb")) SD.mkdir("/arsenal/badusb");
        if (!SD.exists("/arsenal/subghz")) SD.mkdir("/arsenal/subghz");
        if (!SD.exists("/arsenal/ir")) SD.mkdir("/arsenal/ir");
        if (!SD.exists("/arsenal/portals")) SD.mkdir("/arsenal/portals");
        if (!SD.exists("/arsenal/nfc")) SD.mkdir("/arsenal/nfc");
        if (!SD.exists("/arsenal/rfid")) SD.mkdir("/arsenal/rfid");
        if (!SD.exists("/arsenal/ibutton")) SD.mkdir("/arsenal/ibutton");
        if (!SD.exists("/arsenal/scripts")) SD.mkdir("/arsenal/scripts");
    }

    arsenalServer = new AsyncWebServer(80);
    if (strlen(arsenal_config().dashUser) > 0 && strlen(arsenal_config().dashPass) > 0) {
        arsenalServer->setAuthentication(arsenal_config().dashUser, arsenal_config().dashPass);
    }
    setupArsenalRoutes();
    arsenalServer->begin();
    dashboardActive = true;

    Serial.println("[Arsenal] Dashboard at " + WiFi.softAPIP().toString());
}

void arsenal_dashboard_stop(void) {
    if (!dashboardActive) return;
    arsenalServer->end();
    delete arsenalServer;
    arsenalServer = nullptr;
    WiFi.softAPdisconnect(true);
    dashboardActive = false;
}

bool arsenal_dashboard_is_active(void) {
    return dashboardActive;
}


void arsenal_remote_dashboard(void) {
    if (ESP.getFreeHeap() < 40000) {
        displayRedStripe("Low memory!", TFT_WHITE, TFT_RED);
        delay(1500);
        return;
    }

    arsenal_dashboard_start();

    drawMainBorderWithTitle("Arsenal Dashboard");
    int padX = 14;
    int currentY = 45;

    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setTextSize(FP);

    tft.setCursor(padX, currentY);
    tft.print("Net: " + String(AP_SSID));
    currentY += 14;

    tft.setCursor(padX, currentY);
    tft.print("Pwd: " + String(AP_PASS));
    currentY += 14;

    tft.setCursor(padX, currentY);
    tft.print("IP:  " + WiFi.softAPIP().toString());
    currentY += 14;

    tft.setCursor(padX, currentY);
    tft.print("Open browser on your phone");
    currentY += 20;

    tft.setTextColor(TFT_GREEN, bruceConfig.bgColor);
    tft.setCursor(padX, currentY);
    tft.print("Features:");
    currentY += 14;

    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    tft.setCursor(padX, currentY);
    tft.print("- Control all Arsenal tools");
    currentY += 12;
    tft.setCursor(padX, currentY);
    tft.print("- Browse/upload scripts to SD");
    currentY += 12;
    tft.setCursor(padX, currentY);
    tft.print("- OTA firmware update");

    tft.setTextColor(TFT_RED, bruceConfig.bgColor);
    tft.drawCentreString("Press Esc to stop", tftWidth / 2, tftHeight - 20, 1);

    while (true) {
        if (check(EscPress)) break;
        delay(100);
    }

    arsenal_dashboard_stop();
}
