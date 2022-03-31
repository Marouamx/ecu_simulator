## CAN COMMUNICATION BETWEEN 2 STM32F4 CHIPS

Establish a CAN communication between 2 discovery boards (STM32F4) 

Both boards will be transmitter and receiver at the same time so same circuit, configuration and approximately same code will be applied to both.

For the circuit I am using the STM32F4 discovery shield that has already the CAN tranceiver implemented on it.
Connect both boards by CANH, CANL, GND and the circuit is done.

![image](https://user-images.githubusercontent.com/62667666/161021021-5ea8f341-3108-4872-90d3-bb9954cf804d.png)

The configuration on CubeMx: 
(assuming the basic congif like RCC, SWD .. is already done)
1. CAN_tx and CAN_rx on PD0, PD1 that are connected to the trancevier on the board
2. GPIO External interrupt (push button on the board is PA0)
3. GPIO output for the LED (PD12)
  
Ps: activate the interrupt0 of the CAN_rx pin

### Standard CAN Frame

![image](https://user-images.githubusercontent.com/62667666/161023649-1d04a21a-e36d-40f1-9159-9d4bc02ee92c.png)


### Code Part

The CAN HAL driver layer provides a simple, generic multi-instance set of APIs to control the CAN peripheral
thus we will have only to change some bitfields, others are there by default.

Main objective: be able to send a CAN dataframe from an end by an external interrupt (user push botton) and receive it by the other end.
the datafame contains information on how many times and in what frequency you blink an LED

(code explained by comments bellow)

```C
CAN_TxHeaderTypeDef TxHeader; //CAN Tx message header structure definition
CAN_RxHeaderTypeDef RxHeader; //CAN Rx message header structure definition

uint8_t TxData[8]; //data to be transmitted max 8 bytes
uint8_t RxData[8]; // data to be received max: 8 bytes

uint32_t TxMailbox; // whole CAN dataframe

int datacheck = 0; //just a flag
//EXTI interrupt request when user presses the button -> sends the message via CAN
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_0)
	{
		TxData[0] = 100;   // ms Delay
		TxData[1] = 40;    // loop rep

		HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox);
	}
}
//EXTI interrupt request when user presses the button -> sends the message via CAN
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData);
	if (RxHeader.DLC == 2) //checks first if length of data matches
	{
		datacheck = 1;
	}
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


   TxHeader.DLC = 2;  // data length
   TxHeader.IDE = CAN_ID_STD; //use only std IDE ignore the extended one
   TxHeader.RTR = CAN_RTR_DATA; //std
   TxHeader.StdId = 0x103;  // ID


   while (1)
   {
 	  if (datacheck)
 	  {
 		  // blink the LED
 		  for (int i=0; i<RxData[1]; i++) //RxData[1] contains the number of blinks
 		  {
 			  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
 			  HAL_Delay(RxData[0]); //RxData[0] contains the delay between two blinks
 		  }

 		  datacheck = 0;

 	  }
   }
 }
```

### FILTERS

Filters are another cool property in the CAN module.
it allows the receiver to "filter" the incoming data from the CAN bus and only select certain dataframes.

```C
 CAN_FilterTypeDef canfilterconfig; //config module of the filter structure

  canfilterconfig.FilterActivation = CAN_FILTER_ENABLE; //ofc enabling the filtering option
  canfilterconfig.FilterBank = 18;  // for this specific filter, which filter bank to use from the assigned ones, in CAN1 below (20) 
  canfilterconfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
  canfilterconfig.FilterIdHigh = 0x446<<5; /*here is the ID of the can transimitter that we are expecting data from(shift by 5 bits since we are only 	     considering the STD ID not the extended one) */
  canfilterconfig.FilterIdLow = 0;
  canfilterconfig.FilterMaskIdHigh = 0x446<<5; 
  canfilterconfig.FilterMaskIdLow = 0x0000;
  canfilterconfig.FilterMode = CAN_FILTERMODE_IDMASK;
  canfilterconfig.FilterScale = CAN_FILTERSCALE_32BIT;
  canfilterconfig.SlaveStartFilterBank = 20;  // how many filters to assign to the CAN1, up to 28 filter banks

  HAL_CAN_ConfigFilter(&hcan1, &canfilterconfig);

```
To configure the second board: just change the std id of the board to 0x446 and in the filter configuration section change the expeted transmitter ID to 0x103
