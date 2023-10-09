
/*
 * 通过raw socket获取链路层数据包，然后应用解析
 */

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if_ether.h>
#include <net/if.h>
#include <netinet/in.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/tcp.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct arppacket
{
	unsigned short ar_hrd;
	unsigned short ar_pro;
	unsigned char ar_hln;
	unsigned char ar_pln;
	unsigned short ar_op;
	unsigned char ar_sha[ETH_ALEN];
	unsigned char ar_sip[4];
	unsigned char ar_tha[ETH_ALEN];
	unsigned char ar_tip[4];
};


int main(int argc, char *argv[])
{
	char ef[ETH_FRAME_LEN];
	struct ethhdr *p_ethhdr = NULL;
	char eth_dest[ETH_ALEN] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
	char eth_source[ETH_ALEN] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
//	char eth_dest[4] = { };
	int n;
	int i;

	/*
	 * 接收所有的ip包
	 */
	int fd = -1;
	fd = socket(AF_INET, SOCK_PACKET, htons(0x0003));

	p_ethhdr = (struct ethhdr *)ef;
	memcpy(p_ethhdr->h_dest, eth_dest, ETH_ALEN);
	memcpy(p_ethhdr->h_source, eth_source, ETH_ALEN);
	p_ethhdr->h_proto = htons(0x0806);

	struct arppacket *p_arp = NULL;
	p_arp = (struct arppacket *)(ef + ETH_HLEN);
	p_arp->ar_hrd = htons(0x1);
	p_arp->ar_pro = htons(0x0800);
	p_arp->ar_hln = 6;
	p_arp->ar_pln = 4;

	memcpy(p_arp->ar_sha, eth_source, ETH_ALEN);
	*((unsigned int *)p_arp->ar_sip) = inet_addr("192.168.1.152");
	memcpy(p_arp->ar_tha, eth_dest, ETH_ALEN);
	*((unsigned int *)p_arp->ar_tip) = inet_addr("192.168.1.1");

	for (i = 0; i < 8; i++)
	{
		n = write(fd, ef, ETH_FRAME_LEN);
		sleep(1);
	}

	close(fd);
	return 0;
}


#if 0

/*
 * 设置网口为混杂模式
 */
char *ethname = "eth0";
struct ifreq ifr;
strcpy(ifr.ifr_name, ethname);
ret = ioctl(fd, SIOCGIFFLAGS, &ifr);
if (ret < 0)
{
}

ifr.ifr_flags |= IFF_PROMISC;
ret = ioctl(fd, SIOCSIFFLAGS, &ifr);
if (ret < 0)
{
}


char ef[ETH_FRAME_LEN];
struct ethhdr *p_ethhdr;
int n;
p_ethhdr = (struct ethhdr *)ef;
n = read(fd, ef, ETH_FRAME_LEN);


fprintf(stdout, "dest mac: ");
for (i = 0; i < ETH_ALEN - 1; i++)
{
	fprintf(stdout, "%02x-", p_ethhdr->h_dest[i]);
}
fprintf(stdout, "%02x\n", p_ethhdr->h_dest[ETH_ALEN - 1]);

fprintf(stdout, "source mac: ");
for (i = 0; i < ETH_ALEN - 1; i++)
{
	fprintf(stdout, "%02x-", p_ethhdr->h_source[i]);
}
fprintf(stdout, "%02x\n", p_ethhdr->h_source[ETH_ALEN - 1]);

fprintf(stdout, "protocol: 0x%04x\n", ntohs(p_ethhdr->h_proto));



if (ntohs(p_ethhdr->h_proto) == 0x0800) // ip packet
{
	struct iphdr *p_iphdr = (struct iphdr *)(ef + ETH_HLEN);
	fprintf(stdout, "src ip: %s\n", inet_ntoa(p_iphdr->saddr));
	fprintf(stdout, "dst ip: %s\n", inet_ntoa(p_iphdr->daddr));
}



if (p_iphdr->protocol == 6)
{
	struct tcphdr *p_tcphdr = (struct tcphdr *)(p_iphdr + p_iphdr->ihl * 4);
	fprintf(stdout, "src port: %d\n", ntoh(p_tcphdr->source));
	fprintf(stdout, "dest port: %d\n", ntoh(p_tcphdr->dest));
}


if (p_iphdr->protocol == 17)
{
	struct udphdr *p_udphdr = (struct udphdr *)(p_iphdr + p_iphdr->ihl * 4);
	fprintf(stdout, "src port: %d\n", ntoh(p_udphdr->source));
	fprintf(stdout, "dest port: %d\n", ntoh(p_udphdr->dest));
}



char *app_data = NULL;
int app_len = 0;

if (p_iphdr->protocol == 6)
{
	struct tcphdr *p_tcphdr = (struct tcphdr *)(p_iphdr + p_iphdr->ihl * 4);

	app_data = p_tcphdr + 20;
	app_len = n - 16 - p_iphdr->ihl * 4 - 20;
}
else if (p_iphdr->protocol == 17)
{
	struct udphdr *p_udphdr = (struct udphdr *)(p_iphdr + p_iphdr->ihl * 4);

	app_data = p_udphdr + p_udphdr->len;
	app_len = n - 16 - p_iphdr->ihl * 4 - p_udphdr->len;
}

fprintf(stdout, "app data address: 0x%x, len: %d\n", app_data, app_len);

#endif




