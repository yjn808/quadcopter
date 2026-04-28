#include "int_SI24R1.h"




static const uint8_t TX_ADDRESS[TX_ADR_WIDTH] = {0x0A, 0x01, 0x07, 0x0E, 0x01};


static uint8_t SPI_RW(uint8_t byte)
{
	uint8_t rx_data=0;
	if (HAL_SPI_TransmitReceive(&hspi1, &byte, &rx_data, 1, 1000) != HAL_OK)
    {
        return 0;   // 或者做错误处理
    }
	return rx_data;                            
}



/********************************************************
函数功能：写寄存器的值（单字节）                
入口参数：reg:寄存器映射地址（格式：SI24R1_Write_Buf｜reg）
					value:寄存器的值
返回  值：状态寄存器的值
*********************************************************/
uint8_t Int_SI24R1_WRITE_REG(uint8_t reg, uint8_t value)
{
	uint8_t status;

	CS_LOW;              
	status = SPI_RW(reg);				
	SPI_RW(value);
	CS_HIGH;
	
	return(status);
}


/********************************************************
函数功能：写寄存器的值（多字节）                  
入口参数：reg:寄存器映射地址（格式：Int_SI24R1_Write_Buf｜reg）
					pBuf:写数据首地址
					bytes:写数据字节数
返回  值：状态寄存器的值
*********************************************************/
uint8_t Int_SI24R1_Write_Buf(uint8_t reg, const uint8_t *pBuf, uint8_t bytes)
{
	uint8_t status,byte_ctr;

	CS_LOW;
	status=SPI_RW(reg);
	for(byte_ctr = 0;byte_ctr<bytes;byte_ctr ++)
	{
		SPI_RW(*pBuf++);
	}
	CS_HIGH;




	return status;
	
	
}							  					   


/********************************************************
函数功能：读取寄存器的值（单字节）                  
入口参数：reg:寄存器映射地址（格式：SI24R1_Read_Reg｜reg）
返回  值：寄存器值
*********************************************************/
uint8_t Int_SI24R1_READ_REG(uint8_t reg)
{
 	uint8_t value;

	CS_LOW;    
	SPI_RW(reg);			
	value = SPI_RW(0);
	CS_HIGH;             

	return(value);
}


/********************************************************
函数功能：读取寄存器的值（多字节）                  
入口参数：reg:寄存器映射地址（SI24R1_Read_Buf｜reg）
					pBuf:接收缓冲区的首地址
					bytes:读取字节数
返回  值：状态寄存器的值
*********************************************************/
uint8_t Int_SI24R1_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t bytes)
{
	uint8_t status,byte_ctr;

	CS_LOW;                                     
	status = SPI_RW(reg);                           
	for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
		pBuf[byte_ctr] = SPI_RW(0);                   //读取数据，低字节在前
	CS_HIGH;                                        

	return(status);    
}


/********************************************************
 函数功能：SI24R1接收模式初始化                     
入口参数：无
返回  值：无
*********************************************************/
void Int_SI24R1_RX_Mode(void)
{
    CE_LOW;  // 配置寄存器前，先关闭接收

    Int_SI24R1_Write_Buf(SI24R1_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);
    Int_SI24R1_WRITE_REG(SI24R1_WRITE_REG + EN_AA, 0x01);
    Int_SI24R1_WRITE_REG(SI24R1_WRITE_REG | EN_RXADDR, 0x01);
    Int_SI24R1_WRITE_REG(SI24R1_WRITE_REG | RF_CH, CHANNEL);
    Int_SI24R1_WRITE_REG(SI24R1_WRITE_REG | RX_PW_P0, TX_PLOAD_WIDTH);
    Int_SI24R1_WRITE_REG(SI24R1_WRITE_REG | RF_SETUP, 0x0f);
    Int_SI24R1_WRITE_REG(SI24R1_WRITE_REG | CONFIG, 0x0f);

    Int_SI24R1_WRITE_REG(SI24R1_WRITE_REG | STATUS, 0x70);  // 清除 RX_DR、TX_DS、MAX_RT 中断标志

    CE_HIGH;  // 拉高 CE，启动接收模式
}					


/********************************************************
函数功能：SI24R1发送模式初始化                      
入口参数：无
返回  值：无
*********************************************************/
void Int_SI24R1_TX_Mode(void)
{
	CE_LOW;
	Int_SI24R1_Write_Buf(SI24R1_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);     // 写入发送地址
	Int_SI24R1_Write_Buf(SI24R1_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // 为了应答接收设备，接收通道0地址和发送地址相同

	Int_SI24R1_WRITE_REG(SI24R1_WRITE_REG + EN_AA, 0x01);       											// 使能接收通道0自动应答
	Int_SI24R1_WRITE_REG(SI24R1_WRITE_REG + EN_RXADDR, 0x01);   											// 使能接收通道0
	Int_SI24R1_WRITE_REG(SI24R1_WRITE_REG + SETUP_RETR, 0x0a);  											// 自动重发延时等待250us+86us，自动重发10次
	Int_SI24R1_WRITE_REG(SI24R1_WRITE_REG + RF_CH, CHANNEL);         											// 选择射频通道0x40
	Int_SI24R1_WRITE_REG(SI24R1_WRITE_REG + RF_SETUP, 0x06);
	Int_SI24R1_WRITE_REG(SI24R1_WRITE_REG + CONFIG, 0x0e);      											// CRC使能，16位CRC校验，上电
}


/********************************************************
函数功能：读取接收数据        硬件直接接收数据保存到 FIFO队列中 => 通过状态标志位判断队列是否有数据               
入口参数：rxbuf:接收数据存放首地址
返回  值：0:接收到数据
          1:没有接收到数据
*********************************************************/
uint8_t Int_SI24R1_RxPacket(uint8_t *rxbuf)
{
	uint8_t state;
	//将读取到的值 再写回状态寄存器 => 状态寄存器写1清除
	state = Int_SI24R1_READ_REG(STATUS);  			                 //读取状态寄存器的值    	  
	Int_SI24R1_WRITE_REG(SI24R1_WRITE_REG+STATUS,state);               //清除RX_DS中断标志

	if(state & RX_DR)								                           //接收到数据
	{
		Int_SI24R1_Read_Buf(RD_RX_PLOAD,rxbuf,TX_PLOAD_WIDTH);     //读取数据
		Int_SI24R1_WRITE_REG(FLUSH_RX,0xff);					              //清除RX FIFO寄存器
		return 0; 
	}	   
	return 1;                                                   //没收到任何数据
}


/********************************************************
函数功能：发送一个数据包                      
入口参数：txbuf:要发送的数据
返回  值：0:发送成功
		 1:发送失败
             
*********************************************************/
uint8_t Int_SI24R1_TxPacket(uint8_t *txbuf)
{
	uint8_t state;
	CE_LOW;																										  //CE拉低，使能SI24R1配置
  	Int_SI24R1_Write_Buf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH);	    //写数据到TX FIFO,32个字节
 	CE_HIGH;																										  //CE置高，使能发送	   
	
	//while(IRQ == 1);我们没有使用中断判断发送完成 => 使用轮询读取																				  //等待发送完成
	state = Int_SI24R1_READ_REG(STATUS);  											  //读取状态寄存器的值	
	while(((state & TX_DS) == 0) && ((state & MAX_RT) == 0))
	{
		state = Int_SI24R1_READ_REG(STATUS);
		vTaskDelay(1);
	}   
	Int_SI24R1_WRITE_REG(SI24R1_WRITE_REG+STATUS, state); 								//清除TX_DS或MAX_RT中断标志
	if(state&MAX_RT)																	//达到最大重发次数
	{
		Int_SI24R1_WRITE_REG(FLUSH_TX,0xff);										    //清除TX FIFO寄存器 
		return 1; 
	}
	if(state&TX_DS)																			      //发送完成
	{
		return 0;
	}
	return 1;																						  //发送失败
}


/**
 * @brief 硬件接口层SI24R1检查
 * 
 * @return uint8_t 
 * 		0: 检查通过
 * 		1: 检查失败
 */
uint8_t si24r1_rx_buff[5]={0};
uint8_t Int_SI24R1_Check(void)
{
	//1.SPI通讯能否正常读写寄存器、
	//1.0 SI24R1芯片需要先读取一次  保证SPI通讯正常
	Int_SI24R1_Write_Buf(SI24R1_WRITE_REG + TX_ADDR, si24r1_rx_buff, TX_ADR_WIDTH);

	//1.1 写入发送地址
	Int_SI24R1_Write_Buf(SI24R1_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);

	//1.2 读取相同数据
	Int_SI24R1_Read_Buf(SI24R1_WRITE_REG + TX_ADDR, si24r1_rx_buff, TX_ADR_WIDTH);
	for(uint8_t i = 0; i < TX_ADR_WIDTH; i++)
	{
		if(si24r1_rx_buff[i] != TX_ADDRESS[i])
		{
			return 1;
		}
	}
	return 0;
}

/**
 * @brief 硬件接口层SI24R1初始化
 * 
 */
void Int_SI24R1_Init(void)
{
	//手册说明上电芯片延时200ms
	HAL_Delay(200);
	//校验检测
	while(Int_SI24R1_Check()==1)
	{
		HAL_Delay(10);
	}
	//设置默认状态为接收模式 => 每次发送数据时 切换到发送状态
	Int_SI24R1_RX_Mode();
	DEBUG_PRINTF("SI24R1 Init Success\r\n");

}



