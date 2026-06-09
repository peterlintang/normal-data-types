
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "course_info.h"

#define HOLE_NUM	1
#define GREEN_NUM	0

int main(int argc, char *argv[])
{
	int ret = 0;
	char id[16] = { 0 };
	int id_len = 16;
	char ver[16] = { 0 };
	int ver_len = 16;
	int isC = 0;
	int hls_num = 0;
	int teebox_num = 0;

	course_get_id(argv[1], id, &id_len);
	printf("id: %s, len: %d\n", id, id_len);

	course_get_ver(argv[1], ver, &ver_len);
	printf("ver: %s, len: %d\n", ver, ver_len);

	course_get_isC(argv[1], &isC);
	printf("isC: %d\n", isC);

	course_get_hlsNum(argv[1], &hls_num);
	printf("hls_num: %d\n", hls_num);

	course_get_TeeBoxNum(argv[1], &teebox_num);
	printf("teebox_num: %d\n", teebox_num);

	int hl = 0;
	double ha = 0.0;
	int gw = 0;
	int h = 0;
	int p = 0;
	course_get_hole_hl_by_index(argv[1], HOLE_NUM, &hl);
	course_get_hole_ha_by_index(argv[1], HOLE_NUM, &ha);
	course_get_hole_gw_by_index(argv[1], HOLE_NUM, &gw);
	course_get_hole_h_by_index(argv[1], HOLE_NUM, &h);
	course_get_hole_p_by_index(argv[1], HOLE_NUM, &p);
	printf("hl: %d, ha: %f, gw: %d, h: %d, p: %d\n", 
			hl, ha, gw, h, p);

	int pl_num = 0;
	int tpts_num = 0;
	int gpts_num = 0;
	course_get_hole_plNum_by_index(argv[1], HOLE_NUM, &pl_num);
	course_get_hole_tPtsNum_by_index(argv[1], HOLE_NUM, &tpts_num);
	course_get_hole_gPtsNum_by_index(argv[1], HOLE_NUM, &gpts_num);
	printf("pl_num: %d, tpts_num: %d, gpts_num: %d\n", pl_num, tpts_num, gpts_num);


	double alt = 0.0;
	char n[5] = { 0 };
	int n_len = 4;
	struct gps_point pt = { 0.0, 0.0 };
	struct gps_point center = { 0.0, 0.0 };
	for (int i = 0; i < pl_num; i++)
	{
		memset(n, 0, 5);
		n_len = 4;
		course_get_pl_alt_by_index(argv[1], HOLE_NUM, i, &alt);
		course_get_pl_n_by_index(argv[1], HOLE_NUM, i, n, &n_len);
		course_get_pl_pt_by_index(argv[1], HOLE_NUM, i, &pt);
		printf("alt: %f, n: %s, n_len: %d, pt.x: %f, pt.y: %f\n", 
				alt, n, n_len, pt.x, pt.y);
		if (!strcmp(n, "g"))
		{
			center.x = pt.x;
			center.y = pt.y;
		}
	}

	struct gps_point pts_item[2] = { 0.0, 0.0 };
	course_get_tPts_arrayItemInfo_by_index(argv[1], HOLE_NUM, 0, pts_item);
	printf("tpts: (%.6f %.6f) (%.6f %.6f)\n", 
			pts_item[0].x, pts_item[0].y,
			pts_item[1].x, pts_item[1].y);
	printf("distance: %f\n", calc_2gpspoints_distance(&pts_item[0], &pts_item[1]));

	int pts_num = 0;
	course_get_gPts_arrayItemNum_by_index(argv[1], HOLE_NUM, GREEN_NUM, &pts_num);
	printf("pts num: %d\n", pts_num);

	if (pts_num > 0)
	{
		struct gps_point *pts = NULL;
		pts = (struct gps_point *)calloc(pts_num, sizeof(struct gps_point));
		course_get_gPts_arrayItemInfo_by_index(argv[1], HOLE_NUM, GREEN_NUM, pts, pts_num);
		for (int i = 0; i < pts_num; i++)
		{
			printf("x[%d]: %f, y[%d]: %f\n", i, pts[i].x, i, pts[i].y);
		}

		double f = 0.0;
		double b = 0.0;
		double c = 0.0;
		green_fcb(pts, pts_num, &pts_item[0], &center, &f, &c, &b);
		printf("f: %f, c: %f, b: %f\n", f, c, b);

		free(pts);
	}

	for (int i = 0; i < teebox_num; i++)
	{
		char nm[10] = { 0 };
		int nm_len = 9;
		double rt_value = 0.0;
		double sl_value = 0.0;
		double wrt_value = 0.0;
		double wsl_value = 0.0;
		course_get_teebox_nm_by_index(argv[1], i, nm, &nm_len);
		course_get_teebox_rt_by_index(argv[1], i, &rt_value);
		course_get_teebox_sl_by_index(argv[1], i, &sl_value);
		course_get_teebox_wrt_by_index(argv[1], i, &wrt_value);
		course_get_teebox_wsl_by_index(argv[1], i, &wsl_value);
		printf("nm: %s, rt: %f, sl: %f, wrt: %f, wsl: %f\n", 
				nm, rt_value, sl_value, wrt_value, wsl_value);
	}

	return 0;
}

