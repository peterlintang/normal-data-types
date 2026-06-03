

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

int minmea_scan(const char *sentence, const char *format, ...);

struct record {
	char *id;
	char *course;
	char *address;
	char *country;
	char *unknow3;
	char *state;
	char *longtitude;
	char *latitiude;
	char *unknow1;
	char *unknow2;
	char *folderid;
};

struct record *records[41000];

int fs_read_line(FILE *file, char *data)
{
	/*
	char line[1024] = { 0 }; 
	size_t n = 1024;
	char *y = line;

	int x = getline(&y, &n, file);
	memcpy(data, line, 360);
	return x;
	*/
  int i=0;
  int ret = 0;
  char *p = data;
  while ( i < 1024)
  {
    ret = fread(p, 1, 1, file);
    if (ret > 0)
    {
    	i++;
    	if (*p++ == '\n') 
	{
		p--;
		*p = '\0';
		return 1;
	}
    }
    else
    {
	return ret;
    }
   }
}

void print_record(struct record *item)
{
	printf("%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s\n", 
			item->id,
			item->course,
			item->address,
			item->country,
			item->unknow3,
			item->state,
			item->longtitude,
			item->latitiude,
			item->unknow1,
			item->unknow2,
			item->folderid
			);
}

struct record *parse_record(char *n_l)
{

	char line[1024] = { 0 };
	char s1[128] = { 0 };
	char s2[128] = { 0 };
	char s3[128] = { 0 };
	char s4[128] = { 0 };
	char s5[128] = { 0 };
	char s6[128] = { 0 };
	char s7[128] = { 0 };
	char s8[128] = { 0 };
	char s9[128] = { 0 };
	char s10[128] = { 0 };
	char s11[128] = { 0 };
	strcpy(line, n_l);
	minmea_scan(line, "sssssssssss", s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11); 
	/*
	printf("s1: %s\n", s1);
	printf("s2: %s\n", s2);
	printf("s3: %s\n", s3);
	printf("s4: %s\n", s4);
	printf("s5: %s\n", s5);
	printf("s6: %s\n", s6);
	printf("s7: %s\n", s7);
	printf("s8: %s\n", s8);
	printf("s9: %s\n", s9);
	printf("s10: %s\n", s10);
	printf("s11: %s\n", s11);
	*/
	struct record *new = (struct record *)calloc(1, sizeof(struct record));
	new->id = strdup(s1);
	new->course = strdup(s2);
	new->address = strdup(s3);
	new->country = strdup(s4);
	new->unknow3 = strdup(s5);
	new->state = strdup(s6);
	new->longtitude = strdup(s7);
	new->latitiude = strdup(s8);
	new->unknow1 = strdup(s9);
	new->unknow2 = strdup(s10);
	new->folderid = strdup(s11);

	/*
	struct record *new = (struct record *)calloc(1, sizeof(struct record));
	if (new)
	{
		char *start = NULL;
		char *end = NULL;
		char *p = NULL;

//		printf("111111111111\n\n\n%s\n", n_l);

		start = n_l;
		p = strchr(start, '|');
		*p = '\0';
		new->id = strdup(start);
//		strcpy(new->id, start);
//		printf("id: %s\n", start);

		start = p + 1;
		p = strchr(start, '|');
		*p = '\0';
		new->course = strdup(start);
//		strcpy(new->course, start);
//		printf("course: %s\n", start);

		start = p + 1;
		p = strchr(start, '|');
		*p = '\0';
		new->address = strdup(start);
//		strcpy(new->address, start);
//		printf("address: %s\n", start);

		start = p + 1;
		p = strchr(start, '|');
		*p = '\0';
		new->country = strdup(start);
//		strcpy(new->country, start);
//		printf("country: %s\n", start);

		start = p + 1;
		p = strchr(start, '|');
		*p = '\0';
		new->unknow3 = strdup(start);
//		strcpy(new->unknow3, start);
//		printf("unknow3: %s\n", start);

		start = p + 1;
		p = strchr(start, '|');
		*p = '\0';
		new->state = strdup(start);
//		strcpy(new->state, start);
//		printf("state: %s\n", start);

		start = p + 1;
		p = strchr(start, '|');
		*p = '\0';
		new->longtitude = strdup(start);
//		strcpy(new->longtitude, start);
//		printf("long: %s\n", start);

		start = p + 1;
		p = strchr(start, '|');
		*p = '\0';
		new->latitiude = strdup(start);
//		strcpy(new->latitiude, start);
//		printf("lati: %s\n", start);

		start = p + 1;
		p = strchr(start, '|');
		*p = '\0';
		new->unknow1 = strdup(start);
//		strcpy(new->unknow1, start);
//		printf("un1: %s\n", start);

		start = p + 1;
		p = strchr(start, '|');
		*p = '\0';
		new->unknow2 = strdup(start);
//		strcpy(new->unknow2, start);
//		printf("un2: %s\n", start);

		start = p + 1;
		new->folderid = strdup(start);
//		strcpy(new->folderid, start);
//		printf("folder: %s\n", start);
//		printf("111111111111\n\n\n%s\n", n_l);
	}
*/

	return new;
}

int main(int argc, char *argv[])
{
	FILE *fp = NULL;
	int ret = 0;
	char c = 0x0;
	char line[1024] = { 0 };
	int i = 0;

	printf("size: %ld\n", sizeof(struct record));
	fp = fopen(argv[1], "r");

	while (fs_read_line(fp, line) > 0)
	{
		records[i] = parse_record(line);
		print_record(records[i]);
		if (records[i] == NULL)
		{
			printf("no mem\n");
			return -1;
		}
		i++;
		memset(line, 0, 1024);
	}
	/*
	while ((ret = fread(&c, 1, 1, fp)) > 0)
	{
		i++;
		printf("%d: %c\n", i, c);
		if (i == 260)
			break;
	}
	*/

	for (int j = 0; j < i; j++)
	{
		free(records[j]->id);
		free(records[j]->course);
		free(records[j]->address);
		free(records[j]->country);
		free(records[j]->unknow3);
		free(records[j]->state);
		free(records[j]->longtitude);
		free(records[j]->latitiude);
		free(records[j]->unknow1);
		free(records[j]->unknow2);
		free(records[j]->folderid);
		free(records[j]);
	}

	fclose(fp);

	return 0;
}



#define false 0
#define true 1

static inline int minmea_isfield(char c) {
    return c != '|';
}

int minmea_scan(const char *sentence, const char *format, ...)
{
    int result = false;
    int optional = false;

    if (sentence == NULL)
        return false;

    va_list ap;
    va_start(ap, format);

    const char *field = sentence;
#define next_field() \
    do { \
        /* Progress to the next field. */ \
        while (minmea_isfield(*sentence)) \
            sentence++; \
        /* Make sure there is a field there. */ \
        if (*sentence == '|') { \
            sentence++; \
            field = sentence; \
        } else { \
            field = NULL; \
        } \
    } while (0)


    while (*format) {
        char type = *format++;

        if (type == ';') {
            // All further fields are optional.
            optional = true;
            continue;
        }

        if (!field && !optional) {
            // Field requested but we ran out if input. Bail out.
            goto parse_error;
        }

        switch (type) {
            case 's': { // String value (char *).
                char *buf = va_arg(ap, char *);

                if (field) {
                    while (minmea_isfield(*field))
                        *buf++ = *field++;
                }

                *buf = '\0';
            } break;

            default: { // Unknown.
                goto parse_error;
            }
        }

        next_field();
    }

    result = true;

parse_error:
    va_end(ap);
    return result;
}

