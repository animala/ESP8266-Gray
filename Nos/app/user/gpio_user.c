/*
 * gpio_user.c
 *
 *  Created on: 2019��11��2��
 *      Author: 10224
 */

#include "gpio_user.h"
#include "ets_sys.h"
#include "osapi.h"
#include "uart.h"
#include "user_interface.h"
#include "eagle_soc.h"


#define PIN_STATE_GET(NO)   (GPIO_INPUT_GET(GPIO_ID_PIN(NO)) != 0)


//gpio������Ժ���
void ICACHE_FLASH_ATTR
gpio_output_func(void)
{
	//�ܽŹ���ѡ����
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12); // ѡ��GPIO12
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, FUNC_GPIO14); // �ܽŶ�Ӧ���ܲ�һ��������Ҫ���ٷ���Ӧ���ܽ�ӳ��
	//PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13);

	//�ܽ��������
//	GPIO_OUTPUT_SET(GPIO_ID_PIN(14),1); //gpio14 ��� 1
//	GPIO_OUTPUT_SET(GPIO_ID_PIN(13),0);
//	GPIO_OUTPUT_SET(GPIO_ID_PIN(12),1);  //gpio12 ��� 0
}

//gpio ����ioΪ����
void ICACHE_FLASH_ATTR
gpio_set_input_func()
{
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13);   //ѡ��gpio13Ϊ����ܽ�

	GPIO_DIS_OUTPUT(GPIO_ID_PIN(13));   //����GPIO13Ϊ����

	PIN_PULLUP_EN(PERIPHS_IO_MUX_MTCK_U);  //ʹ����������
}

//��ȡ��ֵ
u32 ICACHE_FLASH_ATTR
get_gpio_input_value(gpio_no)
{
	return GPIO_INPUT_GET(gpio_no);
}

//GPIO�жϵ��÷�
// 1 ѡ��ܽ�   PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, FUNC_GPIO14);
// 2 ����Ϊ����  GPIO_DIS_OUTPUT(GPIO_ID_PIN(14));   //����GPIO14Ϊ����
// 3 ��ֹ���е��ж� EST_GPIO_INTR_DISABLE();
// 4 �����жϴ����� EST_GPIO_INTR_ATTACH(GPIO_intr_handler,NULL);  GPIO_intr_handler ���жϻص�����
// 5 ��������Ϊ�½��صĴ�����ʽ gpio_pin_intr_state_get(GPIO_ID_PIN(14),GPIO_PIN_INTR_NEGEDGE);


//�жϷ�����
void GPIO_intr_handler()
{
	u32 pin_state = 0;
	u8 state = 0;
	pin_state = GPIO_REG_READ(GPIO_STATUS_ADDRESS);  //��ȡIO��״̬

	ETS_GPIO_INTR_DISABLE();  //�ر��ж�

	if(pin_state & BIT(13))   //�ж�����
	{
		if(PIN_STATE_GET(13))
		{
			GPIO_OUTPUT_SET(GPIO_ID_PIN(12),0);  //PIN12 ��ת
		}
		else
		{
			GPIO_OUTPUT_SET(GPIO_ID_PIN(12),1);  //PIN12 ��ת
		}

	}

	GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS,BIT(13));    //��������ŵ��жϱ�־λ

	ETS_GPIO_INTR_ENABLE();   //�����ж�

}


void ICACHE_FLASH_ATTR
gpio_set_inter_func()
{
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U, FUNC_GPIO13);   //ѡ��gpio13Ϊ����ܽ�

	GPIO_DIS_OUTPUT(GPIO_ID_PIN(13));   //����GPIO13Ϊ����

	ETS_GPIO_INTR_DISABLE();   //�ر������ж�

	ETS_GPIO_INTR_ATTACH(&GPIO_intr_handler,NULL);  //ע���жϷ�����

	gpio_pin_intr_state_set(GPIO_ID_PIN(13),GPIO_PIN_INTR_ANYEDGE);  //����pin13Ϊ�����ش���

	GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS,BIT(13));    //��������ŵ��жϱ�־λ

	ETS_GPIO_INTR_ENABLE();   //�����ж�
}




