/*
 * EMRE MALTAS BIYOMEDIKAL 4. SINIF OGRENCISI
 * 28.01.2022
 */

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

GPIO_InitTypeDef GPIO_InitStruct;
TIM_TimeBaseInitTypeDef TIMER2_InitStruct;
NVIC_InitTypeDef NVIC_InitStruct;

uint32_t bekle=0,sure=0;

int disp[10]={0X000000BF,0X00000006,0X0000005B,0X0000004F,0X00000066,0X0000006D,0X000000FD,0X00000007,0X000000FF,0X0000006F};

uint8_t uzunluk=0,tim_int_sure=5;

void GPIO_Config()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOE,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOD,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOD,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(GPIOC,&GPIO_InitStruct);

	SysTick_Config(SystemCoreClock/1000000);

}

void TIM2_Config()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;

	NVIC_Init(&NVIC_InitStruct);

	TIMER2_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIMER2_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIMER2_InitStruct.TIM_Period = 1999;
	TIMER2_InitStruct.TIM_Prescaler = 41999 ;
	TIMER2_InitStruct.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM2,&TIMER2_InitStruct);
	TIM_Cmd(TIM2,ENABLE);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

}
void TIM2_IRQHandler()
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update))
	{
		if(tim_int_sure== 0 || tim_int_sure == 1 || tim_int_sure == 2 )
		{
			GPIO_ToggleBits(GPIOC,GPIO_Pin_0);
		}
		else if(tim_int_sure== 5)
		{
			GPIO_ResetBits(GPIOC,GPIO_Pin_0);
		}
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	GPIO_ResetBits(GPIOD,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	tim_int_sure = 5;
}

void SysTick_Handler()
{
	if(bekle>0)
		bekle--;
}
void delay_us(uint32_t time)
{
	bekle=time;
	while(bekle);
}

uint32_t uzaklik_sensoru()
{
	uint32_t  sure=0;

	GPIO_SetBits(GPIOD,GPIO_Pin_0);
	delay_us(5);
	GPIO_ResetBits(GPIOD,GPIO_Pin_0);
	delay_us(10);
	while(!(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1)));

	while(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1))
	{
		sure++;
		delay_us(1);
	}

	return sure;
}

void displayde_goster(uint8_t uzunluk)
{
	GPIO_ResetBits(GPIOE,GPIO_Pin_All);


	if(uzunluk>=0 && uzunluk<=10) {GPIO_SetBits(GPIOE,disp[0]); tim_int_sure = 0;	GPIO_SetBits(GPIOD,GPIO_Pin_14); }

	if(uzunluk>10 && uzunluk<=20) {GPIO_SetBits(GPIOE,disp[1]); tim_int_sure = 1;   GPIO_SetBits(GPIOD,GPIO_Pin_13);  }

	if(uzunluk>20 && uzunluk<=30) {GPIO_SetBits(GPIOE,disp[2]); tim_int_sure = 2;   GPIO_SetBits(GPIOD,GPIO_Pin_12);  }

	if(uzunluk>30 && uzunluk<=40)   GPIO_SetBits(GPIOE,disp[3]);

	if(uzunluk>50 && uzunluk<=60)	GPIO_SetBits(GPIOE,disp[4]);

	if(uzunluk>60 && uzunluk<=70)	GPIO_SetBits(GPIOE,disp[5]);

	if(uzunluk>70 && uzunluk<=80)	GPIO_SetBits(GPIOE,disp[6]);

	if(uzunluk>80 && uzunluk<=90)	GPIO_SetBits(GPIOE,disp[7]);

	if(uzunluk>90 && uzunluk<=100)	GPIO_SetBits(GPIOE,disp[8]);

	if(uzunluk>100)					{GPIO_SetBits(GPIOE,disp[9]); 					GPIO_SetBits(GPIOD,GPIO_Pin_15); }

	delay_us(1000);
}
void TIM2_Yeni_Config()
{
	if(tim_int_sure == 0)
	{
		TIMER2_InitStruct.TIM_Period = 199;
		TIMER2_InitStruct.TIM_Prescaler = 41999 ;
		TIM_TimeBaseInit(TIM2,&TIMER2_InitStruct);
	}
	else if(tim_int_sure == 1)
	{
		TIMER2_InitStruct.TIM_Period = 1999;
		TIMER2_InitStruct.TIM_Prescaler = 41999 ;
		TIM_TimeBaseInit(TIM2,&TIMER2_InitStruct);
	}
	else if(tim_int_sure == 2)
	{
		TIMER2_InitStruct.TIM_Period = 2999;
		TIMER2_InitStruct.TIM_Prescaler = 41999 ;
		TIM_TimeBaseInit(TIM2,&TIMER2_InitStruct);
	}

}

int main(void)
{
	GPIO_Config();
	TIM2_Config();
	while (1)
	{
		sure=uzaklik_sensoru();
		uzunluk = sure/58;
		displayde_goster(uzunluk);
		TIM2_Yeni_Config();
		delay_us(100000);
	}
}

void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size)
{
	/* TODO, implement your code here */
	return;
}


uint16_t EVAL_AUDIO_GetSampleCallBack(void){
	/* TODO, implement your code here */
	return -1;
}
