

/******************************************************
 *
 ****************************************************/
// hu bu lv bo
float K = 0.98;
float angle = 0;	

float gyro_rate = read_gyro();
float mag_x = read_mag_x();
float mag_y = read_mag_y();
float mag_z = read_mag_z();

while (true) {
	gyro_rate = read_gyro();
	angle += gyro_rate * dt;

	float mag_heading = atan2(mag_y, mag_x);
	float mag_yaw = mag_heading - angle;

	if (mag_yaw > M_PI)
	{
		mag_yaw -= 2 * M_PI;
	}
	else if (mag_yaw < -M_PI)
	{
		mag_yaw += 2 * M_PI;
	}

	angle = K * (angle + gyro_rate * dt) + (1 - K) * mag_yaw;
	printf("angle: %f\n", angle);
}





/*********************************************************
 *
 ********************************************************/

struct Quaternion
{
    double w,x,y,z;
}q;

//欧拉角转四元数
void Euler_to_Quaternion(double yaw,double pitch,double roll)
{
    double cy = cos(yaw * 0.5);
    double sy = sin(yaw * 0.5);
    double cp = cos(pitch * 0.5);
    double sp = sin(pitch * 0.5);
    double cr = cos(roll * 0.5);
    double sr = sin(roll * 0.5);

    q.w = cy * cp * cr + sy * sp * sr;
    q.x = cy * cp * sr - sy * sp * cr;
    q.y = sy * cp * sr + cy * sp * cr;
    q.z = sy * cp * cr - cy * sp * sr;
}




#define M_PI 3.141592f
struct Quaternion
{
    double w,x,y,z;
};

struct EulerAngles
{
    double roll,pitch,yaw;
}angles;

void Quaternion_to_Euler(struct Quaternion q)
{
    //roll
    double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
    double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
    angles.roll = atan2(sinr_cosp, cosr_cosp);

    // pitch
    double sinp = 2 * (q.w * q.y - q.z * q.x);
    if (fabs(sinp) >= 1)
        angles.pitch = copysign(M_PI / 2, sinp); // use 90 degrees if out of range
    else
        angles.pitch = asin(sinp);

    // yaw
    double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
    double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
    angles.yaw = atan2(siny_cosp, cosy_cosp);

}






#define  Kp  10.0f   //PI控制器的比例系数
#define  Ki  0.008f  //PI控制器的积分系数
#define halfT  0.001f //采样周期的一半

float q0=1,q1=1,q2=0,q3=0;  //四元数初始化
float exlnt=0,eylnt=0,eylnt = 0;  //向量积误差初始化

struct Q_angle
{
    float roll;
    float pitch;
    float yaw;
};

void IMUupdate(float gx,float gy,float gz,float ax,float ay,float az)
{
    float norm;
    float vx,vy,vz;
    float ex,ey,ez;

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

    if(ax*ay*az ==0)
        return;

    norm = sqrt(ax*ax+ay*ay+az*az);

    //b系的加速度值单位化
    ax = ax/norm;
    ay = ay/norm;
    az = az/norm;

    //n系中的g理论输出转至b系的值
    vx = 2*(q1q3-q0q2);
    vy = 2*(q0q1+q2q3);
    vz = q0q0-q1q1-q2q2+q3q3;

    //将n系中的g理论输出转至b系的值和b系中的加速度计值进行向量叉乘 得到向量积误差
    ex = (ay*vz - az*vy) ;
    ey = (az*vx - ax*vz) ;
    ez = (ax*vy - ay*vx) ;

    //对误差进行积分
    exlnt = exlnt + ex * Ki;
    eyInt = eyInt + ey * Ki;
    ezInt = ezInt + ez * Ki;

    //计算负补偿后b系中陀螺仪的输出
    gx = gx + Kp*ex + exlnt;
    gy = gy + Kp*ey + eylnt;
    gz = gz + Kp*ez + ezlnt;

    //四元素的微分方程
    q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
    q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
    q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
    q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

    //单位化四元数
    norm = sqrt(q0*q0+q1*q1+q2*q2+q3*q3);
    q0 = q0/norm;
    q1 = q1/norm;
    q2 = q2/norm;
    q3 = q3/norm;

    //四元数转欧拉角(角度制)
    Q_ANGLE.yaw = atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2*q2 - 2 * q3* q3 + 1)* 57.3; // yaw
    Q_ANGLE.pitch  = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
    Q_ANGLE.roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll

}




#define  Kp  10.0f   //PI控制器的比例系数
#define  Ki  0.008f  //PI控制器的积分系数
#define halfT  0.001f //采样周期的一半

float q0=1,q1=1,q2=0,q3=0;  //四元数初始化
float exlnt=0,eylnt=0,eylnt = 0;  //向量积误差初始化

struct Q_angle
{
    float roll;
    float pitch;
    float yaw;
};

void IMUupdate(float gx,float gy,float gz,float ax,float ay,float az,float mx,float my,float mz)
{
    float norm;
    float hx,hy,hz,bx,bz;
    float wx,wy,wz;
    float vx,vy,vz;
    float ex,ey,ez;

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

    if(ax*ay*az ==0)
        return;

    if(mx*my*mz==0)
        return;

    //b系的加速度值单位化
    norm = sqrt(ax*ax+ay*ay+az*az);
    ax = ax/norm;
    ay = ay/norm;
    az = az/norm;

    //b系磁力计数据归一化
    norm = sqrt(mx*mx+my*my+mz*mz);
    mx = mx/norm;
    my = my/norm;
    mz = mz/norm;

    //计算b系转到n系的磁力计数据
    hx = 2 * mx * (0.5 - q2q2 - q3q3) + 2 * my * (q1q2 - q0q3) + 2 * mz * (q1q3 + q0q2);
    hy = 2 * mx * (q1q2 + q0q3) + 2 * my * (0.5 - q1q1 - q3q3) + 2 * mz * (q2q3 - q0q1);
    hz = 2 * mx * (q1q3 - q0q2) + 2 * my * (q2q3 + q0q1) + 2 * mz * (0.5 - q1q1 -q2q2);

    //将原始n系的磁力计数据转换到新的n系 其中x轴指向正北方向
    bx = sqrt((hx*hx)+(hy*hy));
    by = 0;
    bz = hz;

    //将新的n系下的磁力计数据转化为b系的数据
    wx = 2 * bx * (0.5 - q2q2 - q3q3) + 2 * bz * (q1q3 - q0q2);
    wy = 2 * bx * (q1q2 - q0q3) + 2 * bz * (q0q1 + q2q3);
    wz = 2 * bx * (q0q2 + q1q3) + 2 * bz * (0.5 - q1q1 - q2q2);


    //n系中的重力加速度理论输出转至b系的值
    vx = 2*(q1q3-q0q2);
    vy = 2*(q0q1+q2q3);
    vz = q0q0-q1q1-q2q2+q3q3;

    //将n系中的g理论输出转至b系的值和b系中的加速度计值进行向量叉乘 得到向量积误差
    //将最新的b系的数据叉乘原始b系中磁力计数据
    //误差由上面两部分组成
    ex = (ay*vz - az*vy) + (my*wz - mz*wy);
    ey = (az*vx - ax*vz) + (mz*wx - mx*wz);
    ez = (ax*vy - ay*vx) + (mx*wy - my*wx);

    //对误差进行积分
    exlnt = exlnt + ex * Ki;
    eyInt = eyInt + ey * Ki;
    ezInt = ezInt + ez * Ki;

    //计算负补偿后b系中陀螺仪的输出
    gx = gx + Kp*ex + exlnt;
    gy = gy + Kp*ey + eylnt;
    gz = gz + Kp*ez + ezlnt;

    //四元素的微分方程
    q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
    q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
    q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
    q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

    //单位化四元数
    norm = sqrt(q0*q0+q1*q1+q2*q2+q3*q3);
    q0 = q0/norm;
    q1 = q1/norm;
    q2 = q2/norm;
    q3 = q3/norm;

    //四元数转欧拉角(角度制)
    Q_ANGLE.yaw = atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2*q2 - 2 * q3* q3 + 1)* 57.3; // yaw
    Q_ANGLE.pitch  = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
    Q_ANGLE.roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll

}

/*  Q_angle 加速度计的过程噪声方差
    Q_bias  陀螺仪漂移的过程噪声方差
    R_measure  测量噪声的方差
    angle  通过卡尔曼滤波器计算出来的角度 是2*1矩阵的一部分
    bias   通过卡尔曼滤波器计算出来的陀螺仪角速度偏差 是2*1矩阵的一部分
    P[2][2]  误差协方差矩阵 是个2*2的矩阵
*/
 
float Q_angle = 0.001f;
float Q_bias = 0.003f;
float R_measure = 0.03f;
 
float angle = 0.0f;
float bias = 0.0f;
 
float P[2][2];
 
P[0][0] = 0.0f;
P[0][1] = 0.0f;
P[1][0] = 0.0f;
P[1][1] = 0.0f;
 
float Kalmen_getAngle(float newAngle,float newRate,float dt)
{
    //角速度 = 当前陀螺仪测得的角速度-陀螺仪角速度偏差量
    rate = newRate - bias;
    //角度 += 时间*角速度
    angle += dt * rate;
 
    //计算当前误差协方差矩阵 
    /*  P[0][0]     P[0][1]
        P[1][0]     P[1][1]
    */
    P[0][0] += dt * (dt*P[1][1] - P[0][1] - P[1][0] + Q_angle);
    P[0][1] -= dt * P[1][1];
    P[1][0] -= dt * P[1][1];
    P[1][1] += Q_bias * dt;
 
    //计算残差的协方差
    float S = P[0][0] + R_measure;
 
    //计算卡尔曼增益
    float K[2];
    K[0] = P[0][0] / S;
    K[1] = P[1][0] / S;
 
    //计算残差
    float y = newAngle - angle;
    
    //进行迭代预测
    angle += K[0] * y;
    bias += K[1] * y;
 
    //计算后验估计对应的误差协方差矩阵
    float P00_temp = P[0][0];
    float P01_temp = P[0][1];
 
    P[0][0] -= K[0] * P00_temp;
    P[0][1] -= K[0] * P01_temp;
    P[1][0] -= K[1] * P00_temp;
    P[1][1] -= K[1] * P01_temp;
 
    return angle;
}


