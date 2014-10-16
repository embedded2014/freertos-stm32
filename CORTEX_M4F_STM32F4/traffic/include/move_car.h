#ifndef MoveCar_H
#define MoveCar_H
#include "draw_graph.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define CAR_WIDTH	10
#define CAR_HEIGHT	20
#define CAR_STEP	10

#define CAR_FENCE_DISTANCE	10	

#define CAR_V_DOWN_X 	(FENCE_V_X + FENCE_V_WIDTH + CAR_FENCE_DISTANCE)
#define CAR_V_UP_X 	(DLINE_V_X + DLINE_TOTAL_WIDTH + CAR_FENCE_DISTANCE)

#define CAR_H_LEFT_Y 	(FENCE_H_Y + FENCE_H_HEIGHT + CAR_FENCE_DISTANCE)
#define CAR_H_RIGHT_Y 	(DLINE_H_Y + DLINE_TOTAL_WIDTH + CAR_FENCE_DISTANCE)

#define CAR_IN_RANGE(x, y) \
	x >= 0 && x < LCD_PIXEL_WIDTH && y >= 0 && y <= LCD_PIXEL_HEIGHT

#define TRAFFIC_GREEN_TICK  4000 / portTICK_RATE_MS /* 4 seconds. */
#define TRAFFIC_YELLOW_TICK 800  / portTICK_RATE_MS /* 800 ms. */
#define TRAFFIC_TICK_SLICE  100  / portTICK_RATE_MS /* 400 ms. */

enum {
	TRAFFIC_RED,
	TRAFFIC_YELLOW,
	TRAFFIC_GREEN,
	TRAFFIC_COUNT
};

struct car_info {
	uint16_t color;
	uint16_t reserved;	
	struct graph_info car_graph;
};


void MoveCar(int, int);
void ReportError(char *);

#endif
