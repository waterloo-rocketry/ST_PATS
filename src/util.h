#pragma once
inline float fmap(float v, float from1, float from2, float to1, float to2) {
   return to1 + (v - from1) / (from2 - from1) * (to2 - to1);
}
