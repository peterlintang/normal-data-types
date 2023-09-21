
#ifndef GPS_HEADER__
#define GPS_HEADER__

struct gps_info {
	int valid;				// 0 : gps info is not valid, 1: gps info valid;
	int la_degree;
	int la_minute;
	int la_second;
	char latitude[32]; 		//	latitude 纬度
	char north_south[2];	// N： 北纬，  S:南纬

	int lo_degree;
	int lo_minute;
	int lo_second;
	char longitude[32];		// longitude 经度
	char east_west[2];		// E:东经，  W:西经
};

/*
 * 获取经纬度信息，
 * @info: 返回的信息
 * 成功返回0，失败返回-1；
 */
int gps_get_loc(struct gps_info *info);

/*
 * 去初始化gps
 */
int gps_exit(void);

/*
 * 初始化gps
 */
int gps_init(void);


#endif	// end of GPS_HEADER__

