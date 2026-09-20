#ifndef PROS_PORT_H
#define PROS_PORT_H
#include <stdint.h>

/**
 * Standardize the type defintions for PROS compatability.
 */

/* Type definitions. */
#define portCHAR                 char
#define portFLOAT                float
#define portDOUBLE               double
#define portLONG                 long
#define portSHORT                short
#define portSTACK_TYPE           uint32_t
#define portBASE_TYPE            long
#define portPOINTER_SIZE_TYPE    intptr_t

typedef portSTACK_TYPE   StackType_t;
typedef portSTACK_TYPE   task_stack_t;
typedef int32_t          BaseType_t;
typedef uint32_t         UBaseType_t;

typedef uint32_t         TickType_t;
#define portMAX_DELAY              ( ( TickType_t ) UINT32_MAX )

#define portTICK_TYPE_IS_ATOMIC    1

/*-----------------------------------------------------------*/

/* Architecture specifics. */
#define portSTACK_GROWTH                   ( -1 )
#define portHAS_STACK_OVERFLOW_CHECKING    ( 1 )
#define portTICK_PERIOD_MS                 ( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#define portTICK_RATE_MICROSECONDS         ( ( TickType_t ) 1000000 / configTICK_RATE_HZ )
#define portBYTE_ALIGNMENT                 8
/*-----------------------------------------------------------*/

// Copied from src/system/refactor.tsv with some edits
// Macro hack instead of awk script to preserve portability across FreeRTOS upgrades
// ahem... ahem...
#define TaskHandle_t		           task_t
#define TaskFunction_t		           task_fn_t
#define eTaskState			           task_state_e_t
#define eNotifyAction                  notify_action_e_t
#define QueueHandle_t		           queue_t
#define SemaphoreHandle_t	           sem_t
#define MutexHandle_t                  mutex_t
#define StackType_t                    task_stack_t
#define StaticTask_t                   static_task_s_t
#define BaseType_t                     int32_t
#define UBaseType_t                    uint32_t
#define TickType_t			           uint32_t
#define ListItem_t			           list_item_t
#define StaticSemaphore_t              static_sem_s_t
#define StaticQueue_t                  static_queue_s_t
#define StreamBufferHandle_t           stream_buf_t
#define StaticStreamBuffer_t           static_stream_buf_s_t
#define MessageBufferHandle_t          msg_buf_t
#define StaticMessageBuffer_t          static_msg_buf_s_t
#define xTaskCreate			           task_create
#define xTaskCreateStatic              task_create_static
#define vTaskDelete			           task_delete
#define vTaskDelay			           task_delay
#define vTaskDelayUntil		           task_delay_until
#define uxTaskPriorityGet	           task_get_priority
#define vTaskPrioritySet	           task_set_priority
#define eTaskGetState		           task_get_state
#define vTaskSuspend		           task_suspend
#define vTaskResume			           task_resume
#define xTaskAbortDelay                task_abort_delay
#define vTaskStartScheduler		       rtos_sched_start
#define vTaskEndScheduler		       rtos_sched_stop
#define vTaskSuspendAll			       rtos_suspend_all
#define xTaskResumeAll			       rtos_resume_all
#define xTaskGetTickCount		       millis
#define uxTaskGetNumberOfTasks 	       task_get_count
#define pcTaskGetName			       task_get_name
#define xTaskGetHandle			       task_get_by_name
#define xTaskGenericNotify		       task_notify_ext
#define ulTaskNotifyTake		       task_notify_take
#define xTaskNotifyWait			       task_notify_wait
#define xTaskNotifyStateClear	       task_notify_clear
#define xTaskGetCurrentTaskHandle	   task_get_current
#define xQueueCreate			       queue_create
#define xQueueSendToFront		       queue_prepend
#define xQueueSendToBack		       queue_append
#define xQueueOverwrite			       queue_overwrite
#define xQueuePeek				       queue_peek
#define xQueueReceive			       queue_recv
#define uxQueueMessagesWaiting	       queue_get_waiting
#define uxQueueSpacesAvailable	       queue_get_available
#define vQueueDelete			       queue_delete
#define xSemaphoreCreateBinary	       sem_binary_create
#define xSemaphoreTake			       sem_wait
#define xSemaphoreTakeRecursive		   mutex_recursive_take
#define xSemaphoreGive				   sem_post
#define xSemaphoreGiveRecursive		   mutex_recursive_give
#define xSemaphoreCreateMutex		   mutex_create
#define xSemaphoreCreateRecursiveMutex mutex_recursive_create
#define xSemaphoreCreateCounting	   sem_create
#define vSemaphoreDelete			   sem_delete
#define xSemaphoreGetMutexHolder	   mutex_get_owner
#define uxSemaphoreGetCount			   sem_get_count
#define pvPortMalloc                   kmalloc
#define vPortFree                      kfree
#define xSemaphoreCreateMutexStatic    mutex_create_static
#define xSemaphoreCreateCountingStatic sem_create_static
#define xQueueCreateStatic             queue_create_static
#define xStreamBufferCreateStatic      stream_buf_create_static
#define xStreamBufferCreate            stream_buf_create
#define xStreamBufferSend              stream_buf_send
#define xStreamBufferReceive           stream_buf_recv
#define xStreamBufferDelete            stream_buf_delete
#define xStreamBufferBytesAvailable    stream_buf_get_used
#define xStreamBufferSpacesAvailable   stream_buf_get_unused
#define xStreamBufferSetTriggerLevel   stream_buf_set_trigger
#define xStreamBufferReset             stream_buf_reset
#define xStreamBufferIsEmpty           stream_buf_is_empty
#define xStreamBufferIsFull            stream_buf_is_full
#define xMessageBufferSend             msg_buf_send
#define xMessageBufferReceive          msg_buf_recv
#define xMessageBufferDelete           msg_buf_delete
#define xMessageBufferSpacesAvailable  msg_buf_get_unused
#define xMessageBufferReset            msg_buf_reset
#define xMessageBufferIsEmpty          msg_buf_is_empty
#define xMessageBufferIsFull           msg_buf_is_full


#endif
