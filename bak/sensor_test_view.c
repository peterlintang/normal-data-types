

/*
 * Copyright (c) 2020 Actions Technology Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */
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
#include <sys_wakelock.h>

#include <math.h>
#include <drivers/i2c.h>


int mts4b_init(void)
{
	const struct device *dev;

	dev = device_get_binding("I2C_1");
	return 0;
}

int mts4b_start(void)
{
	const struct device *dev;

	dev = device_get_binding("I2C_1");
	i2c_reg_write_byte(dev, 0x41, 0x04, 0xc0);
	return 0;
}

float mts4b_read_temp(void)
{
	const struct device *dev;
	uint8_t buffer[2];
	int16_t temp;

	dev = device_get_binding("I2C_1");
	i2c_burst_read(dev, 0x41, 0x00, buffer, 2);
	temp = ((int16_t)buffer[1] << 8) + (buffer[0]);
	temp = (temp & 0x0800) ? (0xF000 | temp) : temp;
//	printf("temp: %d(%f) %x %x\n", temp, temp / 256.0 + 25, buffer[0], buffer[1]);

	return (temp / 256.0 + 25);

}

#define  PM_RATE_1          (0<<4)      //1 measurements pr. sec.
#define  PM_RATE_2          (1<<4)      //2 measurements pr. sec.
#define  PM_RATE_4          (2<<4)      //4 measurements pr. sec.           
#define  PM_RATE_8          (3<<4)      //8 measurements pr. sec.
#define  PM_RATE_16         (4<<4)      //16 measurements pr. sec.
#define  PM_RATE_32         (5<<4)      //32 measurements pr. sec.
#define  PM_RATE_64         (6<<4)      //64 measurements pr. sec.
#define  PM_RATE_128        (7<<4)      //128 measurements pr. sec.

//��ѹ�ز�������(times),Background ģʽʹ��
#define PM_PRC_1            0       //Sigle         kP=524288   ,3.6ms
#define PM_PRC_2            1       //2 times       kP=1572864  ,5.2ms
#define PM_PRC_4            2       //4 times       kP=3670016  ,8.4ms
#define PM_PRC_8            3       //8 times       kP=7864320  ,14.8ms
#define PM_PRC_16           4       //16 times      kP=253952   ,27.6ms
#define PM_PRC_32           5       //32 times      kP=516096   ,53.2ms
#define PM_PRC_64           6       //64 times      kP=1040384  ,104.4ms
#define PM_PRC_128          7       //128 times     kP=2088960  ,206.8ms

//�¶Ȳ�������(sample/sec),Background ģʽʹ��
#define  TMP_RATE_1         (0<<4)      //1 measurements pr. sec.
#define  TMP_RATE_2         (1<<4)      //2 measurements pr. sec.
#define  TMP_RATE_4         (2<<4)      //4 measurements pr. sec.           
#define  TMP_RATE_8         (3<<4)      //8 measurements pr. sec.
#define  TMP_RATE_16        (4<<4)      //16 measurements pr. sec.
#define  TMP_RATE_32        (5<<4)      //32 measurements pr. sec.
#define  TMP_RATE_64        (6<<4)      //64 measurements pr. sec.
#define  TMP_RATE_128       (7<<4)      //128 measurements pr. sec.

//�¶��ز�������(times),Background ģʽʹ��
#define TMP_PRC_1           0       //Sigle
#define TMP_PRC_2           1       //2 times
#define TMP_PRC_4           2       //4 times
#define TMP_PRC_8           3       //8 times
#define TMP_PRC_16          4       //16 times
#define TMP_PRC_32          5       //32 times
#define TMP_PRC_64          6       //64 times
#define TMP_PRC_128         7       //128 times

//SPL06_MEAS_CFG
#define MEAS_COEF_RDY       0x80
#define MEAS_SENSOR_RDY     0x40        //��������ʼ�����
#define MEAS_TMP_RDY        0x20        //���µ��¶�����
#define MEAS_PRS_RDY        0x10        //���µ���ѹ����

#define MEAS_CTRL_Standby               0x00    //����ģʽ
#define MEAS_CTRL_PressMeasure          0x01    //������ѹ����
#define MEAS_CTRL_TempMeasure           0x02    //�����¶Ȳ���
#define MEAS_CTRL_ContinuousPress       0x05    //������ѹ����
#define MEAS_CTRL_ContinuousTemp        0x06    //�����¶Ȳ���
#define MEAS_CTRL_ContinuousPressTemp   0x07    //������ѹ�¶Ȳ���

//FIFO_STS
#define SPL06_FIFO_FULL     0x02
#define SPL06_FIFO_EMPTY    0x01

//INT_STS
#define SPL06_INT_FIFO_FULL     0x04
#define SPL06_INT_TMP           0x02
#define SPL06_INT_PRS           0x01

//CFG_REG
#define SPL06_CFG_T_SHIFT   0x08    //oversampling times>8ʱ����ʹ��
#define SPL06_CFG_P_SHIFT   0x04

#define SP06_PSR_B2     0x00        //��ѹֵ
#define SP06_PSR_B1     0x01
#define SP06_PSR_B0     0x02
#define SP06_TMP_B2     0x03        //�¶�ֵ
#define SP06_TMP_B1     0x04
#define SP06_TMP_B0     0x05

#define SP06_PSR_CFG    0x06        //��ѹ��������
#define SP06_TMP_CFG    0x07        //�¶Ȳ�������
#define SP06_MEAS_CFG   0x08        //����ģʽ����

#define SP06_CFG_REG    0x09
#define SP06_INT_STS    0x0A
#define SP06_FIFO_STS   0x0B

#define SP06_RESET      0x0C
#define SP06_ID         0x0D

#define SP06_COEF       0x10        //-0x21
#define SP06_COEF_SRCE  0x28

#define SP06_Advice_Address  (0x77<<1)  //IIC������ַ


float _kT=0,_kP=0;
int16_t _C0=0,_C1=0,_C01=0,_C11=0,_C20=0,_C21=0,_C30=0;
int32_t _C00=0,_C10=0;

#define I2C_DEV	"I2C_1"
#define SP06_ADDRESS	(0x77)
uint8_t spl06_write_reg(uint8_t reg_addr, uint8_t reg_val)
{
	const struct device *dev;

	dev = device_get_binding(I2C_DEV);

	if (i2c_reg_write_byte(dev, SP06_ADDRESS, reg_addr, reg_val) < 0) {
		printf( "Failed to write to device: %s %d %x %x\n", __func__, __LINE__, reg_addr, reg_val);
		return 1;
	}
//	printf("%s reg: %x, %x\n", __func__, reg_addr, reg_val);
	return 0;
	
}

uint8_t spl06_read_reg(uint8_t reg_addr)
{

	const struct device *dev;
	uint8_t out;
	

	dev = device_get_binding(I2C_DEV);

	if (i2c_reg_read_byte(dev, SP06_ADDRESS, reg_addr, &out) < 0) {
		printf( "Failed to read to device: %s %d %x\n", __func__, __LINE__, reg_addr);
		return 0;
	}

//	printf("%s reg: %x, %x\n", __func__, reg_addr, out);

  return out;

}
uint8_t spl06_read_buffer(uint8_t reg_addr,uint8_t *buffer,uint16_t len)
{

	const struct device *dev;
	int num_bytes;

	dev = device_get_binding(I2C_DEV);

	num_bytes = len;
	
	if (i2c_burst_read(dev, SP06_ADDRESS, reg_addr, buffer, num_bytes) < 0) {
		printf( "Failed to read to device: %s %d %x %d\n", __func__, __LINE__, reg_addr, len);
		return 1;
	}

	/*
	for (int i = 0; i < num_bytes; i++)
	{
		printf("%s reg: %x, i: %d, %x\n", __func__, reg_addr, i, buffer[i]);
	}
	*/
	return 0;
	
}


void spl06_start(uint8_t mode)
{
    spl06_write_reg(SP06_MEAS_CFG, mode);//测量模式配置
}

void spl06_config_temperature(uint8_t rate,uint8_t oversampling)
{
    switch(oversampling)
    {
	case TMP_PRC_1:
		_kT = 524288.0;
		break;
	case TMP_PRC_2:
		_kT = 1572864.0;
		break;
	case TMP_PRC_4:
		_kT = 3670016.0;
		break;
	case TMP_PRC_8:
		_kT = 7864320.0;
		break;
	case TMP_PRC_16:
		_kT = 253952.0;
		break;
	case TMP_PRC_32:
		_kT = 516096.0;
		break;
	case TMP_PRC_64:
		_kT = 1040384.0;
		break;
	case TMP_PRC_128:
		_kT = 2088960.0;
		break;
    }
    spl06_write_reg(SP06_TMP_CFG,rate|oversampling|0x80);   //温度每秒128次测量一次（最快速度）
	printf("%s: %x\n", __func__, rate|oversampling|0x80);
    if(oversampling > TMP_PRC_8)
    {
        uint8_t temp = spl06_read_reg(SP06_CFG_REG);
        spl06_write_reg(SP06_CFG_REG,temp|SPL06_CFG_T_SHIFT);
	printf("%s: %x\n", __func__, temp|SPL06_CFG_T_SHIFT);
    }
}

void spl06_config_pressure(uint8_t rate,uint8_t oversampling)//设置补偿系数及采样速率
{
    switch(oversampling)
    {
	case PM_PRC_1:
		_kP = 524288;
		break;
	case PM_PRC_2:
		_kP = 1572864;
		break;
	case PM_PRC_4:
		_kP = 3670016;
		break;
	case PM_PRC_8:
		_kP = 7864320;
		break;
	case PM_PRC_16:
		_kP = 253952;
		break;
	case PM_PRC_32:
		_kP = 516096;
		break;
	case PM_PRC_64:
		_kP = 1040384;
		break;
	case PM_PRC_128:
		_kP = 2088960;
		break;
    }
	
    spl06_write_reg(SP06_PSR_CFG,rate|oversampling);
    printf("%s: %x\n", __func__, rate|oversampling);
    if(oversampling > PM_PRC_8)
    {
        uint8_t temp = spl06_read_reg(SP06_CFG_REG);
        spl06_write_reg(SP06_CFG_REG,temp|SPL06_CFG_P_SHIFT);
    	printf("%s: %x\n", __func__, temp|SPL06_CFG_P_SHIFT);
    }
}

int32_t spl06_get_pressure_adc(void)//获取压力ADC值
{
    uint8_t buf[3];
    int32_t adc;
	
    spl06_read_buffer(SP06_PSR_B2,buf,3);
    adc = (int32_t)buf[0]<<16 | (int32_t)buf[1]<<8 | (int32_t)buf[2];
    adc = (adc&0x800000)?(0xFF000000|adc):adc;
	
    printf("%s: pre: %d\n", __func__, adc);
    return adc;
}

int32_t spl06_get_temperature_adc(void)//获取温度ADC值
{
    uint8_t buf[3];
    int32_t adc;
	
    spl06_read_buffer(SP06_TMP_B2,buf,3);
    adc = (int32_t)buf[0]<<16 | (int32_t)buf[1]<<8 | (int32_t)buf[2];
    adc = (adc&0x800000)?(0xFF000000|adc):adc;
    printf("%s: tmp: %d\n", __func__, adc);
    return adc;
}
uint8_t spl06_update(float *Temp,float *Press)//获取并计算出温度值、气压值
{
	float Praw_src=0.0,Traw_src=0.0;
    float qua2=0.0, qua3=0.0;
    uint8_t value;

	do {
		k_msleep(1);
    		value = spl06_read_reg(SP06_MEAS_CFG);
		printf("%s: %x\n", __func__, value);
	} while ((value & 0x20) == 0x00);
    Traw_src = spl06_get_temperature_adc()/_kT;
//	printf("%s: tmp adc: %d, _kT: %f (test: %f)\n", __func__, spl06_get_temperature_adc(), _kT, 0.12345);
	do {
		k_msleep(1);
    		value = spl06_read_reg(SP06_MEAS_CFG);
		printf("%s: %x\n", __func__, value);
	} while ((value & 0x10) == 0x00);
    Praw_src = spl06_get_pressure_adc()/_kP ;
//	printf("%s: pre adc: %d, _kP: %f\n", __func__, spl06_get_pressure_adc(), _kP);
	//计算温度
//	printf("%s: Traw_src: %f\n", __func__, Traw_src);
    *Temp = 0.5f*_C0 + Traw_src * _C1;
//	printf("%s: 0.5f * _C0: %f\n", __func__, 0.5f * _C0);
//	printf("%s: Traw_src * _C1: %f\n", __func__, Traw_src * _C1);
//	printf("%s: Temp: %f\n", __func__, *Temp);
    //计算气压
    qua2 = _C10 + Praw_src * (_C20 + Praw_src* _C30);
//	printf("%s: Praw_src: %f\n", __func__, Praw_src);
    qua3 = Traw_src * Praw_src * (_C11 + Praw_src * _C21);
//	printf("%s: qua2: %f\n", __func__, qua2);
//	printf("%s: qua3: %f\n", __func__, qua3);
    *Press = _C00 + Praw_src * qua2 + Traw_src * _C01 + qua3;
  //  *Press = _C00 + Praw_src * qua2;// + Traw_src * _C01 + qua3;
 //   printf("%s: %f %f %f %f\n", __func__, Traw_src, Praw_src, *Temp, *Press);
    return 0;
}

uint8_t spl06_init(void)
{
    uint8_t coef[18];
    uint8_t id;
    uint8_t value;
    /*
    if(spl06_write_reg(SP06_RESET,0x89))
    {
			printf("SP06_RESET erro\n");
		return 1;
    }
	k_msleep(40);
    */
    id = spl06_read_reg(SP06_ID);
		
//		printf("SPL06-ID:%x\n",id);
		
    if(id != 0x10)
    {
		  return 2;
    }
		
//		printf("SPL06 is OK:%x\n",id);
	
    k_msleep(100);        //复位后系数准备好需要至少40ms
	do {
		k_msleep(1);
    		value = spl06_read_reg(SP06_MEAS_CFG);
		printf("%s: %x\n", __func__, value);
	} while ((value & 0x80) == 0x00);
	
    spl06_read_buffer(SP06_COEF,coef,18);//读取相关数据
    _C0 = ((int16_t)coef[0]<<4 ) | ((coef[1]&0xF0)>>4);
    _C0 = (_C0&0x0800)?(0xF000|_C0):_C0;
    _C1 = ((int16_t)(coef[1]&0x0F)<<8 ) | coef[2];
    _C1 = (_C1&0x0800)?(0xF000|_C1):_C1;
    _C00 = (int32_t)coef[3]<<12  | (int32_t)coef[4]<<4  | (int32_t)coef[5]>>4;
    _C10 = (int32_t)(coef[5]&0x0F)<<16  | (int32_t)coef[6]<<8  | (int32_t)coef[7];
    _C00 = (_C00&0x080000)?(0xFFF00000|_C00):_C00;
	_C10 = (_C10&0x080000)?(0xFFF00000|_C10):_C10;
	
	_C01 = ((int16_t)coef[8]<<8 ) | coef[9];
	_C01 = (_C01&0x0800)?(0xF000|_C01):_C01;
	
    _C11 = ((int16_t)coef[10]<<8 ) | coef[11];
    _C11 = (_C11&0x0800)?(0xF000|_C11):_C11;
	
    _C20 = ((int16_t)coef[12]<<8 ) | coef[13];
    _C20 = (_C20&0x0800)?(0xF000|_C20):_C20;
    _C21 = ((int16_t)coef[14]<<8 ) | coef[15];
    _C21 = (_C21&0x0800)?(0xF000|_C21):_C21;
    _C30 = ((int16_t)coef[16]<<8 ) | coef[17];
    _C30 = (_C30&0x0800)?(0xF000|_C30):_C30;
	
//    printf("%s: c0: %d, c1: %d, c00: %d, c10: %d, c20: %d, c30: %d, c01: %d, c11: %d, c21: %d\n", 
//		    __func__, _C0, _C1, _C00, _C10, _C20, _C30, _C01, _C11, _C21);
//    printf("%s: config pressure\n", __func__);
    spl06_config_pressure(PM_RATE_4,PM_PRC_32);
//    printf("%s: config tmp\n", __func__);
    spl06_config_temperature(PM_RATE_4,TMP_PRC_8);
	
    k_msleep(20);

	do {
		k_msleep(1);
    		value = spl06_read_reg(SP06_MEAS_CFG);
		printf("%s: %x\n", __func__, value);
	} while ((value & 0x40) == 0x00);
    return 0;
}
float Caculate_Altitude(float GasPress)
{
	float Altitude=0;
	Altitude =(44330.0 *(1.0-pow((float)(GasPress) / 101325.0, 1.0/5.255)));
	return Altitude;
}

int SPL06ReadRes_Buf(uint8_t ResAddr, uint8_t *pData,uint8_t DataLen)
{
	const struct device *dev;

	dev = device_get_binding("I2C_1");

	i2c_burst_read(dev, SP06_ADDRESS, ResAddr, pData, DataLen);

	return 1;
}

uint8_t SPL06ReadRes_Single(uint8_t ResAddr)
{
	uint8_t DataTemp = 0;
	const struct device *dev;

	dev = device_get_binding("I2C_1");
	i2c_reg_read_byte(dev, SP06_ADDRESS, ResAddr, &DataTemp);
	return DataTemp;
}

int SPL06WriteRes_Single(uint8_t ResAddr,uint8_t DataTemp)
{
	const struct device *dev;

	dev = device_get_binding("I2C_1");

	if (i2c_reg_write_byte(dev, SP06_ADDRESS, ResAddr, DataTemp) < 0) {
		printf( "Failed to write to device: %s %d %x %x\n", __func__, __LINE__, ResAddr, DataTemp);
		return 1;
	}
//	printf("%s reg: %x, %x\n", __func__, reg_addr, reg_val);
	return 0;
}



static uint32_t lcd_test_str_id[] = {
	LCD_TEST,
};

static uint32_t tmp_test_str_id[] = {
	TMP_SENSOR_TEST,
};

static uint32_t pre_test_str_id[] = {
	PRESSURE_SENSOR_TEST,
};

static uint32_t com_test_str_id[] = {
	COMPASS_TEST,
};


struct sensor_test_private {
	lv_obj_t *lcd_test_button;
	lv_obj_t *lcd_test_label;
	lv_style_t lcd_test_button_style;
	lvgl_res_string_t lcd_test_str;
	lv_style_t lcd_test_str_style;

	lv_obj_t *tmp_test_button;
	lv_obj_t *tmp_test_label;
	lv_style_t tmp_test_button_style;
	lvgl_res_string_t tmp_test_str;
	lv_style_t tmp_test_str_style;

	lv_obj_t *pre_test_button;
	lv_obj_t *pre_test_label;
	lv_style_t pre_test_button_style;
	lvgl_res_string_t pre_test_str;
	lv_style_t pre_test_str_style;

	lv_obj_t *com_test_button;
	lv_obj_t *com_test_label;
	lv_style_t com_test_button_style;
	lvgl_res_string_t com_test_str;
	lv_style_t com_test_str_style;


	lvgl_res_scene_t scene;
	lv_font_t font;
};


static int sensor_test_preload_inited = 0;
static struct sensor_test_private *private_data = NULL;

static void init_txt_style(struct sensor_test_private *data, lv_style_t *sty, lvgl_res_string_t *txt, uint32_t num)
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

#include "mag_calibration.h"
#include <math.h>
#include <arm_math.h>
void mat_inverse_float(matrix_instance_f32 * pSrc,matrix_instance_f32 * pDst)  //此函数用于32bit浮点数的逆矩阵求解 。
{ 
	arm_mat_inverse_f32((arm_matrix_instance_f32*)pSrc,(arm_matrix_instance_f32*) pDst); //此函数为CMSIS-DSP中函数，若不使用需自行实现
}

void mat_mult_float(matrix_instance_f32 * pSrcA,matrix_instance_f32 * pSrcB,matrix_instance_f32 * pDst)//此函数用于浮点数的矩阵乘法
{
	arm_mat_mult_f32((arm_matrix_instance_f32*)pSrcA,(arm_matrix_instance_f32*)pSrcB,(arm_matrix_instance_f32*)pDst);//此函数为CMSIS-DSP中函数，若不使用需自行实现
}

void mag_calibration_start(void)
{
	float mag_data[8];
	uint8_t ret = 0;
	int init_value = 3;
	SYS_LOG_INF("%s: start init mag calibration\n", __func__);
//init:
	mag_calibration_init();

//again:
	for (int i = 0; i < 8; i++)
	{
		mag_data[i] = i + 3.587;
	}

	init_value += 3;
	ret = mag_calculate(mag_data);  
	if (ret == 2)
	{
		SYS_LOG_INF("%s: mag calculate again\n", __func__);
//		goto again;
	}
	else if (ret == 1)
		SYS_LOG_INF("%s: mag calculate done\n", __func__);
	else if (ret == 0)
	{
		SYS_LOG_INF("%s: mag calculate failed\n", __func__);
//		goto init;
	}
	SYS_LOG_INF("%s: end init mag calibration\n", __func__);
}

static void VCM1195L_init(void)
{
	const struct device *dev;

	dev = device_get_binding("I2C_0");

	if (i2c_reg_write_byte(dev, 0x0c, 0x0a, 0x4d) < 0) {
		printf( "Failed to write to device: %s %d\n", __func__, __LINE__);
		return ;
	}
	printf("%s: init vcm1195l\n", __func__);
}

void VCM119x_Init(void)
{
	const struct device *dev;

	dev = device_get_binding("I2C_0");
	uint8_t regData = 0;
	regData = 0x00;
	i2c_reg_write_byte(dev, 0x0c, 0x0b, regData);
//	I2CWriteNByte (0x0B, &regData,1);	 //0B寄存器写0x00
//	vcm119x_delay_ms(10);  //延时10ms
	k_msleep(10);

}

/*
static void VCM1195L_ReadData(float *mag_data)
{
	const struct device *dev;
	uint8_t buffer[6] = { 0 };
	int num_bytes;
	int16_t value = 0;

	dev = device_get_binding("I2C_0");

	num_bytes = 6;
	
	if (i2c_burst_read(dev, 0x0c, 0x00, buffer, num_bytes) < 0) {
		printf( "Failed to read to device: %s %d \n", __func__, __LINE__);
		return ;
	}

	
	for (int i = 0; i < num_bytes; i++)
	{
		printf("%s reg: i: %d, %x\n", __func__, i, buffer[i]);
	}
	

	if (buffer[1] & 0x80)
	{
		value = (0xff00 & (buffer[1] << 8)) | buffer[0];
	}
	else
	{
		value = (buffer[1] << 8 | buffer[0]);
	}
	mag_data[0] = (float)value;
	printf("%s: %d %f\n", __func__, value, mag_data[0]);

	if (buffer[3] & 0x80)
	{
		value = (0xff00 & (buffer[3] << 8)) | buffer[2];
	}
	else
	{
		value = (buffer[3] << 8 | buffer[2]);
	}
	mag_data[1] = (float)value;
	printf("%s: %d %f\n", __func__, value, mag_data[1]);

	if (buffer[5] & 0x80)
	{
		value = (0xff00 & (buffer[5] << 8)) | buffer[4];
	}
	else
	{
		value = (buffer[5] << 8 | buffer[4]);
	}
	mag_data[2] = (float)value;
	printf("%s: %d %f\n", __func__, value, mag_data[2]);
}
*/

uint8_t VCM119xL_ReadData(float *mag)
{	
	int32_t  hdata[3];
	uint8_t ucReadBuf[6];
//	uint8_t regData[1] = {0};			
    	uint8_t res = 0;
	const struct device *dev;
			
	dev = device_get_binding("I2C_0");

	/*
	regData[0] = 0x42;
//	I2CWriteNByte (0x0A, regData,1);   //0A寄存器写0x42
	i2c_reg_write_byte(dev, 0x0c, 0x0a, regData[0]);

//	vcm119x_delay_ms(20); //延时20ms
	k_msleep(20);

	uint8_t data = 0x40;
//	I2CWriteNByte (0x0A, &data,1);	//0A寄存器写0x40
	i2c_reg_write_byte(dev, 0x0c, 0x0a, data);
//	res = I2CReadNByte (0x00, ucReadBuf, 6); //连续读取（地址00~地址05）
//	*/
	i2c_burst_read(dev, 0x0c, 0x00, ucReadBuf, 6);
    
	/*
	for (int i = 0; i < 6; i++)
	{
		printf("%s reg: i: %d, %x\n", __func__, i, ucReadBuf[i]);
	}
	*/

    hdata[0] = (int32_t)((ucReadBuf[1] << 8) | (ucReadBuf[0]));
	hdata[1] = (int32_t)((ucReadBuf[3] << 8) | (ucReadBuf[2]));
	hdata[2] = (int32_t)((ucReadBuf[5] << 8) | (ucReadBuf[4]));
		
	if (hdata[0] >= 32768)
	{
		hdata[0] = hdata[0] - 65536;
	}
	if (hdata[1] >= 32768)
	{
		hdata[1] = hdata[1] - 65536;
	}
	if (hdata[2] >= 32768)
	{
		hdata[2] = hdata[2] - 65536;
	}
	
	hdata[0] =-hdata[0] ;
	hdata[1] =-hdata[1] ;
	hdata[2] =-hdata[2] ;
	
	mag[0] = hdata[0] /3000.0f *100;
	mag[1] = hdata[1] /3000.0f *100;
	mag[2] = hdata[2] /3000.0f *100;

	return res;
}

struct mag_data_t {
	int flag;
	float mag_data[3];
	float mag_raw_data[3];
	float angle_yaw;
};
static struct mag_data_t s_mag_data = {
	.flag = 0,
};

static struct k_mutex my_mutex;


int get_mag_data(struct mag_data_t *data, float *acc_mg)
{
	k_mutex_lock(&my_mutex, K_FOREVER);
	data->flag = s_mag_data.flag;
	data->mag_data[0] = s_mag_data.mag_data[0];
	data->mag_data[1] = s_mag_data.mag_data[1];
	data->mag_data[2] = s_mag_data.mag_data[2];
	data->mag_raw_data[0] = s_mag_data.mag_raw_data[0];
	data->mag_raw_data[1] = s_mag_data.mag_raw_data[1];
	data->mag_raw_data[2] = s_mag_data.mag_raw_data[2];
	data->angle_yaw = s_mag_data.angle_yaw;
//	k_mutex_unlock(&my_mutex);
        printf("%s: %.3f %.3f %.3f %.3f %.3f %.3f, angle_yaw=%.3f\n", 
			__func__, 
			data->mag_raw_data[0],
			data->mag_raw_data[1],
			data->mag_raw_data[2],
			data->mag_data[0],
			data->mag_data[1],
			data->mag_data[2],
			data->angle_yaw
			);
	printf("%.3f %.3f\n", cosf(atanf(acc_mg[0] / acc_mg[2])), cosf(atanf(acc_mg[1] / acc_mg[2])));
	printf("%.3f %.3f\n", sinf(atanf(acc_mg[0] / acc_mg[2])), sinf(atanf(acc_mg[1] / acc_mg[2])));

/*	// https://blog.csdn.net/weixin_43614541/article/details/104818380 */
	float new_mag[3];
	float angle;
	new_mag[0] = data->mag_raw_data[0] * cosf(atanf(acc_mg[0] / acc_mg[2])) + data->mag_raw_data[2] * sinf(atanf(acc_mg[0] / acc_mg[2]));
	new_mag[1] = data->mag_raw_data[0] * sinf(atanf(acc_mg[1] / acc_mg[2])) * sinf(atanf(acc_mg[0] / acc_mg[2])) 
		+ data->mag_raw_data[1] * cosf(atanf(acc_mg[1] / acc_mg[2])) 
		- data->mag_raw_data[2] * sinf(atanf(acc_mg[1] / acc_mg[2])) * cosf(atanf(acc_mg[0] / acc_mg[2]));
	printf("mx: %.3f my: %.3f\n", new_mag[0], new_mag[1]);
        get_calibrat_mag_data(new_mag); //输入原始磁数据，得到补偿后的磁数据
	angle = get_angle(new_mag);

            if(angle<=0)
		{
			angle= angle* -1.0f;
		}
		else
		{
		  angle= 360.0 - angle;
		}
		if(angle>=90.0)
		{
			angle= angle-90.0; 				
		}
		else
		{
			angle= angle+270.0;
		}

//		if(angle_yaw>=180.0)
		{
//			angle_yaw = angle_yaw+180.0;
		}
//		else
		{
			angle= 360.0 - angle; 				
		}

	printf("mx: %.3f my: %.3f mz: %.3f angle: %.3f\n", new_mag[0], new_mag[1], new_mag[2], angle);

	k_mutex_unlock(&my_mutex);

	/*
	mx = data->mag_raw_data[0] * cosf(atanf(acc_mg[0] / acc_mg[2])) + data->mag_raw_data[2] * sinf(atanf(acc_mg[0] / acc_mg[2]));
	my = data->mag_raw_data[0] * sinf(atanf(acc_mg[1] / acc_mg[2])) * sinf(atanf(acc_mg[0] / acc_mg[2])) 
		+ data->mag_raw_data[1] * cosf(atanf(acc_mg[1] / acc_mg[2])) 
		- data->mag_raw_data[2] * sinf(atanf(acc_mg[1] / acc_mg[2])) * cosf(atanf(acc_mg[0] / acc_mg[2]));
	printf("22 mx: %.3f my: %.3f\n", mx, my);
	*/

				
	return 0;
}

static k_tid_t m_tid = NULL;
static int mag_flag = 0;
int mag_set_flag(int flag)
{
	return mag_flag = flag;
}

void mag_calibration_test(void)
{
    uint8_t res=0;
    uint8_t step = 0;
    float mag_data[3]={0};//存储磁数据
    float mag_param_value[6] = {0}; //存储校准参数
    float angle_yaw = 0.0;

	k_mutex_init(&my_mutex);
	mag_flag = 1;
    printf("%s: mag calibration test\n", __func__);
    s_mag_data.flag = 1;
    mag_calibration_init(); //初始化校准操作   
    VCM1195L_init();
//	VCM119x_Init();
    while(mag_flag)
    {
//       delay(100);//此延时非必须，根据实际情况修改，我们这边测试是100ms，相当于10Hz的频率采集数据
	k_msleep(20);
//       VCM1195L_ReadData(mag_data); //读取磁数据，将数据存储在mag_data中；
	VCM119xL_ReadData(mag_data);
	//printf("%s: orig data: %.5f %.5f %.5f\n", __func__, mag_data[0], mag_data[1], mag_data[2]);
       if(step==0)//期间需一直转动设备进行8字校准操作，并一直读取磁数据并传入下面的函数；设备检测到晃动时开始校准计算（期间res一直为2），检测到设备静止不动时会输出校准结果（res为1或0）
       {
            res = mag_calculate(mag_data);  

            if(res==1) //校准成功
            {   step = 1;
    		s_mag_data.flag = 2;
                get_calibration_parameter(mag_param_value);
                printf("%s: done\r\n", __func__);
                printf("%s: param=%f,%f,%f,%f,%f,%f\r\n", __func__, mag_param_value[0],mag_param_value[1],mag_param_value[2],mag_param_value[3],mag_param_value[4],mag_param_value[5]);  
            }
            else if(res==2) //校准中
            {
                printf("%s: calibrating\r\n", __func__);
            }
            else if(res==0) //校准失败
            {                     
                printf("%s: fail\r\n", __func__);
                mag_calibration_init(); //初始化校准操作,重新进行校准
            }
            else
            {

            }

       }
       else //校准成功，进行角度验证
       {
		k_mutex_lock(&my_mutex, K_FOREVER);
	       s_mag_data.mag_raw_data[0] = mag_data[0];
	       s_mag_data.mag_raw_data[1] = mag_data[1];
	       s_mag_data.mag_raw_data[2] = mag_data[2];
//		printf("%s: orig: %.5f %.5f %.5f\n", __func__, mag_data[0], mag_data[1], mag_data[2]);
            get_calibrat_mag_data(mag_data); //输入原始磁数据，得到补偿后的磁数据
		//printf("%s: after cal: %.5f %.5f %.5f\n", __func__, mag_data[0], mag_data[1], mag_data[2]);
	       s_mag_data.mag_data[0] = mag_data[0];
	       s_mag_data.mag_data[1] = mag_data[1];
	       s_mag_data.mag_data[2] = mag_data[2];
            angle_yaw = get_angle(mag_data);//使用补偿后的磁数据计算水平状态下的航向角
            //printf("%s: 1 my angle_yaw=%.2f\r\n", __func__, angle_yaw);
            if(angle_yaw<=0)
		{
			angle_yaw = angle_yaw * -1.0f;
		}
		else
		{
		  angle_yaw = 360.0 - angle_yaw;
		}
		if(angle_yaw>=90.0)
		{
			angle_yaw = angle_yaw-90.0; 				
		}
		else
		{
			angle_yaw = angle_yaw+270.0;
		}

//		if(angle_yaw>=180.0)
		{
//			angle_yaw = angle_yaw+180.0;
		}
//		else
		{
			angle_yaw = 360.0 - angle_yaw; 				
		}

		s_mag_data.angle_yaw = angle_yaw;
	k_mutex_unlock(&my_mutex);
            //printf("%s: my angle_yaw=%.2f\r\n", __func__, angle_yaw);

//	    if (count++ > 5) break;

       }
       
    }
	m_tid = 0;
	s_mag_data.angle_yaw = 0.0;
  	s_mag_data.flag = 0;

}

static void mag_calibration_thread(void *p1,void *p2,void *p3)
{
	mag_calibration_test();
//	mag_calibration_start();
}

K_THREAD_STACK_DEFINE(mcal_stack_area, 4096);
static struct k_thread thread_data;
void mag_calibration_thread_init(void)
{
	if (m_tid == NULL)
	{
		m_tid = k_thread_create(&thread_data, mcal_stack_area,
				K_THREAD_STACK_SIZEOF(mcal_stack_area),
				mag_calibration_thread, NULL, NULL, NULL,
				15, 0, K_NO_WAIT);
		k_thread_name_set(m_tid, "mag_thread");
	}
	else
	{
		SYS_LOG_INF("%s: already started", __func__);
	}
}




static void lcd_test_btn_event_handler(lv_event_t * e)
{
	SYS_LOG_INF("%s: event\n", __func__);
	view_stack_push_view(LCD_TEST_VIEW, NULL);
}
static void pre_test_btn_event_handler(lv_event_t * e)
{
	view_stack_push_view(PRE_TEST_VIEW, NULL);

	/*
	float pa =0;
	float hight =0;
	float temp =0;
	int count = 0;
	SYS_LOG_INF("%s: event\n", __func__);
	if(spl06_init()){
		SYS_LOG_INF("spl06_init err\r\n");
	}
	
	
  while(count++ < 2)
 {
    	spl06_start(MEAS_CTRL_PressMeasure); 
	 k_msleep(200);
    	spl06_start(MEAS_CTRL_TempMeasure); 
	 k_msleep(200);
	 printf("count: %d\n", count);
	 spl06_update(&temp,&pa);
	 printf("temp = %f\n",temp);
     	printf("pa = %f\n",pa);
	 hight = Caculate_Altitude(pa);
	 printf("pa:%.2f   gao:%.2fm\n",pa,hight);
 }
 */
}

static void tmp_test_btn_event_handler(lv_event_t * e)
{
	SYS_LOG_INF("%s: event\n", __func__);
}

void com_test_btn_event_handler(lv_event_t * e)
{
	SYS_LOG_INF("%s: event\n", __func__);
//	mag_calibration_start();
	//mag_calibration_test();
	mag_calibration_thread_init();
	view_stack_push_view(PRE_TEST_VIEW, NULL);
}



static int sensor_test_view_layout(view_data_t *view_data)
{
	int ret = 0;
	lv_obj_t *src = NULL;

	private_data = app_mem_malloc(sizeof(*private_data));
	if (private_data == NULL) return -1;

	memset(private_data, 0, sizeof(*private_data));

	view_data->user_data = private_data;

	ret = lvgl_res_load_scene(SCENE_SENSOR_TEST_VIEW, &private_data->scene, DEF_STY_FILE, DEF_RES_FILE, DEF_STR_FILE);
	if (ret != 0)
	{
		goto out;
	}

	if (lvgl_bitmap_font_open(&private_data->font, DEF_FONT24_FILE) < 0)
	{
		goto out;
	}

	ret = lvgl_res_load_strings_from_scene(&private_data->scene, 
			lcd_test_str_id, 
			&private_data->lcd_test_str, 
			1);
	if (ret != 0)
	{
		goto error2;
	}

	init_txt_style(private_data, &private_data->lcd_test_str_style, &private_data->lcd_test_str, 1);

	ret = lvgl_res_load_strings_from_scene(&private_data->scene, 
			tmp_test_str_id, 
			&private_data->tmp_test_str, 
			1);
	if (ret != 0)
	{
		goto error2;
	}

	init_txt_style(private_data, &private_data->tmp_test_str_style, &private_data->tmp_test_str, 1);

	ret = lvgl_res_load_strings_from_scene(&private_data->scene, 
			pre_test_str_id, 
			&private_data->pre_test_str, 
			1);
	if (ret != 0)
	{
		goto error2;
	}

	init_txt_style(private_data, &private_data->pre_test_str_style, &private_data->pre_test_str, 1);

	ret = lvgl_res_load_strings_from_scene(&private_data->scene, 
			com_test_str_id, 
			&private_data->com_test_str, 
			1);
	if (ret != 0)
	{
		goto error2;
	}

	init_txt_style(private_data, &private_data->com_test_str_style, &private_data->com_test_str, 1);



	src = lv_disp_get_scr_act(view_data->display);
	if (src == NULL)
		return -1;

#if 1
	private_data->lcd_test_button = lv_btn_create(src);
	lv_obj_add_style(private_data->lcd_test_button, &private_data->lcd_test_button_style, 0);
	lv_obj_add_style(private_data->lcd_test_button, &private_data->lcd_test_button_style, LV_STATE_PRESSED);
	lv_obj_set_pos(private_data->lcd_test_button, private_data->lcd_test_str.x, private_data->lcd_test_str.y);
	lv_obj_set_size(private_data->lcd_test_button, private_data->lcd_test_str.width, private_data->lcd_test_str.height);
	lv_obj_add_event_cb(private_data->lcd_test_button, lcd_test_btn_event_handler, LV_EVENT_SHORT_CLICKED, view_data);

	private_data->lcd_test_label = lv_label_create(private_data->lcd_test_button);
	lv_label_set_text(private_data->lcd_test_label, private_data->lcd_test_str.txt);
	lv_obj_add_style(private_data->lcd_test_label, &private_data->lcd_test_str_style, LV_PART_MAIN);
	lv_obj_center(private_data->lcd_test_label);

	private_data->tmp_test_button = lv_btn_create(src);
	lv_obj_add_style(private_data->tmp_test_button, &private_data->tmp_test_button_style, 0);
	lv_obj_add_style(private_data->tmp_test_button, &private_data->tmp_test_button_style, LV_STATE_PRESSED);
	lv_obj_set_pos(private_data->tmp_test_button, private_data->tmp_test_str.x, private_data->tmp_test_str.y);
	lv_obj_set_size(private_data->tmp_test_button, private_data->tmp_test_str.width, private_data->tmp_test_str.height);
	lv_obj_add_event_cb(private_data->tmp_test_button, tmp_test_btn_event_handler, LV_EVENT_SHORT_CLICKED, view_data);

	private_data->tmp_test_label = lv_label_create(private_data->tmp_test_button);
	lv_label_set_text(private_data->tmp_test_label, private_data->tmp_test_str.txt);
	lv_obj_add_style(private_data->tmp_test_label, &private_data->tmp_test_str_style, LV_PART_MAIN);
	lv_obj_center(private_data->tmp_test_label);

	private_data->pre_test_button = lv_btn_create(src);
	lv_obj_add_style(private_data->pre_test_button, &private_data->pre_test_button_style, 0);
	lv_obj_add_style(private_data->pre_test_button, &private_data->pre_test_button_style, LV_STATE_PRESSED);
	lv_obj_set_pos(private_data->pre_test_button, private_data->pre_test_str.x, private_data->pre_test_str.y);
	lv_obj_set_size(private_data->pre_test_button, private_data->pre_test_str.width, private_data->pre_test_str.height);
	lv_obj_add_event_cb(private_data->pre_test_button, pre_test_btn_event_handler, LV_EVENT_SHORT_CLICKED, view_data);

	private_data->pre_test_label = lv_label_create(private_data->pre_test_button);
	lv_label_set_text(private_data->pre_test_label, private_data->pre_test_str.txt);
	lv_obj_add_style(private_data->pre_test_label, &private_data->pre_test_str_style, LV_PART_MAIN);
	lv_obj_center(private_data->pre_test_label);

	private_data->com_test_button = lv_btn_create(src);
	lv_obj_add_style(private_data->com_test_button, &private_data->com_test_button_style, 0);
	lv_obj_add_style(private_data->com_test_button, &private_data->com_test_button_style, LV_STATE_PRESSED);
	lv_obj_set_pos(private_data->com_test_button, private_data->com_test_str.x, private_data->com_test_str.y);
	lv_obj_set_size(private_data->com_test_button, private_data->com_test_str.width, private_data->com_test_str.height);
	lv_obj_add_event_cb(private_data->com_test_button, com_test_btn_event_handler, LV_EVENT_SHORT_CLICKED, view_data);

	private_data->com_test_label = lv_label_create(private_data->com_test_button);
	lv_label_set_text(private_data->com_test_label, private_data->com_test_str.txt);
	lv_obj_add_style(private_data->com_test_label, &private_data->com_test_str_style, LV_PART_MAIN);
	lv_obj_center(private_data->com_test_label);
#endif
	lv_refr_now(view_data->display);

error2:
out:
	return 0;
}

static int sensor_test_view_preload(view_data_t *view_data)
{
	if (sensor_test_preload_inited == 0) {
		lvgl_res_preload_scene_compact_default_init(SCENE_SENSOR_TEST_VIEW, NULL, 0,
			NULL, DEF_STY_FILE, DEF_RES_FILE, DEF_STR_FILE);
		sensor_test_preload_inited = 1;
	}

	return lvgl_res_preload_scene_compact_default(SCENE_SENSOR_TEST_VIEW, SENSOR_TEST_VIEW, 0, 0);
}

static int sensor_test_view_delete(view_data_t *view_data)
{
	m_tid = 0;
	lvgl_res_unload_scene_compact(SCENE_SENSOR_TEST_VIEW);
	return 0;
}


int _sensor_test_view_handler(uint16_t view_id, uint8_t msg_id, void *msg_data)
{
	view_data_t *view_data = msg_data;

	assert(view_id == SENSOR_TEST_VIEW);

	switch (msg_id) {
	case MSG_VIEW_PRELOAD:
		return sensor_test_view_preload(view_data);
	case MSG_VIEW_LAYOUT:
		return sensor_test_view_layout(view_data);
	case MSG_VIEW_DELETE:
		return sensor_test_view_delete(view_data);
	default:
		return 0;
	}
}

VIEW_DEFINE(sensor_test_view, _sensor_test_view_handler, NULL,
		NULL, SENSOR_TEST_VIEW, NORMAL_ORDER, UI_VIEW_LVGL, DEF_UI_VIEW_WIDTH, DEF_UI_VIEW_HEIGHT);


