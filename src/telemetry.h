#pragma once
struct Coordinate;

enum TeleMode : uint16_t {
   TELE_MODE_RADIO = 0,
   TELE_MODE_SERIAL,
   TELE_MODE_HOME,
   TELE_MODE_MAX,
};

void tele_init();
void tele_load(TeleMode);
void tele_save(TeleMode);
void tele_update();
void tele_set_mode(TeleMode);
TeleMode tele_get_mode();
void tele_coord(float &lat, float &lon); // radians
