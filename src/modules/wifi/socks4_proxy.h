#ifndef SOCKS4_PROXY_H
#define SOCKS4_PROXY_H

#include <cstdint>

/**
 * SOCKS4 proxy server for Bruce firmware (ESP32).
 *
 * Listens on port 1080 (or given port). Use from your PC e.g.:
 *   ssh -o ProxyCommand='nc -X 4 -x <esp_ip>:1080 %h %p' user@target
 * Or set system/browser SOCKS proxy to <esp_ip>:1080.
 *
 * Implemented with Arduino WiFiServer/WiFiClient (no Asio) for reliability
 * on ESP32. chriskohlhoff/asio could be used for a future port if needed.
 */
void socks4Proxy(uint16_t port = 1080);

#endif
