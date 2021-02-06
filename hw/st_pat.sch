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
L Switch:SW_MEC_5G SW1
U 1 1 6017D3E6
P 5700 5200
F 0 "SW1" H 5700 5485 50  0000 C CNN
F 1 "SW_MEC_5G" H 5700 5394 50  0000 C CNN
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
F 1 "Conn_01x09_Female" H 9078 2135 50  0000 L CNN
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
Text Label 8850 2100 2    50   ~ 0
SCK
Text Label 8850 2200 2    50   ~ 0
MOSI
Text Label 8850 2300 2    50   ~ 0
D5
Wire Wire Line
	8550 1800 8850 1800
Text Label 8850 2500 2    50   ~ 0
D6
$Comp
L power:+3V3 #PWR01
U 1 1 60185239
P 1550 1850
F 0 "#PWR01" H 1550 1700 50  0001 C CNN
F 1 "+3V3" H 1565 2023 50  0000 C CNN
F 2 "" H 1550 1850 50  0001 C CNN
F 3 "" H 1550 1850 50  0001 C CNN
	1    1550 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 1850 1750 1850
Text Label 1750 2000 2    50   ~ 0
D9
$Comp
L power:GND #PWR02
U 1 1 601867C3
P 1550 2300
F 0 "#PWR02" H 1550 2050 50  0001 C CNN
F 1 "GND" H 1555 2127 50  0000 C CNN
F 2 "" H 1550 2300 50  0001 C CNN
F 3 "" H 1550 2300 50  0001 C CNN
	1    1550 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 2300 1750 2300
Text Label 1750 2450 2    50   ~ 0
D10
$Comp
L power:GND #PWR03
U 1 1 60187238
P 1550 2900
F 0 "#PWR03" H 1550 2650 50  0001 C CNN
F 1 "GND" H 1555 2727 50  0000 C CNN
F 2 "" H 1550 2900 50  0001 C CNN
F 3 "" H 1550 2900 50  0001 C CNN
	1    1550 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	1550 2900 1750 2900
Text Label 1750 3050 2    50   ~ 0
RX
Text Label 1750 3200 2    50   ~ 0
TX
$Comp
L power:GND #PWR05
U 1 1 60187F6E
P 3250 2000
F 0 "#PWR05" H 3250 1750 50  0001 C CNN
F 1 "GND" H 3255 1827 50  0000 C CNN
F 2 "" H 3250 2000 50  0001 C CNN
F 3 "" H 3250 2000 50  0001 C CNN
	1    3250 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 2000 2800 2000
Text Label 2800 2900 0    50   ~ 0
D11
$Comp
L power:GND #PWR06
U 1 1 601889EB
P 3250 3050
F 0 "#PWR06" H 3250 2800 50  0001 C CNN
F 1 "GND" H 3255 2877 50  0000 C CNN
F 2 "" H 3250 3050 50  0001 C CNN
F 3 "" H 3250 3050 50  0001 C CNN
	1    3250 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 3050 2800 3050
NoConn ~ 2800 2750
$Comp
L power:+3V3 #PWR04
U 1 1 60189812
P 2800 3200
F 0 "#PWR04" H 2800 3050 50  0001 C CNN
F 1 "+3V3" V 2815 3328 50  0000 L CNN
F 2 "" H 2800 3200 50  0001 C CNN
F 3 "" H 2800 3200 50  0001 C CNN
	1    2800 3200
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x02_Female J4
U 1 1 601931F0
P 6100 5200
F 0 "J4" H 6128 5176 50  0000 L CNN
F 1 "BATTERY" H 6128 5085 50  0000 L CNN
F 2 "" H 6100 5200 50  0001 C CNN
F 3 "~" H 6100 5200 50  0001 C CNN
	1    6100 5200
	1    0    0    -1  
$EndComp
Text Label 5500 5200 2    50   ~ 0
VBAT
$Comp
L power:GND #PWR014
U 1 1 60196448
P 5750 5300
F 0 "#PWR014" H 5750 5050 50  0001 C CNN
F 1 "GND" H 5755 5127 50  0000 C CNN
F 2 "" H 5750 5300 50  0001 C CNN
F 3 "" H 5750 5300 50  0001 C CNN
	1    5750 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 5300 5900 5300
$Comp
L Connector:Conn_01x06_Female J?
U 1 1 601E0556
P 2950 5000
F 0 "J?" H 2978 4976 50  0000 L CNN
F 1 "Conn_01x06_Female" H 2700 4550 50  0000 L CNN
F 2 "" H 2950 5000 50  0001 C CNN
F 3 "~" H 2950 5000 50  0001 C CNN
	1    2950 5000
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x04_Female J?
U 1 1 601DFEB9
P 2300 5000
F 0 "J?" H 2328 4976 50  0000 L CNN
F 1 "Conn_01x04_Female" H 1750 5350 50  0000 L CNN
F 2 "" H 2300 5000 50  0001 C CNN
F 3 "~" H 2300 5000 50  0001 C CNN
	1    2300 5000
	1    0    0    -1  
$EndComp
Text Notes 1750 5450 0    50   ~ 0
ADAFRUIT COMPASS
Wire Wire Line
	8550 2000 8850 2000
$Comp
L power:+3V3 #PWR012
U 1 1 601813E6
P 8550 1800
F 0 "#PWR012" H 8550 1650 50  0001 C CNN
F 1 "+3V3" H 8565 1973 50  0000 C CNN
F 2 "" H 8550 1800 50  0001 C CNN
F 3 "" H 8550 1800 50  0001 C CNN
	1    8550 1800
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR?
U 1 1 60203BD8
P 2650 4800
F 0 "#PWR?" H 2650 4650 50  0001 C CNN
F 1 "+3V3" H 2665 4973 50  0000 C CNN
F 2 "" H 2650 4800 50  0001 C CNN
F 3 "" H 2650 4800 50  0001 C CNN
	1    2650 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 4800 2750 4800
NoConn ~ 2750 4900
$Comp
L power:GND #PWR?
U 1 1 60204755
P 2650 5000
F 0 "#PWR?" H 2650 4750 50  0001 C CNN
F 1 "GND" H 2655 4827 50  0000 C CNN
F 2 "" H 2650 5000 50  0001 C CNN
F 3 "" H 2650 5000 50  0001 C CNN
	1    2650 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 5000 2750 5000
NoConn ~ 2750 5100
NoConn ~ 2750 5200
NoConn ~ 2750 5300
$Comp
L power:GND #PWR?
U 1 1 602057AC
P 1800 4900
F 0 "#PWR?" H 1800 4650 50  0001 C CNN
F 1 "GND" H 1805 4727 50  0000 C CNN
F 2 "" H 1800 4900 50  0001 C CNN
F 3 "" H 1800 4900 50  0001 C CNN
	1    1800 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 4900 2100 4900
Text Label 2100 5100 2    50   ~ 0
MISO
Text Label 2100 5200 2    50   ~ 0
A4
Text Label 2100 5000 2    50   ~ 0
A5
Text Notes 2400 1350 2    50   ~ 0
GPS MODULE
Text Notes 5900 4650 2    50   ~ 0
POWER SWITCH
$Comp
L power:+BATT #PWR011
U 1 1 6017627A
P 6100 1800
F 0 "#PWR011" H 6100 1650 50  0001 C CNN
F 1 "+BATT" V 6115 1927 50  0000 L CNN
F 2 "" H 6100 1800 50  0001 C CNN
F 3 "" H 6100 1800 50  0001 C CNN
	1    6100 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 1800 6300 1800
$Comp
L power:VBUS #PWR010
U 1 1 601756EE
P 6100 2000
F 0 "#PWR010" H 6100 1850 50  0001 C CNN
F 1 "VBUS" V 5950 1950 50  0000 L CNN
F 2 "" H 6100 2000 50  0001 C CNN
F 3 "" H 6100 2000 50  0001 C CNN
	1    6100 2000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR013
U 1 1 60181D61
P 8550 2000
F 0 "#PWR013" H 8550 1750 50  0001 C CNN
F 1 "GND" H 8555 1827 50  0000 C CNN
F 2 "" H 8550 2000 50  0001 C CNN
F 3 "" H 8550 2000 50  0001 C CNN
	1    8550 2000
	1    0    0    -1  
$EndComp
Text Notes 5400 1400 0    50   ~ 0
FEATHER PINS
Wire Wire Line
	6100 2000 6300 2000
Wire Wire Line
	5250 1800 5500 1800
$Comp
L power:GND #PWR09
U 1 1 60174E09
P 5250 1800
F 0 "#PWR09" H 5250 1550 50  0001 C CNN
F 1 "GND" V 5255 1672 50  0000 R CNN
F 2 "" H 5250 1800 50  0001 C CNN
F 3 "" H 5250 1800 50  0001 C CNN
	1    5250 1800
	-1   0    0    1   
$EndComp
Wire Wire Line
	5250 3000 5500 3000
$Comp
L power:GND #PWR08
U 1 1 60174547
P 5250 3000
F 0 "#PWR08" H 5250 2750 50  0001 C CNN
F 1 "GND" H 5255 2827 50  0000 C CNN
F 2 "" H 5250 3000 50  0001 C CNN
F 3 "" H 5250 3000 50  0001 C CNN
	1    5250 3000
	-1   0    0    1   
$EndComp
Wire Wire Line
	5250 3200 5500 3200
$Comp
L power:+3V3 #PWR07
U 1 1 60172EEF
P 5250 3200
F 0 "#PWR07" H 5250 3050 50  0001 C CNN
F 1 "+3V3" H 5265 3373 50  0000 C CNN
F 2 "" H 5250 3200 50  0001 C CNN
F 3 "" H 5250 3200 50  0001 C CNN
	1    5250 3200
	-1   0    0    1   
$EndComp
Text Label 6300 2900 2    50   ~ 0
SDA
Text Label 6300 2800 2    50   ~ 0
SCI
Text Label 6300 2700 2    50   ~ 0
D5
Text Label 6300 2600 2    50   ~ 0
D6
Text Label 6300 2500 2    50   ~ 0
D9
Text Label 6300 2400 2    50   ~ 0
D10
Text Label 6300 2300 2    50   ~ 0
D11
Text Label 6300 2200 2    50   ~ 0
D12
Text Label 6300 2100 2    50   ~ 0
D13
Text Label 6300 1900 2    50   ~ 0
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
A5
Text Label 5500 2500 2    50   ~ 0
A4
Text Label 5500 2600 2    50   ~ 0
A3
Text Label 5500 2700 2    50   ~ 0
A2
Text Label 5500 2800 2    50   ~ 0
A1
Text Label 5500 2900 2    50   ~ 0
A0
Text Label 5500 3100 2    50   ~ 0
AREF
Text Label 5500 3300 2    50   ~ 0
RST
$Comp
L Connector:Conn_01x16_Female J1
U 1 1 6015FC1F
P 5700 2500
F 0 "J1" H 5600 1550 50  0000 C CNN
F 1 "Feather Left" V 5800 2550 50  0000 C CNN
F 2 "" H 5700 2500 50  0001 C CNN
F 3 "~" H 5700 2500 50  0001 C CNN
	1    5700 2500
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x12_Female J2
U 1 1 6015F140
P 6500 2300
F 0 "J2" H 6350 2950 50  0000 L CNN
F 1 "Feather Right" V 6000 1900 50  0000 L CNN
F 2 "" H 6500 2300 50  0001 C CNN
F 3 "~" H 6500 2300 50  0001 C CNN
	1    6500 2300
	1    0    0    -1  
$EndComp
$EndSCHEMATC
