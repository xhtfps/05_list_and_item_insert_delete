/**
 ****************************************************************************************************
 * @file        freertos.c
 * @author      ALIENTEK
 * @version     V1.4
 * @date        2022-01-04
 * @brief       FreeRTOS 任务实践：链表的插入与删除
 * @license     Copyright (c) 2020-2032,  ALIENTEK
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32F407 开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 淘宝地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "freertos_demo.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"

/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "list.h"   /* 必须加上，解决链表函数报错 */

/******************************************************************************************************/
/*FreeRTOS配置*/

/* START_TASK 任务配置
 * 优先级: 1 堆栈大小:128 任务句柄:start_task_handler
 */
#define START_TASK_PRIO			1 
#define START_TASK_STACK_SIZE 	128
TaskHandle_t	start_task_handler;
void start_task(void * pvParameters);

/* TASK1 任务配置
 * 优先级: 2 堆栈大小:128 任务句柄:task1_handler
 */
#define TASK1_PRIO			2 
#define TASK1_STACK_SIZE 	128
TaskHandle_t	task1_handler;
void task1(void * pvParameters);

/* TASK2 任务配置
 * 优先级: 3 堆栈大小:512 任务句柄:task2_handler
 */
#define TASK2_PRIO			3 
#define TASK2_STACK_SIZE 	512
TaskHandle_t	task2_handler;
void task2(void * pvParameters);
/******************************************************************************************************/

List_t 			TestList;		/* 定义链表1 */
ListItem_t 		ListItem1;		/* 定义链表项1 */
ListItem_t 		ListItem2;		/* 定义链表项2 */
ListItem_t 		ListItem3;		/* 定义链表项3 */

/**
 * @brief       FreeRTOS 任务创建
 * @param       无
 * @retval      无
 */
void freertos_demo(void)
{
	xTaskCreate ((TaskFunction_t		) start_task,
				(char *					) "start_task",
				(configSTACK_DEPTH_TYPE	) START_TASK_STACK_SIZE,
				(void * 				) NULL,
				(UBaseType_t			) START_TASK_PRIO,
				(TaskHandle_t *			) &start_task_handler );
	vTaskStartScheduler();		
}

void start_task(void * pvParameters)
{
	taskENTER_CRITICAL();		/* 进入临界区 */
	xTaskCreate ((TaskFunction_t		) task1,
				(char *					) "task1",
				(configSTACK_DEPTH_TYPE	) TASK1_STACK_SIZE,
				(void * 				) NULL,
				(UBaseType_t			) TASK1_PRIO,
				(TaskHandle_t *			) &task1_handler );
				
	xTaskCreate ((TaskFunction_t		) task2,
				(char *					) "task2",
				(configSTACK_DEPTH_TYPE	) TASK2_STACK_SIZE,
				(void * 				) NULL,
				(UBaseType_t			) TASK2_PRIO,
				(TaskHandle_t *			) &task2_handler );
				
	vTaskDelete(NULL);
	taskEXIT_CRITICAL();		/* 退出临界区 */
}

/* 任务1:LED0 每500ms翻转一次 */
void task1(void * pvParameters)
{
	while(1)
	{
		LED0_TOGGLE();
		vTaskDelay(500);
	}
}

/* 任务2:链表操作演示 */
void task2(void * pvParameters)
{	
	/* 第一步：初始化链表与链表项 */
	vListInitialise(&TestList);			/* 初始化链表 */
	vListInitialiseItem(&ListItem1);	/* 初始化链表项1 */
	vListInitialiseItem(&ListItem2);	/* 初始化链表项2 */
	vListInitialiseItem(&ListItem3);	/* 初始化链表项3 */
	ListItem1.xItemValue = 40;			/* 设置链表项1值为40 */
	ListItem2.xItemValue = 60;			/* 设置链表项2值为60 */
	ListItem3.xItemValue = 50;			/* 设置链表项3值为50 */

	/* 第二步：打印链表与链表项初始地址 */
	printf("**************实验1:链表与链表项初始地址**************\r\n");
	printf("变量名\t\t\t地址\r\n");
	printf("TestList\t\t0x%p\t\r\n", &TestList);
	printf("TestList->pxIndex\t0x%p\t\r\n", TestList.pxIndex);
	printf("TestList->xListEnd\t0x%p\t\r\n", (&TestList.xListEnd));
	printf("ListItem1\t\t0x%p\t\r\n", &ListItem1);
	printf("ListItem2\t\t0x%p\t\r\n", &ListItem2);
	printf("ListItem3\t\t0x%p\t\r\n", &ListItem3);
	printf("************************结束************************\r\n");

	/* 第三步：插入链表项1 */
	printf("\r\n/*********************实验2:插入链表项1***********************/\r\n");
	vListInsert((List_t*    )&TestList,
				(ListItem_t*)&ListItem1);
	printf("变量名\t\t\t地址\r\n");
	printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
	printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
	printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
	printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
	printf("/***************************结束*******************************/\r\n");

	/* 第四步：插入链表项2 */
	printf("\r\n/*********************实验3:插入链表项2***********************/\r\n");
	vListInsert((List_t*    )&TestList,
				(ListItem_t*)&ListItem2);
	printf("变量名\t\t\t地址\r\n");
	printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
	printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
	printf("ListItem2->pxNext\t\t0x%p\r\n", (ListItem2.pxNext));
	printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
	printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
	printf("ListItem2->pxPrevious\t\t0x%p\r\n", (ListItem2.pxPrevious));
	printf("/***************************结束*******************************/\r\n");

	/* 第五步：插入链表项3 */
	printf("\r\n/*********************实验4:插入链表项3***********************/\r\n");
	vListInsert((List_t*    )&TestList,
				(ListItem_t*)&ListItem3);
	printf("变量名\t\t\t地址\r\n");
	printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
	printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
	printf("ListItem2->pxNext\t\t0x%p\r\n", (ListItem2.pxNext));
	printf("ListItem3->pxNext\t\t0x%p\r\n", (ListItem3.pxNext));
	printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
	printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
	printf("ListItem2->pxPrevious\t\t0x%p\r\n", (ListItem2.pxPrevious));
	printf("ListItem3->pxPrevious\t\t0x%p\r\n", (ListItem3.pxPrevious));
	printf("/***************************结束*******************************/\r\n");

	/* 第六步：删除链表项2 */
	printf("\r\n/*********************实验5:删除链表项2***********************/\r\n");
	uxListRemove(&ListItem2); 		/* 删除链表项2 */
	printf("变量名\t\t\t地址\r\n");
	printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
	printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
	printf("ListItem3->pxNext\t\t0x%p\r\n", (ListItem3.pxNext));
	printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
	printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
	printf("ListItem3->pxPrevious\t\t0x%p\r\n", (ListItem3.pxPrevious));
	printf("/***************************结束*******************************/\r\n");

	/* 第七步： 列表末尾添加列表项2*/
	printf("\r\n/*********************实验6:列表末尾添加列表项2***********************/\r\n");
	vListInsertEnd((List_t*    )&TestList,
				(ListItem_t*)&ListItem2);
	printf("变量名\t\t\t地址\r\n");
	printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
	printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
	printf("ListItem2->pxNext\t\t0x%p\r\n", (ListItem2.pxNext));
	printf("ListItem3->pxNext\t\t0x%p\r\n", (ListItem3.pxNext));
	printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
	printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
	printf("ListItem2->pxPrevious\t\t0x%p\r\n", (ListItem2.pxPrevious));
	printf("ListItem3->pxPrevious\t\t0x%p\r\n", (ListItem3.pxPrevious));
	printf("/***************************结束*******************************/\r\n");

	uxListRemove(&ListItem2); 		/* 删除链表项2 */
	printf(\r\n删除链表项2后:\r\n);

	/* 第八步：更改列表指针*/
	printf("\r\n/*********************实验7:更改列表指针***********************/\r\n");
	TestList.pxIndex = ListItem1.pxNext;	/* 将列表指针指向链表项1的下一个链表项 */
	vListInsertEnd((List_t*    )&TestList,	/* 列表末尾添加列表项2*/
			(ListItem_t*)&ListItem2);
	printf("变量名\t\t\t地址\r\n");
	printf("TestList->xListEnd->pxNext\t0x%p\r\n", (TestList.xListEnd.pxNext));
	printf("ListItem1->pxNext\t\t0x%p\r\n", (ListItem1.pxNext));
	printf("ListItem2->pxNext\t\t0x%p\r\n", (ListItem2.pxNext));
	printf("ListItem3->pxNext\t\t0x%p\r\n", (ListItem3.pxNext));
	printf("TestList->xListEnd->pxPrevious\t0x%p\r\n", (TestList.xListEnd.pxPrevious));
	printf("ListItem1->pxPrevious\t\t0x%p\r\n", (ListItem1.pxPrevious));
	printf("ListItem2->pxPrevious\t\t0x%p\r\n", (ListItem2.pxPrevious));
	printf("ListItem3->pxPrevious\t\t0x%p\r\n", (ListItem3.pxPrevious));
	printf("/***************************结束*******************************/\r\n");

	while(1)
	{
		vTaskDelay(1000);
	}


}
