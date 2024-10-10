
/*
加计和陀螺仪都能计算出姿态，但为何要对它们融合呢，是因为加速度计对振动之类的扰动很敏感，
但长期数据计算出的姿态可信，而陀螺仪虽然对振动这些不敏感，但长期使用陀螺仪会出现漂移，
因此我们要进行互补，短期相信陀螺，长期相信加计。不过，其实加计无法对航向角进行修正，
修正航向角需要磁力计。

在融合之前先要对传感器原始数据进行一些处理。理想情况下，加速度计水平放置时，XY轴应该是
0输出的，仅Z轴输出1个G，因此，我们需要对加速度计进行XY轴的零点校准（注意Z轴可不能一起校
准去了~）；同样的，陀螺仪在水平静止放置时各轴输出应为0，因此需对陀螺仪进行三轴的校准。
方法就是把机体标准水平静止放置时采集它个一两百次数据求个平均作为校准值保存起来喽，然后
工作状态下各轴输出的数据就是采集来的数据减去校准值喽。仅此还不够，陀螺仪不进行滤波还可
以接受，但加速度计噪声比较大，所以至少也得来个滑动窗口滤波，我用了20深度的滑动窗口，数
据还是有很大波动，不过最后计算出的姿态角只有0.3度左右的抖动（我看大家一般都是建议8深度
就够了，所以单滑动窗口滤波效果是没法做到更好了。

互补滤波是经典的滤波方法，相比卡尔曼滤波的速度要快、计算量小，但是效果不如卡尔曼。
这个文件里实现的是互补滤波。

注意本文使用的互补滤波方法是修正角速度：先求出陀螺仪角度和加速度计角度的偏差，再把该误
差的比例和积分项用于修正角速度，类似PID控制用误差去修正输入控制量。

另有一种更容易理解的方法是修正角度：
陀螺积分角度+=角速度*dt；融合角度=陀螺权值*陀螺积分角度+(1-陀螺权值)*加速度角度；
但是，实际更新四元数需要的是角速度，所以这种修正角度的方法并不实用。

*/

#include <math.h>


#define Kp 10.0f                  // 这里的KpKi是用于调整加速度计修正陀螺仪的速度
#define Ki 0.008f

#define halfT 0.05f             // 采样周期的一半，用于求解四元数微分方程时计算角增量

float q0 = 1, q1 = 0, q2 = 0, q3 = 0;    // 初始位置姿态角为：0、0、0，对应四元数为：1、0、0、0

float exInt = 0, eyInt = 0, ezInt = 0;    
//重力加速度在三轴上的分量与用当前姿态计算得来的重力在三轴上的分量的误差的积分

float  Q_ANGLE_X= 0, Q_ANGLE_Y = 0, Q_ANGLE_Z = 0;   

//互补滤波函数
//输入参数：g表陀螺仪角速度(弧度/s)，a表加计（m/s2或g都可以，会归一化）
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{

	float q0temp,q1temp,q2temp,q3temp;//四元数暂存变量，求解微分方程时要用
	float norm; //矢量的模或四元数的范数
	float vx, vy, vz;//当前姿态计算得来的重力在三轴上的分量
	float ex, ey, ez;//当前加计测得的重力加速度在三轴上的分量
	//与用当前姿态计算得来的重力在三轴上的分量的误差

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
	ax = ax / norm;// 这样变更了量程也不需要修改KP参数，因为这里归一化了
	ay = ay / norm;
	az = az / norm;
	
	//用当前姿态计算出重力在三个轴上的分量，重力在n系下是[0,0,g]，乘以转换矩阵就转到b系
	//参考坐标n系转化到载体坐标b系，用四元数表示的方向余弦矩阵第三列即是
	vx = 2*(q1q3 - q0q2);
	vy = 2*(q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3 ;
	
	//计算测得的重力与计算得重力间的误差，这个误差是通过向量外积（也就是叉乘）求出来的
	ex = (ay*vz - az*vy) ;
	ey = (az*vx - ax*vz) ;
	ez = (ax*vy - ay*vx) ;

	exInt = exInt + ex * Ki;  //对误差进行积分
	eyInt = eyInt + ey * Ki;
	ezInt = ezInt + ez * Ki;
	
	gx = gx + Kp*ex + exInt;  //将误差PI（比例和积分项）补偿到陀螺仪角速度
	gy = gy + Kp*ey + eyInt;
	gz = gz + Kp*ez + ezInt;  //没有磁力计的话就无法修正偏航角
	
	//下面进行姿态的更新，也就是四元数微分方程的求解
	q0temp=q0;
	q1temp=q1;
	q2temp=q2;
	q3temp=q3;
	//采用一阶解法更新四元数
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
	
	//四元数到欧拉角的转换
	//其中YAW航向角由于加速度计对其没有修正作用，因此也可以直接用陀螺仪积分代替
	Q_ANGLE_Z = Q_ANGLE_Z + gz*halfT*2*57.3; // yaw
	//Q_ANGLE_Z = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;
	Q_ANGLE_Y = asin(-2 * q1 * q3 + 2 * q0* q2)*57.3; // pitch
	Q_ANGLE_X = atan2(2 * q2 * q3 + 2 * q0 * q1,-2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
}


void AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz) 
{  
						float q0temp,q1temp,q2temp,q3temp;//四元数暂存变量，求解微分方程时要用
            float norm;               
            float hx, hy, hz, bx, bz;
            float vx, vy, vz, wx, wy, wz; 
            float ex, ey, ez;  
  
            // 定义一些辅助变量用于转换矩阵
            float q0q0 = q0*q0;  
            float q0q1 = q0*q1;  
            float q0q2 = q0*q2;  
            float q0q3 = q0*q3;  
            float q1q1 = q1*q1;  
            float q1q2 = q1*q2;  
            float q1q3 = q1*q3;  
            float q2q2 = q2*q2;  
            float q2q3 = q2*q3;  
            float q3q3 = q3*q3;  
             
            // 归一化加速度计和地磁计的度数 
            norm = sqrt(ax*ax + ay*ay + az*az);   
            ax = ax / norm;  
            ay = ay / norm;  
            az = az / norm;  
            norm = sqrt(mx*mx + my*my + mz*mz);   
            mx = mx / norm;  
            my = my / norm;  
            mz = mz / norm;  
             
			
            //将b系中的地磁计分量[mx,my,mz]转换到n系,得到[hx,hy,hz]  
            hx = 2*mx*(0.5 - q2q2 - q3q3) + 2*my*(q1q2 - q0q3) + 2*mz*(q1q3 + q0q2);  
            hy = 2*mx*(q1q2 + q0q3) + 2*my*(0.5 - q1q1 - q3q3) + 2*mz*(q2q3 - q0q1);  
            hz = 2*mx*(q1q3 - q0q2) + 2*my*(q2q3 + q0q1) + 2*mz*(0.5 - q1q1 - q2q2);  
			//n系x轴指北，则在n系中hy应该为0，但是经过加计修正的旋转矩阵有偏航角误差，
			//所以这里算出的hy不为0，包含了偏航角误差
			
			//因为转换后的n系和实际的n系只差偏航角，所以z方向一样，磁力相等
            //得到实际n系中的地磁向量的真实值[bx,bz,by],其中by=0   
            bx = sqrt((hx*hx) + (hy*hy));  
            bz = hz;     
 
            //再将n系中的地磁向量[bx，by,bz]转换到b系中，得到[wx,wy,wz]
            wx = 2*bx*(0.5 - q2q2 - q3q3) + 2*bz*(q1q3 - q0q2);  
            wy = 2*bx*(q1q2 - q0q3) + 2*bz*(q0q1 + q2q3);  
            wz = 2*bx*(q0q2 + q1q3) + 2*bz*(0.5 - q1q1 - q2q2);  
			//再转回到b系时，[wx,wy,wz]包含了偏航角误差
 
            //n系中重力加速度[0,0,1]转换到b系中得到三个分量[vx,vy,vz]        
            vx = 2*(q1q3 - q0q2);  
            vy = 2*(q0q1 + q2q3);  
            vz = q0q0 - q1q1 - q2q2 + q3q3;    
             
            //计算[wx,wy,wz] X [mx,my,mz],[ax,at,az] X [vx,vy,vz]，得到两个误差后求和
            ex = (ay*vz - az*vy) + (my*wz - mz*wy);  
            ey = (az*vx - ax*vz) + (mz*wx - mx*wz);  
            ez = (ax*vy - ay*vx) + (mx*wy - my*wx);  
             
            //PI控制器中的积分部分
            exInt = exInt + ex*Ki;//* (1.0f / sampleFreq);  
            eyInt = eyInt + ey*Ki;//* (1.0f / sampleFreq);  
            ezInt = ezInt + ez*Ki;//* (1.0f / sampleFreq);  
            
            //误差经过PI控制器后输出,然后补偿到角速度的三个分量，Kp、Ki是需要调节的参数
            gx = gx + Kp*ex + exInt;  
            gy = gy + Kp*ey + eyInt;  
            gz = gz + Kp*ez + ezInt;               
            
            //一阶龙格库塔法更新四元数  
            //q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;  
            //q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;  
            //q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;  
            //q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT; 
	//下面进行姿态的更新，也就是四元数微分方程的求解
	q0temp=q0;
	q1temp=q1;
	q2temp=q2;
	q3temp=q3;
	//采用一阶解法更新四元数
	q0 = q0temp + (-q1temp*gx - q2temp*gy -q3temp*gz)*halfT;
	q1 = q1temp + (q0temp*gx + q2temp*gz -q3temp*gy)*halfT;
	q2 = q2temp + (q0temp*gy - q1temp*gz +q3temp*gx)*halfT;
	q3 = q3temp + (q0temp*gz + q1temp*gy -q2temp*gx)*halfT;						
             
            // 归一化四元数
            norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);  
            q0 = q0 / norm;  
            q1 = q1 / norm;  
            q2 = q2 / norm;  
            q3 = q3 / norm;  
						
							//四元数到欧拉角的转换
	//其中YAW航向角由于加速度计对其没有修正作用，因此也可以直接用陀螺仪积分代替
	//Q_ANGLE_Z = Q_ANGLE_Z + gz*halfT*2*57.3; // yaw
	Q_ANGLE_Z = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;
	Q_ANGLE_Y = asin(-2 * q1 * q3 + 2 * q0* q2)*57.3; // pitch
	Q_ANGLE_X = atan2(2 * q2 * q3 + 2 * q0 * q1,-2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
}