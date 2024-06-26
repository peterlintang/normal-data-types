/****************************包含头文件******************************/

#include "mag_calibration.h"
/************************宏定义**********************************/
#define PI                             (3.14159265358979f)
#define CALIBRATION_SUCCESS            (1) //校准成功
#define CALIBRATION_FAILURE            (0) //校准失败
#define UNDER_CALIBRATION              (2) //正在校准
/**************************函数声明********************************/

float mag_senser_data_buff[MAG_DATA_SAVE_MAX_VALUE][3]={0};
float get_PP (float *dMod, int size, int count);
float get_PP_InWindow(int size, int count);
void Algo_Update_Mag(float MagX, float MagY, float MagZ);
void MAG_Calculate(void);
float calculateMagnitude(float x, float y, float z);
void get_calibration_parameter(float *para);
void axis_interchange(float *data);
uint8_t calibration_init = 0;
uint8_t instantly_calculate_flg = 0;
/****************************变量定义******************************/

float mx_offset,my_offset,mz_offset,mx_k,my_k,mz_k,mx_k_ratio,my_k_ratio,mz_k_ratio;
float mx=0.0f,my=0.0f,mz=0.0f;
char Mag_Clf=0;
uint8_t calibration_result = UNDER_CALIBRATION;  

static float	RAW_X[MAG_RAW_BUF_LEN]	= {0};
static float	RAW_Y[MAG_RAW_BUF_LEN]	= {0};
static float	RAW_Z[MAG_RAW_BUF_LEN]	= {0};

typedef enum 
{
   free=0,
   start_calculate,
   calc_offset_k_value,
   get_result,
   stop_calculate
}work_step_enum;

/******************************函数体*************************************/

float get_PP (float *dMod, int size, int count)
{
    int i;
    float max, min;
    max = min = dMod[size - count];
    for (i = 0; i < count; i++)
    {
        max = (dMod[size - count + i] > max) ? dMod[size - count + i] : max;
        min = (dMod[size - count + i] < min) ? dMod[size - count + i] : min;
    }
    return (max - min);
}

float get_PP_InWindow(int size, int count)
{
    float PPV_X, PPV_Y, PPV_Z, PPV_InWindow;
    PPV_X = get_PP (RAW_X, size, count);
    PPV_Y = get_PP (RAW_Y, size, count);
    PPV_Z = get_PP (RAW_Z, size, count);
    PPV_InWindow = (PPV_X >= PPV_Y ? PPV_X >= PPV_Z ? PPV_X : PPV_Z : PPV_Y >= PPV_Z ? PPV_Y : PPV_Z);
    return PPV_InWindow;
}

void Algo_Update_Mag(float MagX, float MagY, float MagZ)
{
    int i	= 0;
    for(i = 0; i < MAG_RAW_BUF_LEN - 1; i++)
    {
        RAW_X[i] = RAW_X[i + 1];
        RAW_Y[i] = RAW_Y[i + 1];
        RAW_Z[i] = RAW_Z[i + 1];
    }
    RAW_X[MAG_RAW_BUF_LEN - 1] = MagX;
    RAW_Y[MAG_RAW_BUF_LEN - 1] = MagY;
    RAW_Z[MAG_RAW_BUF_LEN - 1] = MagZ;
}

//模值计算
float calculateMagnitude(float x, float y, float z) 
  {
      float magnitude;
      magnitude = sqrt(x*x + y*y + z*z);
      return magnitude;
  }
  
void mag_calibration_init(void)
{
   calibration_init = 1;
}
void mag_res_instantly_calculate(void)
{
  instantly_calculate_flg = 1;
}
void norm_sort(float *out,uint16_t n)
{
	uint16_t i,j;
	float temp = 0;
	for(i=0;i<n;i++)
	{
	  for(j=i+1;j<n;j++)
	  {
		if(out[i]<out[j])
		{
		  temp = out[i];
		  out[i] = out[j];
		  out[j] = temp;				
		}			
	 }		
	}
	
}
uint8_t mag_calculate(float *mag_data)
{
  static work_step_enum task_step = free;
  static uint16_t  mag_senser_save_cnt = 0;
  float norm_buff[MAG_DATA_SAVE_MAX_VALUE]={0};
  float max_value = FLOAT_MIN; // 初始化最大值为float的最小值
  float min_value = FLOAT_MAX; // 初始化最小值为float的最大值
  float norm = 0.0,norm_after = 0.0;    //存储模值
  float mag_cal[3] = {0}; 
#if XY_AXIS_INTERCHANGE
    axis_interchange(mag_data);
#endif 
  if(calibration_init)  //进行了初始化，跳转到初始化任务
  {
    Mag_Clf = 0;   
    mx = mag_data[0];
    my = mag_data[1];
    mz = mag_data[2];    
    MAG_Calculate();
    task_step = free;
    for(uint8_t i = 0; i < MAG_RAW_BUF_LEN; i++)
    {
        RAW_X[i] = mag_data[0];
        RAW_Y[i] = mag_data[1];
        RAW_Z[i] = mag_data[2];
    }
  }
  if(instantly_calculate_flg)  //如果需要立即计算，跳转到计算步骤
  {
    if(calibration_result == UNDER_CALIBRATION) //校准中
    {
      task_step = calc_offset_k_value;
    }
    instantly_calculate_flg  = 0;   
  }  
  Algo_Update_Mag(mag_data[0],mag_data[1],mag_data[2]); 
  switch (task_step)
  {
    case free:   //空闲状态进行判断
      calibration_result = UNDER_CALIBRATION;
//	  norm = calculateMagnitude(mag_data[0],mag_data[1],mag_data[2]);  //三轴磁力计模值计算
//	  BEBUG_LOG("free norm=%f\r\n",norm);
	  BEBUG_LOG("free PP=%f\r\n",get_PP_InWindow(MAG_RAW_BUF_LEN, PP_BUF_LEN));
      if (get_PP_InWindow(MAG_RAW_BUF_LEN, PP_BUF_LEN) > THRESH_START_VALUES)  //启动计算（1、阈值大于50）
      {
        task_step = start_calculate;
        mag_senser_save_cnt = 0;
        Mag_Clf = 0;   //进行数据存储
        BEBUG_LOG("start calc\r\n");
      }
    break;
    
    case start_calculate:  //满足条件开始计算 
//      norm = calculateMagnitude(mag_data[0],mag_data[1],mag_data[2]);  //三轴磁力计模值计算
//	  BEBUG_LOG("start norm=%f\r\n",norm); 
      BEBUG_LOG("start PP=%f\r\n",get_PP_InWindow(MAG_RAW_BUF_LEN, PP_BUF_LEN));   
      if(get_PP_InWindow(MAG_RAW_BUF_LEN, PP_BUF_LEN) < THRESH_STOP_VALUES) //停止计算(1、阈值小于5；2、采集数据大于500条)
      {
        task_step = calc_offset_k_value;
        BEBUG_LOG("stop calc-pp\r\n");
      }
      if(mag_senser_save_cnt == MAG_DATA_SAVE_MAX_VALUE)  //如果超过500条也进行计算
      {
        task_step = calc_offset_k_value;
        BEBUG_LOG("stop calc-cnt\r\n");
      }
      if(mag_senser_save_cnt<MAG_DATA_SAVE_MAX_VALUE)
      {
        mag_senser_data_buff[mag_senser_save_cnt][0] = mag_data[0];
        mag_senser_data_buff[mag_senser_save_cnt][1] = mag_data[1];
        mag_senser_data_buff[mag_senser_save_cnt][2] = mag_data[2];
        mag_senser_save_cnt++;
      }
      mx = mag_data[0];
      my = mag_data[1];
      mz = mag_data[2];    
      MAG_Calculate();
    BEBUG_LOG("stop=%3.4f %3.4f %3.4f %3.4f\r\n",mx, my, mz,get_PP_InWindow(MAG_RAW_BUF_LEN, PP_BUF_LEN));          
    break;
    case calc_offset_k_value:
      Mag_Clf = 1;
      MAG_Calculate(); 
      task_step = get_result;    
    break;
      
    case get_result: //满足条件开始计算  
      for (uint16_t i = 0; i < mag_senser_save_cnt; i++) 
      {
        mag_cal[0] = (mag_senser_data_buff[i][0]-mx_offset)*mx_k_ratio;
        mag_cal[1] = (mag_senser_data_buff[i][1]-my_offset)*my_k_ratio;
        mag_cal[2] = (mag_senser_data_buff[i][2]-mz_offset)*mz_k_ratio; 
        norm_after = calculateMagnitude(mag_senser_data_buff[i][0],mag_senser_data_buff[i][1],mag_senser_data_buff[i][2]);  //三轴磁力计模值计算		
        norm = calculateMagnitude(mag_cal[0],mag_cal[1],mag_cal[2]);  //三轴磁力计模值计算
        norm_buff[i] = norm;		
        if (norm > max_value) 
        {
          max_value = norm;
        }
		
        if (norm < min_value) 
        {
          min_value = norm;
        }                
         BEBUG_LOG("norm=%f,%f,%f,%f\r\n",norm_after,norm,max_value,min_value);
     }
	 norm_sort(norm_buff,mag_senser_save_cnt);
	 for (uint16_t i = 0; i < mag_senser_save_cnt; i++)
	 {
		 BEBUG_LOG("norm_buff[%d]=%f\r\n",i,norm_buff[i]);		 
	 }		 
      
	  /*
      if((max_value-min_value)>THRESH_CALC_SUCCESSFUL_VALUES)
      { 
        calibration_result = CALIBRATION_FAILURE;
        task_step = stop_calculate;       
        BEBUG_LOG("Magnetometer calibration failed\r\n");
        BEBUG_LOG("Peak-valley value = %f\r\n",max_value-min_value);
        return calibration_result;
      }
      else
      {
        calibration_result = CALIBRATION_SUCCESS;
        task_step = stop_calculate;       
        BEBUG_LOG("Magnetometer calibration successful\r\n");
        BEBUG_LOG("Peak-valley value = %f\r\n",max_value-min_value);
        return calibration_result;
      }
	  */
	  uint16_t temp = 0;
	  if(mag_senser_save_cnt>100)
	  {
		temp =  mag_senser_save_cnt/100*5;
	  }
	  else
	  {
		temp = 2;  
	  }
	  if( ((norm_buff[temp]-norm_buff[mag_senser_save_cnt-temp-1])>THRESH_CALC_SUCCESSFUL_VALUES) || (mag_senser_save_cnt <=50))
      { 
        calibration_result = CALIBRATION_FAILURE;
        task_step = stop_calculate;       
        BEBUG_LOG("Magnetometer calibration failed - cnt_err or norm_err \r\n");
        BEBUG_LOG("Peak-valley value = %f\r\n",max_value-min_value);
		BEBUG_LOG("value2 = %f\r\n",norm_buff[temp]-norm_buff[mag_senser_save_cnt-temp-1]);
		BEBUG_LOG("mag_senser_save_cnt = %d,%d\r\n",mag_senser_save_cnt,temp);
        return calibration_result;
      }
      else
      {
		if((mx_k_ratio>=0.96)&&(mx_k_ratio<=1.04)&&(my_k_ratio>=0.96)&&(my_k_ratio<=1.04)&&(mz_k_ratio>=0.96)&&(mz_k_ratio<=1.04)) 
		{
		  calibration_result = CALIBRATION_SUCCESS;
          task_step = stop_calculate;       
          BEBUG_LOG("Magnetometer calibration successful\r\n");
         BEBUG_LOG("Peak-valley value = %f\r\n",max_value-min_value);
		 BEBUG_LOG("value2 = %f,%d\r\n",norm_buff[temp]-norm_buff[mag_senser_save_cnt-temp-1],temp);
		 BEBUG_LOG("mag_senser_save_cnt = %d\r\n",mag_senser_save_cnt);
         return calibration_result;				
		}
		else
		{
		  calibration_result = CALIBRATION_FAILURE;
		  task_step = stop_calculate;       
		  BEBUG_LOG("Magnetometer calibration failed - k_err\r\n");
		  BEBUG_LOG("k_err=%f,%f,%f\r\n",mx_k_ratio,my_k_ratio,mz_k_ratio);
		  return calibration_result;			
		}

      }
      mag_senser_save_cnt =0;
    break;  
    
    case stop_calculate:
    break;
    
    default:
    break;
  }
  return calibration_result;   
}

void axis_interchange(float *data)
{
  float mag_x,mag_y,mag_z;
  mag_x = data[0];
  mag_y = data[1];
  mag_z = data[2];
  data[0] = mag_y * -1.0f; 
  data[1] = mag_x * -1.0f; 
  data[2] = mag_z * -1.0f; ;
}


void get_calibration_parameter(float *param)
{
  if(calibration_result == CALIBRATION_SUCCESS)
  {
    param[0] = mx_offset;
    param[1] = my_offset;
    param[2] = mz_offset;
    param[3] = mx_k_ratio;
    param[4] = my_k_ratio;
    param[5] = mz_k_ratio;
  }
  else
  {
    param[0] = 0.0f;
    param[1] = 0.0f;
    param[2] = 0.0f;
    param[3] = 0.0f;
    param[4] = 0.0f;
    param[5] = 0.0f;
  }

}

void MAG_Calculate(void)
{
    static float n=0.0f;
	//椭球校准
	static float  
	//一次项
	x_sum=0.0f,			  
	y_sum=0.0f,			  
	z_sum=0.0f,			  
	//二次项			  	 
	xx_sum=0.0f,				  	 
	yy_sum=0.0f,				  	 					  
	zz_sum=0.0f,				       
	xy_sum=0.0f,   			       
	xz_sum=0.0f,   			      
	yz_sum=0.0f,   			  
	//三次项  			  
	xxx_sum=0.0f,			  
	xxy_sum=0.0f,			  
	xxz_sum=0.0f,			  
	xyy_sum=0.0f, 			  
	xzz_sum=0.0f, 			  			  
	yyy_sum=0.0f,				  
	yyz_sum=0.0f,				                           
	yzz_sum=0.0f,	 			                           
	zzz_sum=0.0f,	 			  
	//四次项				  	
	yyyy_sum=0.0f,			  	
	zzzz_sum=0.0f,			  	
	xxyy_sum=0.0f,			  	
	xxzz_sum=0.0f,			  
	yyzz_sum=0.0f;
 
	static float  
	//一次项
	x_avr=0.0f,			  
	y_avr=0.0f,			  
	z_avr=0.0f,			  
	//二次项			  	 
	xx_avr=0.0f,				  	 
	yy_avr=0.0f,				  	 					  
	zz_avr=0.0f,				       
	xy_avr=0.0f,   			       
	xz_avr=0.0f,   			      
	yz_avr=0.0f,   			  
	//三次项  			  
	xxx_avr=0.0f,			  
	xxy_avr=0.0f,			  
	xxz_avr=0.0f,			  
	xyy_avr=0.0f, 			  
	xzz_avr=0.0f, 			  			  
	yyy_avr=0.0f,				  
	yyz_avr=0.0f,				                           
	yzz_avr=0.0f,	 			                           
	zzz_avr=0.0f,	 			  
	//四次项				  	
	yyyy_avr=0.0f,			  	
	zzzz_avr=0.0f,			  	
	xxyy_avr=0.0f,			  	
	xxzz_avr=0.0f,			  
	yyzz_avr=0.0f;
  if(calibration_init)
  {
    calibration_init = 0;
    n=0.0f;
    //椭球校准
    //一次项
    x_sum=0.0f,			  
    y_sum=0.0f,			  
    z_sum=0.0f,			  
    //二次项			  	 
    xx_sum=0.0f,				  	 
    yy_sum=0.0f,				  	 					  
    zz_sum=0.0f,				       
    xy_sum=0.0f,   			       
    xz_sum=0.0f,   			      
    yz_sum=0.0f,   			  
    //三次项  			  
    xxx_sum=0.0f,			  
    xxy_sum=0.0f,			  
    xxz_sum=0.0f,			  
    xyy_sum=0.0f, 			  
    xzz_sum=0.0f, 			  			  
    yyy_sum=0.0f,				  
    yyz_sum=0.0f,				                           
    yzz_sum=0.0f,	 			                           
    zzz_sum=0.0f,	 			  
    //四次项				  	
    yyyy_sum=0.0f,			  	
    zzzz_sum=0.0f,			  	
    xxyy_sum=0.0f,			  	
    xxzz_sum=0.0f,			  
    yyzz_sum=0.0f;
  
    //一次项
    x_avr=0.0f,			  
    y_avr=0.0f,			  
    z_avr=0.0f,			  
    //二次项			  	 
    xx_avr=0.0f,				  	 
    yy_avr=0.0f,				  	 					  
    zz_avr=0.0f,				       
    xy_avr=0.0f,   			       
    xz_avr=0.0f,   			      
    yz_avr=0.0f,   			  
    //三次项  			  
    xxx_avr=0.0f,			  
    xxy_avr=0.0f,			  
    xxz_avr=0.0f,			  
    xyy_avr=0.0f, 			  
    xzz_avr=0.0f, 			  			  
    yyy_avr=0.0f,				  
    yyz_avr=0.0f,				                           
    yzz_avr=0.0f,	 			                           
    zzz_avr=0.0f,	 			  
    //四次项				  	
    yyyy_avr=0.0f,			  	
    zzzz_avr=0.0f,			  	
    xxyy_avr=0.0f,			  	
    xxzz_avr=0.0f,			  
    yyzz_avr=0.0f;
  }

	//矩阵定义
	static float                   A[36]   ,      A_inv [36];  //系数矩阵
	static matrix_instance_f32 A_matrix,      A_inv_matrix;	
	static float                   B[6];                       //非齐次项
	static matrix_instance_f32 B_matrix;
	static float                   Par[6];                     //需要拟合的参数
	static matrix_instance_f32 Par_matrix;
	
	switch(Mag_Clf)
	{
		case 0:
		{
			x_sum   =x_sum   +(mx); 
			y_sum   =y_sum   +(my); 
			z_sum   =z_sum   +(mz); 
			xx_sum  =xx_sum  +(mx*mx); 	   
			yy_sum  =yy_sum  +(my*my); 
			zz_sum  =zz_sum  +(mz*mz); 
			xy_sum  =xy_sum  +(mx*my); 
			xz_sum  =xz_sum  +(mx*mz); 
			yz_sum  =yz_sum  +(my*mz); 
			xxx_sum =xxx_sum +(mx*mx*mx); 					
			xxy_sum =xxy_sum +(mx*mx*my); 				
			xxz_sum =xxz_sum +(mx*mx*mz); 				
			xyy_sum =xyy_sum +(mx*my*my); 
			xzz_sum =xzz_sum +(mx*mz*mz); 
			yyy_sum =yyy_sum +(my*my*my);
			yyz_sum =yyz_sum +(my*my*mz);
			yzz_sum =yzz_sum +(my*mz*mz);
			zzz_sum =zzz_sum +(mz*mz*mz);
			yyyy_sum=yyyy_sum+(my*my*my*my);
			zzzz_sum=zzzz_sum+(mz*mz*mz*mz);
			xxyy_sum=xxyy_sum+(mx*mx*my*my);		   
			xxzz_sum=xxzz_sum+(mx*mx*mz*mz);		   
			yyzz_sum=yyzz_sum+(my*my*mz*mz);		   
			n=n+1.0f;
			break;
		}
		case 1://各次累加和统计
		{
			//矩阵初始化	
			A_matrix.numRows=6;   A_inv_matrix.numRows=6;     B_matrix.numRows=6;   Par_matrix.numRows=6;
			A_matrix.numCols=6;   A_inv_matrix.numCols=6;     B_matrix.numCols=1;   Par_matrix.numCols=1;
			A_matrix.pData  =A;   A_inv_matrix.pData  =A_inv; B_matrix.pData  =B;   Par_matrix.pData  =Par;
		
			//各次均值统计
			x_avr   =x_sum/n; 
			y_avr   =y_sum/n; 
			z_avr   =z_sum/n; 
			xx_avr  =xx_sum/n; 
			yy_avr  =yy_sum/n; 			
			zz_avr  =zz_sum/n; 			
			xy_avr  =xy_sum/n; 			
			xz_avr  =xz_sum/n; 			
			yz_avr  =yz_sum/n; 					
			xxx_avr =xxx_sum/n; 		
			xxy_avr =xxy_sum/n; 
			xxz_avr =xxz_sum/n; 
			xyy_avr =xyy_sum/n; 
			xzz_avr =xzz_sum/n; 
			yyy_avr =yyy_sum/n;			
			yyz_avr =yyz_sum/n;						
			yzz_avr =yzz_sum/n;										 	
			zzz_avr =zzz_sum/n;	
			yyyy_avr=yyyy_sum/n;
			zzzz_avr=zzzz_sum/n;
			xxyy_avr=xxyy_sum/n;
			xxzz_avr=xxzz_sum/n;
			yyzz_avr=yyzz_sum/n;							
																			
			//系数矩阵计算
			A[0 ]=yyyy_avr;A[1 ]=yyzz_avr;A[2 ]=xyy_avr;A[3 ]=yyy_avr;A[4 ]=yyz_avr; A[5 ]=yy_avr;
			A[6 ]=yyzz_avr;A[7 ]=zzzz_avr;A[8 ]=xzz_avr;A[9 ]=yzz_avr;A[10]=zzz_avr; A[11]=zz_avr;
			A[12]=xyy_avr ;A[13]=xzz_avr ;A[14]=xx_avr ;A[15]=xy_avr ;A[16]=xz_avr ; A[17]=x_avr ;
			A[18]=yyy_avr ;A[19]=yzz_avr ;A[20]=xy_avr ;A[21]=yy_avr ;A[22]=yz_avr ; A[23]=y_avr ;
			A[24]=yyz_avr ;A[25]=zzz_avr ;A[26]=xz_avr ;A[27]=yz_avr ;A[28]=zz_avr ; A[29]=z_avr ;
			A[30]=yy_avr  ;A[31]=zz_avr  ;A[32]=x_avr  ;A[33]=y_avr  ;A[34]=z_avr  ; A[35]=1     ;
			
			//非齐次列向量赋值
			B[0]=-xxyy_avr;
			B[1]=-xxzz_avr;
			B[2]=-xxx_avr ;
			B[3]=-xxy_avr ;
			B[4]=-xxz_avr ;
			B[5]=-xx_avr  ;
 
			//系数矩阵求逆
			mat_inverse_float(&A_matrix,&A_inv_matrix);
			
			//解方程组得出拟合参数
			mat_mult_float(&A_inv_matrix,&B_matrix,&Par_matrix);
			
			//计算椭球参数
			mx_offset=(-Par[2]/2.0f);             //拟合出的x轴中心坐标
			my_offset=(-Par[3]/(2.0f*Par[0]));    //拟合出的y轴中心坐标
			mz_offset=(-Par[4]/(2.0f*Par[1]));    //拟合出的z轴中心坐标
			mx_k= sqrt(mx_offset*mx_offset + Par[0]*my_offset*my_offset + Par[1]*mz_offset*mz_offset - Par[5]);   //拟合出的x方向上的轴半径
			my_k= mx_k/sqrt(Par[0]);                                                                              //拟合出的y方向上的轴半径
			mz_k= mx_k/sqrt(Par[1]);                                                                              //拟合出的z方向上的轴半径
			mx_k_ratio = 1.0f/1.0f;
			my_k_ratio = 1.0f/(my_k/mx_k);
			mz_k_ratio = 1.0f/(mz_k/mx_k);
//       HAL_Delay(20); 
			BEBUG_LOG(" x0=%f\r\n y0=%f\r\n z0=%f\r\n A=%f\r\n B=%f\r\n C=%f\r\n AK=%f\r\n BK=%f\r\n CK=%f\r\n\r\n",mx_offset,my_offset,mz_offset,mx_k,my_k,mz_k,mx_k_ratio,my_k_ratio,mz_k_ratio);
//			 HAL_Delay(20); 
       Mag_Clf = 0;
			break;
		}
		default :break;
	}
}

void get_calibrat_mag_data(float *mag_data)
{
  
#if XY_AXIS_INTERCHANGE
    axis_interchange(mag_data);
#endif 
  
  mag_data[0] = (mag_data[0]-mx_offset)*mx_k_ratio;
  mag_data[1] = (mag_data[1]-my_offset)*my_k_ratio;
  mag_data[2] = (mag_data[2]-mz_offset)*mz_k_ratio;
}

float get_angle(float *mag_data)
{
  float yaw = 0.0f;
  yaw = atan2(mag_data[1],mag_data[0])* 180.0/PI;
  return yaw;
}
