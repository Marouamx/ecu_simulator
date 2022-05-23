#include "main.h"
#include"string.h"
#include<stdlib.h>
/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef hcan1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */
CAN_TxHeaderTypeDef TxHeader; //CAN Tx message header structure definition
CAN_RxHeaderTypeDef RxHeader; //CAN Rx message header structure definition

uint8_t TxData[8]; //data to be transmitted max 8 bytes
uint8_t RxData[8]; // data to be received max: 8 bytes

uint32_t TxMailbox; // whole CAN dataframe
char text[] = "CAR-4\r04/12/2021_12:23:22 1.232 34.31234 12 1200 88 1321 44 0\n";
char *text_Ptr;
char buffer[100];
char *buffer_Ptr;

int datacheck = 0;

struct recevied_struct{
	uint32_t OBD_Header;
	uint8_t PID_len;
	uint8_t mode;
	uint16_t PID;

} recevied_frame;


void send_tram_over_CAN() {

	HAL_UART_Transmit(&huart2, "sent:\n", sizeof("sent\n"), 100);

			text_Ptr = text;

			while(*text_Ptr != '\0') {
				for (int i = 0; i < 8 ; i++) {
					TxData[i] = (uint8_t) *text_Ptr;
					text_Ptr ++;
				}
				HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox);
				HAL_UART_Transmit(&huart2, TxData, sizeof(TxData), 100);
			}
}
void engine_coolant_temp() {

			TxData[0] = 0x03; // length of data + mode
			TxData[1] = 0x41; // 41 is like mode + 40H
			TxData[2] = 0x05; // PID replying to
			TxData[3] = 0x76; // data0
			TxData[4] = 0x55; // data1
			TxData[5] = 0x55;
			TxData[6] = 0x55;
			TxData[7] = 0x55;

			HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox);
			//HAL_UART_Transmit(&huart2, TxData, sizeof(TxData), 100);

}
void SupportedPID() {
	//  this is the response to 0100 which sets the protocol 2 to OBD
			TxData[0] = 0x01;
			TxData[1] = 0x02;
			TxData[2] = 0x55;
			TxData[3] = 0x55;
			TxData[4] = 0x55;
			TxData[5] = 0x55;
			TxData[6] = 0x55;
			TxData[7] = 0x55;

			HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox);
			//HAL_UART_Transmit(&huart2, TxData, sizeof(TxData), 100);

}
void fuel_system_status() {

	    TxData[0] = 0x04; // length of data + mode
		TxData[1] = 0x41; // 41 is like mode + 40H
		TxData[2] = 0x03; // PID replying to
		TxData[3] = 0x08; // data0
		TxData[4] = 0x08; // data1
		TxData[5] = 0x55;
		TxData[6] = 0x55;
		TxData[7] = 0x55;

		HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox);
		//HAL_UART_Transmit(&huart2, , sizeof(TxData), 100);

}
void engine_load_value() {

	int random_number = rand() % 100 + 1;
	TxData[0] = 0x03; // length of data + mode
	TxData[1] = 0x41; // 41 is like mode + 40H
	TxData[2] = 0x04; // PID replying to
	TxData[3] = random_number; // data0
	TxData[4] = 0x55; // data1
	TxData[5] = 0x55;
	TxData[6] = 0x55;
	TxData[7] = 0x55;


	HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox);
	memset(buffer,0, sizeof(buffer));
	sprintf(buffer,"\n Engine load Value Sent: %d %%",random_number);
	HAL_UART_Transmit(&huart2, buffer, 40, 100);

}
void fuel_pressure() {

	    TxData[0] = 0x03; // length of data + mode
		TxData[1] = 0x41; // 41 is like mode + 40H
		TxData[2] = 0x0A; // PID replying to
		TxData[3] = 0x55; // data0
		TxData[4] = 0x55; // data1
		TxData[5] = 0x55;
		TxData[6] = 0x55;
		TxData[7] = 0x55;

		HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox);
		//HAL_UART_Transmit(&huart2, TxData, sizeof(TxData), 100);

}
void abs_pressure() {

	    TxData[0] = 0x03; // length of data + mode
		TxData[1] = 0x41; // 41 is like mode + 40H
		TxData[2] = 0x0B; // PID replying to
		TxData[3] = 0xFF; // data0
		TxData[4] = 0x55; // data1
		TxData[5] = 0x55;
		TxData[6] = 0x55;
		TxData[7] = 0x55;

		HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox);
		//HAL_UART_Transmit(&huart2, TxData, sizeof(TxData), 100);

}
void engine_rpm() {

	    TxData[0] = 0x04; // length of data + mode
		TxData[1] = 0x41; // 41 is like mode + 40H
		TxData[2] = 0x0C; // PID replying to
		TxData[3] = 0xFA; // data0
		TxData[4] = 0x00; // data1
		TxData[5] = 0x55;
		TxData[6] = 0x55;
		TxData[7] = 0x55;

		HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox);
		//HAL_UART_Transmit(&huart2, TxData, sizeof(TxData), 100);

}
void vehicule_speed() {

		int random_number = rand() % 255;
	    TxData[0] = 0x03; // length of data + mode
		TxData[1] = 0x41; // 41 is like mode + 40H
		TxData[2] = 0x0D; // PID replying to
		TxData[3] = random_number; // data0
		TxData[4] = 0x55; // data1
		TxData[5] = 0x55;
		TxData[6] = 0x55;
		TxData[7] = 0x55;

		HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox);
		memset(buffer,0, sizeof(buffer));
		sprintf(buffer,"\n Vehicle speed Value Sent: %d km/h",random_number);
		HAL_UART_Transmit(&huart2, buffer, 40, 100);


}
void intake_air_temperature() {

		int random_number =  rand() % (215 + 40 + 1) - 40;

	    TxData[0] = 0x03; // length of data + mode
		TxData[1] = 0x41; // 41 is like mode + 40H
		TxData[2] = 0x0F; // PID replying to
		TxData[3] = random_number; // data0
		TxData[4] = 0x55; // data1
		TxData[5] = 0x55;
		TxData[6] = 0x55;
		TxData[7] = 0x55;

		HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox);
		memset(buffer,0, sizeof(buffer));
		char ch=248;
		sprintf(buffer,"\n intake air temperature Value Sent: %d degree C",random_number);
		HAL_UART_Transmit(&huart2, buffer, 50, 100);

}
//EXTI interrupt request when user presses the button -> sends the message via CAN
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if (GPIO_Pin == GPIO_PIN_0){
		HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox);
		HAL_UART_Transmit(&huart2, "\n Hex sent: \n", sizeof("\n Hex sent: \n"), 100); //start rec
		HAL_UART_Transmit(&huart2, TxData, sizeof(TxData), 100);
	}
}

void ECU_send_response (){

	if (recevied_frame.mode == 0x1)
		switch(recevied_frame.PID) {
			case 0x00:
				SupportedPID();
				break;
			case 0x03:
				fuel_system_status(); //bit encoded
				break;
			case 0x04:
				engine_load_value(); //0   100     %  A*100/255
				break;
			case 0x05:
				engine_coolant_temp(); // -40 	215 	°C  	A-40
				break;
			case 0x0A:
			 	fuel_pressure(); // 0 	765 	kPa (gauge)   A*3
				break;
			case 0x0B:
				abs_pressure(); // 0 	255 	kPa (absolute) 	 A
				break;
			case 0x0C:
				engine_rpm(); // 	0 	16,383.75 	rpm    ((A*256)+B)/4
				break;
			case 0x0D:
				vehicule_speed(); // 	0 	255 	km/h    A
				break;
			case 0x0F:
				intake_air_temperature(); // 	-40 	215 	°C  	A-40
				break;
		}
}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{

	//get buffer data
	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData); // get first 8 bytes

	// load receiving structure
	recevied_frame.OBD_Header = RxHeader.StdId;
	recevied_frame.PID_len = RxData[0];
	recevied_frame.mode = RxData[1];
	if (recevied_frame.mode != 0x5) recevied_frame.PID = RxData[2];
	else recevied_frame.PID =  RxData[2] << 8 | RxData[3]; // shift higher bits

	//next line just for debugging purposes -- print hex directly
	HAL_UART_Transmit(&huart2, "\n\n\n\nreceiving: \n", sizeof("\nreceiving: \n"), 100); //start rec
	sprintf(buffer,"%02X %02X %02X %02X %02X %02X %02X %02X", RxData[0], RxData[1], RxData[2], RxData[3], RxData[5], RxData[6], RxData[7], RxData[8]);
	HAL_UART_Transmit(&huart2, buffer, 32, 100); //print em
	HAL_UART_Transmit(&huart2, "\n by: ", sizeof("\n by: "), 100); //start rec
	sprintf(buffer, "%X",recevied_frame.OBD_Header);
	HAL_UART_Transmit(&huart2,buffer, 4, 100);
	sprintf(buffer, "\n PID length is %X and mode is %X \n DATA is:",recevied_frame.PID_len,recevied_frame.mode);
	HAL_UART_Transmit(&huart2,buffer, sizeof("\n PID length is X and mode is X \n DATA is:"), 100); //print em
	sprintf(buffer,"%04X", recevied_frame.PID);
	HAL_UART_Transmit(&huart2,buffer, 4, 100); //print em


	// send response to OBD accordingly
	ECU_send_response();


	memset(buffer, 0, sizeof(buffer));
	memset(RxData, 0, 8*sizeof(*RxData)); //flush rx buffer

}

int main(void)
{
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_CAN1_Init();
  MX_USART2_UART_Init();
  HAL_CAN_Start(&hcan1);

   // Activate the notification
   HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);


   TxHeader.DLC = 8;  // data length
   TxHeader.IDE = CAN_ID_STD; //use only std IDE ignore the extended one
   TxHeader.RTR = CAN_RTR_DATA; //std
   TxHeader.StdId = 0x7E8;  // ID of ECU -- really important

   HAL_UART_Transmit(&huart2, "while(1) \n", sizeof("while(1) \n"), 100);


   srand(time(NULL));

   while (1)
   {
	//HAL_UART_Transmit(&huart2, TxData, sizeof(TxData), 100);

	   //HAL_Delay(100);

   }
 }

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 6;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_6TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_7TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */
  CAN_FilterTypeDef canfilterconfig;

  //Giving mask 0 and Id 0 will allow all IDs to pass
  canfilterconfig.FilterActivation = CAN_FILTER_ENABLE;
  canfilterconfig.FilterBank = 18;  // which filter bank to use from the assigned ones
  canfilterconfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
  canfilterconfig.FilterIdHigh = 0;
  canfilterconfig.FilterIdLow = 0;
  canfilterconfig.FilterMaskIdHigh = 0x0000;
  canfilterconfig.FilterMaskIdLow = 0x0000;
  canfilterconfig.FilterMode = CAN_FILTERMODE_IDMASK;
  canfilterconfig.FilterScale = CAN_FILTERSCALE_32BIT;
  canfilterconfig.SlaveStartFilterBank = 20;  // how many filters to assign to the CAN1 (master can)

 HAL_CAN_ConfigFilter(&hcan1, &canfilterconfig);

  /* USER CODE END CAN1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  char buffer[] = "CAN tx message couldnt be constructed";
  HAL_UART_Transmit(&huart2, buffer, sizeof(buffer), HAL_MAX_DELAY);
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

