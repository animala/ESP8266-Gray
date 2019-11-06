/*
 * gpio_user.c
 *
 *  Created on: 2019年11月2日
 *      Author: 10224
 */

#include "gpio_user.h"
#include "ets_sys.h"
#include "osapi.h"
#include "uart.h"
#include "user_interface.h"
#include "eagle_soc.h"


#define PIN_STATE_GET(NO)   (GPIO_INPUT_GET(GPIO_ID_PIN(NO)) != 0)


//gpio输出测试函数
void ICACHE_FLASH_ATTR
gpio_output_func(void)
{
	//管脚功能选择函数
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12); // 选择GPIO12
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, FUNC_GPIO14); // 管脚对应功能不一样，这里要看官方对应个管脚映射
	//PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13);

	//管脚输出函数
//	GPIO_OUTPUT_SET(GPIO_ID_PIN(14),1); //gpio14 输出 1
//	GPIO_OUTPUT_SET(GPIO_ID_PIN(13),0);
//	GPIO_OUTPUT_SET(GPIO_ID_PIN(12),1);  //gpio12 输出 0
}

//gpio 设置io为输入
void ICACHE_FLASH_ATTR
gpio_set_input_func()
{
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13);   //选择gpio13为输入管脚

	GPIO_DIS_OUTPUT(GPIO_ID_PIN(13));   //设置GPIO13为输入

	PIN_PULLUP_EN(PERIPHS_IO_MUX_MTCK_U);  //使能输入上拉
}

//获取键值
u32 ICACHE_FLASH_ATTR
get_gpio_input_value(gpio_no)
{
	return GPIO_INPUT_GET(gpio_no);
}

//GPIO中断的用法
// 1 选择管脚   PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, FUNC_GPIO14);
// 2 设置为输入  GPIO_DIS_OUTPUT(GPIO_ID_PIN(14));   //设置GPIO14为输入
// 3 禁止所有的中断 EST_GPIO_INTR_DISABLE();
// 4 设置中断处理函数 EST_GPIO_INTR_ATTACH(GPIO_intr_handler,NULL);  GPIO_intr_handler 是中断回调函数
// 5 配置引脚为下降沿的触发方式 gpio_pin_intr_state_get(GPIO_ID_PIN(14),GPIO_PIN_INTR_NEGEDGE);


//中断服务函数
void GPIO_intr_handler()
{
	u32 pin_state = 0;
	u8 state = 0;
	pin_state = GPIO_REG_READ(GPIO_STATUS_ADDRESS);  //获取IO的状态

	ETS_GPIO_INTR_DISABLE();  //关闭中断

	if(pin_state & BIT(13))   //判断引脚
	{
		if(PIN_STATE_GET(13))
		{
			GPIO_OUTPUT_SET(GPIO_ID_PIN(12),0);  //PIN12 翻转
		}
		else
		{
			GPIO_OUTPUT_SET(GPIO_ID_PIN(12),1);  //PIN12 翻转
		}

	}

	GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS,BIT(13));    //清除该引脚的中断标志位

	ETS_GPIO_INTR_ENABLE();   //开启中断

}


void ICACHE_FLASH_ATTR
gpio_set_inter_func()
{
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13);   //选择gpio13为输入管脚

	GPIO_DIS_OUTPUT(GPIO_ID_PIN(13));   //设置GPIO13为输入

	ETS_GPIO_INTR_DISABLE();   //关闭所有中断

	ETS_GPIO_INTR_ATTACH(&GPIO_intr_handler,NULL);  //注册中断服务函数

	gpio_pin_intr_state_set(GPIO_ID_PIN(13),GPIO_PIN_INTR_ANYEDGE);  //设置pin13为上下沿触发

	GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS,BIT(13));    //清除该引脚的中断标志位

	ETS_GPIO_INTR_ENABLE();   //开启中断
}




