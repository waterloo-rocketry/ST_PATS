#pragma once

void compass_init();
float compass_heading(); // return radians
void compass_calibrate_load();
void compass_calibrate_start();
void compass_calibrate_stop();
void compass_calibrate_toggle();
void compass_update();
