
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "cJSON.h"
#include "lvgl.h"
#include "parse_geo.h"
#include <ui_manager.h>


float compute_angle_by_2points(int x1, int y1, int x2, int y2)
{
        float x = 0.0f;
        float y = 0.0f;
        float angle = 0.0f;

        x = (x1 - x2);
        y = (y1 - y2);

        angle = atan(x/y);

        return angle;
}


int get_hole_angle(cJSON *hole, struct screen_scale_xy *scale_xy)
{
	struct gps_point *gps_points = NULL;
	int points_num = 0;
	struct screen_point screen_points;
	int ret = 0;
	char *p = NULL;
	lv_point_t start;
	lv_point_t end;
	float angle = 0.0f;

	p = cjson_hole_get_points_by_type_index(hole, "teeboxcenter", 0);
	ret = parse_points(p, &gps_points, &points_num);
	convert_points_gps_to_screen(scale_xy, gps_points, &screen_points, points_num);

	start.x = screen_points.x;
	start.y = screen_points.y;

	app_mem_free(gps_points);

	p = cjson_hole_get_points_by_type_index(hole, "greencenter", 0);
	ret = parse_points(p, &gps_points, &points_num);
	convert_points_gps_to_screen(scale_xy, gps_points, &screen_points, points_num);

	end.x = screen_points.x;
	end.y = screen_points.y;

	app_mem_free(gps_points);

	angle = compute_angle_by_2points(start.x, start.y, end.x, end.y);
	printf("%s: angle: %.12f, %d, %d, %d, %d\n", __func__, angle, start.x, start.y, end.x, end.y);

	ret = (int)((angle) / (2 * 3.14) * 3600);

	if (start.y < end.y)
		ret += 1800;

	return ret;
}





