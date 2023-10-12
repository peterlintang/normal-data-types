

#ifndef DRAW_MAP_H__
#define DRAW_MAP_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ui_manager.h>

#include "lvgl.h"
#include "cJSON.h"
#include "parse_geo.h"




extern int convert_concave_polygon(float* x, float* y, int num, float*** pox, float*** poy, int* pnum, int** precords);



int draw_polygon(lv_obj_t *canvas, 
		lv_point_t *points, int points_num, 
		lv_color_t bg_color, lv_color_t line_color);

int draw_hole_map_poly_by_type(
		lv_obj_t *canvas, cJSON *hole, 
		struct screen_scale_xy *scale_xy, 
		char *type, lv_color_t bg_color);

int draw_hole_map_cycle_by_type(
		lv_obj_t *canvas, cJSON *hole, 
		struct screen_scale_xy *scale_xy, 
		char *type, int r, lv_color_t color);

int draw_hole_map(lv_obj_t *canvas, cJSON *hole);



#endif // end of DRAW_MAP_H__



