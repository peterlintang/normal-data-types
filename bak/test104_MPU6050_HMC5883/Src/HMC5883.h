#ifndef _HMC5883_H
#define _HMC5883_H


void  Single_Write_HMC5883(unsigned char  REG_Address,unsigned char REG_data);   //??????
unsigned char Single_Read_HMC5883(unsigned char  REG_Address);                   //???????????
void  Multiple_Read_HMC5883(void);                                  //????????????
void Init_HMC5883(void);
unsigned char Init_HMC5883_SelfTestMode(void);
#endif
