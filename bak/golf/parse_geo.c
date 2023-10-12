

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ui_manager.h>

#include "cJSON.h"
#include "parse_geo.h"


int cjson_hole_get_type_count(cJSON *hole, char *type)
{
	cJSON *node = NULL;
	cJSON *shapecount = NULL;

	node = cJSON_GetObjectItem(hole, type);
	if (node == NULL)
		return -1;

	shapecount = cJSON_GetObjectItem(node, "shapecount");
	if (shapecount == NULL)
		return -1;

	return shapecount->valueint;
}

char *cjson_hole_get_points_by_type_index(cJSON *hole, char *type, int index)
{
	cJSON *node = NULL;
	cJSON *shapecount = NULL;
	cJSON *shapes = NULL;
	cJSON *shape = NULL;
	cJSON *shape_num = NULL;
	cJSON *points = NULL;

	node = cJSON_GetObjectItem(hole, type);
	if (node == NULL)
		return NULL;

	shapecount = cJSON_GetObjectItem(node, "shapecount");
	if (shapecount == NULL)
		return NULL;


	if (index >= shapecount->valueint)
	{
		printf("index: %d, count: %d\n", index, shapecount->valueint);
		return NULL;
	}

	shapes = cJSON_GetObjectItem(node, "shapes");
	if (shapes == NULL)
		return NULL;

	shape = cJSON_GetObjectItem(shapes, "shape");
	if (shape == NULL)
		return NULL;

	shape_num= cJSON_GetArrayItem(shape, index);
	if (shape_num == NULL)
		return NULL;

	points = cJSON_GetObjectItem(shape_num, "points");
	if (points == NULL)
		return NULL;

	return points->valuestring;
}

cJSON *cjson_get_hole_by_id(cJSON *root, uint8_t id)
{
	cJSON *vectorGPSObject = NULL;
	cJSON *holes = NULL;
	cJSON *hole = NULL;
	cJSON *hole_id = NULL;
	cJSON *holecount = NULL;

	vectorGPSObject = cJSON_GetObjectItem(root, "vectorGPSObject");
	holes = cJSON_GetObjectItem(vectorGPSObject, "holes");
	holecount = cJSON_GetObjectItem(vectorGPSObject, "holecount");

	if (id >= holecount->valueint)
		return NULL;

	hole = cJSON_GetObjectItem(holes, "hole");
	hole_id = cJSON_GetArrayItem(hole, id);

	return hole_id;
}

int cjson_get_hole_count(cJSON *root)
{
	cJSON *vectorGPSObject = NULL;
	cJSON *holecount = NULL;

	vectorGPSObject = cJSON_GetObjectItem(root, "vectorGPSObject");
	if (vectorGPSObject == NULL)
		return -1;

	holecount = cJSON_GetObjectItem(vectorGPSObject, "holecount");
	if (holecount == NULL)
		return -1;

	return holecount->valueint;

}


int parse_points(char *str, struct gps_point **opoints, int *pnum)
{
	int length = 0;
	int i = 0;
	int count = 0;
	struct gps_point *points = NULL;
	char *p = NULL;
	char *start = NULL;
	char *end = NULL;
//	char tmp[256] = { 0 };

	length = strlen(str);


//	SYS_LOG_INF("%s: str: %s\n", __func__, str);

	for (i = 0; i < length; i++)
	{
		if (str[i] == ',')
		{
			count++;
		}
	}
	count++;

	points = (struct gps_point *)app_mem_malloc(count * sizeof(struct gps_point));
	if (points == NULL)
	{
		printf("%s %d: malloc %d * %d bytes failed\n", __func__, __LINE__, count, sizeof(struct gps_point));
		return -1;
	}

	p = str;
	start = str;
	for (i = 0; i < count; i++)
	{
		start = p;
		p = strchr(start, ' ');
		end = strchr(start, ',');
		points[i].x = strtof(start, NULL);
		points[i].y = strtof(p + 1, NULL);
/*
		SYS_LOG_INF("11: %s\n", start);
		SYS_LOG_INF("11: %s\n", p);
		SYS_LOG_INF("11: %s\n", end);
		SYS_LOG_INF("%s: %f, %f  test: %f\n", __func__, points[i].x, points[i].y, 12.333f);
		snprintf(tmp, 256, "%f, %f, %f\n", points[i].x, points[i].y, 12.333f);
		SYS_LOG_INF("22222: %s", tmp);
*/
		p = end + 1;

	}

	*opoints = points;
	*pnum = count;


	return 0;
}


int caculate_screen_scale_xy(int width, int height, 
			struct gps_point *gps_points, int points_num, 
			struct screen_scale_xy *scale_xy)
{
	float min_longitude = 0.0f;
	float max_longitude = 0.0f;
	float min_latitude = 0.0f;
	float max_latitude = 0.0f;

	int i = 0;

	min_longitude = gps_points[0].x;
	max_longitude = gps_points[0].x;
	min_latitude = gps_points[0].y;
	max_latitude = gps_points[0].y;

	for (i = 0; i < points_num; i++)
	{
		if (min_longitude > gps_points[i].x)
			min_longitude = gps_points[i].x;
		if (max_longitude < gps_points[i].x)
			max_longitude = gps_points[i].x;
		if (min_latitude > gps_points[i].y)
			min_latitude = gps_points[i].y;
		if (max_latitude < gps_points[i].y)
			max_latitude = gps_points[i].y;
	}

	scale_xy->height = height;
	scale_xy->width = width;
	scale_xy->min_longitude = min_longitude;
	scale_xy->max_longitude = max_longitude;
	scale_xy->min_latitude = min_latitude;
	scale_xy->max_latitude = max_latitude;
	scale_xy->scaleX = ((max_longitude - min_longitude) * 3600 / width);
	scale_xy->scaleY = ((max_latitude - min_latitude) * 3600 / height);

	return 0;
}

int convert_points_gps_to_screen(struct screen_scale_xy *scale_xy,
				struct gps_point *gps_points, 
				struct screen_point *screen_points, 
				int points_num)
{
	int i = 0;

	for (i = 0; i < points_num; i++)
	{
		screen_points[i].x = (gps_points[i].x - scale_xy->min_longitude) * 3600 / scale_xy->scaleX;
		screen_points[i].y = (scale_xy->max_latitude - gps_points[i].y) * 3600 / scale_xy->scaleY;
	}

	return 0;
}

#if 0
int main(int argc, char *argv[])
{
	int ret = 0;
	long length = 0;
	FILE *fp = NULL;
	unsigned char *buff = NULL;

	cJSON *root = NULL;
	cJSON *hole = NULL;

	char *p = NULL;
	struct gps_point *gps_points = NULL;
	struct screen_point *screen_points = NULL;
	int points_num = 0;

	struct screen_scale_xy scale_xy;

	fp = fopen(argv[1], "r");
	fseek(fp, 0, SEEK_END);
	length = ftell(fp);
	buff = (unsigned char *)calloc(1, length);
	fseek(fp, 0, SEEK_SET);
	ret = fread(buff, 1, length, fp);
//      fprintf(stdout, "length: %ld, ret: %d\n", length, ret);
	fclose(fp);

	root = cJSON_Parse(buff);
	hole = cjson_get_hole_by_id(root, 8);
	if (hole == NULL)
	{
		fprintf(stdout, "cant find hole: %d\n", 0);
		return -1;
	}

	p = cjson_hole_get_points_by_type_index(hole, "perimeter", 0);
	fprintf(stdout, "hole: %d, type: %s, index: %d, points: %s\n", 0, "perimeter", 0, p);

	parse_points(p, &gps_points, &points_num);
	for (int i = 0; i < points_num; i++)
	{
		fprintf(stdout, "i: %d, (%.12lf, %.12lf)\n", i, gps_points[i].x, gps_points[i].y);
	}

	caculate_screen_scale_xy(360, 360, gps_points, points_num, &scale_xy);
	fprintf(stdout, "scaleX: %.12lf, scaleY: %.12lf, %.12lf, %.12lf, %.12lf, %.12lf\n", 
			scale_xy.scaleX, scale_xy.scaleY, 
			scale_xy.min_longitude, scale_xy.max_longitude,
			scale_xy.min_latitude, scale_xy.max_latitude);

	screen_points = (struct screen_point *)calloc(points_num, sizeof(struct screen_point));
	convert_points_gps_to_screen(&scale_xy, gps_points, screen_points, points_num);
	for (int i = 0; i < points_num; i++)
	{
		fprintf(stdout, "i: %d, %d %d\n", i, screen_points[i].x, screen_points[i].y);
	}



	free(gps_points);
	free(screen_points);

	cJSON_free(root);
	free(buff);
	return 0;
}

int main(int argc, char *argv[])
{
	int ret = 0;
	long length = 0;
	FILE *fp = NULL;
	unsigned char *buff = NULL;
	cJSON *root = NULL;
	cJSON *vectorGPSObject = NULL;
	cJSON *holes = NULL;
	cJSON *holecount = NULL;
	cJSON *hole = NULL;
	cJSON *hole_1 = NULL;
	cJSON *holenumber = NULL;
	cJSON *perimeter = NULL;
	cJSON *shapes = NULL;
	cJSON *shapecount = NULL;
	cJSON *shape = NULL;
	cJSON *shape_1 = NULL;
	cJSON *points = NULL;


	fp = fopen(argv[1], "r");
	fseek(fp, 0, SEEK_END);
	length = ftell(fp);
	buff = (unsigned char *)calloc(1, length);
	fseek(fp, 0, SEEK_SET);
	ret = fread(buff, 1, length, fp);
//      fprintf(stdout, "length: %ld, ret: %d\n", length, ret);
	fclose(fp);

	root = cJSON_Parse(buff);
	vectorGPSObject = cJSON_GetObjectItem(root, "vectorGPSObject");
	holes = cJSON_GetObjectItem(vectorGPSObject, "holes");
	holecount = cJSON_GetObjectItem(vectorGPSObject, "holecount");
	hole = cJSON_GetObjectItem(holes, "hole");
	hole_1 = cJSON_GetArrayItem(hole, 0);
	holenumber = cJSON_GetObjectItem(hole_1, "holenumber");
	perimeter = cJSON_GetObjectItem(hole_1, "perimeter");
	shapes = cJSON_GetObjectItem(perimeter, "shapes");
	shapecount = cJSON_GetObjectItem(perimeter, "shapecount");
	shape = cJSON_GetObjectItem(shapes, "shape");
	shape_1 = cJSON_GetArrayItem(shape, 0);
	points = cJSON_GetObjectItem(shape_1, "points");

//	fprintf(stdout, "%s\n", cJSON_Print(holenumber));
//	fprintf(stdout, "%s\n", cJSON_Print(shapecount));
//	fprintf(stdout, "%s\n", cJSON_Print(shape_1));
	fprintf(stdout, "%d\n", holecount->valueint);
	fprintf(stdout, "%s\n", (points->valuestring));
	cJSON_free(root);

	return 0;
}
#endif
