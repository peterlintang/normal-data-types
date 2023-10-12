
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ui_manager.h>

#include "lvgl.h"
#include "cJSON.h"
#include "parse_geo.h"
#include "draw_map.h"


extern int convert_concave_polygon(float* x, float* y, int num, float*** pox, float*** poy, int* pnum, int** precords);



int draw_polygon(lv_obj_t *canvas, 
		lv_point_t *points, int points_num, 
		lv_color_t bg_color, lv_color_t line_color)
{
	float *x = NULL;
	float *y = NULL;
	float **ox = NULL;
	float **oy = NULL;
	int num = 0;
	int *records = NULL;
	int i = 0;
	int ret = 0;
	lv_draw_rect_dsc_t polygon_dsc;
	lv_draw_line_dsc_t line_dsc;

	lv_draw_rect_dsc_init(&polygon_dsc);
	polygon_dsc.bg_color = bg_color;
	polygon_dsc.border_side = LV_BORDER_SIDE_NONE;
	polygon_dsc.border_opa = LV_OPA_0;
	polygon_dsc.outline_opa = LV_OPA_0;
	polygon_dsc.shadow_opa = LV_OPA_0;

	polygon_dsc.bg_grad_color = lv_color_hex(0x00FF00);
	polygon_dsc.outline_color = lv_color_hex(0x00FF00);
	polygon_dsc.border_color = lv_color_hex(0x00FF00);
	polygon_dsc.bg_grad_color = lv_color_hex(0x00FF00);
	polygon_dsc.shadow_color = lv_color_hex(0x00FF00);

	x = (float *)app_mem_malloc(points_num * sizeof(float));
	y = (float *)app_mem_malloc(points_num * sizeof(float));

	if (x == NULL || y == NULL)
	{
		printf("%s %d: malloc %d * %d bytes failed\n", __func__, __LINE__, points_num, sizeof(float));
		app_mem_free(x);
		app_mem_free(y);
		return -1;
	}

	for (i = 0; i < points_num; i++)
	{
		x[i] = points[i].x;
		y[i] = points[i].y;
	}

//	printf("%s %d: begin\n", __func__, __LINE__);
	if (convert_concave_polygon(x, y, points_num, &ox, &oy, &num, &records) == -1)
	{
		printf("%s %d: convert polygon failed\n", __func__, __LINE__);
		app_mem_free(x);
		app_mem_free(y);
		return -1;
	}
//	printf("%s %d: end\n", __func__, __LINE__);

	app_mem_free(x);
	app_mem_free(y);
	x = NULL;
	y = NULL;

	for (i = 0; i < num; i++)
	{
		int j = 0;
		lv_point_t *poly_points = NULL;

		poly_points = (lv_point_t *)app_mem_malloc(records[i] * sizeof(lv_point_t));
		if (poly_points == NULL)
		{
			printf("%s %d: malloc %d * %d bytes failed\n", __func__, __LINE__, records[i], sizeof(lv_point_t));
			ret = -1;
			goto error;
		}

		for (j = 0; j < records[i]; j++)
		{
			poly_points[j].x = ox[i][j];
			poly_points[j].y = oy[i][j];
		}
//		printf("%s %d: i: %d, begin\n", __func__, __LINE__, i);
		lv_canvas_draw_polygon(canvas, poly_points, records[i], &polygon_dsc);
		lv_draw_line_dsc_init(&line_dsc);
		line_dsc.color = bg_color;
		line_dsc.width = 2;
		lv_canvas_draw_line(canvas, poly_points, records[i], &line_dsc);
//		printf("%s %d: i: %d, end\n", __func__, __LINE__, i);
		app_mem_free(poly_points);
		poly_points = NULL;
	}

error:
	for (i = 0; i < num; i++)
	{
		app_mem_free(ox[i]);
		app_mem_free(oy[i]);
	}

	app_mem_free(ox);
	app_mem_free(oy);
	app_mem_free(records);

	/*
	lv_draw_line_dsc_init(&line_dsc);
	line_dsc.color = bg_color;
	line_dsc.width = 3;
	lv_canvas_draw_line(canvas, points, points_num, &line_dsc);
	*/

	return ret;
}

int draw_hole_map_poly_by_type(
		lv_obj_t *canvas, cJSON *hole, 
		struct screen_scale_xy *scale_xy, 
		char *type, lv_color_t bg_color)
{
	struct gps_point* gps_points = NULL;
	struct screen_point* screen_points = NULL;
	lv_point_t* polygonPoints = NULL;
	int points_num = 0;
	char *p = NULL;
	int total_count = 0;
	int ret = 0;
//	char tmp[256] = { 0 };

	total_count = cjson_hole_get_type_count(hole, type);
	if (total_count == -1)
	{
		printf("%s %d: get hole %s failed\n", __func__, __LINE__, type);
		return -1;
	}

//	SYS_LOG_INF("%s: type: %s, total: %d\n", __func__, type, total_count);

	for (int i = 0; i < total_count; i++)
	{
		p = cjson_hole_get_points_by_type_index(hole, type, i);

		if (parse_points(p, &gps_points, &points_num) == -1)
		{
			printf("%s %d: parse points failed\n", __func__, __LINE__);
			return -1;
		}

		screen_points = (struct screen_point*)app_mem_malloc(points_num * sizeof(struct screen_point));
		polygonPoints = (lv_point_t*)app_mem_malloc(points_num * sizeof(lv_point_t));
		if (screen_points == NULL || polygonPoints == NULL)
		{
			printf("%s %d: malloc %d * %d bytes failed\n", __func__, __LINE__, points_num, sizeof(lv_point_t));
			app_mem_free(gps_points);
			app_mem_free(screen_points);
			app_mem_free(polygonPoints);
			return -1;
		}

//		SYS_LOG_INF("%s: type: %s, points num: %d, i: %d\n", __func__, type, points_num, i);

		convert_points_gps_to_screen(scale_xy, gps_points, screen_points, points_num);
		for (int i = 0; i < points_num; i++)
		{
			polygonPoints[i].x = screen_points[i].x + 55;
			polygonPoints[i].y = screen_points[i].y + 55;
			/*
			memset(tmp, 0, 256);
			snprintf(tmp, 256, "type: %s, i: %d, %.12f, %.12f, %d, %d\n", type, i, gps_points[i].x, gps_points[i].y, polygonPoints[i].x, polygonPoints[i].y);
			printf("%s: %s\n", __func__, tmp);
//			SYS_LOG_INF("%s: %s\n", __func__, tmp);
			*/
		}
//		printf("%s: begin draw polygon\n", __func__);
		ret = draw_polygon(canvas, polygonPoints, points_num, bg_color, lv_color_hex(0x000000));
		if (ret == -1)
		{
			printf("%s %d: draw polygon failed\n", __func__, __LINE__);
			return -1;
		}
//		printf("%s: end draw polygon\n", __func__);

		app_mem_free(screen_points);
		app_mem_free(polygonPoints);
		app_mem_free(gps_points);
	}

	return 0;
}

int draw_hole_map_cycle_by_type(
		lv_obj_t *canvas, cJSON *hole, 
		struct screen_scale_xy *scale_xy, 
		char *type, int r, lv_color_t color)
{
	struct gps_point* gps_points = NULL;
	struct screen_point* screen_points = NULL;
	lv_point_t* polygonPoints = NULL;
	int points_num = 0;
	char *p = NULL;
	int total_count = 0;
	int ret = 0;

	total_count = cjson_hole_get_type_count(hole, type);

	for (int i = 0; i < total_count; i++)
	{
		p = cjson_hole_get_points_by_type_index(hole, type, i);

		ret = parse_points(p, &gps_points, &points_num);
		if (ret == -1)
		{
			printf("%s %d: parse points failed\n", __func__, __LINE__);
			return -1;
		}

		screen_points = (struct screen_point*)app_mem_malloc(points_num * sizeof(struct screen_point));
		polygonPoints = (lv_point_t*)app_mem_malloc(points_num * sizeof(lv_point_t));
		if (screen_points == NULL || polygonPoints == NULL)
		{
			printf("%s %d: malloc %d * %d bytes failed\n", __func__, __LINE__, points_num, sizeof(lv_point_t));
			app_mem_free(gps_points);
			app_mem_free(screen_points);
			app_mem_free(polygonPoints);
			return -1;
		}

		convert_points_gps_to_screen(scale_xy, gps_points, screen_points, points_num);
		for (int i = 0; i < points_num; i++)
		{
			polygonPoints[i].x = screen_points[i].x + 55;
			polygonPoints[i].y = screen_points[i].y + 55;
		}

		lv_draw_arc_dsc_t arc_dsc;
		lv_draw_arc_dsc_init(&arc_dsc);
		arc_dsc.color = color;
		arc_dsc.width = 3;
		lv_canvas_draw_arc(canvas, polygonPoints[0].x, polygonPoints[0].y, r, 0, 3600, &arc_dsc);

		app_mem_free(screen_points);
		app_mem_free(polygonPoints);
		app_mem_free(gps_points);
	}

	return 0;
}

int draw_hole_map_line_by_type(
	lv_obj_t* canvas, cJSON* hole,
	struct screen_scale_xy* scale_xy,
	char* type, int width, lv_color_t color)
{
	struct gps_point* gps_points = NULL;
	struct screen_point* screen_points = NULL;
	lv_point_t* polygonPoints = NULL;
	int points_num = 0;
	char* p = NULL;
	int total_count = 0;
	int ret = 0;

	total_count = cjson_hole_get_type_count(hole, type);

	for (int i = 0; i < total_count; i++)
	{
		p = cjson_hole_get_points_by_type_index(hole, type, i);
		
		ret = parse_points(p, &gps_points, &points_num);
		if (ret == -1)
		{
			printf("%s %d: parse points failed\n", __func__, __LINE__);
			return -1;
		}

		screen_points = (struct screen_point*)app_mem_malloc(points_num * sizeof(struct screen_point));
		polygonPoints = (lv_point_t*)app_mem_malloc(points_num * sizeof(lv_point_t));
		if (screen_points == NULL || polygonPoints == NULL)
		{
			printf("%s %d: malloc %d * %d bytes failed\n", __func__, __LINE__, points_num, sizeof(lv_point_t));
			app_mem_free(gps_points);
			app_mem_free(screen_points);
			app_mem_free(polygonPoints);
			return -1;
		}

		convert_points_gps_to_screen(scale_xy, gps_points, screen_points, points_num);
		for (int i = 0; i < points_num; i++)
		{
			polygonPoints[i].x = screen_points[i].x + 55;
			polygonPoints[i].y = screen_points[i].y + 55;
		}

		lv_draw_line_dsc_t line_dsc;
		lv_draw_line_dsc_init(&line_dsc);
		line_dsc.color = color;
		line_dsc.width = 3;
		lv_canvas_draw_line(canvas, polygonPoints, points_num, &line_dsc);

		app_mem_free(screen_points);
		app_mem_free(polygonPoints);
		app_mem_free(gps_points);
	}

	return 0;
}


int draw_hole_map(lv_obj_t *canvas, cJSON *hole)
{
	char *p = NULL;
	struct gps_point *gps_points = NULL;
	int points_num = 0;
	struct screen_scale_xy scale_xy;

	p = cjson_hole_get_points_by_type_index(hole, "perimeter", 0);
	if (p == NULL)
	{
		printf("%s %d: get %s %d failed\n ", __func__, __LINE__, "perimeter", 0);
		return -1;
	}

	if (parse_points(p, &gps_points, &points_num) == -1)
	{
		printf("%s %d: parse points failed\n", __func__, __LINE__);
		return -1;
	}

	caculate_screen_scale_xy(240, 240, gps_points, points_num, &scale_xy);


	draw_hole_map_poly_by_type(canvas, hole, &scale_xy, "perimeter", lv_color_hex(0x0000ff));
	draw_hole_map_poly_by_type(canvas, hole, &scale_xy, "teebox", lv_color_hex(0x00ff00));
	draw_hole_map_poly_by_type(canvas, hole, &scale_xy, "green", lv_color_hex(0x00ff00));
	draw_hole_map_poly_by_type(canvas, hole, &scale_xy, "fairway", lv_color_hex(0x003333));
	draw_hole_map_poly_by_type(canvas, hole, &scale_xy, "bunker", lv_color_hex(0xff0000));
	
	draw_hole_map_cycle_by_type(canvas, hole, &scale_xy, "greencenter", 2, lv_color_hex(0xff0000));
	draw_hole_map_cycle_by_type(canvas, hole, &scale_xy, "teeboxcenter", 2, lv_color_hex(0xff0000));

	draw_hole_map_line_by_type(canvas, hole, &scale_xy, "centralpath", 3, lv_color_hex(0xff00ff));
	
	app_mem_free(gps_points);
	gps_points = NULL;

	return 0;
}






