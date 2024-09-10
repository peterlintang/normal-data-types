/*
 * Copyright (c) 2019 Actions Semiconductor Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file system shell
 */

#define SYS_LOG_NO_NEWLINE
#ifdef SYS_LOG_DOMAIN
#undef SYS_LOG_DOMAIN
#endif
#define SYS_LOG_DOMAIN "system_shell"

#include <os_common_api.h>
#include <mem_manager.h>
#include <stdio.h>
#include <string.h>
#include <shell/shell.h>
#include <stdlib.h>
#include <limits.h>
#include <property_manager.h>
#include <sys_wakelock.h>
#ifdef CONFIG_MEDIA_PLAYER
#include <media_player.h>
#endif

extern void mem_manager_dump_ext(int dump_detail, const char* match_value);

static int shell_set_config(const struct shell *shell,
					size_t argc, char **argv)
{
	int ret = 0;

	if (argc < 2) {
		SYS_LOG_INF("argc <\n");
		return 0;
	}

#ifdef CONFIG_PROPERTY
	if (argc < 3) {
		ret = property_set(argv[1], argv[1], 0);
	} else {
		ret = property_set(argv[1], argv[2], strlen(argv[2]));
	}
#endif
	if (ret < 0) {
		SYS_LOG_INF("%s failed %d\n", argv[1], ret);
		return -1;
	}
#ifdef CONFIG_PROPERTY
	property_flush(NULL);
#endif
	SYS_LOG_INF("%s : %s ok\n", argv[1], argv[2]);
	return 0;
}

#ifdef CONFIG_MEDIA_PLAYER
static int shell_set_effect_config(const struct shell *shell,
					size_t argc, char **argv)
{
	if (argc < 2) {
		printk("argc %d < 2\n",argc);
		return 0;
	}

    media_player_set_effect_bypass(atoi(argv[1]));

	SYS_LOG_INF("set_music_effect %s ok\n", argv[1]);
	return 0;
}

static int shell_set_voice_effect_config(const struct shell *shell,
					size_t argc, char **argv)
{
	if (argc < 2) {
		SYS_LOG_INF("argc %d < 2\n",argc);
		return 0;
	}

	media_player_set_voice_effect_bypass(atoi(argv[1]), atoi(argv[2]));

	SYS_LOG_INF("set_voice %s : %s ok\n", argv[1], argv[2]);
	return 0;
}
#endif /* CONFIG_MEDIA_PLAYER */

static int shell_dump_meminfo(const struct shell *shell,
					size_t argc, char **argv)
{
	if (argc == 1) {
		mem_manager_dump();
	} else if (argc == 2) {
		SYS_LOG_INF("argv[1] %s \n",argv[1]);
		mem_manager_dump_ext(atoi(argv[1]), NULL);
	} else if (argc == 3) {
		SYS_LOG_INF("argv[1] %s  argv[2] %s \n",argv[1], argv[2]);
		mem_manager_dump_ext(atoi(argv[1]), argv[2]);
	}

	return 0;
}

#ifdef CONFIG_SYS_WAKELOCK
static int shell_wake_lock(const struct shell *shell, size_t argc, char **argv)
{
	if (argc == 2) {
		if (!strcmp(argv[1], "lock")) {
			shell_print(shell, "wake dbg use lock\n");
			sys_wake_lock_ext(FULL_WAKE_LOCK,SYS_WAKE_LOCK_USER);
		} else if (!strcmp(argv[1], "unlock")) {
			shell_print(shell, "wake dbg use unlock\n");
			sys_wake_unlock_ext(FULL_WAKE_LOCK,SYS_WAKE_LOCK_USER);
		} else if (!strcmp(argv[1], "dump")) {
			sys_wakelocks_dump();
		} 
	}
	return 0;
}
#endif

static int shell_gps_test(const struct shell *shell, size_t argc, char **argv)
{
extern void hello_my(void);
	hello_my();
	return 0;
}

static int shell_mm_test(const struct shell *shell, size_t argc, char **argv)
{
extern void mm_init(void);
	mm_init();
	return 0;
}

//#include "MadgwickAHRS.h"
#include "EKF.h"
#include "math.h"


#if 1
//https://wenku.baidu.com/view/218c876d856a561253d36f2a.html?_wkts_=1725587903391&needWelcomeRecommand=1
//https://blog.csdn.net/Nirvana_Tai/article/details/105554473
//https://blog.csdn.net/u010097644/article/details/70881395/

#defineKp 10.0f                        // 这里的KpKi是用于调整加速度计修正陀螺仪的速度
#defineKi 0.008f
#definehalfT 0.001f             // 采样周期的一半，用于求解四元数微分方程时计算角增量
floatq0 = 1, q1 = 0, q2 = 0, q3 = 0;    // 初始姿态四元数，由上篇博文提到的变换四元数公式得来
floatexInt = 0, eyInt = 0, ezInt = 0;    //当前加计测得的重力加速度在三轴上的分量
                                //与用当前姿态计算得来的重力在三轴上的分量的误差的积分
voidIMUupdate(float gx, float gy, float gz, float ax, float ay, float az)//g表陀螺仪，a表加计
{

  float q0temp,q1temp,q2temp,q3temp;//四元数暂存变量，求解微分方程时要用
  float norm; //矢量的模或四元数的范数
  float vx, vy, vz;//当前姿态计算得来的重力在三轴上的分量
  float ex, ey, ez;//当前加计测得的重力加速度在三轴上的分量
              //与用当前姿态计算得来的重力在三轴上的分量的误差

  // 先把这些用得到的值算好
  float q0q0 = q0*q0;
  float q0q1 = q0*q1;
  float q0q2 = q0*q2;
  float q1q1 = q1*q1;
  float q1q3 = q1*q3;
  float q2q2 = q2*q2;
  float q2q3 = q2*q3;
  float q3q3 = q3*q3;
  if(ax*ay*az==0)//加计处于自由落体状态时不进行姿态解算，因为会产生分母无穷大的情况
        return;
  norm = sqrt(ax*ax + ay*ay + az*az);//单位化加速度计，
  ax = ax /norm;// 这样变更了量程也不需要修改KP参数，因为这里归一化了
  ay = ay / norm;
  az = az / norm;
  //用当前姿态计算出重力在三个轴上的分量，
  //参考坐标n系转化到载体坐标b系的用四元数表示的方向余弦矩阵第三列即是（博文一中有提到）
  vx = 2*(q1q3 - q0q2);
  vy = 2*(q0q1 + q2q3);
  vz = q0q0 - q1q1 - q2q2 + q3q3 ;
  //计算测得的重力与计算得重力间的误差，向量外积可以表示这一误差
  //原因我理解是因为两个向量是单位向量且sin0等于0
  //不过要是夹角是180度呢~这个还没理解
  ex = (ay*vz - az*vy) ;
  ey = (az*vx - ax*vz) ;
  ez = (ax*vy - ay*vx) ;

  exInt = exInt + ex * Ki;                                           //对误差进行积分
  eyInt = eyInt + ey * Ki;
  ezInt = ezInt + ez * Ki;
  // adjusted gyroscope measurements
  gx = gx + Kp*ex + exInt;  //将误差PI后补偿到陀螺仪，即补偿零点漂移
  gy = gy + Kp*ey + eyInt;
  gz = gz + Kp*ez + ezInt;    //这里的gz由于没有观测者进行矫正会产生漂移，表现出来的就是积分自增或自减
  //下面进行姿态的更新，也就是四元数微分方程的求解
  q0temp=q0;//暂存当前值用于计算
  q1temp=q1;//网上传的这份算法大多没有注意这个问题，在此更正
  q2temp=q2;
  q3temp=q3;
  //采用一阶毕卡解法，相关知识可参见《惯性器件与惯性导航系统》P212
  q0 = q0temp + (-q1temp*gx - q2temp*gy -q3temp*gz)*halfT;
  q1 = q1temp + (q0temp*gx + q2temp*gz -q3temp*gy)*halfT;
  q2 = q2temp + (q0temp*gy - q1temp*gz +q3temp*gx)*halfT;
  q3 = q3temp + (q0temp*gz + q1temp*gy -q2temp*gx)*halfT;
  //单位化四元数在空间旋转时不会拉伸，仅有旋转角度，这类似线性代数里的正交变换
  norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
  q0 = q0 / norm;
  q1 = q1 / norm;
  q2 = q2 / norm;
  q3 = q3 / norm;
  //四元数到欧拉角的转换，公式推导见博文一
  //其中YAW航向角由于加速度计对其没有修正作用，因此此处直接用陀螺仪积分代替
  Q_ANGLE.Z = GYRO_I.Z; // yaw
  Q_ANGLE.Y = asin(-2 * q1 * q3 + 2 * q0* q2)*57.3; // pitch
  Q_ANGLE.X = atan2(2 * q2 * q3 + 2 * q0 * q1,-2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
}

#endif



#if 1
float	q0 = 1, q1 = 0, q2 = 0, q3 = 0;
float	x_total = 0, y_total = 0, z_total= 0;
float oo[3] = { 0.0 };
                             
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
	printf("%s: %f %f %f, %f %f %f\n", __func__, gx, gy, gz, ax, ay, az);

  float q0temp,q1temp,q2temp,q3temp;
  float norm; 
  float vx, vy, vz;
  float ex, ey, ez;
              


  float q0q0 = q0*q0;
  float q0q1 = q0*q1;
  float q0q2 = q0*q2;
  float q1q1 = q1*q1;
  float q1q3 = q1*q3;
  float q2q2 = q2*q2;
  float q2q3 = q2*q3;
  float q3q3 = q3*q3;
//  if(ax*ay*az==0)
  if(ax == 0 && ay == 0 && az == 0)
        return;
  norm = sqrt(ax*ax + ay*ay + az*az);
  ax = ax /norm;
  ay = ay / norm;
  az = az / norm;

  vx = 2*(q1q3 - q0q2);
  vy = 2*(q0q1 + q2q3);
  vz = q0q0 - q1q1 - q2q2 + q3q3 ;

  ex = (ay*vz - az*vy) ;
  ey = (az*vx - ax*vz) ;
  ez = (ax*vy - ay*vx) ;

  x_total = x_total + ex * 0.008;
  y_total = y_total + ey * 0.008;
  z_total = z_total + ez * 0.008;

  gx = gx + 10.0*ex + x_total;
  gy = gy + 10.0*ey + y_total;
  gz = gz + 10.0*ez + z_total;
			  
  q0temp=q0;
  q1temp=q1;
  q2temp=q2;
  q3temp=q3;

  q0 = q0temp + (-q1temp*gx - q2temp*gy -q3temp*gz)*0.001;
  q1 = q1temp + (q0temp*gx + q2temp*gz -q3temp*gy)*0.001;
  q2 = q2temp + (q0temp*gy - q1temp*gz +q3temp*gx)*0.001;
  q3 = q3temp + (q0temp*gz + q1temp*gy -q2temp*gx)*0.001;

  norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
  q0 = q0 / norm;
  q1 = q1 / norm;
  q2 = q2 / norm;
  q3 = q3 / norm;


                    oo[0] = atan2(2 * (q0 * q1 + q2 * q3), 1 - 2 * (q1 * q1 + q2 * q2));
                    oo[1] = asin(2 * (q0 * q2 - q3 * q1));
                    oo[2] = atan2(2 * (q0 * q3 + q1 * q2),  1 - 2 * (q2 * q2 + q3 * q3));
		    #define M_PI        3.14159265358979323846

                    // Conversione da radianti a gradi
                    oo[0] *= (float)((float)180.0 / (float)M_PI);
                    oo[1] *= (float)((float)180.0 / (float)M_PI);
                    oo[2] *= (float)((float)180.0 / (float)M_PI);


  printf("angle: %f %f %f\n", oo[0], oo[1], oo[2]);						    

										    
}

void oo_get_(float out[3])
{
	out[0] = oo[0];
	out[1] = oo[1];
	out[2] = oo[2];
}

#endif

static int quit_flag = 0;
static void aa_calibration_thread(void *p1,void *p2,void *p3)
{
	float acc_mg[3] = { 0, 0 , 0 };
	float ang_mdps[3] = { 0, 0, 0 };
//	float euler_ekf[3];
	ekf_t ekf;
//	float yaw= 0.0;
//	float pitch= 0.0;
//	float roll= 0.0;
    	float mag_data[3]={0};//存储磁数据
	extern void lsm6dsv16x_read_data_polling(float *acc_mg, float *ang_mdps);
	extern void lsm6dsv16x_init(void);
	extern void VCM1195L_init(void);
	extern uint8_t VCM119xL_ReadData(float *mag);

	lsm6dsv16x_init();
    	VCM1195L_init();
//	VCM119xL_ReadData(mag_data);
//	EKF_init(&ekf, mag_data[2], mag_data[1], mag_data[0], 0.1, 1, 100);
	k_msleep(1000 * 8);
	float axsum = 0;
	float aysum = 0;
	float azsum = 0;
	float gxsum = 0;
	float gysum = 0;
	float gzsum = 0;
	float axoff = 0;
	float ayoff = 0;
	float azoff = 0;
	float gxoff = 0;
	float gyoff = 0;
	float gzoff = 0;
	
	for (int i = 0; i < 200; i++)
	{
		k_msleep(2);
		lsm6dsv16x_read_data_polling(acc_mg, ang_mdps);
		axsum = axsum + acc_mg[0] / 1000.0;
		aysum = aysum + acc_mg[1] / 1000.0;
		azsum = azsum + (acc_mg[2] - 1000) / 1000.0;
		gxsum = gxsum + ang_mdps[0] / 1000.0;
		gysum = gysum + ang_mdps[1] / 1000.0;
		gzsum = gzsum + ang_mdps[2] / 1000.0;
		printf("sum: %f %f %f, %f %f %f\n", axsum, aysum, azsum, gxsum, gysum, gzsum);
	}

	axoff = axsum / 200;
	ayoff = aysum / 200;
	azoff = azsum / 200;

	gxoff = gxsum / 200;
	gyoff = gysum / 200;
	gzoff = gzsum / 200;
	

		printf("off: %f %f %f, %f %f %f\n", axoff, ayoff, azoff, gxoff, gyoff, gzoff);

		extern void MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
	while (quit_flag != 1)
	{
		k_msleep(2);
	//	VCM119xL_ReadData(mag_data);
		lsm6dsv16x_read_data_polling(acc_mg, ang_mdps);
	//	EKF_update(&ekf, euler_ekf, acc_mg[0], acc_mg[1], acc_mg[2], ang_mdps[0], ang_mdps[1], ang_mdps[2], mag_data[2], mag_data[1], mag_data[0], 0.01);
//		yaw = euler_ekf[0] * 180.0f / 3.14159265f;
//		pitch = euler_ekf[1] * 180.0f / 3.14159265f;
//		roll = euler_ekf[2] * 180.0f / 3.14159265f;
//		printf("aaa: %f %f %f\n", acc_mg[0] - axoff, acc_mg[1] - ayoff, acc_mg[2] - azoff);
//		printf("bbb: %f %f %f\n", ang_mdps[0] - gxoff, ang_mdps[1] - gyoff, ang_mdps[2] - gzoff);
		IMUupdate(
				(ang_mdps[0] / 1000.0 - gxoff) * 0.0174532925, 
				(ang_mdps[1] / 1000.0 - gyoff) * 0.0174532925, 
				(ang_mdps[2] / 1000.0 - gzoff) * 0.0174532925 ,
				acc_mg[0] / 1000.0 - axoff, 
				acc_mg[1] / 1000.0 - ayoff, 
				acc_mg[2] / 1000.0 - azoff); 
//		MadgwickAHRSupdate(ang_mdps[0] - gxoff, ang_mdps[1] - gyoff, ang_mdps[2] - gzoff, acc_mg[0] - axoff, acc_mg[1] - ayoff, acc_mg[2] - azoff, 0, 0, 0 );
		//printf("yaw: %f, pitch: %f, roll: %f\n", yaw, pitch, roll);
	}
}

static k_tid_t m_tid = NULL;
K_THREAD_STACK_DEFINE(aa_stack_area, 2048);
static struct k_thread thread_data;
void aa_thread_init(void)
{
	if (m_tid == NULL)
	{
		//extern void lsm6dsv16x_init(void);
		//lsm6dsv16x_init();
		m_tid = k_thread_create(&thread_data, aa_stack_area,
				K_THREAD_STACK_SIZEOF(aa_stack_area),
				aa_calibration_thread, NULL, NULL, NULL,
				15, 0, K_NO_WAIT);
		k_thread_name_set(m_tid, "aa_thread");
	}
	else
	{
		SYS_LOG_INF("%s: already started", __func__);
	}
}

int shell_aa_test_exit(void)
{
	quit_flag = 1;
	return 0;
}

int shell_aa_test(const struct shell *shell, size_t argc, char **argv)
{
	quit_flag = 0;
	aa_thread_init();
	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_system,
	SHELL_CMD(dumpmem, NULL, "dump mem info.", shell_dump_meminfo),
	SHELL_CMD(set_config, NULL, "set system config ", shell_set_config),
#ifdef CONFIG_MEDIA_PLAYER
	SHELL_CMD(set_voice_effect, NULL, "set voice effect bypass ", shell_set_voice_effect_config),
	SHELL_CMD(set_music_effect, NULL, "set music effect bypass ", shell_set_effect_config),
#endif
#ifdef CONFIG_SYS_WAKELOCK
	SHELL_CMD(wlock, NULL, "wlock lock[unlock] ", shell_wake_lock),
#endif
	SHELL_CMD(gps_test, NULL, "wlock lock[unlock] ", shell_gps_test),
	SHELL_CMD(mm_test, NULL, "wlock lock[unlock] ", shell_mm_test),
	SHELL_CMD(aa_test, NULL, "wlock lock[unlock] ", shell_aa_test),
	SHELL_SUBCMD_SET_END /* Array terminated. */
);

SHELL_CMD_REGISTER(system, &sub_system, "system commands", NULL);
