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


### code part

The CAN HAL driver layer provides a simple, generic multi-instance set of APIs to control the CAN peripheral
thus we will have only to change some bitfields, others are there by default.


