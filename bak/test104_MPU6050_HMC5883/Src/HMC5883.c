

#include "i2c.h"
#include "HMC5883.h"

#define	SlaveAddress   0x3C	  // HMC5883L�ĵ�ַ
//#define	SlaveAddress   0x1a	  //

unsigned char BUF[8];

#define	HMC5883_I2C hi2c2

void IIC_Write_5883(unsigned char dev_addr, unsigned char mem_addr, 
								unsigned char data)
{
	HAL_I2C_Mem_Write(&HMC5883_I2C, dev_addr, mem_addr,
						I2C_MEMADD_SIZE_8BIT, &data, 1, 2);
}

void IIC_Read_5883(unsigned char dev_addr, unsigned char mem_addr, 
	unsigned char *buf, unsigned char len)
{
	HAL_I2C_Mem_Read(&HMC5883_I2C, dev_addr, mem_addr, 
					I2C_MEMADD_SIZE_8BIT, buf, len, 2);
}

void Single_Write_HMC5883(unsigned char  REG_Address, unsigned char  REG_data)
{
	IIC_Write_5883(SlaveAddress, REG_Address, REG_data);
}

unsigned char Single_Read_HMC5883(unsigned char REG_Address)
{  unsigned char  REG_data;

	IIC_Read_5883(SlaveAddress, REG_Address, &REG_data, 1);
    return REG_data; 
}


//******************************************************
//
//��ȡ5883�Ĵų����ݣ��Ĵ�����ַ��03��ʼ��6�ֽ�
//
//******************************************************
void Multiple_Read_HMC5883(void)
{   
	IIC_Read_5883(SlaveAddress, 0x03, BUF, 6);
}


/*
// ��ʼ��5883
 00��Configuration Register A  
 01��Configuration Register B
 02��Mode Register 
 09��Status Register 
*/
void Init_HMC5883()
{
	
	Single_Write_HMC5883(0x00,				//00��Configuration Register A  
												(3<<5)|			//Ĭ�ϣ�8��ƽ��ֵ�˲�
												(1<<4)|			//Ĭ�ϣ�15Hz
												(0)					//00�������ԣ�01�ڲ���������10�ڲ�������
											);  
	
	
  Single_Write_HMC5883(0x01,
												(1<<5)|		//�������ã�Ĭ��+-1.3Ga
												0					//����Ϊ0
											);  

	Single_Write_HMC5883(0x02,0x00);  // 00����ת��ģʽ��01���Σ�����idle
	
	
	//Single_Write_HMC5883(0x00,0x10|(0));  //	
	//Single_Write_HMC5883(0x01,0x40);  // 
	//Single_Write_HMC5883(0x02,0x00);
}





/*
�Լ�ģʽ�������ڲ�����������ת��
������Ĳ���ֵ����ԼΪ��
x��951
y��951
z��886
�Լ���ɺ���������Ϊ��������ģʽ������ת��
*/
unsigned char Init_HMC5883_SelfTestMode()
{
	short x,y,z;
	Single_Write_HMC5883(0x00,0x70|(1));  //	�ڲ�����
	HAL_Delay(10);
	//Single_Write_HMC5883(0x01,0x40);  // Ĭ������
  Single_Write_HMC5883(0x02,0x01);  // �ϵ�Ĭ���ǵ���ת��
	
	HAL_Delay(2000);
	
	Multiple_Read_HMC5883();
	
	x = (short)(BUF[0] << 8) | BUF[1]; //Combine MSB and LSB of X Data output register
  z = (short)(BUF[2] << 8) | BUF[3]; //Combine MSB and LSB of Z Data output register
  y = (short)(BUF[4] << 8) | BUF[5]; //Combine MSB and LSB of Y Data output register
	//printf("x = %d\r\ny = %d\r\nz = %d\r\n",x,y,z);
	
	Single_Write_HMC5883(0x00,0x70|(0)); 
	Single_Write_HMC5883(0x02,0x00);	
	
	return 0;
}

