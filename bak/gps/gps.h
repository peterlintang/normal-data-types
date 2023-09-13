
#ifndef GPS_HEADER__
#define GPS_HEADER__

struct gps_info {
	int valid;				// 0 : gps info is not valid, 1: gps info valid;
	char latitude[32]; 		//	latitude
	char north_south[2];	// N S
	char longitude[32];		// longitude
	char east_west[2];		// E W
};

int gps_get_loc(struct gps_info *info);
int gps_exit(void);
int gps_init(void);


#endif	// end of GPS_HEADER__

