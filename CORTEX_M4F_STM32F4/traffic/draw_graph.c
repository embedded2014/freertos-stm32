#include <stdio.h>

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "draw_graph.h"

/* 8 fences need to be drawn. */
static struct graph_info fence_info [] = {
	{FENCE_V_X, FENCE_V_Y, {FENCE_V_WIDTH}, {FENCE_V_HEIGHT}},
	{FENCE_H_X, FENCE_H_Y, {FENCE_H_WIDTH}, {FENCE_H_HEIGHT}},
	{FENCE_V_X, FENCE_V_Y_NEXT, {FENCE_V_WIDTH}, {FENCE_V_HEIGHT}},
	{FENCE_H_X, FENCE_H_Y_NEXT, {FENCE_H_WIDTH}, {FENCE_H_HEIGHT}},
	{FENCE_V_X_NEXT, FENCE_V_Y, {FENCE_V_WIDTH}, {FENCE_V_HEIGHT}},
	{FENCE_H_X_NEXT, FENCE_H_Y, {FENCE_H_WIDTH}, {FENCE_H_HEIGHT}},
	{FENCE_V_X_NEXT, FENCE_V_Y_NEXT, {FENCE_V_WIDTH}, {FENCE_V_HEIGHT}},
	{FENCE_H_X_NEXT, FENCE_H_Y_NEXT, {FENCE_H_WIDTH}, {FENCE_H_HEIGHT}},
	{0}
};

static struct graph_info dline_info [] = {
	{DLINE_V_X, 			DLINE_V_Y, 		{DLINE_V_LEN},
	 {LCD_DIR_VERTICAL}},
	{DLINE_V_X, 			DLINE_V_Y_NEXT, 	{DLINE_V_LEN},
	 {LCD_DIR_VERTICAL}},
	{DLINE_V_X + DLINE_SPACE, 	DLINE_V_Y, 		{DLINE_V_LEN},
	 {LCD_DIR_VERTICAL}}, 
	{DLINE_V_X + DLINE_SPACE, 	DLINE_V_Y_NEXT, 	{DLINE_V_LEN},
	 {LCD_DIR_VERTICAL}}, 

	{DLINE_H_X, 			DLINE_H_Y, 		{DLINE_H_LEN},
	 {LCD_DIR_HORIZONTAL}},
	{DLINE_H_X, 			DLINE_H_Y + DLINE_SPACE,{DLINE_H_LEN},
	 {LCD_DIR_HORIZONTAL}}, 
	{DLINE_H_X_NEXT, 		DLINE_H_Y, 		{DLINE_H_LEN},
	 {LCD_DIR_HORIZONTAL}}, 
	{DLINE_H_X_NEXT,		DLINE_H_Y + DLINE_SPACE,{DLINE_H_LEN},
	 {LCD_DIR_HORIZONTAL}}, 
	{0}
};

static struct graph_info traffic_light_v_info[] = {
	{60,  220, {TRAFFIC_LIGHT_RADIUS}, {LCD_COLOR_DARK_RED}},
	{60,  230, {TRAFFIC_LIGHT_RADIUS}, {LCD_COLOR_DARK_YELLOW}},
	{60,  240, {TRAFFIC_LIGHT_RADIUS}, {LCD_COLOR_DARK_GREEN}},
	{180, 100, {TRAFFIC_LIGHT_RADIUS}, {LCD_COLOR_DARK_RED}},
	{180,  90, {TRAFFIC_LIGHT_RADIUS}, {LCD_COLOR_DARK_YELLOW}},
	{180,  80, {TRAFFIC_LIGHT_RADIUS}, {LCD_COLOR_DARK_GREEN}},
};

static struct graph_info traffic_light_h_info[] = {
	{60,  100, {TRAFFIC_LIGHT_RADIUS}, {LCD_COLOR_DARK_RED}},
	{50,  100, {TRAFFIC_LIGHT_RADIUS}, {LCD_COLOR_DARK_YELLOW}},
	{40,  100, {TRAFFIC_LIGHT_RADIUS}, {LCD_COLOR_DARK_GREEN}},
	{180, 220, {TRAFFIC_LIGHT_RADIUS}, {LCD_COLOR_DARK_RED}},
	{190, 220, {TRAFFIC_LIGHT_RADIUS}, {LCD_COLOR_DARK_YELLOW}},
	{200, 220, {TRAFFIC_LIGHT_RADIUS}, {LCD_COLOR_DARK_GREEN}},
};

static uint16_t traffic_light_color[TRAFFIC_LIGHT_NUM] = 
			{LCD_COLOR_RED, LCD_COLOR_YELLOW, LCD_COLOR_GREEN};


static void DrawEachTrafficLight(struct graph_info *graph, int index,
					int state)
{
	uint16_t color;

	color = ((index % TRAFFIC_LIGHT_NUM) == state) ?
		traffic_light_color[state] : graph->hd.color;

	LCD_SetTextColor(color);
	LCD_DrawFullCircle(graph->x, graph->y, graph->wl.radius);

}

void DrawTrafficLight(int v_state, int h_state)
{
	int i;
	int elements = sizeof(traffic_light_v_info) / 
				sizeof(traffic_light_v_info[0]);
					
	for (i=0;i<elements;i++) {
		DrawEachTrafficLight(&traffic_light_v_info[i], i, v_state);
		DrawEachTrafficLight(&traffic_light_h_info[i], i, h_state);
	}
	
}

void DrawBackground(void)
{
	struct graph_info *graph;

	LCD_SetTextColor(LCD_COLOR_WHITE);
	for (graph = fence_info; graph->wl.width; graph++) {
		LCD_DrawFullRect(graph->x, graph->y, 
					graph->wl.width, graph->hd.height);
	}

	LCD_SetTextColor(LCD_COLOR_YELLOW);
	for (graph = dline_info; graph->wl.len; graph++) {
		LCD_DrawLine(graph->x, graph->y, graph->wl.len, graph->hd.dir);
	}
}
