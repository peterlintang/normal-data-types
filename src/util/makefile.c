
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include "makefile.h"

int make_one_file(int fd, struct file_info *info)
{
	int in_fd = -1;
	int total = 0;
	int ret = 0;
	int size = 0;
	char buf[1024] = { 0 };
	
	in_fd = open(info->name, O_RDWR);
	if (in_fd < 0) return -1;

	while (total < info->file_size)
	{
		size = info->file_size - total;
		if (size > 1024) size = 1024;

		ret = read(in_fd, buf, size);
		if (ret < 0) return -2;
		write(fd, buf, ret);
		total += ret;
	}

	return 0;
}

int get_file_info(char *file, struct file_info *info)
{
	struct stat stat_buf;

	if (stat(file, &stat_buf) < 0)
	{
		return -1;
	}

	strncpy(info->name, file, NAME_LEN);
	info->file_size = stat_buf.st_size;

	return 0;
}

int extract_file(int fd, struct file_info *info)
{
	int total = 0;
	int ret = 0;
	int out_fd = -1;
	int size = 0;
	char buf[1024] = { 0 };
	char file_name[64] = { 0 };

	snprintf(file_name, 64, "%s_tmp", info->name);
	out_fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
//	out_fd = open(info->name, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (out_fd < 0) return -1;

	lseek(fd, info->offset, SEEK_SET);

	while (total < info->file_size)
	{
		size = info->file_size - total;
		if (size > 1024) size = 1024;

		ret = read(fd, buf, size);
		if (ret < 0) return -2;

		ret = write(out_fd, buf, ret);
		if (ret != ret) return -3;

		total += ret;
	}

	close(out_fd);

	return 0;
}

int extract_files(int fd, struct header *head)
{
	int i;

	for (i = 0; i < head->files; i++)
	{
		extract_file(fd, &(head->infos[i]));
	}
}

int main(int argc, char *argv[])
{
	int i;
	char *out_file = NULL;
	int out_fd = NULL;
	struct header head;
	int ret = 0;

	out_file = argv[argc - 1];
	out_fd = open(out_file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (out_fd < 0)
	{
		fprintf(stderr, "open %s failed\n", out_file);
		return -1;
	}

	memset(&head, 0, sizeof(head));
	if (write(out_fd, &head, sizeof(head)) != sizeof(head))
	{
		fprintf(stderr, "write head %s failed\n", out_file);
		return -1;
	}

	head.files = argc - 2 > FILES_MAX ? FILES_MAX : argc - 2;
	head.infos[0].offset = sizeof(head);

	for (i = 0; i < head.files; i++)
	{
		if (get_file_info(argv[i + 1], &(head.infos[i])) < 0)
		{
			fprintf(stderr, "get %s info failed\n", argv[i + 1]);
			return -1;
		}

		ret = make_one_file(out_fd, &(head.infos[i]));
		if (ret < 0)
		{
			fprintf(stderr, "make %s failed %d\n", argv[i + 1], ret);
			return -1;
		}

		if (i + 1 < head.files)
		{
			head.infos[i + 1].offset = head.infos[i].offset + head.infos[i].file_size;
		}
	}

	if (lseek(out_fd, 0, SEEK_SET) < 0)
	{
		fprintf(stderr, "seek error\n");
		return -1;
	}

	if (write(out_fd, &head, sizeof(head)) != sizeof(head))
	{
		fprintf(stderr, "write head info %s failed\n", out_file);
		return -1;
	}

	for (i = 0; i < head.files; i++)
	{
		fprintf(stdout, "name: %s, offset: %d, size: %d, index: %d\n", 
			head.infos[i].name,  head.infos[i].offset, head.infos[i].file_size, i);
	}

	fprintf(stdout, "extract file...\n");
	lseek(out_fd, 0, SEEK_SET);
	read(out_fd, &head, sizeof(head));
	extract_files(out_fd, &head);

	return 0;
}



