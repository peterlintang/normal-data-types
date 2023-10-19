
#ifndef GPS_UTIL_H__
#define GPS_UTIL_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "cJSON.h"

double compute_angle_by_2points(int x1, int y1, int x2, int y2);


int get_hole_angle(cJSON *hole, struct screen_scale_xy *scale_xy);


#endif // end of GPS_UTIL_H__
