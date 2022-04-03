#pragma once
struct Coordinate;

enum TeleMode {
   TELE_RADIO,
   TELE_SERIAL,
};

void tele_init();
void tele_update();
void tele_mode(TeleMode);
void tele_coord(float &lat, float &lon); // radians
