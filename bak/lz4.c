
/*
 * gcc bak/lz4.c -llz4
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lz4.h>

#define M_SIZE	(1024 * 2)

int main(int argc, char *argv[])
{
	int version_num = 0;
	const char *version_str = NULL;

	version_num = LZ4_versionNumber();
	version_str = LZ4_versionString();

	printf("version: %d\n", version_num);
	printf("version: %s\n", version_str);

	const char * const src = "Lorem ipsum dolor sit amet, "
		"consectetur adipiscing elit. Lorem ipsum dolor site amat."
		"consectetur adipiscing elit. Lorem ipsum dolor site amat."
		"consectetur adipiscing elit. Lorem ipsum dolor site amat."
		"consectetur adipiscing elit. Lorem ipsum dolor site amat."
		"consectetur adipiscing elit. Lorem ipsum dolor site amat."
		"consectetur adipiscing elit. Lorem ipsum dolor site amat."
		"consectetur adipiscing elit. Lorem ipsum dolor site amat."
		"consectetur adipiscing elit. Lorem ipsum dolor site amat."
		"consectetur adipiscing elit. Lorem ipsum dolor site amat."
		"consectetur adipiscing elit. Lorem ipsum dolor site amat."
		"consectetur adipiscing elit. Lorem ipsum dolor site amat."
		"consectetur adipiscing elit. Lorem ipsum dolor site amat."
		"consectetur adipiscing elit. Lorem ipsum dolor site amat."
		"consectetur adipiscing elit. Lorem ipsum dolor site amat."
		"consectetur adipiscing elit. Lorem ipsum dolor site amat."
		"consectetur adipiscing elit. Lorem ipsum dolor site amat."
		"consectetur adipiscing elit. Lorem ipsum dolor site amat."
		"consectetur adipiscing elit. Lorem ipsum dolor site amat."
		"consectetur adipiscing elit. Lorem ipsum dolor site amat."
		"consectetur adipiscing elit. Lorem ipsum dolor site amat."
		"consectetur adipiscing elit. Lorem ipsum dolor site amat."
		"onsectetur adipiscing elit. Lorem ipsum dolor site amat."
		"consectetur adipiscing elit. Lorem ipsum dolor site amat."
		"consectetur adipiscing elit. Lorem ipsum dolor site amat."
		"consectetur adipiscing elit. Lorem ipsum dolor site amat.";
	const int src_size = (int)strlen(src + 1);
	const int max_dst_size = LZ4_compressBound(src_size);

	char *compressed_data = malloc((size_t)max_dst_size);

	int ret = LZ4_compress_default(src, compressed_data, src_size, max_dst_size);
	if (ret <= 0)
	{
		fprintf(stderr, "compress error: %d\n", ret);
	}

	printf("ret: %d, src_size: %d, %f\n", ret, src_size, (float)ret / src_size);

	char *dst = calloc(1, M_SIZE);
	int decompressed_size = LZ4_decompress_safe(compressed_data, dst, ret, M_SIZE);
	printf("decompressed_size: %d\n", decompressed_size);
	printf("%s\n", dst);
	if (memcmp(src, dst, src_size) != 0)
	{
		printf("\n\n\nerror\n");
		return -1;
	}
	printf("\n\n\nok\n");

	return 0;
}

