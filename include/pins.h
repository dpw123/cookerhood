#ifndef PINS

#define PINS
// D4 for Relay 1  (GPIO2)
#define Relay1 2
// D3 for Relay 2 (GPIO0)
#define Relay2 0
// D2 for Relay 2 (GPIO4)
#define Relay3 4
// D1 for Relay 2 (GPIO5)
#define Relay4 5


// D6 connected to wire 3 for the on_off button (GPIO 14)
#define onoff_button 14
// D7 connected to wire 8 for the on_off button (GPIO 13)
#define speed_button 13
// D0 connected to wire 4 for the timer button (GPIO 12)
#define timer_button 12
// D5 connected to wire 2 for the light button (GPIO16)
#define light_button 16 


#endif

/* header tape from control board

RED
1 VC
2 x
3 x
4 pin 17 and light switch button
5 pin 2 
6 pin 3?
7  
8 x
9 x
10 GND

*/

/*  pin reference

Label	GPIO	Input	        Output	                Notes
D0	    GPIO16	no interrupt	no PWM or I2C support	HIGH at boot, used to wake up from deep sleep
D1	    GPIO5	OK	            OK	                    often used as SCL (I2C)
D2	    GPIO4	OK	            OK	                    often used as SDA (I2C)
D3	    GPIO0	pulled up	    OK	                    connected to FLASH button, boot fails if pulled LOW
D4	    GPIO2	pulled up	    OK	                    HIGH at boot, connected to on-board LED, boot fails if pulled LOW
D5	    GPIO14	OK	            OK	                    SPI (SCLK)
D6	    GPIO12	OK	            OK	                    SPI (MISO)
D7	    GPIO13	OK	            OK	                    SPI (MOSI)
D8	    GPIO15	pulled to GND	OK	                    SPI (CS), Boot fails if pulled HIGH
RX	    GPIO3	OK	            RX pin	                HIGH at boot
TX	    GPIO1	TX pin	        OK	                    HIGH at boot, debug output at boot, boot fails if pulled LOW
A0	    ADC0	Analog Input	X	

*/