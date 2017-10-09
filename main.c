#include <CoOS.h>
#include "stm32f4xx_conf.h"
#include "BSP.h"
#include "LedTask.h"
#include "JoyTmr.h"
#include "motor_driver.h"

/*************************************INICIO DECLARACIONES BLUETOOTH*************************************/
char incomingByte;
#define cmdL 'L'
#define cmdR 'R'
#define cmdS 'S'
#define cmdA 'A'
#define cmdT 'T'
#define cmdG 'G'
/*************************************INICIO DECLARACIONES BLUETOOTH*************************************/

void SystemInit(void);

/*OS_FlagID miBandera;*/

int Stop = 1;

void CreateSystemObjetcs(void){
	//Inicialización de los elementos compartidos: flags, semaf., colas...
	/*CreateJoyFlags();		//Crear banderas del joystick
	CreateSerialObjetcs();
	miBandera= CoCreateFlag(0,1);*/

}

void CreateUserTasks(void){
	//Creación de las tareas de usuario
	/*CreateLedTask();
	CreateJoyTask();
	CreateSerialTask();*/
}

void Delay(void){
	uint32_t cont = 0;
	while(cont < 0x000FFFFF){
		cont ++;
	}
}

/************************************INICIO FUNCIONES BLUETOOTH*******************************/

void Usart3_Config(void){
	//USART1 (PA9/PA10)
	//USART3 (PB10/11 and PC10/11)
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);

	// Configuramos USART TX/RX
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	// Conectamos los pines USART a AF
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6);

    USART_InitTypeDef USART_InitStructure;

 /*
        - BaudRate = 115200 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 57600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART6, &USART_InitStructure);

  USART_Cmd(USART6, ENABLE);

  USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);

}

int putchar(USART_TypeDef* USARTx, int c){

	USART_SendData(USARTx,c);
	while (USART_GetFlagStatus(USARTx , USART_FLAG_TC) == RESET);
	//USART1 ->DR = (c & 0xff);
return 0;
}

int getchar(USART_TypeDef* USARTx){
	while (USART_GetFlagStatus(USARTx , USART_FLAG_RXNE) == RESET);
	return USART_ReceiveData(USARTx);
}


void USART6_IRQHandler(void)
{
int cont = 0;
	if (USART_GetITStatus(USART6,USART_IT_RXNE) != (u16)RESET)
	{
		incomingByte = USART_ReceiveData(USART6);

		USART_ClearITPendingBit(USART6,USART_IT_RXNE);


		if(incomingByte == '3'){
			setServoPos(70);
			GPIO_WriteBit(GPIOD,GPIO_Pin_13,Bit_SET);
			GPIO_WriteBit(GPIOD,GPIO_Pin_15,Bit_SET);
		}else if(incomingByte == '1'){
			setServoPos(0);
		}else if (incomingByte == '5'){
            setMotorSpeed(1.5f);
		}else if (incomingByte == '2'){

				setMotorSpeed(-1.f);

		}else if (incomingByte == 'S'){
			setMotorSpeed(0);
		}


/*
		switch(incomingByte){
		  		case '3':
		  			setServoPos(70);
		  			GPIO_WriteBit(GPIOD,GPIO_Pin_13,Bit_SET);
		  			GPIO_WriteBit(GPIOD,GPIO_Pin_15,Bit_SET);
		  			break;
		  		case '1':
		  			setServoPos(0);
		  			//setMotorSpeed(0.5f);
		  			break;
		  		case '5':
		  			//setServoPos(0);
		  			//CoResetFlag(miBandera);
		  			setMotorSpeed(0.5f);
		  			/*while (Stop != 0){
		  								b=getchar(USART3);
		  					       		putchar(USART6,b);
		  					      		Delay();
		  					      	    incomingByte = USART_ReceiveData(USART6);
		  					      		while(incomingByte != 'S'){
		  					      			if(incomingByte = '1'){
		  					      			     setServoPos(0);
											}if (incomingByte = '2'){
												setMotorSpeed(-0.5f);
											}if (incomingByte = '3'){
												setServoPos(70);
											}

		  					      		}
		  			}
		  			break;
		  		case '2':
		  			setMotorSpeed(-0.5f);
		  			break;
		  		case cmdS:
		  			//CoSetFlag(miBandera);
		  			setMotorSpeed(0);
		  			break;
		  		case cmdG:
		  		  	setServoPos(0);
		  		  	break;
		  		case '0':
		  		  	setServoPos(35);
		  		  	break;
		  		default:
		  			setMotorSpeed(0);
		  		}*/


	}
}
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/************************************FIN FUNCIONES BLUETOOTH*******************************/
/*16/02/2017*/
void Motor_Configuration(void)
{
		GPIO_InitTypeDef GPIO_InitStruct;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA, &GPIO_InitStruct);
}
/*16/02/2017*/
int main(void)
{
	float i;
	uint8_t j;

	SystemInit();			//Inicialización del reloj

	//Init_AnalogJoy();

	Init_Leds();

	Init_Servo_PWM();

	Init_Motor_PWM();

	//CoInitOS ();			//Inicialización del CoOs

	//CreateSystemObjetcs();	//Inicialización Sem, flags, queues...

	//CreateUserTasks();		//Creación de Tareas

	//CoStartOS ();			//Comienzo de ejecución del planificador

	/*int i;
	while (1) {
		    setServoPos(0);
			Delay();
			setServoPos(180);
	        Delay();
	for( i = 0; i<=4; i++){
		LED_On(i);
		Delay();
		LED_Off(i);
		Delay();
	}

	    }*/

/********************************PRUEBA BLUETHOOT****************************************************/

		Usart3_Config();
		NVIC_Configuration();


/********************************FIN PRUEBA BLUETHOOT****************************************************/

			char a;

		  while(1)
		  {
		    	    a=getchar(USART3);
		       		putchar(USART6,a);
		      		Delay();
		    }

			/*while(1)
			{

				setMotorSpeed(0.5f);
				Delay();
				setMotorSpeed(0);
				Delay();
				setMotorSpeed(-0.5f);
				Delay();
				/*for(i=0;i<1.0f;i+=0.1f){
					setMotorSpeed(2.0f);
					for(j=0;j<20;j++){
						Delay();
					}
				}*/




	return 0;
}

