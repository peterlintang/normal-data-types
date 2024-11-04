

#include <assert.h>
#include <lvgl.h>
#include <lvgl/lvgl_res_loader.h>
#include <lvgl/lvgl_bitmap_font.h>

#include <ui_manager.h>
#include <view_manager.h>
#include "widgets/anim_img.h"
#include "widgets/img_number.h"


#include <res_manager_api.h>
#include "app_ui.h"
#include "app_defines.h"
#include "system_app.h"
#include <view_stack.h>
#include <acts_bluetooth/host_interface.h>

#include <bt_manager.h>
#include "uart2_to_gps.h"
#include <sys_wakelock.h>
#include "spl06001.h"
#include "SPA06_003.h"
#include "lps22df_reg.h"

static int pre_test_preload_inited = 0;
static uint32_t pre_test_str_id[] = {
	PRE_TEST_STRING,
};

struct mag_data_t {
	int flag;
	float mag_data[3];
	float mag_raw_data[3];
	float angle_yaw;
};

extern int get_mag_data(struct mag_data_t *data, float *acc_mg);

struct pre_test_data {
	lv_obj_t *pre_test_label;
	lvgl_res_string_t pre_test_str;
	lv_style_t pre_test_str_style;

	lvgl_res_scene_t scene;
	lv_font_t font;
};
static struct pre_test_data *private_data = NULL;

static void init_txt_style(struct pre_test_data *data, lv_style_t *sty, lvgl_res_string_t *txt, uint32_t num)
{
	for (int i = 0; i < num; i++)
	{
		lv_style_init(&sty[i]);
		lv_style_set_x(&sty[i], txt[i].x);
		lv_style_set_y(&sty[i], txt[i].y);
		lv_style_set_width(&sty[i], txt[i].width);
		lv_style_set_height(&sty[i], txt[i].height);
		lv_style_set_text_color(&sty[i], txt[i].color);
		lv_style_set_text_align(&sty[i], txt[i].align);
		lv_style_set_text_font(&sty[i], &data->font);
		SYS_LOG_INF("%s: %d %d %d %d\n", __func__, txt[i].x, txt[i].y, txt[i].width, txt[i].height);
	}
}

static int _pre_test_view_preload(view_data_t *view_data)
{

	if (pre_test_preload_inited == 0) {
		lvgl_res_preload_scene_compact_default_init(SCENE_PRE_TEST_VIEW, NULL, 0,
			NULL, DEF_STY_FILE, DEF_RES_FILE, DEF_STR_FILE);
		pre_test_preload_inited = 1;
	}

	return lvgl_res_preload_scene_compact_default(SCENE_PRE_TEST_VIEW, PRE_TEST_VIEW, 0, 0);
}

static lv_timer_t *timer;

static uint8_t chip_id;
static void timer_ing_update(lv_timer_t *timer)
{
	extern uint8_t spl06_init(void);
	extern void SPL06_Init(T_SPL06_calibPara *ptSPL06_calibPara);
	extern T_SPL06_calibPara t_SPL06_calibPara;
	extern float ReadSPL06_Pressure(T_SPL06_calibPara *ptSPL06_calibPara);
	extern void spl06_start(uint8_t mode);
	extern uint8_t spl06_update(float *Temp,float *Press);
	extern float Caculate_Altitude(float GasPress);
	extern float mts4b_read_temp(void);
	extern int mts4b_start(void);
	extern void lsm6dsl_read_data_polling(float *acc_mg, float *ang_mdps);
	extern void lsm6dsv16x_read_data_polling(float *acc_mg, float *ang_mdps);

	view_data_t *view_data = timer->user_data;
	lv_obj_t *src = NULL;
	float pa =0;
	float hight =0;
	float temp =0;
	float acc_mg[3] = { 0, 0 , 0 };
	float ang_mdps[3] = { 0, 0, 0 };
	char text[220] = { 0 };
	
	
	/*
    	spl06_start(0x01); 
	k_msleep(200);
    	spl06_start(0x02); 
    	spl06_start(0x07); 
	k_msleep(200);
	spl06_update(&temp,&pa);
    	spl06_start(0x07); 
//	 printf("temp = %f\n",temp);
//     	printf("pa = %f\n",pa);
	*/
//	lsm6dsl_read_data_polling(acc_mg, ang_mdps);
	lsm6dsv16x_read_data_polling(acc_mg, ang_mdps);
	/*
	temp = mts4b_read_temp();
	mts4b_start();
//	pa = ReadSPL06_Pressure(&t_SPL06_calibPara);
	
	float deg_c = 0.0;
	if (chip_id == LPS22DF_ID)
	{
		extern void lps22df_read_data_polling(float *hpa, float *deg_c);

		lps22df_read_data_polling(&pa, &deg_c);
	 	hight = Caculate_Altitude(pa * 100);
	}
	else
	{
		printf("%p\n", &t_SPL06_calibPara);
		spl07_003_get_raw_temp();
        	spl07_003_get_raw_pressure();
        	spl07_003_get_temperature();
        	pa = spl07_003_get_pressure();
		pa = pa / 100.0;
		hight = Caculate_Altitude(pa * 100);
		printf("spl003 tmp: %.2f\n", spl07_003_get_temperature());
	}
	
	 printf("pa:%.2f   gao:%.2fm, deg: %.2f, temp: %.2f\n",pa,hight, deg_c, temp);
	 */

	src = lv_disp_get_scr_act(view_data->display);
	if (src == NULL)
		return ;

extern int get_mag_data(struct mag_data_t *data, float *acc_mg);
	struct mag_data_t mag_data;
	mag_data.flag = 0;
	get_mag_data(&mag_data, acc_mg);
	int charge_status = power_manager_get_charge_status();
	printf("charge_status: %d(%d)\n", charge_status, BAT_STATUS_CHARGING);
//	if (BAT_STATUS_CHARGING == charge_status)
	{
		float p = 0.0;
		float q = 0.0;
		float r = 0.0;

		q = atan2f(-acc_mg[1] , (-acc_mg[2])) ;

		p = atanf(acc_mg[0] / (-acc_mg[1] * sinf(q) + (-acc_mg[2] * cosf(q))));

	r = atan2f( (-mag_data.mag_data[2] * sin(q) - (-mag_data.mag_data[0]) * cos(q)) , \
		( -mag_data.mag_data[1] * cos(p) + (-mag_data.mag_data[0]) * sin(p) * sin(q) + (-mag_data.mag_data[2] * sin(p) * cos(q)) ) );

extern int32_t get_iPsi(void);
extern int32_t low_pass_filter(void);
extern void iecompass(int16_t iBpx,int16_t  iBpy,int16_t  iBpz,
			int16_t iGpx,int16_t  iGpy,int16_t  iGpz);
	iecompass(	(int16_t)(-mag_data.mag_data[1] * 30),
			(int16_t)(-mag_data.mag_data[0] * 30),
			(int16_t)(-mag_data.mag_data[2] * 30),
			(int16_t)(-acc_mg[0] / 0.061), 
			(int16_t)(-acc_mg[1] / 0.061), 
			(int16_t)(-acc_mg[2] / 0.061));

	snprintf(text, 220, "\ntmp: %.2f %.2f %.2f\ncalbrating: %d %d\nmag: %.3f\npower: %4.2f %4.2f %4.2f\nmg: %4.2f %4.2f %4.2f\nmdps: %4.2f\n %4.2f\n %4.2f", 
			-mag_data.mag_data[1],
			-mag_data.mag_data[0],
			-mag_data.mag_data[2],
			mag_data.flag, low_pass_filter(),
			mag_data.angle_yaw,
			(q) * 180 / 3.14159265358979f,
			(p) * 180 / 3.14159265358979f,
			(r) * 180 / 3.14159265358979f > 0 ? (r) * 180 / 3.14159265358979f : 360 + (r) * 180 / 3.14159265358979f,
			/*
			(q = atanf(-acc_mg[1] / (-acc_mg[2])) ) * 180 / 3.14159265358979f,
			(p = atanf(acc_mg[0] / (-acc_mg[1] * sinf(q) + (-acc_mg[2] * cosf(q))))) * 180 / 3.14159265358979f,
			*/
			/*
			(q = atanf(-acc_mg[1] / (-acc_mg[2])) ) * 180 / 3.14159265358979f,
			(p = atanf(acc_mg[0] / (-acc_mg[1] * sinf(q) + (-acc_mg[2] * cosf(q))))) * 180 / 3.14159265358979f,
			*/
			//power_manager_get_battery_capacity(),
			-acc_mg[0],
			-acc_mg[1],
			-acc_mg[2],
			ang_mdps[0],
			ang_mdps[1],
			ang_mdps[2]
			);
	}
	/*
	else
	{
	snprintf(text, 220, "tmp: %.2f\npre: %.2f\nheight: %.2f\ncalbrating: %d\nmag: %.3f\npower: %d%%\nmg: %4.2f %4.2f %4.2f\nmdps: %4.2f\n %4.2f\n %4.2f", temp, pa, hight,
			mag_data.flag,
			mag_data.angle_yaw,
			power_manager_get_battery_capacity(),
			acc_mg[0],
			acc_mg[1],
			acc_mg[2],
			ang_mdps[0],
			ang_mdps[1],
			ang_mdps[2]
			);
	}
	*/

	lv_label_set_text(private_data->pre_test_label, text);

}

static int create_view(view_data_t *view_data)
{
	lv_obj_t *src = NULL;

	sys_wake_lock(FULL_WAKE_LOCK);
	src = lv_disp_get_scr_act(view_data->display);
	if (src == NULL)
		return -1;

	private_data->pre_test_label = lv_label_create(src);
	lv_label_set_text(private_data->pre_test_label, "hello world");
	lv_obj_add_style(private_data->pre_test_label, &private_data->pre_test_str_style, LV_PART_MAIN);
	lv_obj_center(private_data->pre_test_label);

//	extern void lsm6dsl_init(void);
	extern void lsm6dsv16x_init(void);
	extern uint8_t spl06_init(void);
	extern void spl07_003_init(void);
	extern T_SPL06_calibPara t_SPL06_calibPara;
	extern void SPL06_Init(T_SPL06_calibPara *ptSPL06_calibPara);
	extern void spl06_start(uint8_t mode);
	void lps22df_init(void);
	extern void pressure_lps22df_id_get(uint8_t *chip_id);
	/*
	pressure_lps22df_id_get(&chip_id);
	printf("%s: pressure chip id: %x, %x\n", __func__, chip_id, LPS22DF_ID);
	if (chip_id == LPS22DF_ID)
	{
		lps22df_init();
	}
	else
	{
		spl07_003_init();
		spl07_003_start_continuous(CONTINUOUS_P_AND_T);
	}
	*/
	/*
	if(spl06_init()){
		SYS_LOG_INF("spl06_init err\r\n");
	}
	*/
	//lsm6dsl_init();
	lsm6dsv16x_init();
//	SPL06_Init(&t_SPL06_calibPara);
	printf("%p\n", &t_SPL06_calibPara);
	timer = lv_timer_create(timer_ing_update, 1000, view_data);
	//spl07_003_init();
	//spl07_003_start_continuous(CONTINUOUS_P_AND_T);
	if (timer)
		lv_timer_ready(timer);
	return 0;
}

static int _pre_test_view_layout(view_data_t *view_data)
{
	int ret = 0;
	private_data = app_mem_malloc(sizeof(*private_data));
	if (private_data == NULL) return -1;

	memset(private_data, 0, sizeof(*private_data));

	view_data->user_data = private_data;

	ret = lvgl_res_load_scene(SCENE_PRE_TEST_VIEW, &private_data->scene, DEF_STY_FILE, DEF_RES_FILE, DEF_STR_FILE);
	if (ret != 0)
	{
		return -1;
	}

	if (lvgl_bitmap_font_open(&private_data->font, DEF_FONT24_FILE) < 0)
	{
		return -1;
	}

	ret = lvgl_res_load_strings_from_scene(&private_data->scene, 
			pre_test_str_id, 
			&private_data->pre_test_str, 
			1);
	init_txt_style(private_data, &private_data->pre_test_str_style, &private_data->pre_test_str, 1);

	create_view(view_data);
	lv_refr_now(view_data->display);
	return 0;
}

static int _pre_test_view_delete(view_data_t *view_data)
{
//	lvgl_res_preload_cancel_scene(SCENE_PRE_TEST_VIEW);


	lv_style_reset(&private_data->pre_test_str_style);
	lvgl_bitmap_font_close(&private_data->font);

	app_mem_free(private_data);
	private_data = NULL;

	if (timer) 
	{
		lv_timer_del(timer);
		timer = NULL;
	}
	lvgl_res_unload_scene_compact(SCENE_PRE_TEST_VIEW);
extern int mag_set_flag(int flag);
	mag_set_flag(0);
	return 0;
}

int pre_test_view_handler(uint16_t view_id, uint8_t msg_id, void *msg_data)
{
	int ret = 0;
	view_data_t *view_data = msg_data;

	SYS_LOG_INF("%s: %d %d\n", __func__, view_id, PRE_TEST_VIEW);
	assert(view_id == PRE_TEST_VIEW);

	switch (msg_id) {
	case MSG_VIEW_PRELOAD:
		ret = _pre_test_view_preload(view_data);
		break;
	case MSG_VIEW_LAYOUT:
		ret = _pre_test_view_layout(view_data);
		break;
	case MSG_VIEW_DELETE:
		ret = _pre_test_view_delete(view_data);
		break;
//	case MSG_VIEW_UPDATE:
		break;
	default:
		break;
	}
	return ret;
}

VIEW_DEFINE(pre_test_view, pre_test_view_handler, NULL,
		NULL, PRE_TEST_VIEW, NORMAL_ORDER, UI_VIEW_LVGL, DEF_UI_VIEW_WIDTH, DEF_UI_VIEW_HEIGHT);

