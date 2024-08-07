// Copyright (c) 2023 Shandong University
// Copyright (c) 2023 Junchi Ren, Jinrun Yang

#include "aip1944.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <memory.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

static char filename[] = "/dev/aip";
static int fd = -1;
unsigned char test_demo[] = {
    0x00, 0x04, 0x00, 0x28, 
    0x27, 0xc4, 0x20, 0x24, 
    0x14, 0x44, 0x10, 0x24, 
    0x14, 0x54, 0x10, 0x20, 
    0x85, 0x54, 0x07, 0xfe, 
    0x45, 0x54, 0x00, 0x20, 
    0x45, 0x54, 0xf0, 0x20, 
    0x15, 0x54, 0x17, 0xe0, 
    0x15, 0x54, 0x11, 0x20, 
    0x25, 0x54, 0x11, 0x10, 
    0xe5, 0x54, 0x11, 0x10, 
    0x21, 0x04, 0x15, 0x10, 
    0x22, 0x84, 0x19, 0xca, 
    0x22, 0x44, 0x17, 0x0a, 
    0x24, 0x14, 0x02, 0x06, 
    0x08, 0x08, 0x00, 0x02
};

unsigned char aip1944_demo[] = {
	0x80, 0x0,  0x80, 0x0,	0x80, 0x0,  0x80, 0x0,	0x84, 0x10, 0x84, 0x10,
	0x84, 0x10, 0x84, 0x10, 0x84, 0x10, 0x84, 0x10, 0x84, 0x10, 0x84, 0x10,
	0x84, 0x10, 0xFC, 0x1F, 0x0,  0x10, 0x0,  0x0,	0x40, 0x0,  0x40, 0x0,
	0x40, 0x0,  0xFE, 0x3F, 0x20, 0x0,  0x90, 0x0,	0x88, 0x0,  0x84, 0x0,
	0xFC, 0x1F, 0x80, 0x0,	0x90, 0x4,  0x88, 0x8,	0x84, 0x10, 0x82, 0x20,
	0xA0, 0x0,  0x40, 0x0,	0x80, 0x0,  0x80, 0x0,	0x80, 0x0,  0x80, 0x0,
	0x80, 0x0,  0xFF, 0x7F, 0x80, 0x0,  0x80, 0x0,	0x40, 0x1,  0x40, 0x1,
	0x20, 0x2,  0x20, 0x2,	0x10, 0x4,  0x8,  0x8,	0x4,  0x10, 0x3,  0x60,
	0x44, 0x10, 0x88, 0x10, 0x88, 0x8,  0x0,  0x4,	0xFE, 0x7F, 0x2,  0x40,
	0x1,  0x20, 0xF8, 0x7,	0x0,  0x2,  0x80, 0x1,	0xFF, 0x7F, 0x80, 0x0,
	0x80, 0x0,  0x80, 0x0,	0xA0, 0x0,  0x40, 0x0,	0x0,  0x2,  0x4,  0x2,
	0x8,  0x2,  0x8,  0x2,	0x0,  0x2,  0x0,  0x2,	0xEF, 0x7F, 0x8,  0x2,
	0x8,  0x2,  0x8,  0x2,	0x8,  0x2,  0x8,  0x2,	0x28, 0x2,  0x18, 0x2,
	0x8,  0x2,  0x0,  0x2,	0x4,  0x2,  0x7C, 0x7E, 0x12, 0x9,  0x1,  0x0,
	0xFC, 0x1F, 0x4,  0x10, 0xFC, 0x1F, 0x4,  0x10, 0xFC, 0x1F, 0x4,  0x10,
	0xFC, 0x1F, 0x10, 0x4,	0xFF, 0x7F, 0x10, 0x4,	0x8,  0x4,  0x4,  0x4,
	0x8,  0x0,  0x88, 0xF,	0x88, 0x8,  0x88, 0x8,	0xBF, 0x8,  0x88, 0x8,
	0x8C, 0x8,  0x9C, 0x8,	0xAA, 0x8,  0xAA, 0x8,	0x89, 0x8,  0x88, 0x48,
	0x88, 0x48, 0x48, 0x48, 0x48, 0x70, 0x28, 0x0,	0x10, 0x8,  0xB8, 0x8,
	0xF,  0x9,  0x8,  0x9,	0x8,  0x8,  0xBF, 0x8,	0x8,  0x9,  0x1C, 0x9,
	0x2C, 0x8,  0xA,  0x78, 0xCA, 0xF,  0x9,  0x8,	0x8,  0x8,  0x8,  0x8,
	0x8,  0x8,  0x8,  0x8,	0x44, 0x10, 0x88, 0x10, 0x88, 0x8,  0x0,  0x4,
	0xFE, 0x7F, 0x2,  0x40, 0x1,  0x20, 0xF8, 0x7,	0x0,  0x2,  0x80, 0x1,
	0xFF, 0x7F, 0x80, 0x0,	0x80, 0x0,  0x80, 0x0,	0xA0, 0x0,  0x40, 0x0,
	0x8,  0x0,  0x8,  0x0,	0x8,  0x0,  0xF8, 0x3F, 0x8,  0x0,  0x4,  0x0,
	0x4,  0x0,  0xFC, 0x1F, 0x0,  0x10, 0x0,  0x10, 0x0,  0x10, 0xFF, 0x13,
	0x0,  0x10, 0x0,  0x10, 0x0,  0xA,  0x0,  0x4,	0x8,  0x4,  0x8,  0x4,
	0x8,  0x4,  0xC8, 0x7F, 0x3F, 0x4,  0x8,  0x4,	0x8,  0x4,  0xA8, 0x3F,
	0x18, 0x21, 0xC,  0x11, 0xB,  0x12, 0x8,  0xA,	0x8,  0x4,  0x8,  0xA,
	0x8A, 0x11, 0x64, 0x60, 0x80, 0x0,  0x80, 0x4,	0x80, 0x8,  0x80, 0x8,
	0xFE, 0x3F, 0xC0, 0x1,	0xA0, 0x2,  0xA0, 0x2,	0x90, 0x4,  0x88, 0x8,
	0x84, 0x10, 0x82, 0x20, 0x81, 0x40, 0x80, 0x0,	0x80, 0x0,  0x80, 0x0,
};




static unsigned char databuf[32] = { 0 };

int open_aip1944(void)
{
	fd = open(filename, O_RDWR);
	if (fd < 0) {
		printf("can't open file %s\n", filename);
		return -1;
	}
	return fd;
}

int close_aip1944(void)
{
	int ret;
	aip1944_display_clear();
	ret = close(fd);
	if (ret < 0) {
		printf("can't close file %s\n", filename);
		return -1;
	}
	fd = -1;
	return ret;
}

void aip1944_flush(void)
{
	if (fd < 0) {
		printf("没有打开文件：%s\n", filename);
		return;
	}
	if (write(fd, databuf, sizeof(databuf)) != sizeof(databuf))
	{
		printf("AIP1944写入失败\n");
	}
}

void aip1944_set_data(unsigned char *buf)
{
	int i;
	for (i = 0; i < sizeof(databuf); i++)
	{
		databuf[i] = buf[i];
	}
	aip1944_flush();
}

void aip1944_set_mask(unsigned char *buf)
{
	int i;
	for (i = 0; i < sizeof(databuf); i++)
	{
		databuf[i] |= buf[i];
	}
	aip1944_flush();
}

void aip1944_clear_mask(unsigned char *buf)
{
	int i;
	for (i = 0; i < sizeof(databuf); i++)
	{
		databuf[i] &= ~buf[i];
	}
	aip1944_flush();
}

static void _display_slide_(unsigned char *data, int byte_cnt)
{
	int ret;
	int count = (int)byte_cnt / 32;
	for (int i = 0; i < count; i++) {
		aip1944_set_data(data + i * 32);
		sleep(1);
	}
}

static void _display_roll_(unsigned char *data, int byte_cnt)
{
	int ret;
	int frames = byte_cnt - 32;
	int begin = 0;
	while (frames >= 0) {
		aip1944_set_data(data + begin);
		usleep(1000 * 33);
		frames -= 2;
		begin += 2;
	}
}

void aip1944_display(unsigned char *data, int byte_cnt, int mode)
{
	if (fd < 0) {
		printf("没有打开文件：%s\n", filename);
		return;
	}
	switch (mode) {
	case AIP1944_SLIDE_MODE:
		_display_slide_(data, byte_cnt);
		break;
	case AIP1944_ROLL_MODE:
		_display_roll_(data, byte_cnt);
		break;
	default:
		break;
	}
}

void aip1944_display_clear(void)
{
	int ret;

	if (fd < 0) {
		printf("没有打开文件：%s\n", filename);
		return;
	}
	memset(databuf, 0x00, sizeof(databuf));
	aip1944_flush();
}