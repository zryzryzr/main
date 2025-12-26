/**
 * @file TaskStackTracker.h
 * @author Kelvin Hsu @Weilianzhikong (magic.zju@gmail.com)
 * @brief 任务堆栈信息跟踪
 * @version 1.0
 * @date 2021-12-13
 *
 * @copyright Copyright (c) 2021
 *
 *
 * @note 启动后，在调试软件中，实时查看 “xTaskDepth” 的值，此变量为结构体数组，包含所有任务的任务名及堆栈信息
 * @note 数据中数值单位均为“WORD”，即4字节。和FreeRTOS的任务创建计量单位一致
 */

#ifndef __TASK_STACK_TRACKER_H
#define __TASK_STACK_TRACKER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#define TASK_STACK_DEBUG       ///< 任务堆栈追踪功能 使能。 如需要关闭任务堆栈追踪，注释掉此语句即可
#define ESTIMATE_TASK_COUNT 12 ///< 追踪的任务数量，设置一个预估大于可能创建任务的数量。(Lora当前最大20)

  /**
   * @brief 启动任务堆栈追踪
   *
   * @param period 任务堆栈信息刷新周期，单位ms，建议5000-10000
   */
  void Task_Tracker_Init(uint16_t period);

  /**
   * @brief 如果不启动任务堆栈追踪,也可以直接调用任务流程进行处理
   *
   */
  void vgCheckTaskStack(void);

  uint32_t Task_Tracker_GetFreeRoom(void);

#ifdef __cplusplus
}
#endif

#endif /* __TASK_STACK_TRACKER_H */
