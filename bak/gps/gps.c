
/*
 *
 */

#include <termios.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>

#include "gps.h"
#include "utils/Log.h"

static int exit_flag = 0;
static struct gps_info location_info;
static pthread_mutex_t  gps_info_lock;

static int _gps_write(int fd, char *data, int len)
{
	int left = 0;
	int written = 0;
	int ret = 0;

	left = len;
	while (left > 0)
	{
		ret = write(fd, data + written, left);
		if (ret < 0)
		{
			LOGE("%s write data error ret: %d (%d %d)\n", __func__, ret, left, written);
			return ret;
		}
		left -= ret;
		written += ret;
	}

	return written;
}

static int _gps_read(int fd, char *data, int len)
{
	char buf[128] = { 0 };
	int left = 0;
	int read_len = 0;
	int ret = 0;

	left = len;
	while (left > 0)
	{
		ret = read(fd, buf + read_len, left);
		if (ret < 0)
		{
			LOGE("%s read data error ret: %d (%d %d)\n", __func__, ret, left, read_len);
			return ret;
		}
		left -= ret;
		read_len += ret;
	}

	memcpy(data, buf, len);

	return read_len;

}

static int _gps_get_line(int fd, char *data, int len)
{
	char buf[1024] = { 0 };
	int read_len = 0;
	int ret = 0;
	char c = 0;
	int copy_len = 0;

	while (1)
	{
		ret = _gps_read(fd, &c, 1);
		if (ret < 0)
		{
			return -1;
		}
		if (c == '\n')
		{
			break;
		}
//		fprintf(stdout, "%s c: %c, 0x%02x\n", __func__, c, c);
		buf[read_len++] = c;
	}

	if (len < read_len)
	{
		LOGE("%s len: %d, read_len: %d\n", __func__, len, read_len);
	}

	copy_len = len > read_len ? read_len : len;
	memcpy(data, buf, copy_len);

	return copy_len;
}

static int open_uart(char *dev)
{
	int fd = -1;
	struct termios oldtio = {0};
	struct termios newtio = {0};

	if (dev == NULL)
	{
		LOGE("%s arguments : %s error\n", __func__, dev);
		return -2;
	}

	fd = open(dev, O_RDWR | O_NOCTTY);
	if (fd < 0)
	{
		LOGE("%s open device: %s failed %d\n", __func__, dev, fd);
		return -1;
	}

	tcgetattr(fd, &oldtio);

	newtio.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
	newtio.c_iflag = 0;
	newtio.c_oflag = 0;
	newtio.c_lflag &= ~ECHO;
	newtio.c_cc[VTIME] = 0;
	newtio.c_cc[VMIN] = 1;
	tcflush(fd, TCIOFLUSH);
	tcsetattr(fd, TCSANOW, &newtio);

//	fcntl(fd, F_SETFL, O_NONBLOCK);

	LOGD("%s open %s success rate 115200\n", __func__, dev);

	return fd;
}

static int close_uart(int fd)
{
	LOGD("%s close gps %d\n", __func__, fd);
	if (fd >= 0)
	{
		close(fd);
	}
	return 0;
}


static int gps_enable_loc(int fd)
{
	int ret = 0;
	char buf[128] = { 0 };

	ret = snprintf(buf, 128, "%s", "AT+QGPS=1\r\n");
	buf[ret] = '\0';

	LOGD("%s buf: %s\n", __func__, buf);

	ret = _gps_write(fd, buf, strlen(buf));
	if (ret < 0)
	{
		LOGE("%s %s failed\n", __func__, buf);
		return -1;
	}

	return 0;

	memset(buf, 0, sizeof(buf));
	ret = _gps_get_line(fd, buf, 128);
	if (ret < 0)
	{
		LOGE("%s get result failed %d\n", __func__, ret);
		return -2;
	}

	LOGD("%s read buf: %s\n", __func__, buf);

	memset(buf, 0, sizeof(buf));
	ret = _gps_get_line(fd, buf, 128);
	if (ret < 0)
	{
		LOGE("%s get result failed %d\n", __func__, ret);
		return -2;
	}
	LOGD("%s read buf 2: %s\n", __func__, buf);

	if (strncmp(buf, "OK", strlen("OK")) != 0)
	{
		LOGE("%s enable gps failed\n", __func__);
		return -3;
	}

	return 0;
}

static int gps_disable_loc(int fd)
{
	int ret = 0;
	char buf[128] = { 0 };

	ret = snprintf(buf, 128, "%s", "AT+QGPSEND\r\n");
	buf[ret] = '\0';

	ret = _gps_write(fd, buf, strlen(buf));
	if (ret < 0)
	{
		LOGE("%s %s failed\n", __func__, buf);
		return -1;
	}

	return 0;

	memset(buf, 0, sizeof(buf));
	ret = _gps_get_line(fd, buf, 128);
	if (ret < 0)
	{
		LOGE("%s get result failed %d\n", __func__, ret);
		return -2;
	}

	LOGD("%s read buf: %s\n", __func__, buf);

	memset(buf, 0, sizeof(buf));
	ret = _gps_get_line(fd, buf, 128);
	if (ret < 0)
	{
		LOGE("%s get result failed %d\n", __func__, ret);
		return -2;
	}

	LOGD("%s read buf 2: %s\n", __func__, buf);

	if (strncmp(buf, "OK", strlen("OK")) != 0)
	{
		LOGE("%s enable gps failed\n", __func__);
		return -3;
	}

	return 0;
}

int gps_get_loc(struct gps_info *info)
{
	pthread_mutex_lock(&gps_info_lock);
	info->valid = location_info.valid;
	strncpy(info->latitude, location_info.latitude, 32);
	strncpy(info->north_south, location_info.north_south, 2);
	strncpy(info->longitude, location_info.longitude, 32);
	strncpy(info->east_west, location_info.east_west, 2);
	pthread_mutex_unlock(&gps_info_lock);

	return 0;
}

#if 0
int gps_get_loc(int fd, char *result, int len)
{
	int ret = 0;
	char buf[512] = { 0 };

	ret = snprintf(buf, 512, "%s", "AT+QGPSLOC=0\r\n");
	buf[ret] = '\0';

	LOGD("%s buf: %s\n", __func__, buf);
	ret = _gps_write(fd, buf, strlen(buf));
	if (ret < 0)
	{
		LOGE("%s %s failed\n", __func__, buf);
		return -1;
	}

	/* process echo, dont know how the data send echo */
	memset(buf, 0, sizeof(buf));
	ret = _gps_get_line(fd, buf, 512);
	if (ret < 0)
	{
		LOGE("%s get result failed %d\n", __func__, ret);
		return -2;
	}

	LOGD("%s read buf: %s\n", __func__, buf);

	memset(buf, 0, sizeof(buf));
	ret = _gps_get_line(fd, buf, 512);
	if (ret < 0)
	{
		LOGE("%s get result failed %d\n", __func__, ret);
		return -2;
	}

	memcpy(result, buf, strlen(buf) > len ? len : strlen(buf));

	LOGD("%s read buf 2: %s\n", __func__, buf);

	return 0;
}
#endif


#if 1
#include <pthread.h>

#define GPS_DEVICE_UART1	"/dev/ttyUSB1"
#define GPS_DEVICE_UART2	"/dev/ttyUSB2"

static int process_gps(char *buf, int ret)
{
	char *p = NULL;
    char *start = NULL;

    char utc[32] = { 0 };
    char status[4] = { 0 };
    char latitude[32] = { 0 };
    char north_south[2] = { 0 };
    char longitude[32] = { 0 };
    char east_west[2] = { 0 };
    char speed[32] = { 0 };
    char azimuth[32] = { 0 };
    char date[32] = { 0 };
    char MagneticDeclination[16] = { 0 };
    char MagneticDeclinationDirection[8] = { 0 };
    char mode[8] = { 0 };

    start = buf;
    if (strncmp(buf, "$GPRMC", 5) == 0)
    {
        p = strchr(start, ',');
        start = p + 1;
        p = strchr(start, ',');
        strncpy(utc, start, p - start);

        start = p + 1;
        p = strchr(start, ',');
        strncpy(status, start, p - start);

        start = p + 1;
        p = strchr(start, ',');
        strncpy(latitude, start, p - start);

        start = p + 1;
        p = strchr(start, ',');
        strncpy(north_south, start, p - start);

        start = p + 1;
        p = strchr(start, ',');
        strncpy(longitude, start, p - start);

        start = p + 1;
        p = strchr(start, ',');
        strncpy(east_west, start, p - start);

        start = p + 1;
        p = strchr(start, ',');
        strncpy(speed, start, p - start);

        start = p + 1;
        p = strchr(start, ',');
        strncpy(azimuth, start, p - start);

        start = p + 1;
        p = strchr(start, ',');
        strncpy(date, start, p - start);

        start = p + 1;
        p = strchr(start, ',');
        strncpy(MagneticDeclination, start, p - start);

        start = p + 1;
        p = strchr(start, ',');
        strncpy(MagneticDeclinationDirection, start, p - start);

        start = p + 1;
        strncpy(mode, start, 8);

		/*
		printf("utc: %s\n", utc);
        printf("status: %s\n", status);
        printf("latitude: %s\n", latitude);
        printf("north_south: %s\n", north_south);
        printf("longitude: %s\n", longitude);
        printf("east_west: %s\n", east_west);
        printf("speed: %s\n", speed);
        printf("azimuth: %s\n", azimuth);
        printf("date: %s\n", date);
        printf("MagneticDeclination: %s\n", MagneticDeclination);
        printf("MagneticDeclinationDirection: %s\n", MagneticDeclinationDirection);
        printf("mode: %s\n", mode);
		*/

		if (strncmp(status, "A", 1) == 0)
		{
			pthread_mutex_lock(&gps_info_lock);
			location_info.valid = 1;
			strncpy(location_info.latitude, latitude, 32);
			strncpy(location_info.north_south, north_south, 2);
			strncpy(location_info.longitude, longitude, 32);
			strncpy(location_info.east_west, east_west, 2);
			pthread_mutex_unlock(&gps_info_lock);
			LOGD("new info valid: %d, latitude: %s, north_south: %s, longitude: %s, east_west: %s\n", 
					location_info.valid,
					location_info.latitude, 
					location_info.north_south, 
					location_info.longitude, 
					location_info.east_west);
		}
		else if (strncmp(status, "V", 1) == 0)
		{
			pthread_mutex_lock(&gps_info_lock);
			location_info.valid = 0;
			pthread_mutex_unlock(&gps_info_lock);
			LOGD("info valid: %d, latitude: %s, north_south: %s, longitude: %s, east_west: %s\n", 
					location_info.valid,
					location_info.latitude, 
					location_info.north_south, 
					location_info.longitude, 
					location_info.east_west);
		}

        return 0;
    }
    else
    {
        return -1;
    }
}

static void *(gps_thread)(void *noused)
{
	int ret = 0;
	int fd = -1;
	int count = 0;
	char buf[512] = { 0 };
	pthread_t pid = 0;

	pthread_detach(pthread_self());


	while (1)
	{
		fd = open_uart(GPS_DEVICE_UART2);
		if (fd < 0)
		{
			count++;
			sleep(1);
		}
		else
		{
			break;
		}

		if (count < 20)
			continue;
		else
		{
			LOGE("cant open device: %s", GPS_DEVICE_UART2);
			pthread_exit(NULL);
		}
	}

	ret = gps_enable_loc(fd);
	if (ret < 0)
	{
		LOGE("%s %s enable gps location report failed\n", __func__, buf);
		close_uart(fd);
		return  NULL;
	}

	close_uart(fd);

	fd = open_uart(GPS_DEVICE_UART1);

	while (exit_flag == 0)
	{
		memset(buf, 0, 512);
		ret = _gps_get_line(fd, buf, 512);
		if (ret > 0)
		{
			/* process gps data */
//			LOGD("ret: %d, buf: %s\n", ret, buf);
			process_gps(buf, ret);
		}
	}

	close_uart(fd);
	pthread_mutex_destroy(&gps_info_lock);
	printf("exit thread gps\n");
	return NULL;
}

//int main(int argc, char *argv[])
int gps_init(void)
{
	int ret = 0;
	pthread_t pid = 0;

	memset(&location_info, 0, sizeof(location_info));
	pthread_mutex_init(&gps_info_lock, NULL);

	pthread_create(&pid, NULL, gps_thread, NULL);
	system("echo 5 > /sys/class/gpio/export\n");
	system("echo out > /sys/class/gpio/gpio5/direction ");
	system("echo 0 > /sys/class/gpio/gpio5/value");

//	while (1) sleep(3);

	return ret;
}

int gps_exit(void)
{
	exit_flag = 1;
	sleep(3);
	return 0;
}

#endif

