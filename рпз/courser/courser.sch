EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A3 16535 11693
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Microchip_ATtiny:ATtiny13-20PU U?
U 1 1 5F989FBC
P 2700 2500
F 0 "U?" H 2171 2546 50  0000 R CNN
F 1 "ATtiny13-20PU" H 2171 2455 50  0000 R CNN
F 2 "Package_DIP:DIP-8_W7.62mm" H 2700 2500 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/doc2535.pdf" H 2700 2500 50  0001 C CNN
	1    2700 2500
	1    0    0    -1  
$EndComp
$Comp
L MCU_Microchip_ATtiny:ATtiny13-20PU U?
U 1 1 5F98AB96
P 7100 4700
F 0 "U?" H 7909 4654 50  0000 R CNN
F 1 "ATtiny13-20PU" H 6570 4745 50  0000 R CNN
F 2 "Package_DIP:DIP-8_W7.62mm" H 7100 4700 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/doc2535.pdf" H 7100 4700 50  0001 C CNN
	1    7100 4700
	-1   0    0    1   
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 5F98D178
P 4200 3600
F 0 "SW?" H 4200 3885 50  0000 C CNN
F 1 "SW_Push" H 4200 3794 50  0000 C CNN
F 2 "" H 4200 3800 50  0001 C CNN
F 3 "~" H 4200 3800 50  0001 C CNN
	1    4200 3600
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW?
U 1 1 5F98E018
P 4200 4100
F 0 "SW?" H 4200 4385 50  0000 C CNN
F 1 "SW_Push" H 4200 4294 50  0000 C CNN
F 2 "" H 4200 4300 50  0001 C CNN
F 3 "~" H 4200 4300 50  0001 C CNN
	1    4200 4100
	1    0    0    -1  
$EndComp
$Comp
L LED:TSAL4400 D?
U 1 1 5F98E7CF
P 4900 2250
F 0 "D?" V 4896 2171 50  0000 R CNN
F 1 "TSAL4400" V 4805 2171 50  0000 R CNN
F 2 "LED_THT:LED_D3.0mm_IRBlack" H 4900 2425 50  0001 C CNN
F 3 "http://www.vishay.com/docs/81006/tsal4400.pdf" H 4850 2250 50  0001 C CNN
	1    4900 2250
	0    -1   -1   0   
$EndComp
$Comp
L Transistor_Array:ULN2003 U?
U 1 1 5F994C44
P 8400 2050
F 0 "U?" H 8400 2717 50  0000 C CNN
F 1 "ULN2003" H 8400 2626 50  0000 C CNN
F 2 "" H 8450 1500 50  0001 L CNN
F 3 "http://www.ti.com/lit/ds/symlink/uln2003a.pdf" H 8500 1850 50  0001 C CNN
	1    8400 2050
	1    0    0    -1  
$EndComp
NoConn ~ 8800 2450
NoConn ~ 8800 2350
NoConn ~ 8800 2250
NoConn ~ 8000 2450
NoConn ~ 8000 2350
NoConn ~ 8000 2250
Wire Wire Line
	8800 1850 9000 1850
Wire Wire Line
	9000 1850 9000 1400
Wire Wire Line
	9000 1400 9400 1400
Wire Wire Line
	9400 1600 9050 1600
Wire Wire Line
	9050 1600 9050 1950
Wire Wire Line
	9050 1950 8800 1950
Wire Wire Line
	8800 2050 9600 2050
Wire Wire Line
	9600 2050 9600 1800
Wire Wire Line
	8800 2150 9800 2150
Wire Wire Line
	9800 2150 9800 1800
$Comp
L Motor:Stepper_Motor_unipolar_5pin M?
U 1 1 5F99F5C4
P 9700 1500
F 0 "M?" V 9668 1312 50  0000 R CNN
F 1 "Stepper_Motor_unipolar_5pin" V 9577 1312 50  0000 R CNN
F 2 "" H 9710 1490 50  0001 C CNN
F 3 "http://www.infineon.com/dgdl/Application-Note-TLE8110EE_driving_UniPolarStepperMotor_V1.1.pdf?fileId=db3a30431be39b97011be5d0aa0a00b0" H 9710 1490 50  0001 C CNN
	1    9700 1500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8800 1650 8800 1700
Wire Wire Line
	8800 1700 9400 1700
$EndSCHEMATC
