

#ifndef PARSE_GEO_H__
#define PARSE_GEO_H__

#include <stdint.h>

#include "cJSON.h"

struct gps_point {
	double x;
	double y;
};

struct screen_point {
	int x;
	int y;
};

struct screen_scale_xy {
	int height;
	int width;
	double scaleX;
	double scaleY;
	double min_longitude;
	double max_longitude;
	double min_latitude;
	double max_latitude;
};

int cjson_hole_get_type_count(cJSON *hole, char *type);

char *cjson_hole_get_points_by_type_index(cJSON *hole, char *type, int index);

cJSON *cjson_get_hole_by_id(cJSON *root, uint8_t id);

int cjson_get_hole_count(cJSON *root);

int parse_points(char *str, struct gps_point **opoints, int *pnum);

int caculate_screen_scale_xy(int width, int height, 
			struct gps_point *gps_points, int points_num, 
			struct screen_scale_xy *scale_xy);

int convert_points_gps_to_screen(struct screen_scale_xy *scale_xy,
				struct gps_point *gps_points, 
				struct screen_point *screen_points, 
				int points_num);

int cjson_map_get_type_count(cJSON* map, char* type);

char *cjson_map_get_points_by_type_index(cJSON* map, char* type, int index);

#endif // end of PARSE_GEO_H__
   


