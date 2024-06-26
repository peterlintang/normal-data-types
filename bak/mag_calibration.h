#ifndef __MAG_CALIBRATION_H__
#define __MAG_CALIBRATION_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"
#include "math.h"
#include "float.h"

#define BEBUG_FLAG                     (1)

#if BEBUG_FLAG
#include "stdio.h"
//#include "stm32f4xx.h"
#endif

#if BEBUG_FLAG
	#define BEBUG_LOG(...)  	printf(__VA_ARGS__);
#else
	#define BEBUG_LOG(...)  ((void)0);
#endif


#define FLOAT_MIN                      FLT_MIN 
#define FLOAT_MAX                      FLT_MAX 
#define THRESH_CALC_SUCCESSFUL_VALUES  (5.0f)
#define MAG_RAW_BUF_LEN        		     (20)
#define PP_BUF_LEN             	       (20)
#define MAG_DATA_SAVE_MAX_VALUE        (500)
#define XY_AXIS_INTERCHANGE            (1)       
#define THRESH_START_VALUES            (50.0f)
#define THRESH_STOP_VALUES             (30.0f)

typedef struct
{
  uint16_t numRows;     /**< number of rows of the matrix.     */
  uint16_t numCols;     /**< number of columns of the matrix.  */
  float    *pData;      /**< points to the data of the matrix. */
} matrix_instance_f32;

#if 0   //请将下面代码移植或重写到工程中,若可使用CMSIS-DSP可直接使用以下代码
#include "arm_math.h"
void mat_inverse_float(matrix_instance_f32 * pSrc,matrix_instance_f32 * pDst) //此函数用于32bit浮点数的逆矩阵求解 。
{ 
  arm_mat_inverse_f32((arm_matrix_instance_f32*)pSrc,(arm_matrix_instance_f32*) pDst);//此函数为CMSIS-DSP中函数，若不使用需自行实现
}

void mat_mult_float(matrix_instance_f32 * pSrcA,matrix_instance_f32 * pSrcB,matrix_instance_f32 * pDst)//此函数用于浮点数的矩阵乘法
{
  arm_mat_mult_f32((arm_matrix_instance_f32*)pSrcA,(arm_matrix_instance_f32*)pSrcB,(arm_matrix_instance_f32*)pDst);//此函数为CMSIS-DSP中函数，若不使用需自行实现
}

#endif

extern void mat_inverse_float(matrix_instance_f32 * pSrc,matrix_instance_f32 * pDst); 
extern void mat_mult_float(matrix_instance_f32 * pSrcA,matrix_instance_f32 * pSrcB,matrix_instance_f32 * pDst);

/**
  * @brief  校准初始化
  * @param  none
  * @note   仅在校准开始时调用一次
  * @retval none
  */
void mag_calibration_init(void); 


/**
  * @brief  触发校准结果计算
  * @param  none
  * @note   立即触发 
  * @retval none
  */
void mag_res_instantly_calculate(void);  


/**
  * @brief  磁力计校准
  * @param  三轴磁力计数据 {float mag_data[3]}
  * @note   周期调用计算，需一直更新mag_data输入数据 
  * @retval 0:校准失败 1：校准成功  2：校准中
  */
uint8_t mag_calculate(float *mag_data);  



/**
  * @brief  获取校准参数值
  * @param  {float param[6]}
  * @note   输入参数为数组，校准结果存在数组里
  * @retval 校准失败全部为0，校准成功数据如下：
              param[0] = mx_offset;
              param[1] = my_offset;
              param[2] = mz_offset;
              param[3] = mx_k_ratio;
              param[4] = my_k_ratio;
              param[5] = mz_k_ratio; 
  */
void get_calibration_parameter(float *param); 

/**
  * @brief  获取磁力计校准后数据
  * @param  三轴磁力计原始数据 {float mag_data[3]}
  * @note   使用指针方式，调用完后直接将结果存储在存入输入数组
  * @retval none
  */
void get_calibrat_mag_data(float *mag_data);

/**
  * @brief  获取水平状态下的偏航角
  * @param  三轴磁力计补偿后数据 {float mag_data[3]}
  * @note   仅支持水平放置下的航向角测量，非水平放置，请自行实现。
  * @retval 航向角
  */
float get_angle(float *mag_data);


#ifdef __cplusplus
}
#endif

#endif 
