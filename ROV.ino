#define REMOTEXY_MODE__ESP32CORE_WIFI_POINT
#include <WiFi.h>

#include <RemoteXY.h>

// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID "ROV"
#define REMOTEXY_WIFI_PASSWORD "RedRightHand"
#define REMOTEXY_SERVER_PORT 6377
//esc variables
//#define ESC1 (33) // left bldc pin 33 GPIO
//#define ESC2 (32) // right bldc pin 32 GPIO
//#define ESC3 (25)// thrust bldc pin 25 GPIO

// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 36 bytes
  { 255,4,0,0,0,29,0,16,20,0,5,32,67,14,30,30,2,26,31,4,
  0,10,12,8,32,2,26,1,0,41,42,12,12,2,31,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  int8_t joystick_1_x; // from -100 to 100  
  int8_t joystick_1_y; // from -100 to 100  
  int8_t slider_1; // =0..100 slider position 
  uint8_t button_1; // =1 if button pressed, else =0 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)
#include <ESP32Servo.h>

int servo1_Center = 1500;
int servo2_Center = 1500;
Servo firstESC, secondESC, thirdESC, servo1, servo2; 

void setup() 
{
  RemoteXY_Init();

  firstESC.attach(25, 1000, 2000);
  secondESC.attach(33, 1000, 2000);
  servo1.attach(19);
  servo2.attach(18);
}

void loop() {
  RemoteXY_Handler();
  int xValue = RemoteXY.joystick_1_x;
  int yValue = RemoteXY.joystick_1_y;
  if (RemoteXY.button_1!=0) {
   ESP.restart(); //if button pressed reset esp
  }
  else {
    /*  button not pressed */
  }

  if (RemoteXY.slider_1>0){  //f the slider is greater than 0 then the motor will activate
    if (xValue > 50) {   //if the x greater than 50 then move right
    firstESC.write(1750);
    secondESC.write(1250);
    }
    else if (xValue < -50) { //if the x smaller than -50 then move right
    secondESC.write(1750);
    firstESC.write(1250);
    }
    else{ //if x = 0 then read the slider numbers
    int sliderValue = RemoteXY.slider_1; 
int mappedValue = map(sliderValue, 0, 100, 1000, 2000); // map slider value to pulse width range firstESC.writeMicroseconds(mappedValue); // send pulse width to ESC to control
secondESC.writeMicroseconds(mappedValue);
firstESC.writeMicroseconds(mappedValue);
  
    }
  }
  else if (RemoteXY.slider_1=0){  //slider  equal 0 then motor will not activate
    secondESC.write(0);
    firstESC.write(0);
  }

  servo1.writeMicroseconds(yValue * 10 + servo1_Center);   //servo1 code
  servo2.writeMicroseconds(yValue * 10 + servo2_Center);   //servo2 code

}