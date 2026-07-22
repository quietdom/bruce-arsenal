#pragma once

// Universal IR remote "app".
//
// Instead of browsing 218 .ir files on the SD card, this presents a
// branded-remote UI: pick Brand -> pick Model (or All) -> get a button
// grid (Power, Mute, Vol+, Vol-, Ch+, Ch-, Source, Menu...) that sends
// the right protocol/address/command directly from an embedded flash
// database. No SD files needed; the codes live in PROGMEM like
// TV-B-Gone's WORLD_IR_CODES.h.
//
// The .ir file system stays for power users and custom captures; this
// is the targeted, friendly version.

void ir_remote_app();
