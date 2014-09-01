#ifndef DrawBackground_H
#define DrawBackground_H

#define ROAD_WIDTH	80

/* Definition for fence */
#define FENCE_V_WIDTH 	10
#define FENCE_V_HEIGHT 	\
	((LCD_PIXEL_HEIGHT - ROAD_WIDTH - (FENCE_V_WIDTH << 1)) >> 1)

/* The starting x-axis.	*/
#define FENCE_V_X	\
	((LCD_PIXEL_WIDTH - ROAD_WIDTH - (FENCE_V_WIDTH << 1)) >> 1)

/* The starting y-axis.	*/
#define FENCE_V_Y	0	

/* 
 * The width of the horzontal fence. Note that it is identical to the 
 * starting x-axis of the vertical fence.
 */
#define FENCE_H_WIDTH 	(FENCE_V_X + FENCE_V_WIDTH)

/* 
 * The height of the horzontal fence. Note that it is identical to the 
 * width of the vertical fence.
 */
#define FENCE_H_HEIGHT 	FENCE_V_WIDTH

/* 
 * The starting x-axis of the horzontal fence. Note that it is identical 
 * to the starting y-axis of the vertical fence.
 */
#define FENCE_H_X	FENCE_V_Y

/* 
 * The starting y-axis of the horzontal fence. Note that it is identical 
 * to the x-axis of the vertical fence.
 */
#define FENCE_H_Y	FENCE_V_HEIGHT

#define FENCE_V_X_NEXT \
		(FENCE_V_X + FENCE_V_WIDTH + ROAD_WIDTH)
#define FENCE_V_Y_NEXT \
		(FENCE_V_Y + FENCE_V_HEIGHT + (FENCE_V_WIDTH << 1) + ROAD_WIDTH)

#define FENCE_H_X_NEXT \
		(FENCE_H_X + FENCE_H_WIDTH + ROAD_WIDTH)
#define FENCE_H_Y_NEXT \
		(FENCE_H_Y + FENCE_H_HEIGHT + ROAD_WIDTH)
/* End for the definition of fence. */

/* Defintion for solid double yellow lines. */
#define DLINE_TOTAL_WIDTH	10
#define DLINE_SPACE		8

/* The starting x-axis in ROAD section. It's the relative offset. */
#define DLINE_V_X_ROAD 	((ROAD_WIDTH - DLINE_TOTAL_WIDTH) >> 1)

#define DLINE_V_X 	(FENCE_V_X + FENCE_V_WIDTH + DLINE_V_X_ROAD)
#define DLINE_V_Y	FENCE_V_Y
#define DLINE_V_LEN	FENCE_V_HEIGHT
#define DLINE_V_Y_NEXT	(FENCE_V_HEIGHT + (FENCE_V_WIDTH << 1) + ROAD_WIDTH)

/* The starting y-axis in ROAD section. It's the relative offset. */
#define DLINE_H_Y_ROAD 	DLINE_V_X_ROAD

#define DLINE_H_X	FENCE_V_Y
#define DLINE_H_Y	(FENCE_V_HEIGHT + FENCE_V_WIDTH + DLINE_H_Y_ROAD)
#define DLINE_H_LEN	FENCE_V_X
#define DLINE_H_X_NEXT	(FENCE_V_X + (FENCE_H_HEIGHT << 1) + ROAD_WIDTH)
/* End for the defintion of solid yellow lines. */

#define TRAFFIC_LIGHT_NUM	3
#define TRAFFIC_LIGHT_RADIUS	5

#define LCD_COLOR_DARK_RED 	ASSEMBLE_RGB(0x75, 0, 0)
#define LCD_COLOR_DARK_YELLOW 	ASSEMBLE_RGB(0xA6, 0xA6, 0)
#define LCD_COLOR_DARK_GREEN 	ASSEMBLE_RGB(0, 0xA6, 0)

enum {
	CAR_V_DOWN,
	CAR_V_UP,
	CAR_H_LEFT,
	CAR_H_RIGHT,
	CAR_NUM_DIRECTIONS
};

struct graph_info {
	int16_t	x;
	int16_t	y;

	union {
		uint16_t	width; 	/* width for rectagle.	*/
		uint16_t	len; 	/* length for line.	*/
		uint16_t	radius;
	} wl;

	union {
		uint16_t	height; /* height for rectagle.	*/
		uint8_t		dir; 	/* direction for line.	*/
		uint16_t	color;
	} hd;

};

void DrawBackground(void);
void DrawTrafficLight(int, int);

#endif
