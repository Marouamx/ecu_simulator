## SIMULATOR FIRMWARE FEATURES

Maybe be the most important point in this is to configure the baudrate of the CAN at 500kb/s
by setting the parameters as follows: 

#### Prescaler 6, quanta1 6 times, quanta2 7 times

![image](https://user-images.githubusercontent.com/62667666/167127822-bf0330a3-aca4-4627-bfd7-89af6193090f.png)


First configure OBD as in: 
http://www.obdtester.com/elm-usb-commands

and ID to 7E8 corresponds to one of ECU 
and response to 0100


then write all PIDs from: http://www.totalcardiagnostics.com/support/Knowledgebase/Article/View/104/0/obd2-pids-for-programmers-technical
add features as: https://www.microport.com.tw/en/prod/OBD-II-Calibration-Diagnosis-Validation-in-Vehicle-Communication/OBD-II-ECU-Simulator/ECU-Simulator-Standard.html


Important: 

![image](https://user-images.githubusercontent.com/62667666/166656708-d523614a-dfcb-4ca3-9262-55957ff5777b.png)


FLowChart:

![image](https://user-images.githubusercontent.com/62667666/166743350-3aaaf98e-6891-4add-8b12-f9a22da1087a.png)
