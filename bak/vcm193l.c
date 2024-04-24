void VCM119x_Init(void)
{
	uint8_t regData = 0;
	regData = 0x00;
	I2CWriteNByte (0x0B, &regData,1);	 //0B寄存器写0x00
	vcm119x_delay_ms(10);  //延时10ms

}

uint8_t VCM119xL_ReadData(float *mag)
{	
	int  hdata[3];
	uint8_t ucReadBuf[6];
	uint8_t regData[1] = {0};			
    uint8_t res;
			
	regData[0] = 0x42;
	I2CWriteNByte (0x0A, regData,1);   //0A寄存器写0x42

	vcm119x_delay_ms(20); //延时20ms

	uint8_t data = 0x40;
	I2CWriteNByte (0x0A, &data,1);	//0A寄存器写0x40
	res = I2CReadNByte (0x00, ucReadBuf, 6); //连续读取（地址00~地址05）
    
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
