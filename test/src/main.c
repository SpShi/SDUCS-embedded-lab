#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "aht20.h"
#include "stk8ba.h"
#include "motor.h"
#include "beep.h"
#include "ch422g.h"
#include "rc522.h"
#include "aip1944.h"
#include "key16.h"
#include "led.h"
#include "ir.h"

#define msleep(ms)	usleep(1000 * (ms))
#define SLIDE_CONST 100

struct ir_info_struct {
	struct input_event ir_event;
	int ir_flag;
	pthread_mutex_t ir_lock;
};

typedef enum key_op {
	OP_LED1,
	OP_LED2,
	OP_LED3,
	OP_SHOW_TEM,
	OP_SHOW_HUM,
	OP_BEEP,
	OP_MOTOR_STANDBY,
	OP_MOTOR_FORWARD,
	OP_MOTOR_BACKWARD,
	OP_MOTOR_BRAKE,
	OP_ERROR,
	OP_SET_INTERVAL,
	OP_CHANGE_MODE
} System_OP_Type;

pthread_t logo_pth, ir_pth, rc_pth;
int logo_mode = AIP1944_SLIDE_MODE;
int xyz[3], tem, hum;
int interval= 20000;
struct ir_info_struct ir_info;
int nfc_finish=0;
int key_finish=0;
int system_finish=0;
int action=0;
int action1=0,action2=0,action3=0;
int block=0;
int terminate=0;

enum {
	CH422G_NOTHTING,
	CH422G_TEM,
	CH422G_HUM,
	CH422G_NUM
} ch422g_mode;

void system_init(void);
void system_exit(void);
System_OP_Type key_decode(uint16_t key);
void key_handler(uint16_t key);
void key_handler_wel(uint16_t key);
void key_handler_set(uint16_t key);
void key_handler_rgb(uint16_t key);
void ir_handler(struct input_event *ir_event);
void ch422g_showdata(void);


//display input pswd or use NFC
void *logo_display_inputpswd(void *arg)
{
	unsigned char test_demo[] = {
0x4, 0x2, 0x4, 0x5, 0x84, 0x8, 0x5F, 0x10, 0xA4, 0x6F, 0x2, 0x0, 0xCA, 0x23, 0x49, 0x2A, 0x5F, 0x2A, 0xC8, 0x2B, 0x58, 0x2A, 0x4F, 0x2A, 0xCA, 0x2B, 0x48, 0x22, 0x48, 0x2A, 0x48, 0x13, 0x20, 0x0, 0x40, 0x0, 0x80, 0x0, 0x80, 0x0, 0x80, 0x0, 0x40, 0x1, 0x40, 0x1, 0x40, 0x1, 0x20, 0x2, 0x20, 0x2, 0x10, 0x4, 0x10, 0x4, 0x8, 0x8, 0x4, 0x8, 0x2, 0x10, 0x1, 0x60, 0x40, 0x0, 0x80, 0x0, 0xFE, 0x7F, 0x2, 0x40, 0x41, 0x24, 0x90, 0x2, 0x14, 0x11, 0xD2, 0x28, 0x38, 0x8, 0xE7, 0xF, 0x80, 0x0, 0x84, 0x10, 0x84, 0x10, 0x84, 0x10, 0xFC, 0x1F, 0x0, 0x10, 0x0, 0x0, 0x80, 0x1F, 0x3F, 0x10, 0x8, 0x10, 0x8, 0x11, 0x4, 0x11, 0x3C, 0x11, 0x26, 0x7F, 0x26, 0x40, 0x25, 0x40, 0x24, 0x40, 0xA4, 0x5F, 0x3C, 0x40, 0x24, 0x40, 0x4, 0x28, 0x0, 0x10,0x0, 0x12, 0x0, 0x22, 0x0, 0x2, 0xFF, 0x7F, 0x0, 0x2, 0x0, 0x2, 0x7C, 0x22, 0x44, 0x22, 0x44, 0x22, 0x44, 0x14, 0x7C, 0x14, 0x0, 0x48, 0xE0, 0x4C, 0x1E, 0x52, 0x4, 0x61, 0xC0, 0x40, 0x0, 0x40, 0xFC, 0x43, 0x4, 0x42, 0x4, 0x42, 0xFC, 0x4B, 0x44, 0x48, 0x44, 0x48, 0x44, 0x48, 0xFC, 0x4B, 0x4A, 0x4A, 0x4A, 0x4A, 0x4A, 0x4A, 0xC9, 0x42, 0x48, 0x41, 0x40, 0x50, 0x40, 0x20, 0x40, 0x0, 0x40, 0x0, 0xC0, 0x1F, 0x40, 0x0, 0x40, 0x0, 0x40, 0x0, 0xFF, 0x7F, 0x40, 0x0, 0x40, 0x0, 0x40, 0x2, 0x40, 0x4, 0x40, 0x8, 0x40, 0x10, 0x40, 0x0, 0x40, 0x0, 0x40, 0x0};
	//收到cancel信号，取消此线程
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        //立即取消此线程
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	while (1)
	{
		aip1944_display(test_demo,sizeof(test_demo),logo_mode);
		aip1944_display_clear();
	}
}


//display welcome
void *logo_display_welcome(void *arg)
{
	unsigned char demo[] = {0x0, 0x1, 0x0, 0x1, 0x3F, 0x1, 0x20, 0x3F, 0xA0, 0x20, 0x92, 0x10, 0x54, 0x2, 0x28, 0x2, 0x8, 0x2, 0x14, 0x5, 0x24, 0x5, 0xA2, 0x8, 0x81, 0x8, 0x40, 0x10, 0x20, 0x20, 0x10, 0x40, 0x0, 0x0, 0x4, 0x1, 0xC8, 0x3C, 0x48, 0x24, 0x40, 0x24, 0x40, 0x24, 0x4F, 0x24, 0x48, 0x24, 0x48, 0x24, 0x48, 0x2D, 0xC8, 0x14, 0x48, 0x4, 0x8, 0x4, 0x14, 0x4, 0xE2, 0x7F, 0x0, 0x0, 0x8, 0x2, 0x8, 0x2, 0xF8, 0x7F, 0x4, 0x2, 0x4, 0x2, 0xE6, 0x3F, 0x26, 0x22, 0x25, 0x22, 0xE4, 0x3F, 0x4, 0x2, 0x44, 0x2, 0x84, 0x2, 0x4, 0x1, 0x84, 0x2, 0x44, 0xC, 0x34, 0x70,  0x0, 0x0, 0xFC, 0x1F, 0x84, 0x10, 0x84, 0x10, 0x84, 0x10, 0xFC, 0x1F, 0x84, 0x10, 0x84, 0x10, 0x84, 0x10, 0xFC, 0x1F, 0x84, 0x10, 0x84, 0x10, 0x84, 0x10, 0x82, 0x10, 0x82, 0x14, 0x1, 0x8, 0x4, 0x0, 0x7C, 0x3E, 0x12, 0x22, 0x10, 0x22, 0xFF, 0x22, 0x28, 0x22, 0x44, 0x3E, 0x2, 0x0, 0xF8, 0xF, 0x8, 0x8, 0x8, 0x8, 0xF8, 0xF, 0x8, 0x8, 0x8, 0x8, 0xF8, 0xF, 0x8, 0x8,0x8, 0x2, 0x24, 0x22, 0x42, 0x12, 0xFF, 0xE, 0x80, 0x2, 0x0, 0x42, 0x7E, 0x42, 0x42, 0x7C, 0x42, 0x0, 0x7E, 0x22, 0x42, 0x12, 0x42, 0xE, 0x7E, 0x2, 0x42, 0x42, 0x52, 0x42, 0x22, 0x7C, 0x0, 0x0, 0xFE, 0x3F, 0x22, 0x0, 0x22, 0x0, 0xF2, 0x1F, 0xA, 0x1, 0x2, 0x1, 0xFA, 0x3F, 0x2, 0x1, 0x82, 0x2, 0x42, 0x4, 0x22, 0x8, 0x1A, 0x10, 0x2, 0x0, 0xFE, 0x7F, 0x0, 0x0, 0x0, 0x1, 0x0, 0x2, 0xF8, 0x7F, 0x8, 0x0, 0x9, 0x0, 0xEA, 0x3F, 0xA, 0x10, 0x8, 0x8, 0xC, 0x4, 0xA, 0x2, 0x9, 0x2, 0x8, 0x2, 0x4, 0x2, 0x4, 0x2, 0x82, 0x2, 0x1, 0x1, 0x0, 0x1F, 0xFC, 0x0, 0x20, 0x0, 0x10, 0x4, 0x8, 0x2, 0xFC, 0x1, 0x80, 0x0, 0x60, 0x8, 0x18, 0x10, 0xFE, 0x3F, 0x80, 0x20, 0x90, 0x4, 0x88, 0x8, 0x84, 0x10, 0xA2, 0x20, 0x40, 0x0, 0x8, 0x2, 0x8, 0x4, 0x4, 0x4, 0xC4, 0x7F, 0x12, 0x2, 0x1F, 0x11, 0x88, 0x20, 0xC4, 0x7F, 0x2, 0x49, 0x1F, 0x9, 0x2, 0x9, 0x0, 0x9, 0x98, 0x48, 0x87, 0x48, 0x42, 0x70, 0x20, 0x0};
	// 收到cancel信号，取消此线程
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	// 立即取消此线程
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	while (1)
	{
		aip1944_display(demo, sizeof(demo), logo_mode);
		aip1944_display_clear();
	}
}


//display set time
void *logo_display_set(void *arg){
	unsigned char demo[] = { 0x0, 0x0, 0x84, 0xF, 0x88, 0x8, 0x88, 0x8, 0x80, 0x8, 0x40, 0x70, 0x2F, 0x0, 0xC8, 0x1F, 0x88, 0x10, 0x88, 0x8, 0x8, 0x9, 0x28, 0x5, 0x18, 0x2, 0x8, 0x5, 0xC0, 0x18, 0x30, 0x60, 0xFE, 0x3F, 0x22, 0x22, 0xFE, 0x3F, 0x80, 0x0, 0xFE, 0x3F, 0x80, 0x0, 0xF8, 0xF, 0x8, 0x8, 0xF8, 0xF, 0x8, 0x8, 0xF8, 0xF, 0x8, 0x8, 0xF8, 0xF, 0x8, 0x8, 0xFF, 0x7F, 0x0, 0x0, 0x0, 0x10, 0x0, 0x10, 0x3E, 0x10, 0x22, 0x10, 0xA2, 0x7F, 0x22, 0x10, 0x22, 0x10, 0x3E, 0x10, 0x22, 0x11, 0x22, 0x12, 0x22, 0x12, 0x22, 0x10, 0x3E, 0x10, 0x22, 0x10, 0x0, 0x14, 0x0, 0x8,0x4, 0x0, 0xC8, 0x3F, 0x8, 0x20, 0x2, 0x20, 0xE2, 0x23, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0xE2, 0x23, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0xE2, 0x23, 0x2, 0x20, 0x2, 0x28, 0x2, 0x10};
	// 收到cancel信号，取消此线程
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	// 立即取消此线程
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	while (1)
	{
		aip1944_display(demo, sizeof(demo), logo_mode);
		aip1944_display_clear();
	}
}


//display SDUCS
void *logo_display_input(void *arg)
{
		//收到cancel信号，取消此线程
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        //立即取消此线程
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	while (1)
	{
		aip1944_display(aip1944_demo,sizeof(aip1944_demo),logo_mode);
		aip1944_display_clear();
	}
}



void *ir_receive(void *arg)
{
	struct input_event eventbuf;

	//收到cancel信号，取消此线程
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        //立即取消此线程
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	while (1)
	{
		ir_read(&eventbuf);

		pthread_mutex_lock(&ir_info.ir_lock);
		if (!ir_info.ir_flag)
		{
			memcpy(&ir_info.ir_event, &eventbuf, sizeof(eventbuf));
			ir_info.ir_flag = 1;
		}
		pthread_mutex_unlock(&ir_info.ir_lock);
	}
}

void *rc522_handler(void *arg)
{
	char readbuf[RC522_BLOCK_SIZE];
	int ret;

	//收到cancel信号，取消此线程
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        //立即取消此线程
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);	

	while (1)
	{
		ret = rc522_read(readbuf);
		if (ret == sizeof(readbuf))
		{
			led_on(0);
		}
		else
		{
			led_off(0);
		}
	}
}

void *password_keybord(void *arg)
{
	System_OP_Type pswd[6] = {OP_LED2,
							  OP_LED3,
							  OP_SHOW_TEM,
							  OP_SHOW_HUM,
							  OP_BEEP,
							  OP_MOTOR_STANDBY};
	uint16_t pswd_input[6];
	uint16_t key;
	int ps = 0;//is ps word right?
	//int ret = pthread_create(&logo_pth, NULL, logo_display_inputpswd, NULL);
	// if (ret)
	// {
	// 	printf("logo初始化失败！\n");
	// 	return;
	// }
	while (ps != 1)
	{
		int i=0;
		while(ps!=1){
			key = key_scan();
			sleep(0.1);
			if (key != 0)
			{
				// System_OP_Type op = key_decode(key);
				printf("key:%u\n", key);
				if (key & KEY16(15))
				{
					system_finish=1;
					return;
				}
				else if (key & KEY16(14))
				{
					i = 0;
				}
				else
				{
					pswd_input[i] = key;
					i++;
					if(i==6) break;			
				}
			}
		}
		for (int i = 0; i <= 6; i++)
		{
			if (i == 6)
			{
				//beep_on();
				led_on(0);
				msleep(1000);
				//beep_off();
				led_off(2);
				ps = 1;
				key_finish=1;
				printf("welcome!\n");
				break;
			}
			if (key_decode(pswd_input[i]) != pswd[i])
			{
				ps = 0;
				beep_on();
				led_on(2);
				msleep(1000);
				beep_off();
				led_off(2);
				printf("retype the password!\n");
				break;
			}
		}
	}
}

void *password_NFC(void *arg){
	char readbuf[RC522_BLOCK_SIZE];
	int ret;
	// 收到cancel信号，取消此线程
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	// 立即取消此线程
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	while (1)
	{
		ret = rc522_read(readbuf);
		if (ret == sizeof(readbuf))
		{
			led_on(0);
			msleep(1000);
			led_off(0);
			nfc_finish=1;
			printf("welcome\n");
			return;
		}
		else
		{
			led_off(0);
		}
	}
}

void *welcomefunc(void *arg){
	int ret = pthread_create(&logo_pth, NULL, logo_display_welcome, NULL);
	if (ret)
	{
		printf("logo初始化失败\n");
		return -1;
	}
	uint16_t key;
	struct input_event ir_event;
	while(1){

		key = key_scan();
		if(key&KEY16(14)){
			printf("stop welcome\n");
			pthread_cancel(logo_pth);
			return;
		}
		else if (key & KEY16(15)){
			printf("stop machine\n");
			terminate=1;
			pthread_cancel(logo_pth);
			return;
		}
		key_handler_wel(key);
		ir_handler(&ir_event);
		read_aht20(&tem, &hum);
		ch422g_showdata();
	}
}

void *setIntervalFunc(void *arg){
	pthread_create(&logo_pth,NULL,logo_display_set,NULL);
	uint16_t key;
	ch422g_mode=CH422G_NUM;
	while(1){
		if(key&KEY16(14)){
			pthread_cancel(logo_pth);
			printf("set\n");
			return;
		}
		read_aht20(&tem, &hum);
		ch422g_showdata();
		key=key_scan();
		key_handler_set(key);
		//sleep(0.1);	
	}

}

void *beepFunc(void *arg){
	while(1){
		beep_on();
		sleep(2);
		beep_off();
		sleep(1);
	}
}

//pao ma deng
void *ledFunc(void *arg){
	int color=(int)arg;
	while(1){
		led_on(color);
		sleep(1);
		led_off(color);
		sleep(1);
	}
}


void *greenFunc(void *arg){
	uint16_t key;
	while(1){
		if (key & KEY16(14))
		{
			//terminate,go to welcome
			printf("set\n");
			block=1;
			return;
		}
		if(key&KEY16(15)){
			terminate=1;
			printf("terminate\n");
			return;
		}
		read_aht20(&tem, &hum);
		key=key_scan();
		key_handler_rgb(key);
		ch422g_showdata();
	}
}


void *yellowFunc(void *arg){
	uint16_t key;
	while (1)
	{
		if (key & KEY16(14))
		{
			printf("set\n");
			block=1;
			return;
		}
		if (key & KEY16(15))
		{
			terminate=1;
			printf("terminate\n");
			return;
		}
		read_aht20(&tem, &hum);
		key = key_scan();
		key_handler_rgb(key);
		ch422g_showdata();
	}
}

void *redFunc(void *arg){
	uint16_t key;
	//pthread_create(&redbeep,NULL,beepFunc,NULL);
	while (1)
	{
		if (key & KEY16(14))
		{
			printf("set\n");
			block=1;
			//pthread_cancel(redbeep);
			return;
		}
		if (key & KEY16(15))
		{
			terminate=1;
			printf("terminate\n");
			return;
		}
		if(action1==1&&action2==1&&action3==1){
			printf("done\n");
			//pthread_cancel(redbeep);
			return;
		}
		key = key_scan();
		key_handler_rgb(key);
		ch422g_showdata();
	}
}

	pthread_t welcome;
int main(int argc, char *argv[])
{
	uint16_t key;
	//int ps = 0;
	struct input_event ir_event;
	
	system_init();
	int ret = pthread_create(&logo_pth, NULL, logo_display_inputpswd, NULL);
	if (ret)
	{
		printf("logo初始化失败！\n");
		return -1;
	}
	printf("choose a method: \n");
	//key=key_scan();
	//if(key&KEY16(0)){
	pthread_t keyboard,NFC;
	//password_keybord();
	//password_NFC();
	pthread_create(&keyboard,NULL,password_keybord,(void *)1);
	pthread_create(&NFC,NULL,password_NFC,(void *)2);
	while (key_finish == 0 && nfc_finish == 0 &&system_finish==0){
		printf("waiting\n");
		}
	if(system_finish==1){
		pthread_cancel(NFC);
		pthread_cancel(logo_pth);
		system_exit();
		return 0;
	}
	else if (nfc_finish == 0)
	{
		pthread_cancel(NFC);
		printf("by password\n");
	}
	else if(key_finish==0)
	{
		pthread_cancel(keyboard);
		printf("by NFC\n");
	}
	pthread_cancel(logo_pth);
	printf("welcome!!!\n");
	printf("1\n");

	pthread_create(&welcome,NULL,welcomefunc,NULL);
	pthread_join(welcome,NULL);
	if(terminate==1){
		system_exit();
		return 0;
	}
	printf("stop wel3\n");
	ret = pthread_create(&logo_pth, NULL, logo_display_input, NULL);
	printf("stop wel4\n");
	if (ret)
	{
		printf("logo初始化失败！\n");
		return -1;
	}
	pthread_t green,yellow,red,led,beep;
	int slide;
	int actionYellow=1,actionRed=1;
	while (1)
	{
		actionYellow=1;
		actionRed=1;
		action1=0;
		action2=0;
		action3=0;
		block=0;
		terminate=0;
		led_off(0);
		led_off(1);
		led_off(2);
		pthread_create(&green,NULL,greenFunc,NULL);
		printf("create green:\n");
		pthread_create(&led,NULL,ledFunc,(void *)0);
		slide = interval / SLIDE_CONST;
		printf("slide : %d\n",slide);
		//polling
		//when action 1&2&3 done, cancel thread
		for(int i=0;i<SLIDE_CONST/2;i++){
			if((action1==1)&&(action2==1)&&(action3==1)){
				actionYellow=0;
				actionRed=0;
				pthread_cancel(green);
				pthread_cancel(led);
				led_off(0);
				break;
			}
			if(block==1){
				actionYellow = 0;
				actionRed = 0;
				pthread_cancel(green);
				pthread_cancel(led);
				led_off(0);
				pthread_cancel(logo_pth);
				pthread_create(&welcome,NULL,welcomefunc,NULL);
				pthread_join(welcome,NULL);
				pthread_create(&logo_pth, NULL, logo_display_input, NULL);
				if (terminate == 1)
				{
					system_exit();
					return 0;
				}
				break;
			}
			//sleep(slide);
			msleep(slide);
			printf("sleep slide : %d\n",i);
		}
		if(actionYellow==1){
			pthread_cancel(green);
			pthread_cancel(led);
			led_off(0);
			pthread_create(&yellow,NULL,yellowFunc,NULL);
			printf("create yellow :\n");
			pthread_create(&led,NULL,ledFunc,(void *)1);
			for(int i=0;i<SLIDE_CONST/2;i++){
				if(action1==1&&action2==1&&action3==1){
					actionRed = 0;
					pthread_cancel(yellow);
					pthread_cancel(led);
					led_off(1);
					break;
				}
				if(block==1){
					actionRed = 0;
					pthread_cancel(yellow);
					pthread_cancel(led);
					led_off(1);
					pthread_cancel(logo_pth);
					pthread_create(&welcome, NULL, welcomefunc, NULL);
					pthread_join(welcome, NULL);
					pthread_create(&logo_pth, NULL, logo_display_input, NULL);
					if(terminate==1){
						system_exit();
						return 0;
					}
					break;
				}
				msleep(slide);
				printf("sleep slide : %d\n",i);
				//sleep(slide);
			}
			if(actionRed==1){
				pthread_cancel(yellow);
				pthread_cancel(led);
				led_off(1);
				pthread_create(&red,NULL,redFunc,NULL);
				pthread_create(&led,NULL,ledFunc,(void *)2);
				pthread_create(&beep,NULL,beepFunc,NULL);
				printf("create red\n");
				pthread_join(red,NULL);
				pthread_cancel(beep);
				pthread_cancel(led);
				led_off(2);
				beep_off();
				if(block==1){
					pthread_cancel(logo_pth);
					pthread_create(&welcome, NULL, welcomefunc, NULL);
					pthread_join(welcome, NULL);
					pthread_create(&logo_pth, NULL, logo_display_input, NULL);
					if (terminate == 1)
					{
						system_exit();
						return 0;
					}
				}
			}
		}
		// key = key_scan();
		// if (key & KEY16(15))
		// 	break;

		// key_handler(key);
		// ir_handler(&ir_event);
		// read_aht20(&tem, &hum);
		// // read_stk8ba_xyz(xyz);
		// //  printf("x:%d, y:%d, z:%d\n", xyz[0], xyz[1], xyz[2]);
		// ch422g_showdata();
	}

	system_exit();
	return 0;
}

void system_init(void)
{
	int ret = 0;
	//open_stk8ba();
	open_aht20();//tem hum
	open_beep();
	open_motor();
	open_aip1944();//dianzhen 
	open_rc522();
	open_ch422g();
	open_key16();
	open_led();
	open_ir();
	
	// ret = pthread_create(&logo_pth, NULL, logo_display, NULL);
	// if (ret)
	// {
	// 	printf("logo初始化失败！\n");
	// 	return;
	// }

	ret = pthread_create(&ir_pth, NULL, ir_receive, NULL);
	if (ret)
	{
		printf("红外处理初始化失败！\n");
		return;
	}

	// ret = pthread_create(&rc_pth, NULL, rc522_handler, NULL);
	// if (ret)
	// {
	// 	printf("智能卡初始化失败！\n");
	// 	return;
	// }

	printf("系统初始化成功\n");
}

void system_exit(void)
{
	printf("exit1\n");
	pthread_cancel(logo_pth);
	pthread_cancel(ir_pth);
	printf("exit2\n");
	//pthread_cancel(rc_pth);
	printf("exit3\n");
	pthread_join(logo_pth, NULL);
	pthread_join(ir_pth, NULL);
	//thread_join(rc_pth, NULL);
	//printf("exit3\n");
	//close_stk8ba();
	close_aht20();
	close_beep();
	close_motor();
	close_aip1944();
	close_rc522();
	close_ch422g();
	close_key16();
	close_led();
	close_ir();
	
	printf("系统退出成功\n");
}

System_OP_Type key_decode(uint16_t key)
{
	if (key & KEY16(0))
		return OP_LED1;
	if (key & KEY16(1))
		return OP_LED2;
	if (key & KEY16(2))
		return OP_LED3;
	if (key & KEY16(3))
		return OP_SHOW_TEM;
	if (key & KEY16(4))
		return OP_SHOW_HUM;
	if (key & KEY16(5))
		return OP_BEEP;
	if (key & KEY16(6))
		return OP_MOTOR_STANDBY;
	if (key & KEY16(7))
		return OP_MOTOR_FORWARD;
	if (key & KEY16(8))
		return OP_MOTOR_BACKWARD;
	if (key & KEY16(9))
		return OP_MOTOR_BRAKE;
	if (key & KEY16(10))
		return OP_ERROR;
	if (key & KEY16(11))
		return OP_SET_INTERVAL;
	if (key & KEY16(12))
		return OP_CHANGE_MODE;

	return OP_ERROR;
}

void key_handler(uint16_t key)
{
	static int beep_is_on = 0;

	System_OP_Type op = key_decode(key);
	switch (op)
	{
	case OP_LED1:
		led_neg(0);
		break;
	case OP_LED2:
		led_neg(1);
		break;
	case OP_LED3:
		led_neg(2);
		break;
	case OP_SHOW_TEM:
		ch422g_mode = CH422G_TEM;
		break;
	case OP_SHOW_HUM:
		ch422g_mode = CH422G_HUM;
		break;
	case OP_BEEP:
		if (beep_is_on)
			beep_off();
		else
			beep_on();
		beep_is_on = beep_is_on ? 0 : 1;
		break;
	case OP_MOTOR_STANDBY:
		motor_standby();
		break;
	case OP_MOTOR_FORWARD:{
		printf("motor7\n");
		motor_forward();
		break;
	}
	case OP_MOTOR_BACKWARD:{
printf("motor8\n");
		motor_backward();
		break;
	}
	case OP_MOTOR_BRAKE:
		motor_brake();
		break;
	case OP_ERROR:
		break;
	case OP_SET_INTERVAL:{
		pthread_cancel(logo_pth);
		pthread_create(&welcome, NULL, welcomefunc, NULL);
		pthread_join(welcome, NULL);
		pthread_create(&logo_pth,NULL,logo_display_input,NULL);
		break;
	}
	default:
		break;
	}
}

void key_handler_wel(uint16_t key)
{
	static int beep_is_on = 0;

	System_OP_Type op = key_decode(key);
	switch (op)
	{
	case OP_LED1:
		led_neg(0);
		break;
	case OP_LED2:
		led_neg(1);
		break;
	case OP_LED3:
		led_neg(2);
		break;
	case OP_SHOW_TEM:
		ch422g_mode = CH422G_TEM;
		printf("tem : %d\n",tem);
		break;
	case OP_SHOW_HUM:
		ch422g_mode = CH422G_HUM;
		break;
	case OP_BEEP:
		if (beep_is_on)
			beep_off();
		else
			beep_on();
		beep_is_on = beep_is_on ? 0 : 1;
		break;
	case OP_MOTOR_STANDBY:
		motor_standby();
		break;
	case OP_MOTOR_FORWARD:{
		printf("motor7\n");
		motor_forward();
		break;
	}
	case OP_MOTOR_BACKWARD:
		motor_backward();
		break;
	case OP_MOTOR_BRAKE:
		motor_brake();
		break;
	case OP_ERROR:
		break;
	case OP_SET_INTERVAL:{
		ch422g_mode=CH422G_NUM;
		pthread_t setInterval;
		pthread_cancel(logo_pth);
		pthread_create(&setInterval,NULL,setIntervalFunc,NULL);
		pthread_join(setInterval,NULL);
		pthread_create(&logo_pth,NULL,logo_display_welcome,NULL);
		break;
	}
	default:
		break;
	}
}

void key_handler_set(uint16_t key)
{
	static int beep_is_on = 0;
	
	System_OP_Type op = key_decode(key);
	switch (op)
	{
	case OP_LED1:
		interval+=1000;
		printf("%d\n",interval);
		break;
	case OP_LED2:{
		if(interval>0)
		interval-=1000;
		printf("%d\n",interval);
		break;
	}
	case OP_LED3:{
		interval=20000;
		break;
	}
	default:
		break;
	}
}


void key_handler_rgb(uint16_t key)
{
	static int beep_is_on = 0;

	System_OP_Type op = key_decode(key);
	switch (op)
	{
	case OP_LED1://display TEM
		ch422g_mode=CH422G_TEM;
		break;
	case OP_LED2://display HUM
		ch422g_mode=CH422G_HUM;
		break;
	//action1
	case OP_LED3:{
		motor_standby();
		action1=1;
		break;
	}
	//action2
	case OP_SHOW_TEM:{

		if(action1==1){

			motor_forward();
			sleep(1);
			motor_standby();
			action2=1;
		}
		break;
	}
	//action3
	case OP_SHOW_HUM:{
		if(action2==1){

			motor_backward();
			sleep(1);
			motor_standby();
			action3 = 1;
		}
		break;
	}
	case OP_BEEP:
		if (beep_is_on)
			beep_off();
		else
			beep_on();
		beep_is_on = beep_is_on ? 0 : 1;
		break;
	case OP_MOTOR_STANDBY:
		motor_brake();
		break;
	default:
		break;
	}
}


void ch422g_showdata(void)
{
	switch (ch422g_mode)
	{
	case CH422G_TEM:
		ch422g_set_num(2, tem / 100);
		ch422g_set_num(3, tem / 10);
		ch422g_set_mask(3, CH422G_PT);
		ch422g_set_num(4, tem);
		ch422g_flush();
		break;
	case CH422G_HUM:
		ch422g_set_num(2, hum / 100);
		ch422g_set_num(3, hum / 10);
		ch422g_set_mask(3, CH422G_PT);
		ch422g_set_num(4, hum);
		ch422g_flush();
		break;
	case CH422G_NUM:
		ch422g_set_num(1, interval/100000);
		ch422g_set_num(2, interval/10000);
		//ch422g_set_mask(3, CH422G_PT);
		ch422g_set_num(3, interval/1000);
		ch422g_set_num(4,0);
		ch422g_flush();
	default:
		break;
	}
}

void ir_handler(struct input_event *ir_event)
{
	struct key_table_t ir_key;
	pthread_mutex_lock(&ir_info.ir_lock);
	if (ir_info.ir_flag)
	{
		ir_encode(&ir_info.ir_event, &ir_key);
		ir_info.ir_flag = 0;
	}
	else
	{
		pthread_mutex_unlock(&ir_info.ir_lock);
		return;
	}
	pthread_mutex_unlock(&ir_info.ir_lock);
	
	switch (ir_key.key_code)
	{
	case KEY_1:
		led_neg(0);
		led_neg(1);
		led_neg(2);
		break;
	case KEY_2:
		printf("key2\n");
		if (logo_mode == AIP1944_SLIDE_MODE)
			logo_mode = AIP1944_ROLL_MODE;
		else
			logo_mode = AIP1944_SLIDE_MODE;
		pthread_cancel(logo_pth);
		pthread_join(logo_pth, NULL);
		pthread_create(&logo_pth, NULL, logo_display_welcome, NULL);
		break;
	default:
		break;
	}
}

