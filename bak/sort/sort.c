
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*
 * 35AF4C09|Miramar Memorial G.C.|U.S. Marine Corps|US|CA|San Diego|32.884857|-117.141106|1|3.8|007
 */
#define ID_LEN		32
#define NAME_LEN	64
#define ADDRESS_LEN	128
#define COUNTRY_LEN	16
#define STATE_LEN	16
#define CITY_LEN	32
#define LATITUDE_LEN	32
#define LONGTITUDE_LEN	32
#define PUBLISHED_LEN	2
#define VERSION_LEN	32
#define FOLDERINDEX_LEN	32

struct course_info {
	char id[ID_LEN];
	char name[NAME_LEN];
	char address[ADDRESS_LEN];
	char country[COUNTRY_LEN];
	char state[STATE_LEN];
	char city[CITY_LEN];
	char latitude[LATITUDE_LEN];
	char longtitude[LONGTITUDE_LEN];
	char published[PUBLISHED_LEN];
	char version[VERSION_LEN];
	char folderIndex[FOLDERINDEX_LEN];
};

int parse_line(char *line, struct course_info *info)
{
	char *p = NULL;
	char *start = NULL;

	start = line;
	p = strchr(start, '|');
	strncpy(info->id, start, p - start);

	start = p + 1;
	p = strchr(start, '|');
	strncpy(info->name, start, p - start);

	start = p + 1;
	p = strchr(start, '|');
	strncpy(info->address, start, p - start);

	start = p + 1;
	p = strchr(start, '|');
	strncpy(info->country, start, p - start);

	start = p + 1;
	p = strchr(start, '|');
	strncpy(info->state, start, p - start);

	start = p + 1;
	p = strchr(start, '|');
	strncpy(info->city, start, p - start);

	start = p + 1;
	p = strchr(start, '|');
	strncpy(info->latitude, start, p - start);

	start = p + 1;
	p = strchr(start, '|');
	strncpy(info->longtitude, start, p - start);

	start = p + 1;
	p = strchr(start, '|');
	strncpy(info->published, start, p - start);

	start = p + 1;
	p = strchr(start, '|');
	strncpy(info->version, start, p - start);

	start = p + 1;
//	p = strchr(start, '|');
	strncpy(info->folderIndex, start, FOLDERINDEX_LEN);

	return 0;
}

static void print_course(struct course_info *info)
{
	fprintf(stdout, "%s %s %s %s %s", 
			info->country, info->latitude, info->longtitude,
			info->id, info->folderIndex);
	/*
	fprintf(stdout, "%s %s %s %s %s %s %s %s %s %s %s",
		info->id,
		info->name,
		info->address,
		info->country,
		info->state,
		info->city,
		info->latitude,
		info->longtitude,
		info->published,
		info->version,
		info->folderIndex);
		*/
}

struct Point {
	double x;
	double y;
};

static double distance(struct Point *A, struct Point *B)
{
	double Ax = A->x * M_PI / 180.0;
	double Ay = A->y * M_PI / 180.0;
	double Bx = B->x * M_PI / 180.0;
	double By = B->y * M_PI / 180.0;
	double lat;
	double lon;
	double a;
	double c;
	double d;
	double R = 6371.0 * 1000;

	lat = Bx - Ax;
	lon = By - Ay;
	a = pow(sin(lat / 2), 2) + cos(Ax) * cos(Bx) * pow(sin(lon / 2), 2);
	c = 2 * atan2(sqrt(a), sqrt(1 - a));
	return d = R * c;
}

struct Point point = { 0.0f, 0.0f };

static int compare(const void *arg1, const void *arg2)
{
	struct course_info *a = (struct course_info *)arg1;
	struct course_info *b = (struct course_info *)arg2;

	struct Point ap;
	double a_distance = 0.0f;

	struct Point bp;
	double b_distance = 0.0f;

	ap.x = strtod(a->latitude, NULL);
	ap.y = strtod(a->latitude, NULL);

	bp.x = strtod(b->latitude, NULL);
	bp.y = strtod(b->latitude, NULL);

	a_distance = distance(&point, &ap);
	b_distance = distance(&point, &bp);

	if (a_distance < b_distance)
		return -1;
	else if (a_distance > b_distance)
		return 1;
	else
		return 0;
}

static int compare2(const void *arg1, const void *arg2)
{
	struct course_info *a = (struct course_info *)arg1;
	struct course_info *b = (struct course_info *)arg2;



	if (strcmp(a->folderIndex, b->folderIndex) < 0)
		return -1;
	else if (strcmp(a->folderIndex, b->folderIndex) > 0)
		return 1;
	else
		return 0;
}

int main(int argc, char *argv[])
{
#define COURSES_NUM	40000
	FILE *fp = NULL;
	char line[1024];
	char *pline = line;
	size_t n = 1024;
	struct course_info *courses_info = NULL;
	int courses_num = 0;
	int ret = 0;

	courses_info = (struct course_info *)calloc(COURSES_NUM, sizeof(struct course_info));
again:
	n = 1024;
	pline = line;
	ret = getline(&pline, &n, stdin);
//	fprintf(stdout, "line: %s\n", pline);
	if (ret == -1)
	{
//		fprintf(stdout, "end of file\n");
	}
	else
	{
		parse_line(pline, &courses_info[courses_num]);
#ifdef TEST
		print_course(&courses_info[courses_num]);
#endif
		courses_num++;
		goto again;
	}

	qsort(courses_info, courses_num, sizeof(struct course_info), compare2);

//#ifdef TEST
#if 1
//	fprintf(stdout, "after sort\n");
	for (int i = 0; i < courses_num; i++)
		print_course(&courses_info[i]);
#endif


	free(courses_info);

	return ret;
}

