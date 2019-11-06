/*
 * gpio_user.h
 *
 *  Created on: 2019Äê11ÔÂ2ÈÕ
 *      Author: 10224
 */

#ifndef APP_INCLUDE_GPIO_USER_H_
#define APP_INCLUDE_GPIO_USER_H_

#include "eagle_soc.h"
#include "c_types.h"


void gpio_output_func(void);
u32 get_gpio_input_value(gpio_no);
void gpio_set_input_func(void);
void gpio_set_inter_func(void);


#endif /* APP_INCLUDE_GPIO_USER_H_ */
