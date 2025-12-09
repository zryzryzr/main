#include "led.h"
#include "main.h"
bool led_status = false;
bool beep_status = false;
void Led_Set(_Bool status)
{
    led_status = status;
    if (led_status)
        Bsp_LedON();
    else
        Bsp_LedOFF();
}
bool Get_Fire(void)
{
    // 强制类型转化
    return HAL_GPIO_ReadPin(Fire_GPIO_Port, Fire_Pin);
}
