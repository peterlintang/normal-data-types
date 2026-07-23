
/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#ifndef _DEFAULT_SOURCE
  #define _DEFAULT_SOURCE /* needed for usleep() */
#endif

#include <stdlib.h>
#include <stdio.h>
#ifdef _MSC_VER
  #include <Windows.h>
#else
  #include <unistd.h>
  #include <pthread.h>
#endif
#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"
#include <SDL.h>

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static lv_display_t * hal_init(int32_t w, int32_t h);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

extern void freertos_main(void);

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *      VARIABLES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

#include "course_info.h"

static struct gps_point ref_min = { 361.0, 361.0 };
static struct gps_point ref_max = { -361.0, -361.0 };
static lv_obj_t *obj = NULL;
static lv_obj_t *canvas = NULL;

#define MY_WIDTH	800
#define MY_HEIGHT	800

static int to_x(double x)
{
#if 0
	double ref = ref_max.x - ref_min.x > ref_max.y - ref_min.y ? ref_max.x - ref_min.x : ref_max.y - ref_min.y;
//	double ref = ref_max.x - ref_min.x;
	 int a = (x - (ref_min.x )) / ref * MY_WIDTH;
	 if (a <= 0 || a >= MY_WIDTH)
	 {
		 printf("\n\nERROR: x: %.8f, ref_min.x: %.8f, ref: %.8f, a: %d, width: %d\n\n", 
				 x, ref_min.x, ref, a, MY_WIDTH);
	 }
	return (x - (ref_min.x )) / ref * MY_WIDTH;
#else
	double ref = ref_max.x - ref_min.x > ref_max.y - ref_min.y ? ref_max.x - ref_min.x : ref_max.y - ref_min.y;

	double x_min = 0;
	double x_max = (ref_max.x - ref_min.x) / ref * MY_WIDTH;

	int aa = (int)(((x - ref_min.x) / ref * MY_WIDTH) - ((x_min + x_max) / 2 - MY_WIDTH / 2));
	if (aa <= 0 || aa >= MY_WIDTH)
	{
		printf("X VERY ERROR\n\n");
	}
	return ((x - ref_min.x) / ref * MY_WIDTH) - ((x_min + x_max) / 2 - MY_WIDTH / 2);
#endif
}

static int to_y(double y)
{
#if 0
	double ref = ref_max.x - ref_min.x > ref_max.y - ref_min.y ? ref_max.x - ref_min.x : ref_max.y - ref_min.y;
//	double ref = ref_max.y - ref_min.y;
	int a = MY_HEIGHT - (y - (ref_min.y )) / ref * MY_HEIGHT;
	 if (a <= 0 || a >= MY_HEIGHT)
	 {
		 printf("\n\nERROR: y: %.8f, ref_min.y: %.8f, ref: %.8f, a: %d, width: %d\n\n", 
				 y, ref_min.y, ref, a, MY_HEIGHT);
	 }
	return MY_HEIGHT - (y - (ref_min.y )) / ref * MY_HEIGHT;
#else
	double ref = ref_max.x - ref_min.x > ref_max.y - ref_min.y ? ref_max.x - ref_min.x : ref_max.y - ref_min.y;

	double y_min = 0;
	double y_max = (ref_max.y - ref_min.y) / ref * MY_HEIGHT;

	int aa= (int)(((y - ref_min.y) / ref * MY_HEIGHT) - ((y_min + y_max) / 2 - MY_HEIGHT / 2));
	if (aa <= 0 || aa >= MY_HEIGHT)
	{
		printf("Y VERY ERROR\n\n");
	}
	return ((y - ref_min.y) / ref * MY_HEIGHT) - ((y_min + y_max) / 2 - MY_HEIGHT / 2);
#endif
}

static void lvgl_draw_pl(char *pl_type, struct gps_point *point)
{
//	printf("%s: begin\n", __func__);
    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);

    lv_draw_arc_dsc_t dsc;
    lv_draw_arc_dsc_init(&dsc);
    if (strncmp("t", pl_type, 1) == 0)
    	dsc.color = lv_palette_main(LV_PALETTE_RED);
    else if (strncmp("f", pl_type, 1) == 0)
    	dsc.color = lv_palette_main(LV_PALETTE_BLUE);
    else if (strncmp("r", pl_type, 1) == 0)
    	dsc.color = lv_palette_main(LV_PALETTE_GREEN);
    else
    	dsc.color = lv_palette_main(LV_PALETTE_YELLOW);
    dsc.width = 1;
    dsc.center.x = to_x(point->x);
    dsc.center.y = to_y(point->y);
    dsc.width = 1;
    dsc.radius = 1;
    dsc.start_angle = 0;
    dsc.end_angle = 360;
//    printf("%s: x: %d, y: %d\n", __func__, to_x(point->x), to_y(point->y));

    lv_draw_arc(&layer, &dsc);

    lv_canvas_finish_layer(canvas, &layer);
//	printf("%s: done\n", __func__);
}

static void draw_pl(char *course, int h_id, int pl_id)
{
    char pl_type[8] = { 0 };
    int pl_type_len = 8;
    struct gps_point point = { 0.0, 0.0 };

    course_get_pl_n_by_index(course, h_id, pl_id, pl_type, &pl_type_len);
    course_get_pl_pt_by_index(course, h_id, pl_id, &point);
    printf("type: %s, %.8f %.8f (%d %d)\n", pl_type, point.y, point.x, to_y(point.y), to_x(point.x));

    lvgl_draw_pl(pl_type, &point);
}

static void draw_pls(char *course, int h_id)
{
    int pls_num = 0;
    course_get_hole_plNum_by_index(course, h_id, &pls_num);

    for (int i = 0; i < pls_num; i++)
    {
	draw_pl(course, h_id, i);
    }
}

static void lvgl_draw_green(struct gps_point *points, int points_num)
{
//	printf("%s: begin\n", __func__);
    for (int i = 0; i < points_num; i++)
    {
	    lv_layer_t layer;
	    lv_canvas_init_layer(canvas, &layer);

	    lv_draw_line_dsc_t dsc;
	    lv_draw_line_dsc_init(&dsc);
	    dsc.color = lv_palette_main(LV_PALETTE_GREEN);
	    dsc.width = 1;
	    dsc.round_end = 1;
	    dsc.round_start = 1;
	    dsc.p1.x = to_x(points[i % points_num].x);
	    dsc.p1.y = to_y(points[i % points_num].y);
	    dsc.p2.x = to_x(points[(i + 1) % points_num].x);
	    dsc.p2.y = to_y(points[(i + 1) % points_num].y);
	    lv_draw_line(&layer, &dsc);
	    //printf("%s: x: %f y: %f, x: %f y: %f\n", __func__, dsc.p1.x, dsc.p1.y, dsc.p2.x, dsc.p2.y);

	    lv_canvas_finish_layer(canvas, &layer);

    }
//	printf("%s: done\n", __func__);
}

static void draw_green(char *course, int h_id, int g_id)
{
    int points_num = 0;
    struct gps_point *points = NULL;

    course_get_gPts_arrayItemNum_by_index(course, h_id, g_id, &points_num);

    points = (struct gps_point *)calloc(points_num, sizeof(struct gps_point));

    course_get_gPts_arrayItemInfo_by_index(course, h_id, g_id, points, points_num); 

    lvgl_draw_green(points, points_num);

    free(points);
}

static void draw_greens(char *course, int h_id)
{
	int greens_num = 0;
	course_get_hole_gPtsNum_by_index(course, h_id, &greens_num);

	for (int i = 0; i < greens_num; i++)
	{
	    draw_green(course, h_id, i);
	}
}

static void lvgl_draw_teebox(struct gps_point pts[2])
{
#if 0
//	printf("%s: begin\n", __func__);
	lv_layer_t layer;
	lv_canvas_init_layer(canvas, &layer);

	lv_draw_rect_dsc_t dsc;
	lv_draw_rect_dsc_init(&dsc);
	dsc.bg_color = lv_palette_main(LV_PALETTE_GREEN);
	dsc.border_color = lv_palette_main(LV_PALETTE_GREEN);
	dsc.border_width = 1;
	dsc.outline_color = lv_palette_main(LV_PALETTE_GREEN);
        dsc.outline_width = 1;
        dsc.outline_pad = 1;
        dsc.outline_opa = LV_OPA_50;
    	dsc.radius = 1;
    	dsc.border_width = 1;



	int x_min = to_x(pts[0].x) > to_x(pts[1].x) ? to_x(pts[1].x) : to_x(pts[0].x);
	int x_max = to_x(pts[0].x) > to_x(pts[1].x) ? to_x(pts[0].x) : to_x(pts[1].x);
	int y_min = to_y(pts[0].y) > to_y(pts[1].y) ? to_y(pts[1].y) : to_y(pts[0].y);
	int y_max = to_y(pts[0].y) > to_y(pts[1].y) ? to_y(pts[0].y) : to_y(pts[1].y);
	lv_area_t coords = {x_min, y_min, x_max, y_max};
	//printf("%s: x_min: %d, x_max: %d, y_min: %d, y_max: %d\n", __func__, x_min, x_max, y_min, y_max);

	lv_draw_rect(&layer, &dsc, &coords);

	lv_canvas_finish_layer(canvas, &layer);
//	printf("%s: done\n", __func__);
#else
//	printf("%s: begin\n", __func__);
	int x_min = to_x(pts[0].x) > to_x(pts[1].x) ? to_x(pts[1].x) : to_x(pts[0].x);
	int x_max = to_x(pts[0].x) > to_x(pts[1].x) ? to_x(pts[0].x) : to_x(pts[1].x);
	int y_min = to_y(pts[0].y) > to_y(pts[1].y) ? to_y(pts[1].y) : to_y(pts[0].y);
	int y_max = to_y(pts[0].y) > to_y(pts[1].y) ? to_y(pts[0].y) : to_y(pts[1].y);

	int x[4] = { x_min, x_max, x_max, x_min };
	int y[4] = { y_min, y_min, y_max, y_max };

	for (int i = 0; i < 4; i++)
	{
	    lv_layer_t layer;
	    lv_canvas_init_layer(canvas, &layer);

	    lv_draw_line_dsc_t dsc;
	    lv_draw_line_dsc_init(&dsc);
	    dsc.color = lv_palette_main(LV_PALETTE_GREEN);
	    dsc.width = 1;
	    dsc.round_end = 1;
	    dsc.round_start = 1;
	    dsc.p1.x = x[i];
	    dsc.p1.y = y[i];
	    dsc.p2.x = x[(i + 1) % 4];
	    dsc.p2.y = y[(i + 1) % 4];
	    lv_draw_line(&layer, &dsc);
	    lv_canvas_finish_layer(canvas, &layer);
	}
//	printf("%s: done\n", __func__);
#endif
}

static void draw_teebox(char *course, int h_id, int t_id)
{
	struct gps_point pts[2] = { 0.0, 0.0 };

	course_get_tPts_arrayItemInfo_by_index(course, h_id, t_id, pts);

	lvgl_draw_teebox(pts);
}

static void draw_teeboxes(char *course, int h_id)
{
	int t_num = 0;
	course_get_hole_tPtsNum_by_index(course, h_id, &t_num);

	for (int i = 0; i < t_num; i++)
	{
		draw_teebox(course, h_id, i);
	}
}

static void find_min_max_points(char *course, int h_id)
{
    struct gps_point point =  { 0.0, 0.0 };

    int t_num = 0;
    course_get_hole_tPtsNum_by_index(course, h_id, &t_num);

    for (int i = 0; i < t_num; i++)
    {
	struct gps_point points[2] = { 0.0, 0.0 };

	course_get_tPts_arrayItemInfo_by_index(course, h_id, i, points);
	for (int j = 0; j < 2; j++)
	{
		if (points[j].x < ref_min.x)
			ref_min.x = points[j].x;
		if (points[j].y < ref_min.y)
			ref_min.y = points[j].y;
		if (points[j].x > ref_max.x)
			ref_max.x = points[j].x;
		if (points[j].y > ref_max.y)
			ref_max.y = points[j].y;
	}
    }

    int pls_num = 0;
    course_get_hole_plNum_by_index(course, h_id, &pls_num);
    //printf("pls_num: %d\n", pls_num);

    for (int i = 0; i < pls_num; i++)
    {
        course_get_pl_pt_by_index(course, h_id, i, &point);
	if (point.x < ref_min.x)
		ref_min.x = point.x;
	if (point.y < ref_min.y)
		ref_min.y = point.y;
	if (point.x > ref_max.x)
		ref_max.x = point.x;
	if (point.y > ref_max.y)
		ref_max.y = point.y;
    }

    int greens_num = 0;
    course_get_hole_gPtsNum_by_index(course, h_id, &greens_num);

    for (int i = 0; i < greens_num; i++)
    {
        int points_num = 0;
        struct gps_point *points = NULL;

        course_get_gPts_arrayItemNum_by_index(course, h_id, i, &points_num);

        points = (struct gps_point *)calloc(points_num, sizeof(struct gps_point));
        course_get_gPts_arrayItemInfo_by_index(course, h_id, i, points, points_num); 

	for (int j = 0; j < points_num; j++)
	{
		if (points[j].x < ref_min.x)
			ref_min.x = points[j].x;
		if (points[j].y < ref_min.y)
			ref_min.y = points[j].y;
		if (points[j].x > ref_max.x)
			ref_max.x = points[j].x;
		if (points[j].y > ref_max.y)
			ref_max.y = points[j].y;
	}

	free(points);
    }

    
    
}

static void draw_hole(char *course, int h_id)
{
    draw_teeboxes(course, h_id);
    draw_pls(course, h_id);
    draw_greens(course, h_id);
}

static void draw_course(char *course)
{
    int hls_num = 0;
    course_get_hlsNum(course, &hls_num);

    for (int i = 0; i < hls_num; i++)
    {
	printf("drawing hole: %d\n", i);
	draw_hole(course, i);
    }
}

static void init_canvas(void)
{
    obj = lv_obj_create(lv_screen_active());
    lv_obj_set_size(obj, MY_WIDTH, MY_HEIGHT);

    LV_DRAW_BUF_DEFINE_STATIC(draw_buf, MY_WIDTH, MY_HEIGHT, LV_COLOR_FORMAT_ARGB8888);
    LV_DRAW_BUF_INIT_STATIC(draw_buf);

    canvas = lv_canvas_create(obj);
    lv_canvas_set_draw_buf(canvas, &draw_buf);
    lv_obj_center(canvas);
    lv_canvas_fill_bg(canvas, lv_palette_main(LV_PALETTE_NONE), LV_OPA_COVER);
//    lv_canvas_fill_bg(canvas, lv_color_white(), LV_OPA_COVER);
}

static void set_ref(char *course, int h_id)
{
    int hls_num = 0;
    course_get_hlsNum(course, &hls_num);

    if (h_id == -1)
    {
        for (int i = 0; i < hls_num; i++)
    	    find_min_max_points(course, i);
    }
    else
    	find_min_max_points(course, h_id);

    
    ref_min.x -= 0.0001;
    ref_min.y -= 0.0001;
    ref_max.x += 0.0001;
    ref_max.y += 0.0001;
    

    printf("ref x: %.8f, %.8f, %.8f\n", ref_min.x, ref_max.x, ref_max.x - ref_min.x);
    printf("ref y: %.8f, %.8f, %.8f\n", ref_min.y, ref_max.y, ref_max.y - ref_min.y);
}

int main(int argc, char **argv)
{
  char *course = NULL;
  int hole_id = 0;

  if (argc != 3)
  {
	  return -1;
  }
  course = argv[1];
  hole_id = atoi(argv[2]);
  printf("course: %s, hole id: %d\n", course, hole_id);

  /*Initialize LVGL*/
  lv_init();

  /*Initialize the HAL (display, input devices, tick) for LVGL*/
  hal_init(MY_WIDTH, MY_HEIGHT);

//lv_example_canvas_8();

  init_canvas();
  
  set_ref(course, -1);
  draw_course(course);
  
  
  /*
  set_ref(course, hole_id);
  draw_hole(course, hole_id);
  */
  
  #if LV_USE_OS == LV_OS_NONE

  /* Run the default demo */
  /* To try a different demo or example, replace this with one of: */
  /* - lv_demo_benchmark(); */
  /* - lv_demo_stress(); */
  /* - lv_example_label_1(); */
  /*
lv_example_animimg_1();
lv_example_arc_1();
lv_example_arc_2();
lv_example_arc_3();
*/
  /*
    lv_obj_t * btn;
    lv_obj_t * label;

    btn = lv_button_create(lv_screen_active());
    lv_obj_align(btn, LV_ALIGN_TOP_MID, 0, 20);

    label = lv_label_create(btn);
    lv_label_set_text(label, "Original theme");
    */
//lv_example_style_14();
/*
lv_example_arclabel_1();
lv_example_bar_1();
lv_example_bar_2();
lv_example_bar_3();
lv_example_bar_4();
lv_example_bar_5();
lv_example_bar_6();
lv_example_bar_7();
lv_example_button_1();
lv_example_button_2();
lv_example_button_3();

lv_example_buttonmatrix_1();
lv_example_buttonmatrix_2();
lv_example_buttonmatrix_3();

lv_example_calendar_1();
lv_example_calendar_2();

lv_example_canvas_1();
lv_example_canvas_2();
lv_example_canvas_3();
lv_example_canvas_4();
lv_example_canvas_5();
lv_example_canvas_6();
lv_example_canvas_7();
lv_example_canvas_8();
lv_example_canvas_9();
lv_example_canvas_10();
lv_example_canvas_11();

lv_example_chart_1();
lv_example_chart_2();
lv_example_chart_3();
lv_example_chart_4();
lv_example_chart_5();
lv_example_chart_6();
lv_example_chart_7();
lv_example_chart_8();

lv_example_checkbox_1();
lv_example_checkbox_2();

lv_example_dropdown_1();
lv_example_dropdown_2();
lv_example_dropdown_3();

lv_example_image_1();
lv_example_image_2();
lv_example_image_3();
lv_example_image_4();
lv_example_image_5();

lv_example_imagebutton_1();

lv_example_keyboard_1();
lv_example_keyboard_2();
lv_example_keyboard_3();

lv_example_label_1();
lv_example_label_2();
lv_example_label_3();
lv_example_label_4();
lv_example_label_5();
lv_example_label_6();

lv_example_led_1();

lv_example_line_1();

lv_example_list_1();
lv_example_list_2();

lv_example_lottie_1();
lv_example_lottie_2();

lv_example_menu_1();
lv_example_menu_2();
lv_example_menu_3();
lv_example_menu_4();
lv_example_menu_5();

lv_example_msgbox_1();
lv_example_msgbox_2();

lv_example_obj_1();
lv_example_obj_2();
lv_example_obj_3();

lv_example_roller_1();
lv_example_roller_2();
lv_example_roller_3();

lv_example_scale_1();
lv_example_scale_2();
lv_example_scale_3();
lv_example_scale_4();
lv_example_scale_5();
lv_example_scale_6();
lv_example_scale_7();
lv_example_scale_8();
lv_example_scale_9();
lv_example_scale_10();
lv_example_scale_11();
lv_example_scale_12();

lv_example_slider_1();
lv_example_slider_2();
lv_example_slider_3();
lv_example_slider_4();

lv_example_span_1();

lv_example_spinbox_1();

lv_example_spinner_1();

lv_example_switch_1();
lv_example_switch_2();

lv_example_table_1();
lv_example_table_2();

lv_example_tabview_1();
lv_example_tabview_2();

lv_example_textarea_1();
lv_example_textarea_2();
lv_example_textarea_3();
lv_example_textarea_4();
lv_example_tileview_1();
lv_example_win_1();
*/

//    lv_example_arc_3();
  /* - etc. */
  /*lv_demo_widgets();*/

  while(1) {
    /* Periodically call the lv_task handler.
     * It could be done in a timer interrupt or an OS task too.*/
    lv_timer_handler();
#ifdef _MSC_VER
    Sleep(5);
#else
    usleep(5 * 1000);
#endif
  }

  #elif LV_USE_OS == LV_OS_FREERTOS

  /* Run FreeRTOS and create lvgl task */
  freertos_main();

  #endif

  return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the LVGL graphics
 * library
 */
static lv_display_t * hal_init(int32_t w, int32_t h)
{

  lv_group_set_default(lv_group_create());

  lv_display_t * disp = lv_sdl_window_create(w, h);

  lv_indev_t * mouse = lv_sdl_mouse_create();
  lv_indev_set_group(mouse, lv_group_get_default());
  lv_indev_set_display(mouse, disp);
  lv_display_set_default(disp);

  LV_IMAGE_DECLARE(mouse_cursor_icon); /*Declare the image file.*/
  lv_obj_t * cursor_obj;
  cursor_obj = lv_image_create(lv_screen_active()); /*Create an image object for the cursor */
  lv_image_set_src(cursor_obj, &mouse_cursor_icon);           /*Set the image source*/
  lv_indev_set_cursor(mouse, cursor_obj);             /*Connect the image  object to the driver*/

  lv_indev_t * mousewheel = lv_sdl_mousewheel_create();
  lv_indev_set_display(mousewheel, disp);
  lv_indev_set_group(mousewheel, lv_group_get_default());

  lv_indev_t * kb = lv_sdl_keyboard_create();
  lv_indev_set_display(kb, disp);
  lv_indev_set_group(kb, lv_group_get_default());

  return disp;
}
