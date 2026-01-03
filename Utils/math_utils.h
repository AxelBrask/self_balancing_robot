/*
 * math_utils.h
 *
 *  Created on: Jan 3, 2026
 *      Author: axel
 */

#pragma once
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

static inline float wrap_pi(float a)
{
  while (a >  (float)M_PI) a -= 2.0f * (float)M_PI;
  while (a < -(float)M_PI) a += 2.0f * (float)M_PI;
  return a;
}

static inline float wrap_to_near(float meas, float ref)
{
  float e = wrap_pi(meas - ref);
  return ref + e;
}

#define RAD2DEG_X100 5729.57795f



