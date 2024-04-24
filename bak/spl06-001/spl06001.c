
/**************************************************************************
*全局变量定义
**************************************************************************/
T_SPL06_calibPara t_SPL06_calibPara;

/**************************************************************************
*函数实现
**************************************************************************/


/****************************************************************************
    *  @function :  SPL06ReadRes_Buf
    *  @brief    :  SPL06 I2C 读取多字节函数
    *  @input    :
    *  @output   :
    *  @return   :

    *  @author   :  CaiNiao  2021/06/07 13:40
*****************************************************************************/
bool SPL06ReadRes_Buf(UINT8 ResAddr, UINT8 *pData,UINT8 DataLen)
{
	UINT8 i = 0;
	I2C1_Start();					//I2C start

	if (FALSE == I2C1_Write(SPL06DeviceAdd & 0xFE))	//写器件
		return FALSE;

	I2C1_Write(ResAddr);			//写入寄存器地址
	I2C1_Start();
	I2C1_Write(SPL06DeviceAdd | 0x01);					//读器件

	if (DataLen > 1)
	{//读取多个字节
		for (i = 0; i < DataLen; i++)
		{
			if (i < DataLen - 1)
				*(pData + i) = I2C1_Read(ACK);
			else
				*(pData + i) = I2C1_Read(NACK);
		}
	}
	else//读单个字节
		*(pData) = I2C1_Read(NACK);

	I2C1_Stop();
	return TRUE;
}

/****************************************************************************
    *  @function :  SPL06ReadRes_Single
    *  @brief    :  SPL06 I2C读取单字节函数
    *  @input    :
    *  @output   :
    *  @return   :

    *  @author   :  CaiNiao  2021/06/07 13:40
*****************************************************************************/
UINT8 SPL06ReadRes_Single(UINT8 ResAddr)
{
	UINT8 DataTemp = 0;
	I2C1_Start();					//I2C start

	if (FALSE == I2C1_Write(SPL06DeviceAdd & 0xFE))	//写器件
		return DataTemp;

	I2C1_Write(ResAddr);			//写入寄存器地址
	I2C1_Start();
	I2C1_Write(SPL06DeviceAdd | 0x01);					//读器件
	DataTemp = I2C1_Read(NACK);
	I2C1_Stop();
	return DataTemp;
}

/****************************************************************************
    *  @function :  SPL06WriteRes_Single
    *  @brief    :  SPL06 I2C WriteRes
    *  @input    :
    *  @output   :
    *  @return   :

    *  @author   :  CaiNiao  2021/06/07 13:40
*****************************************************************************/
bool SPL06WriteRes_Single(UINT8 ResAddr,UINT8 DataTemp)
{
	I2C1_Start();					//I2C start

	if (FALSE == I2C1_Write(SPL06DeviceAdd & 0xFE))	//写器件
		return FALSE;

	I2C1_Write(ResAddr);			//写入寄存器地址
	I2C1_Write(DataTemp);			//写数据

	I2C1_Stop();					//I2C Stop
	return TRUE;
}

void SPL06_Config_Temperature(UINT8 rate, UINT8 oversampling, T_SPL06_calibPara *ptSPL06_calibPara)
{
	UINT8 temp;
	switch (oversampling)
	{
	case TMP_PRC_1:
		ptSPL06_calibPara->kT = 524288;
		break;
	case TMP_PRC_2:
		ptSPL06_calibPara->kT = 1572864;
		break;
	case TMP_PRC_4:
		ptSPL06_calibPara->kT = 3670016;
		break;
	case TMP_PRC_8:
		ptSPL06_calibPara->kT = 7864320;
		break;
	case TMP_PRC_16:
		ptSPL06_calibPara->kT = 253952;
		break;
	case TMP_PRC_32:
		ptSPL06_calibPara->kT = 516096;
		break;
	case TMP_PRC_64:
		ptSPL06_calibPara->kT = 1040384;
		break;
	case TMP_PRC_128:
		ptSPL06_calibPara->kT = 2088960;
		break;
	}

	SPL06WriteRes_Single(SPL06_TMP_CFG, rate | oversampling | 0x80);   //温度每秒128次测量一次
	if (oversampling > TMP_PRC_8)
	{
		temp = SPL06ReadRes_Single(SPL06_CFG_REG);
		SPL06WriteRes_Single(SPL06_CFG_REG, temp | SPL06_CFG_T_SHIFT);
	}
}

void SPL06_Config_Pressure(UINT8 rate, UINT8 oversampling, T_SPL06_calibPara *ptSPL06_calibPara)
{
	UINT8 temp;
	switch (oversampling)
	{
	case PM_PRC_1:
		ptSPL06_calibPara->kP = 524288;
		break;
	case PM_PRC_2:
		ptSPL06_calibPara->kP = 1572864;
		break;
	case PM_PRC_4:
		ptSPL06_calibPara->kP = 3670016;
		break;
	case PM_PRC_8:
		ptSPL06_calibPara->kP = 7864320;
		break;
	case PM_PRC_16:
		ptSPL06_calibPara->kP = 253952;
		break;
	case PM_PRC_32:
		ptSPL06_calibPara->kP = 516096;
		break;
	case PM_PRC_64:
		ptSPL06_calibPara->kP = 1040384;
		break;
	case PM_PRC_128:
		ptSPL06_calibPara->kP = 2088960;
		break;
	}

	SPL06WriteRes_Single(SPL06_PSR_CFG, rate | oversampling);
	if (oversampling > PM_PRC_8)
	{
		temp = SPL06ReadRes_Single(SPL06_CFG_REG);
		SPL06WriteRes_Single(SPL06_CFG_REG, temp | SPL06_CFG_P_SHIFT);
	}
}

void SPL06_Start(UINT8 mode)
{
	SPL06WriteRes_Single(SPL06_MEAS_CFG, mode);
}

/****************************************************************************
    *  @function :  SPL06_Init
    *  @brief    :  SPL06 初始化函数
    *  @input    :
    *  @output   :
    *  @return   :

    *  @author   :  CaiNiao  2021/06/07 13:40
*****************************************************************************/
void SPL06_Init(T_SPL06_calibPara *ptSPL06_calibPara)
{
	UINT8 coef[18];
	UINT8 id;

	id = SPL06ReadRes_Single(SPL06_ID);//ID 正常情况是0x10

	SPL06ReadRes_Buf(SPL06_COEF, coef, 18);
	ptSPL06_calibPara->C0 = ((INT16)coef[0] << 4) + ((coef[1] & 0xF0) >> 4);
	ptSPL06_calibPara->C0 = (ptSPL06_calibPara->C0 & 0x0800) ? (0xF000 | ptSPL06_calibPara->C0) : ptSPL06_calibPara->C0;
	ptSPL06_calibPara->C1 = ((INT16)(coef[1] & 0x0F) << 8) + coef[2];
	ptSPL06_calibPara->C1 = (ptSPL06_calibPara->C1 & 0x0800) ? (0xF000 | ptSPL06_calibPara->C1) : ptSPL06_calibPara->C1;
	ptSPL06_calibPara->C00 = ((INT32)coef[3] << 12) + ((INT32)coef[4] << 4) + (coef[5] >> 4);
	ptSPL06_calibPara->C00 = (ptSPL06_calibPara->C00 & 0x080000) ? (0xFFF00000 | ptSPL06_calibPara->C00) : ptSPL06_calibPara->C00;
	ptSPL06_calibPara->C10 = ((INT32)(coef[5] & 0x0F) << 16) + ((INT32)coef[6] << 8) + coef[7];
	ptSPL06_calibPara->C10 = (ptSPL06_calibPara->C10 & 0x080000) ? (0xFFF00000 | ptSPL06_calibPara->C10) : ptSPL06_calibPara->C10;
	ptSPL06_calibPara->C01 = ((INT16)coef[8] << 8) + coef[9];
	ptSPL06_calibPara->C11 = ((INT16)coef[10] << 8) + coef[11];
	ptSPL06_calibPara->C20 = ((INT16)coef[12] << 8) + coef[13];
	ptSPL06_calibPara->C21 = ((INT16)coef[14] << 8) + coef[15];
	ptSPL06_calibPara->C30 = ((INT16)coef[16] << 8) + coef[17];

	SPL06_Config_Pressure(PM_RATE_4, PM_PRC_32, ptSPL06_calibPara);
	SPL06_Config_Temperature(PM_RATE_4, TMP_PRC_8, ptSPL06_calibPara);

	SPL06_Start(MEAS_CTRL_ContinuousPressTemp); //启动连续的气压温度测量
}

/****************************************************************************
	*  @function :  SPL06_SoftReset
	*  @brief    :  SPL06软件复位
	*  @input    :
	*  @output   :
	*  @return   :

	*  @author   :  CaiNiao  2021/06/07 13:40
*****************************************************************************/
void SPL06_SoftReset()
{
	I2C1_Start();				//I2C start

	I2C1_Write(SPL06DeviceAdd & 0xFE);			//写入从机地址与写标志位
	I2C1_Write(SPL06_RESET);					//写入寄存器地址--芯片复位
	I2C1_Write(0x89);			//写数据
	I2C1_Stop();				//I2C Stop
}

INT32 SPL06_Get_Pressure_Adc()
{
	UINT8 buf[3];
	INT32 adc;

	SPL06ReadRes_Buf(SPL06_PSR_B2, buf, 3);
	adc = (INT32)(buf[0] << 16) + (INT32)(buf[1] << 8) + buf[2];
	adc = (adc & 0x800000) ? (0xFF000000 | adc) : adc;
	return adc;
}

INT32 SPL06_Get_Temperature_Adc()
{
	UINT8 buf[3];
	INT32 adc;

	SPL06ReadRes_Buf(SPL06_TMP_B2, buf, 3);
	adc = (INT32)(buf[0] << 16) + (INT32)(buf[1] << 8) + buf[2];
	adc = (adc & 0x800000) ? (0xFF000000 | adc) : adc;
	return adc;
}

float ReadSPL06_Pressure(T_SPL06_calibPara *ptSPL06_calibPara)
{
	float Traw_src,Praw_src;
	float Temp;
	float qua2, qua3;
	INT32  raw_temp,raw_press;

	raw_temp = SPL06_Get_Temperature_Adc();
	raw_press = SPL06_Get_Pressure_Adc();

	Traw_src = raw_temp/ptSPL06_calibPara->kT;
	Praw_src = raw_press/ptSPL06_calibPara->kP;

	//计算温度
	Temp = 0.5f*ptSPL06_calibPara->C0 + Traw_src * ptSPL06_calibPara->C1;

	//计算气压
	qua2 = ptSPL06_calibPara->C10 + Praw_src * (ptSPL06_calibPara->C20 + Praw_src * ptSPL06_calibPara->C30);
	qua3 = Traw_src * Praw_src * (ptSPL06_calibPara->C11 + Praw_src * ptSPL06_calibPara->C21);
	return ptSPL06_calibPara->C00 + Praw_src * qua2 + Traw_src * ptSPL06_calibPara->C01 + qua3;
}

