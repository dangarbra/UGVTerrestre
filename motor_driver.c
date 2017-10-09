#include "stm32f4xx_conf.h"
#include "math.h"


//------------Inicializaciones

void Init_Motor_PWM(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);



	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOA,GPIO_Pin_0 );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

/***************ERROR EN LA FUNCION DE ABAJO*********************/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);


	/* TIM14 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);

	//Prescaler=0 -> 84Mhz / 20khz -> Period=4200
	/* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 4200;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* Channel 2 and 4 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;


	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);



	/* TIM1 counter enable */
	TIM_Cmd(TIM2, ENABLE);

	/* TIM1 Main Output Enable */
	TIM_CtrlPWMOutputs(TIM2, ENABLE);

}

void setMotorSpeed(float speed){
	uint16_t speed_abs;

		speed_abs=((float)fabs(speed)*4200.0f);

		if(speed>0){
			GPIO_ResetBits(GPIOA, GPIO_Pin_0);
		}else if(speed==0){
			GPIO_ResetBits(GPIOA, GPIO_Pin_0);
			speed_abs=0;
		}
		else{
			GPIO_SetBits(GPIOA, GPIO_Pin_0);
			speed_abs=4200-speed_abs;
		}
		TIM_SetCompare2(TIM2,speed_abs);
		//CoWaitForSingleFlag (miBandera, 1000);
}



