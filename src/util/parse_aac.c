
/*
 * 解析aac adts音频文件格式
 */
#include <stdio.h>
#include <stdlib.h>

#define ADTS_HEAD_LEN	7
#define ADTS_CRC_LEN	2

struct adts_fix_head {
	unsigned int syncword : 12;
	unsigned int id	: 1;
	unsigned int layer : 2;
	unsigned int protection : 1;
	unsigned int profile : 2;
	unsigned int fre : 4;
	unsigned int priv : 1;
	unsigned int channel_conf : 3;
	unsigned int org : 1;
	unsigned int home : 1;
};

struct adts_variable_head {
	unsigned int ident_bit : 1;
	unsigned int ident_start : 1;
	unsigned int len : 13;
	unsigned int fullness : 11;
	unsigned int blocks : 2;
};

struct adts_head {
	struct adts_fix_head fix;
	struct adts_variable_head var;
};

static void adts_print_head_fix(struct adts_fix_head *fix)
{
	fprintf(stdout, "sync: %x\n"
			"id: %x\n"
			"layer: %x\n"
			"protection: %x\n"
			"profile: %x\n"
			"fre: %x\n"
			"priv: %x\n"
			"channel_conf: %x\n"
			"org: %x\n"
			"home: %x\n",
			fix->syncword,
			fix->id,
			fix->layer,
			fix->protection,
			fix->profile,
			fix->fre,
			fix->priv,
			fix->channel_conf,
			fix->org,
			fix->home
	       );
}
static void adts_print_head_var(struct adts_variable_head *var)
{
	fprintf(stdout, "bit: %x \nstart: %x \nlen: %x \nfullness: %x \nblocks: %x\n", 
			var->ident_bit,
			var->ident_start,
			var->len,
			var->fullness,
			var->blocks);
}
static void adts_print_head(struct adts_head *head)
{
	adts_print_head_fix(&head->fix);
	adts_print_head_var(&head->var);
}

struct adts_head *parse_adts_head(FILE *fp)
{
	int read_len = 0;
	struct adts_head *head = NULL;
	unsigned char header[ADTS_HEAD_LEN] = { 0 };
	unsigned char crc[ADTS_CRC_LEN] = { 0 };

	read_len = fread(header, 1, ADTS_HEAD_LEN, fp);
	if (ADTS_HEAD_LEN != read_len)
	{
		return NULL;
	}

	for (int i = 0; i < ADTS_HEAD_LEN; i++)
	{
		fprintf(stdout, "%x", header[i]);
	}
	fprintf(stdout, "\n");

	if (header[0] != 0xff && (header[1] & 0xf0) != 0xf0)
	{
		fprintf(stdout, "no adts start\n");
		return NULL;
	}

	head = (struct adts_head *)calloc(1, sizeof(*head));
	if (head == NULL)
	{
		return NULL;
	}

	head->fix.syncword = (header[0] << 4) | ((header[1] & 0xf0) >> 4);
	head->fix.id = (header[1] & 0x08) >> 3;
	head->fix.layer = (header[1] & 0x6) >> 1;
	head->fix.protection = (header[1] & 0x1) >> 0;
	head->fix.profile = (header[2] & 0xc0) >> 6;
	head->fix.fre = (header[2] & 0x3c) >> 2;
	head->fix.priv = (header[2] & 0x02) >> 1;
	head->fix.channel_conf = (((header[2] & 0x01)) << 2) | ((header[3] & 0xc0) >> 6);
	head->fix.org = (header[3] & 0x20 ) >> 5;
	head->fix.home = (header[3] & 0x10) >> 4;
		


	head->var.ident_bit  = (header[3] & 0x08) >> 3;
	head->var.ident_start = (header[3] & 0x04) >> 2;
	head->var.len  = (((header[3] & 0x03)) << 11) | (header[4] << 3) | ((header[5] & 0xe0) >> 5);
	head->var.fullness = (((header[5] & 0x1f)) << 6) | ((header[6] & 0xfc) >> 2);
	head->var.blocks  = (header[6] & 0x3);

	if (head->fix.protection == 0)
	{
		head->var.len -= 2;
		fread(crc, 1, ADTS_CRC_LEN, fp);
		fprintf(stdout, "crc: ");
		for (int i = 0; i < ADTS_CRC_LEN; i++)
		{
			fprintf(stdout, "%x,", crc[i]);
		}	
		fprintf(stdout, "\n");
	}

	head->var.len -= 7;
	


	return head;

}

int parse_adts_data(FILE *fp, struct adts_head *head)
{
	int ret = 0;
	unsigned char *data = NULL;
	short crc = 0;

	data = (unsigned char *)calloc(1, head->var.len);
	ret = fread(data, 1, head->var.len, fp);
	for (int i = 0; i < ret; i++)
	{
		crc += data[i];
		/*
		fprintf(stdout, "%x ", data[i]);
		if ((i + 1) % 16 == 0)
			fprintf(stdout, "\n");
			*/
	}
//	fprintf(stdout, "\n");
//	fprintf(stdout, "crc: %x\n", crc);
	return 0;
}

int main(int argc, char *argv[])
{
	FILE *fp = NULL;
	struct adts_head *head = NULL;
	int count = 0;

	fp = fopen(argv[1], "rb");
	while (1)
	{
		fprintf(stdout, "frames: %d\n", count);
		head = parse_adts_head(fp);
		if (head == NULL)
		{
			if (feof(fp)) break;
			fprintf(stderr, "parse head failed\n");
			break;
		}
		count++;

		adts_print_head(head);

		parse_adts_data(fp, head);
	}

	fclose(fp);

	fprintf(stdout, "frames: %d\n", count);

	return 0;
}




