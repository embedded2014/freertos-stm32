#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "move_car.h"

static struct car_info car_v_down_list[] = {
	{0, 0, {CAR_V_DOWN_X, 90, {CAR_WIDTH}, {CAR_HEIGHT}}},
	{0, 0, {CAR_V_DOWN_X, 60, {CAR_WIDTH}, {CAR_HEIGHT}}},
	{0, 0, {CAR_V_DOWN_X, 30, {CAR_WIDTH}, {CAR_HEIGHT}}},
	{0, 0, {CAR_V_DOWN_X, 0, {CAR_WIDTH}, {CAR_HEIGHT}}},
	{0}
};

static struct car_info car_v_up_list[] = {
	{0, 0, {CAR_V_UP_X, LCD_PIXEL_HEIGHT - 120, {CAR_WIDTH}, {CAR_HEIGHT}}},
	{0, 0, {CAR_V_UP_X, LCD_PIXEL_HEIGHT - 90, {CAR_WIDTH}, {CAR_HEIGHT}}},
	{0, 0, {CAR_V_UP_X, LCD_PIXEL_HEIGHT - 60, {CAR_WIDTH}, {CAR_HEIGHT}}},
	{0, 0, {CAR_V_UP_X, LCD_PIXEL_HEIGHT - 30, {CAR_WIDTH}, {CAR_HEIGHT}}},
	{0}
};


static struct car_info car_h_left_list[] = {
	{0, 0, {LCD_PIXEL_WIDTH-60, CAR_H_LEFT_Y, {CAR_HEIGHT}, {CAR_WIDTH}}},
	{0, 0, {LCD_PIXEL_WIDTH-30, CAR_H_LEFT_Y, {CAR_HEIGHT}, {CAR_WIDTH}}},
	{0}
};

static struct car_info car_h_right_list[] = {
	{0, 0, {60, CAR_H_RIGHT_Y, {CAR_HEIGHT}, {CAR_WIDTH}}},
	{0, 0, {30, CAR_H_RIGHT_Y, {CAR_HEIGHT}, {CAR_WIDTH}}},
	{0}
};


static struct car_info *cars_all[CAR_NUM_DIRECTIONS] = {
	car_v_down_list, car_v_up_list, car_h_left_list, car_h_right_list
};

static uint16_t color_array[] = {LCD_COLOR_GREY, LCD_COLOR_BLUE, 
				 LCD_COLOR_BLUE2, LCD_COLOR_RED,
			 	 LCD_COLOR_MAGENTA, LCD_COLOR_GREEN,
			         LCD_COLOR_CYAN, LCD_COLOR_YELLOW};

void ReportError(char *str)
{
	sFONT *font;

 	font = LCD_GetFont();

 	LCD_SetBackColor(LCD_COLOR_BLACK);
  	LCD_ClearLine(LCD_PIXEL_HEIGHT - font->Height);

 	LCD_SetBackColor(LCD_COLOR_BLUE);

  	LCD_DisplayStringLine(LCD_PIXEL_HEIGHT - font->Height, 
				(uint8_t *) str);

}

static uint16_t CarGetColor(void)
{
	uint16_t color_index;
	
	/* Polling. */
	while (RNG_GetFlagStatus(RNG_FLAG_DRDY) != SET)
		;

	color_index = RNG_GetRandomNumber() % 
			(sizeof(color_array) / sizeof(color_array[0]));

	return color_array[color_index]; 
}


/**
  * @brief  Check the cars of the opposite direction (red light). If those 
  * 	    cars drive through a red ligt, they must keep driving while cars
  *	    that see a green light have to wait.
  * @retval 0: keep moving, 1: stop moving
  */
static int CheckOppositeDirCars(struct graph_info *g_ptr, int dir, 
					int distance, unsigned int limit) 
{
	struct car_info *c_ptr;
	struct graph_info *g_tmp;
	int val, tmp_val;

	/* Check if it is the vertical or horizontal direction. */
	val = (dir <= CAR_V_UP ? g_ptr->y : g_ptr->x);

	/* This car is behind a red light, so stop moving. */
	if (val + distance == limit) 
		return 1;

	/* This for-loop scan if cars are overlapping. */
	for (c_ptr=cars_all[dir];c_ptr->car_graph.wl.width;c_ptr++) {
		g_tmp = &c_ptr->car_graph;

		if (g_ptr == g_tmp)
			continue;

		tmp_val = (dir <= CAR_V_UP ? g_tmp->y : g_tmp->x);

		/* Prevent from the car overlapping. */
		if (distance > 0) {
			if (tmp_val == (val + (distance +  CAR_HEIGHT)))
				return 1;
		} else {
			if (tmp_val == (val - (abs(distance) + CAR_HEIGHT)))
				return 1;
		}
	}

	return 0;
}

static void UpdateXY(struct car_info *c_ptr, int dir, int distance, 
							int opposite)
{
	struct graph_info *g_ptr = &c_ptr->car_graph;

	switch (dir) {
	case CAR_V_DOWN:
		if (opposite && CheckOppositeDirCars(g_ptr, dir, distance, 
							FENCE_V_HEIGHT))
			break;

		g_ptr->y += distance;

		if (g_ptr->y >= LCD_PIXEL_HEIGHT) {
			g_ptr->y = 0;
			c_ptr->color = 0;
		}

		break;
	case CAR_V_UP:
		if (opposite && CheckOppositeDirCars(g_ptr, dir, -distance, 
							FENCE_V_Y_NEXT))
			break;

		g_ptr->y -= distance;

		if (g_ptr->y <= 0) {
			g_ptr->y = LCD_PIXEL_HEIGHT;
			c_ptr->color = 0;
		}

		break;
	case CAR_H_LEFT:
		if (opposite && CheckOppositeDirCars(g_ptr, dir, -distance, 
							FENCE_H_X_NEXT))
			break;

		g_ptr->x -= distance;
		if (g_ptr->x < 0) {
			/* 
			 * If we want to draw a rectangle whose (x, y) is   
			 * (230, 100) with witdth '20', the rectangle will  
			 * be drawn between (230, 100)-(10, 100. So, we need
			 * to substract the width with the maximum with when
			 * resetting the x-axis value.
			 */
			g_ptr->x = (LCD_PIXEL_WIDTH - g_ptr->wl.width);
			c_ptr->color = 0;
		}

		break;
	case CAR_H_RIGHT:
		if (opposite && CheckOppositeDirCars(g_ptr, dir, distance, 
							FENCE_V_X))
			break;

		g_ptr->x += distance;

		if (g_ptr->x > (LCD_PIXEL_WIDTH - g_ptr->wl.width)) {
			g_ptr->x = 0;
			c_ptr->color = 0;
		}

		break;
	default:
		break;

	}

	return;
}

static int CheckFallInZone(int x, int y)
{
	if (x > FENCE_H_WIDTH && x < FENCE_H_X_NEXT && 
			y > (FENCE_H_Y + FENCE_V_WIDTH) && y < FENCE_H_Y_NEXT)
		return 1;

	return 0;
}

static int CheckOppositeDirection(int dir, int v_state, int h_state) 
{
	if (v_state == TRAFFIC_RED && (dir == CAR_H_LEFT || dir == CAR_H_RIGHT))
		return 1;

	if (h_state == TRAFFIC_RED && (dir == CAR_V_DOWN || dir == CAR_V_UP))
		return 1;

	return 0;
}

static int CarsDriveRed(int dir, int v_state, int h_state)
{
	struct car_info *c_ptr;
	struct graph_info *g_ptr;

	int i;
	int start_index = 0, stop_index = 0;

	if (h_state == TRAFFIC_GREEN && 
				(dir == CAR_V_DOWN || dir == CAR_V_UP)) {
		start_index = CAR_H_LEFT;
		stop_index = CAR_H_RIGHT;
	}

	if (v_state == TRAFFIC_GREEN && 
				(dir == CAR_H_LEFT || dir == CAR_H_RIGHT)) {
		start_index = CAR_V_DOWN;
		stop_index = CAR_V_UP;
	}

	if (!stop_index)
		return 0;

	for (i=start_index;i<=stop_index;i++) {
		for (c_ptr=cars_all[i];c_ptr->car_graph.wl.width;c_ptr++) {
			g_ptr = &c_ptr->car_graph;
			if (CheckFallInZone(g_ptr->x, g_ptr->y)) 
				return 1;
		}	
	}

	return 0;
}

void MoveCar(int traffic_v_state, int traffic_h_state)
{
	struct car_info *c_ptr;
	struct graph_info *g_ptr;

	int i; 
	int opposite;

	DrawTrafficLight(traffic_v_state, traffic_h_state);

	for (i=0;i<CAR_NUM_DIRECTIONS;i++) {
		/* 
		 * If those cars drive throgh a red light, let them keep 
		 * moving on. To achive this, those cars that see a green
		 * light has to wait. That's why the 'continue' instruction
		 * is used to stop moving the cars.  
		 */
		if (CarsDriveRed(i, traffic_v_state, traffic_h_state))
			continue;

		opposite = CheckOppositeDirection(i, traffic_v_state, 
							traffic_h_state);

		for (c_ptr=cars_all[i];c_ptr->car_graph.wl.width;c_ptr++) {
			g_ptr = &c_ptr->car_graph;

			/* Clear the previous car */
			LCD_SetTextColor(LCD_COLOR_BLACK);
			LCD_DrawFullRect(g_ptr->x, g_ptr->y, g_ptr->wl.width, 
					 g_ptr->hd.height);


			UpdateXY(c_ptr, i, CAR_STEP, opposite);

			if (!c_ptr->color)
				c_ptr->color = CarGetColor();

			/* Draw each car with a new postion. */
			LCD_SetTextColor(c_ptr->color);
			LCD_DrawFullRect(g_ptr->x, g_ptr->y, g_ptr->wl.width, 
					 g_ptr->hd.height);

		}
	}
}
