#pragma once
struct Coordinate;

enum TeleMode {
   TELE_MODE_RADIO,
   TELE_MODE_SERIAL,
};

void tele_init();
void tele_load();
void tele_save();
void tele_update();
void tele_set_mode(TeleMode);
TeleMode tele_get_mode();
void tele_coord(float &lat, float &lon); // radians
