#include "led.h"
#include "main.h"
bool led_status = false;
bool fire_status = false;
bool body_status = false;
void Led_Set(_Bool status)
{
    led_status = status;
    if (led_status)
        Bsp_LedON();
    else
        Bsp_LedOFF();
}
void Get_Fire_State(void)
{
    fire_status = (HAL_GPIO_ReadPin(Fire_GPIO_Port, Fire_Pin) == GPIO_PIN_RESET) ? 1 : 0;
    if (HAL_GPIO_ReadPin(Fire_GPIO_Port, Fire_Pin) == GPIO_PIN_RESET) // 低电平有效
    {
        printf("有火\r\n");
    }
    else
    {
        printf("无火\r\r");
    }
}

void Body_State(void)
{
    body_status = (HAL_GPIO_ReadPin(Body_PA1_GPIO_Port, Body_PA1_Pin) == GPIO_PIN_RESET) ? 1 : 0;
    if (body_status) // 低电平有效
    {
        printf("有人\r\n");
    }
    else
    {
        printf("无有人\r\r");
    }
}

void HandleSingleClick()
{
    // 蜂鸣器响
    Beep_OnOff(1);
}

void HandleDoubleClick()
{
    // 处理双击事件
    // 蜂鸣器响
    Beep_OnOff(0);
}

void HandleLongPress()
{
    // 处理长按事件
    // 蜂鸣器响
    Beep_OnOff(1);
}

#define SINGLE_CLICK_TIME 300 // 单击最大时间间隔（ms）
#define DOUBLE_CLICK_TIME 600 // 双击最大时间间隔（ms）
#define LONG_PRESS_TIME 1500  // 长按时间（ms）

typedef enum
{
    BUTTON_IDLE,             // 初始状态
    BUTTON_PRESSED,          // 按键按下
    BUTTON_RELEASED,         // 按键松开
    BUTTON_DOUBLE_CLICK_WAIT // 等待双击
} ButtonState;

volatile ButtonState buttonState = BUTTON_IDLE;
volatile uint32_t buttonPressTime = 0;
volatile uint32_t buttonReleaseTime = 0;

void ButtonHandler(void)
{
    uint8_t curKey1State = HAL_GPIO_ReadPin(Key1_GPIO_Port, Key1_Pin);
    uint8_t curKey2State = HAL_GPIO_ReadPin(Key2_GPIO_Port, Key2_Pin);
    switch (buttonState)
    {
    case BUTTON_IDLE:
        if (curKey1State == GPIO_PIN_RESET)
        { // 按键按下
            buttonPressTime = xTaskGetTickCount();
            buttonState = BUTTON_PRESSED;
        }
        break;

    case BUTTON_PRESSED:
        if (curKey1State == GPIO_PIN_SET)
        { // 按键松开
            buttonReleaseTime = xTaskGetTickCount();
            uint32_t pressDuration = buttonReleaseTime - buttonPressTime;

            if (pressDuration < SINGLE_CLICK_TIME)
            {
                // 单击事件
                HandleSingleClick();
            }
            else if (pressDuration >= LONG_PRESS_TIME)
            {
                // 长按事件
                HandleLongPress();
            }
            else
            {
                // 进入双击等待状态
                buttonState = BUTTON_DOUBLE_CLICK_WAIT;
            }
        }
        break;

    case BUTTON_DOUBLE_CLICK_WAIT:
        if (curKey1State == GPIO_PIN_RESET)
        {
            uint32_t currentTime = xTaskGetTickCount();
            if (currentTime - buttonReleaseTime < DOUBLE_CLICK_TIME)
            {
                // 双击事件
                HandleDoubleClick();
                buttonState = BUTTON_IDLE; // 处理完后回到初始状态
            }
            else
            {
                buttonState = BUTTON_IDLE; // 超过双击最大时间间隔，回到初始状态
            }
        }
        break;
    }
}
