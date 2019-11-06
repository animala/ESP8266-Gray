/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2016 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP8266 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "ets_sys.h"
#include "osapi.h"
#include "uart.h"
#include "user_interface.h"
#include "gpio.h"
#include "pwm.h"
#include "gpio_user.h"
#include "gpio16.h"
#include "OLED.h"

#if ((SPI_FLASH_SIZE_MAP == 0) || (SPI_FLASH_SIZE_MAP == 1))
#error "The flash map is not supported"
#elif (SPI_FLASH_SIZE_MAP == 2)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x81000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0xfb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0xfc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0xfd000
#elif (SPI_FLASH_SIZE_MAP == 3)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x81000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0x1fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0x1fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0x1fd000
#elif (SPI_FLASH_SIZE_MAP == 4)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x81000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0x3fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0x3fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0x3fd000
#elif (SPI_FLASH_SIZE_MAP == 5)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x101000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0x1fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0x1fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0x1fd000
#elif (SPI_FLASH_SIZE_MAP == 6)
#define SYSTEM_PARTITION_OTA_SIZE							0x6A000
#define SYSTEM_PARTITION_OTA_2_ADDR							0x101000
#define SYSTEM_PARTITION_RF_CAL_ADDR						0x3fb000
#define SYSTEM_PARTITION_PHY_DATA_ADDR						0x3fc000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR				0x3fd000
#else
#error "The flash map is not supported"
#endif

static const partition_item_t at_partition_table[] = {
    { SYSTEM_PARTITION_BOOTLOADER, 						0x0, 												0x1000},
    { SYSTEM_PARTITION_OTA_1,   						0x1000, 											SYSTEM_PARTITION_OTA_SIZE},
    { SYSTEM_PARTITION_OTA_2,   						SYSTEM_PARTITION_OTA_2_ADDR, 						SYSTEM_PARTITION_OTA_SIZE},
    { SYSTEM_PARTITION_RF_CAL,  						SYSTEM_PARTITION_RF_CAL_ADDR, 						0x1000},
    { SYSTEM_PARTITION_PHY_DATA, 						SYSTEM_PARTITION_PHY_DATA_ADDR, 					0x1000},
    { SYSTEM_PARTITION_SYSTEM_PARAMETER, 				SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR, 			0x3000},
};


//延时ms级函数，但不宜延时太久
void ICACHE_FLASH_ATTR delay_ms(u32 time)
{
	for(;time>0;time--)
	{
		os_delay_us(1000);
	}


}



void ICACHE_FLASH_ATTR user_pre_init(void)
{
    if(!system_partition_table_regist(at_partition_table, sizeof(at_partition_table)/sizeof(at_partition_table[0]),SPI_FLASH_SIZE_MAP)) {
		os_printf("system_partition_table_regist fail\r\n");
		while(1);
	}
}


//软件定时器必须定义为全局变量
os_timer_t timer;   //

//软件定时器的回调函数
void pwm_run(void)
{
	static uint16 duty=0;
	static uint8 ch = 0;
	static flag=false;
	if(duty<=0)
	{
		duty=0;
		flag=false;
	}
	else if(duty>=22200)
	{
		/*ch ++;
		if(ch > 2)
		{
			ch = 0;
		}*/
		duty=22200;
		flag=true;
	}
	if(!flag)
	{
		duty+=200;
	}
	else
	duty-=200;
	os_printf("ch： %d  duty=%d\n",ch, duty);
	//设置占空比
	pwm_set_duty(duty,0);
  //设置完成后，需要调用 pwm_start
	pwm_start();
}

//定时器封装初始化函数
void ICACHE_FLASH_ATTR Timer_init(u32 time, u8 mode)
{

	//关闭软件定时器
	os_timer_disarm(&timer);
	//1，定时器结构体，2 定时器回调函数   3 回调函数的参数
	os_timer_setfn(&timer,(ETSTimerFunc *)pwm_run,NULL);
	//定时器开始。 50：定时时间 50ms  1：重复执行  0：只执行一次
	os_timer_arm(&timer,time,mode);


}







void ICACHE_FLASH_ATTR
user_init(void)
{


	uint8 pwm_duty[1] = {0};

	//定义3路pwm输出io
	uint32 io_info[][1] =
	{
		{PERIPHS_IO_MUX_MTDI_U,FUNC_GPIO12,12},
		//{PERIPHS_IO_MUX_MTDI_U,FUNC_GPIO13,13},
		//{PERIPHS_IO_MUX_MTDI_U,FUNC_GPIO14,14},
	};

	uint32_t status = 0;

	uint32 duty = 0;
	uint8 num = 0;
	uart_init(9600,9600);   //串口0 和串口1的波特率设定
	//os_delay_us(10000);     //延时10ms
	os_printf("---------------------------\r\n");
    os_printf("SDK version:%s\r\n", system_get_sdk_version());
	os_printf("Hello Gray!\r\n");
	os_printf("I will be better!\r\n");
	os_printf("I will be better123!\r\n");
	os_printf("---------------------------\r\n");
	OLED_Init();
	OLED_P6x8Str(0,0,"Hello Gray");
	//pwm_init(1000,pwm_duty,1,io_info);   //初始化PWM，周期为1000，占空比为pwm_duty数组，pwm通道数为3，3路IO输出配置

	gpio_output_func();
	gpio_set_inter_func();

#if 0
	while(1)
	{
		//status = ~status;
		//system_soft_wdt_feed();   //喂狗



	//	os_printf("duty is %d\r\n",duty);
		//os_delay_us(50000);   //500ms
	}
#endif
}
