EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "ST_PAT SLIM"
Date "2021-02-05"
Rev ""
Comp "RIO LIU"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L st_pat:FGPMMOPA6H U1
U 1 1 600D122C
P 2200 1600
F 0 "U1" H 2275 1665 50  0000 C CNN
F 1 "FGPMMOPA6H" H 2275 1574 50  0000 C CNN
F 2 "" H 2200 1600 50  0001 C CNN
F 3 "https://cdn-shop.adafruit.com/datasheets/GlobalTop-FGPMMOPA6H-Datasheet-V0A.pdf" H 2200 1600 50  0001 C CNN
	1    2200 1600
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_SPST SW1
U 1 1 6017D3E6
P 5700 5200
F 0 "SW1" H 5850 5500 50  0000 C CNN
F 1 "SW_PWR" H 5850 5400 50  0000 C CNN
F 2 "" H 5700 5400 50  0001 C CNN
F 3 "http://www.apem.com/int/index.php?controller=attachment&id_attachment=488" H 5700 5400 50  0001 C CNN
	1    5700 5200
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x09_Female J3
U 1 1 6017DBC3
P 9050 2200
F 0 "J3" H 9078 2226 50  0000 L CNN
F 1 "SHARP Display" H 8600 1600 50  0000 L CNN
F 2 "" H 9050 2200 50  0001 C CNN
F 3 "~" H 9050 2200 50  0001 C CNN
	1    9050 2200
	1    0    0    -1  
$EndComp
Text Notes 9400 1450 2    50   ~ 0
SHARP DISPLAY
NoConn ~ 8850 1900
NoConn ~ 8850 2600
NoConn ~ 8850 2400
Wire Wire Line
	8200 1800 8850 1800
$Comp
L power:+3V3 #PWR01
U 1 1 60185239
P 950 1700
F 0 "#PWR01" H 950 1550 50  0001 C CNN
F 1 "+3V3" H 965 1873 50  0000 C CNN
F 2 "" H 950 1700 50  0001 C CNN
F 3 "" H 950 1700 50  0001 C CNN
	1    950  1700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR03
U 1 1 60187238
P 950 3250
F 0 "#PWR03" H 950 3000 50  0001 C CNN
F 1 "GND" H 955 3077 50  0000 C CNN
F 2 "" H 950 3250 50  0001 C CNN
F 3 "" H 950 3250 50  0001 C CNN
	1    950  3250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR05
U 1 1 60187F6E
P 3050 2000
F 0 "#PWR05" H 3050 1750 50  0001 C CNN
F 1 "GND" H 3055 1827 50  0000 C CNN
F 2 "" H 3050 2000 50  0001 C CNN
F 3 "" H 3050 2000 50  0001 C CNN
	1    3050 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 2000 2800 2000
$Comp
L power:GND #PWR06
U 1 1 601889EB
P 3050 3050
F 0 "#PWR06" H 3050 2800 50  0001 C CNN
F 1 "GND" H 3055 2877 50  0000 C CNN
F 2 "" H 3050 3050 50  0001 C CNN
F 3 "" H 3050 3050 50  0001 C CNN
	1    3050 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 3050 2800 3050
NoConn ~ 2800 2750
$Comp
L Connector:Conn_01x02_Female J4
U 1 1 601931F0
P 6100 5200
F 0 "J4" H 6128 5176 50  0000 L CNN
F 1 "BATTERY" H 5800 5000 50  0000 L CNN
F 2 "" H 6100 5200 50  0001 C CNN
F 3 "~" H 6100 5200 50  0001 C CNN
	1    6100 5200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR014
U 1 1 60196448
P 5300 5300
F 0 "#PWR014" H 5300 5050 50  0001 C CNN
F 1 "GND" H 5305 5127 50  0000 C CNN
F 2 "" H 5300 5300 50  0001 C CNN
F 3 "" H 5300 5300 50  0001 C CNN
	1    5300 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 5300 5900 5300
$Comp
L Connector:Conn_01x06_Female J6
U 1 1 601E0556
P 3200 4950
F 0 "J6" H 3228 4926 50  0000 L CNN
F 1 "Compass Right" H 2900 5300 50  0000 L CNN
F 2 "" H 3200 4950 50  0001 C CNN
F 3 "~" H 3200 4950 50  0001 C CNN
	1    3200 4950
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x04_Female J5
U 1 1 601DFEB9
P 2300 5000
F 0 "J5" H 2328 4976 50  0000 L CNN
F 1 "Compass Left" H 1700 5250 50  0000 L CNN
F 2 "" H 2300 5000 50  0001 C CNN
F 3 "~" H 2300 5000 50  0001 C CNN
	1    2300 5000
	1    0    0    -1  
$EndComp
Text Notes 1750 5450 0    50   ~ 0
ADAFRUIT COMPASS
Wire Wire Line
	8200 2000 8850 2000
$Comp
L power:+3V3 #PWR012
U 1 1 601813E6
P 8200 1800
F 0 "#PWR012" H 8200 1650 50  0001 C CNN
F 1 "+3V3" H 8215 1973 50  0000 C CNN
F 2 "" H 8200 1800 50  0001 C CNN
F 3 "" H 8200 1800 50  0001 C CNN
	1    8200 1800
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR016
U 1 1 60203BD8
P 2700 4750
F 0 "#PWR016" H 2700 4600 50  0001 C CNN
F 1 "+3V3" H 2715 4923 50  0000 C CNN
F 2 "" H 2700 4750 50  0001 C CNN
F 3 "" H 2700 4750 50  0001 C CNN
	1    2700 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2700 4750 3000 4750
NoConn ~ 3000 4850
$Comp
L power:GND #PWR017
U 1 1 60204755
P 2700 4950
F 0 "#PWR017" H 2700 4700 50  0001 C CNN
F 1 "GND" H 2705 4777 50  0000 C CNN
F 2 "" H 2700 4950 50  0001 C CNN
F 3 "" H 2700 4950 50  0001 C CNN
	1    2700 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	2700 4950 3000 4950
NoConn ~ 3000 5250
$Comp
L power:GND #PWR015
U 1 1 602057AC
P 1550 4900
F 0 "#PWR015" H 1550 4650 50  0001 C CNN
F 1 "GND" H 1555 4727 50  0000 C CNN
F 2 "" H 1550 4900 50  0001 C CNN
F 3 "" H 1550 4900 50  0001 C CNN
	1    1550 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 4900 2100 4900
Text Notes 2400 1350 2    50   ~ 0
GPS MODULE
Text Notes 5900 4750 2    50   ~ 0
POWER SWITCH
$Comp
L power:+BATT #PWR011
U 1 1 6017627A
P 6250 1800
F 0 "#PWR011" H 6250 1650 50  0001 C CNN
F 1 "+BATT" H 6150 2000 50  0000 L CNN
F 2 "" H 6250 1800 50  0001 C CNN
F 3 "" H 6250 1800 50  0001 C CNN
	1    6250 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6250 1800 6450 1800
$Comp
L power:GND #PWR013
U 1 1 60181D61
P 8200 2000
F 0 "#PWR013" H 8200 1750 50  0001 C CNN
F 1 "GND" H 8205 1827 50  0000 C CNN
F 2 "" H 8200 2000 50  0001 C CNN
F 3 "" H 8200 2000 50  0001 C CNN
	1    8200 2000
	1    0    0    -1  
$EndComp
Text Notes 5400 1400 0    50   ~ 0
FEATHER M0 ADALOGGER PINS
Wire Wire Line
	5200 1800 5500 1800
$Comp
L power:GND #PWR09
U 1 1 60174E09
P 5200 1800
F 0 "#PWR09" H 5200 1550 50  0001 C CNN
F 1 "GND" H 5300 1600 50  0000 R CNN
F 2 "" H 5200 1800 50  0001 C CNN
F 3 "" H 5200 1800 50  0001 C CNN
	1    5200 1800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR08
U 1 1 60174547
P 5000 3800
F 0 "#PWR08" H 5000 3550 50  0001 C CNN
F 1 "GND" H 5005 3627 50  0000 C CNN
F 2 "" H 5000 3800 50  0001 C CNN
F 3 "" H 5000 3800 50  0001 C CNN
	1    5000 3800
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR07
U 1 1 60172EEF
P 5000 2950
F 0 "#PWR07" H 5000 2800 50  0001 C CNN
F 1 "+3V3" H 5000 3100 50  0000 C CNN
F 2 "" H 5000 2950 50  0001 C CNN
F 3 "" H 5000 2950 50  0001 C CNN
	1    5000 2950
	1    0    0    -1  
$EndComp
Text Label 6450 2900 2    50   ~ 0
SDA
Text Label 6450 2800 2    50   ~ 0
SCI
Text Label 6450 2700 2    50   ~ 0
SHARP_CS_D5
Text Label 6450 2600 2    50   ~ 0
SHARP_DISP_D6
Text Label 6450 2400 2    50   ~ 0
GPS_RST_D10
Text Label 6450 2300 2    50   ~ 0
GPS_FIX_D11
Text Label 6450 2200 2    50   ~ 0
GPS_PPS_D12
Text Label 6450 2100 2    50   ~ 0
D13
Text Label 6450 1900 2    50   ~ 0
EN
Text Label 5500 1900 2    50   ~ 0
TX
Text Label 5500 2000 2    50   ~ 0
RX
Text Label 5500 2100 2    50   ~ 0
MISO
Text Label 5500 2200 2    50   ~ 0
MOSI
Text Label 5500 2300 2    50   ~ 0
SCK
Text Label 5500 2400 2    50   ~ 0
CPS_RDY_A5
Text Label 5500 2500 2    50   ~ 0
CPS_CS_A4
Text Label 5500 3100 2    50   ~ 0
AREF
Text Label 5500 3300 2    50   ~ 0
RST
$Comp
L Connector:Conn_01x12_Female J2
U 1 1 6015F140
P 6650 2300
F 0 "J2" H 6500 2950 50  0000 L CNN
F 1 "Feather Right" H 6350 1550 50  0000 L CNN
F 2 "" H 6650 2300 50  0001 C CNN
F 3 "~" H 6650 2300 50  0001 C CNN
	1    6650 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	950  1850 950  1700
$Comp
L power:+BATT #PWR0101
U 1 1 60232364
P 5300 5200
F 0 "#PWR0101" H 5300 5050 50  0001 C CNN
F 1 "+BATT" H 5315 5373 50  0000 C CNN
F 2 "" H 5300 5200 50  0001 C CNN
F 3 "" H 5300 5200 50  0001 C CNN
	1    5300 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 5200 5500 5200
Wire Wire Line
	1750 2150 1150 2150
NoConn ~ 2800 3200
NoConn ~ 1750 2300
$Comp
L Device:C_Small C1
U 1 1 601E1CF3
P 950 2000
F 0 "C1" H 800 2100 50  0000 L CNN
F 1 "1uF" H 950 1900 50  0000 L CNN
F 2 "" H 950 2000 50  0001 C CNN
F 3 "~" H 950 2000 50  0001 C CNN
	1    950  2000
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C2
U 1 1 601E34E0
P 1150 2000
F 0 "C2" H 1000 2100 50  0000 L CNN
F 1 "0.01uF" H 1200 1900 50  0000 L CNN
F 2 "" H 1150 2000 50  0001 C CNN
F 3 "~" H 1150 2000 50  0001 C CNN
	1    1150 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	950  1850 950  1900
Wire Wire Line
	1150 1900 1150 1850
Wire Wire Line
	1150 1850 950  1850
Wire Wire Line
	1150 2100 1150 2150
Wire Wire Line
	1150 2150 950  2150
Wire Wire Line
	950  2100 950  2150
Wire Wire Line
	950  3250 950  2900
Wire Wire Line
	950  2900 1750 2900
Connection ~ 950  2900
Connection ~ 950  2150
Text Label 8850 2100 2    50   ~ 0
SCK
Text Label 8850 2200 2    50   ~ 0
MOSI
Text Label 8850 2300 2    50   ~ 0
SHARP_CS_D5
Text Label 8850 2500 2    50   ~ 0
SHARP_DISP_D6
Text Label 2800 2900 0    50   ~ 0
GPS_PPS_D12
Text Label 1750 3200 2    50   ~ 0
TX
Text Label 1600 2750 2    50   ~ 0
GPS_FIX_D11
Text Label 1750 2000 2    50   ~ 0
GPS_RST_D10
Text Label 1750 3050 2    50   ~ 0
RX
Connection ~ 1150 1850
Connection ~ 1150 2150
Text Label 2100 5000 2    50   ~ 0
CPS_RDY_A5
Text Label 2100 5200 2    50   ~ 0
CPS_CS_A4
Text Label 2100 5100 2    50   ~ 0
MISO
Text Label 3000 5050 2    50   ~ 0
SCK
NoConn ~ 6450 1900
NoConn ~ 6450 2100
NoConn ~ 6450 2800
NoConn ~ 6450 2900
NoConn ~ 5500 2900
Text Label 5500 2900 2    50   ~ 0
A0
Text Label 5500 2800 2    50   ~ 0
BTN_A1
Text Label 5500 2700 2    50   ~ 0
BTN_A2
Text Label 5500 2600 2    50   ~ 0
BTN_A3
Text Label 3000 5150 2    50   ~ 0
MOSI
Text Notes 1800 7100 2    50   ~ 0
MAKES ERC HAPPY
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 602045BA
P 1850 7350
F 0 "#FLG0101" H 1850 7425 50  0001 C CNN
F 1 "PWR_FLAG" H 1850 7523 50  0000 C CNN
F 2 "" H 1850 7350 50  0001 C CNN
F 3 "~" H 1850 7350 50  0001 C CNN
	1    1850 7350
	-1   0    0    1   
$EndComp
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 60204D20
P 1450 7350
F 0 "#FLG0102" H 1450 7425 50  0001 C CNN
F 1 "PWR_FLAG" H 1450 7523 50  0000 C CNN
F 2 "" H 1450 7350 50  0001 C CNN
F 3 "~" H 1450 7350 50  0001 C CNN
	1    1450 7350
	-1   0    0    1   
$EndComp
$Comp
L power:+3V3 #PWR0104
U 1 1 60205316
P 1850 7350
F 0 "#PWR0104" H 1850 7200 50  0001 C CNN
F 1 "+3V3" H 1865 7523 50  0000 C CNN
F 2 "" H 1850 7350 50  0001 C CNN
F 3 "" H 1850 7350 50  0001 C CNN
	1    1850 7350
	1    0    0    -1  
$EndComp
$Comp
L power:+BATT #PWR0105
U 1 1 60205FF4
P 1450 7350
F 0 "#PWR0105" H 1450 7200 50  0001 C CNN
F 1 "+BATT" H 1465 7523 50  0000 C CNN
F 2 "" H 1450 7350 50  0001 C CNN
F 3 "" H 1450 7350 50  0001 C CNN
	1    1450 7350
	1    0    0    -1  
$EndComp
NoConn ~ 5500 3100
Text Label 6450 2000 2    50   ~ 0
VBUS
NoConn ~ 6450 2000
$Comp
L power:GND #PWR0103
U 1 1 601FE51F
P 1050 7350
F 0 "#PWR0103" H 1050 7100 50  0001 C CNN
F 1 "GND" H 1055 7177 50  0000 C CNN
F 2 "" H 1050 7350 50  0001 C CNN
F 3 "" H 1050 7350 50  0001 C CNN
	1    1050 7350
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG0103
U 1 1 6020CF02
P 1050 7350
F 0 "#FLG0103" H 1050 7425 50  0001 C CNN
F 1 "PWR_FLAG" H 1050 7523 50  0000 C CNN
F 2 "" H 1050 7350 50  0001 C CNN
F 3 "~" H 1050 7350 50  0001 C CNN
	1    1050 7350
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D2
U 1 1 6021AE05
P 5000 3350
F 0 "D2" V 4900 3500 50  0000 C CNN
F 1 "PWR_RED" V 5000 3600 50  0000 C CNN
F 2 "" H 5000 3350 50  0001 C CNN
F 3 "~" H 5000 3350 50  0001 C CNN
	1    5000 3350
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_US R2
U 1 1 6021D72C
P 5000 3650
F 0 "R2" H 4900 3750 50  0000 C CNN
F 1 "160" H 4886 3650 50  0000 C CNN
F 2 "" V 5040 3640 50  0001 C CNN
F 3 "~" H 5000 3650 50  0001 C CNN
	1    5000 3650
	-1   0    0    1   
$EndComp
Wire Wire Line
	5000 3200 5500 3200
$Comp
L power:GND #PWR02
U 1 1 6022F145
P 5200 3000
F 0 "#PWR02" H 5200 2750 50  0001 C CNN
F 1 "GND" H 5200 2850 50  0000 C CNN
F 2 "" H 5200 3000 50  0001 C CNN
F 3 "" H 5200 3000 50  0001 C CNN
	1    5200 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 3000 5500 3000
Wire Wire Line
	5000 3200 5000 2950
Text Label 5500 3200 2    50   ~ 0
3v3
Text Label 5500 3000 2    50   ~ 0
GND
Text Label 5500 1800 2    50   ~ 0
GND
Text Label 6450 1800 2    50   ~ 0
VBAT
Text Label 6450 2500 2    50   ~ 0
D9
NoConn ~ 6450 2500
Wire Wire Line
	1150 1850 1750 1850
Connection ~ 950  1850
Connection ~ 5000 3200
Text Notes 9600 4100 2    50   ~ 0
BUTTONS - ALL OF THEM HAVE INTERNAL PULLUP
$Comp
L Connector:Conn_01x16_Female J1
U 1 1 6015FC1F
P 5700 2500
F 0 "J1" H 5550 3350 50  0000 C CNN
F 1 "Feather Left" H 5650 1600 50  0000 C CNN
F 2 "" H 5700 2500 50  0001 C CNN
F 3 "~" H 5700 2500 50  0001 C CNN
	1    5700 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	1750 2450 1650 2450
$Comp
L Device:LED D1
U 1 1 60235515
P 1500 2450
F 0 "D1" H 1500 2350 50  0000 C CNN
F 1 "FIX_BLU" H 1500 2550 50  0000 C CNN
F 2 "" H 1500 2450 50  0001 C CNN
F 3 "~" H 1500 2450 50  0001 C CNN
	1    1500 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1350 2450 1300 2450
$Comp
L Device:R_US R1
U 1 1 60234526
P 1150 2450
F 0 "R1" V 1250 2450 50  0000 C CNN
F 1 "100" V 1050 2450 50  0000 C CNN
F 2 "" V 1190 2440 50  0001 C CNN
F 3 "~" H 1150 2450 50  0001 C CNN
	1    1150 2450
	0    1    1    0   
$EndComp
Wire Wire Line
	950  2150 950  2450
Wire Wire Line
	1000 2450 950  2450
Connection ~ 950  2450
Wire Wire Line
	950  2450 950  2900
Wire Wire Line
	1650 2750 1650 2450
Connection ~ 1650 2450
Wire Wire Line
	1650 2750 1600 2750
Text Label 8500 5700 2    50   ~ 0
BTN_A1
Text Label 8500 5300 2    50   ~ 0
BTN_A2
Text Label 8500 4900 2    50   ~ 0
BTN_A3
Connection ~ 8900 5700
Wire Wire Line
	8900 5850 8900 5700
$Comp
L power:GND #PWR04
U 1 1 60261C13
P 8900 5850
F 0 "#PWR04" H 8900 5600 50  0001 C CNN
F 1 "GND" H 8905 5677 50  0000 C CNN
F 2 "" H 8900 5850 50  0001 C CNN
F 3 "" H 8900 5850 50  0001 C CNN
	1    8900 5850
	1    0    0    -1  
$EndComp
Connection ~ 8900 5300
Wire Wire Line
	8900 5300 8900 5700
Wire Wire Line
	8900 4900 8900 5300
$Comp
L Switch:SW_Push SW3
U 1 1 6025805E
P 8700 4900
F 0 "SW3" H 8700 5185 50  0000 C CNN
F 1 "BTN" H 8700 5094 50  0000 C CNN
F 2 "" H 8700 5100 50  0001 C CNN
F 3 "~" H 8700 5100 50  0001 C CNN
	1    8700 4900
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW4
U 1 1 60258B51
P 8700 5300
F 0 "SW4" H 8700 5585 50  0000 C CNN
F 1 "BTN" H 8700 5494 50  0000 C CNN
F 2 "" H 8700 5500 50  0001 C CNN
F 3 "~" H 8700 5500 50  0001 C CNN
	1    8700 5300
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW5
U 1 1 602593BA
P 8700 5700
F 0 "SW5" H 8700 5985 50  0000 C CNN
F 1 "BTN" H 8700 5894 50  0000 C CNN
F 2 "" H 8700 5900 50  0001 C CNN
F 3 "~" H 8700 5900 50  0001 C CNN
	1    8700 5700
	1    0    0    -1  
$EndComp
Text Label 8500 4500 2    50   ~ 0
RST
Connection ~ 8900 4900
Wire Wire Line
	8900 4500 8900 4900
$Comp
L Switch:SW_Push SW2
U 1 1 6025657F
P 8700 4500
F 0 "SW2" H 8700 4785 50  0000 C CNN
F 1 "BTN_RST" H 8700 4694 50  0000 C CNN
F 2 "" H 8700 4700 50  0001 C CNN
F 3 "~" H 8700 4700 50  0001 C CNN
	1    8700 4500
	1    0    0    -1  
$EndComp
$EndSCHEMATC
