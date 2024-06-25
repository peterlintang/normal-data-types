
/*
 * 
 *
 */
#ifndef MAKEFILE_HEADER_H__
#define MAKEFILE_HEADER_H__

#define NAME_LEN	16
#define FILES_MAX	8

struct file_info {
	char name[NAME_LEN];
	unsigned int file_size;
	unsigned int offset;
};

struct header {
	int files;
	struct file_info infos[FILES_MAX];
};

#endif // end of MAKEFILE_HEADER_H__

