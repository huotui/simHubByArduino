// Simple gamepad example that demonstraits how to read five Arduino
// digital pins and map them to the Arduino Joystick library.
//
// The digital pins 2 - 6 are grounded when they are pressed.
// Pin 2 = UP
// Pin 3 = RIGHT
// Pin 4 = DOWN
// Pin 5 = LEFT
// Pin 6 = FIRE
//
// NOTE: This sketch file is for use with Arduino Leonardo and
//       Arduino Micro only.
//
// by Matthew Heironimus
// 2016-11-24
//--------------------------------------------------------------------

#include <Joystick.h>

#define SERIAL_BUFFER_SIZE 1

//
//Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
//  6, 0,                  // Button Count, Hat Switch Count
//  true, true, false,     // X and Y, but no Z Axis
//  false, false, false,   // No Rx, Ry, or Rz
//  false, false,          // No rudder or throttle
//  false, false, false);  // No accelerator, brake, or steering


short keyMax = 10;
short ButtonMap[10] = {2, 3, 4, 5, 6, 7,8, 10, 14, 15};
void setup() {
  // Initialize Button Pins
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);


  //透传模块设置
  short d9 = 9;
  pinMode(d9, OUTPUT);
  digitalWrite(d9, HIGH); //高电平透传


  // Serial.begin(9600); //初始化串口并设置波特率为9600 串口1pc通信专用
  //Serial.println("osh");
  Serial1.begin(9600); //初始化串口并设置波特率为9600 串口2 板载使用 连接无线透传模块，默认9600
  //Serial1.println("osh");
}

// Last state of the buttons
short lastButtonState[10] = {0, 0, 0, 0, 0, 0,0, 0, 0, 0};
//
short sendTimes=3;
short lastButtonStateAck[10] = {0, 0, 0, 0, 0, 0,0, 0, 0, 0};

//TODO 
//1 注册设备 设置id，设备按键占用x-y
//2 接收注册设备，判断按键是否已经注册，注册就返回不能注册
//3 接收是否注册成功，成功就开始发送按键信息，失败就不发现 增加led 标示是否注册成功
//4 定义数据包格式


//发送 优化通信模式 增加按键send-ack 3次

void sendListClear(short key) {
  lastButtonStateAck[key]=-1;
}

void sendList(short key) {
  lastButtonStateAck[key]=sendTimes;
}

void sendRow(short key, short value) {
  short keyAndValue = 0;
  //high is value
  //low is keyNum 0-99
  keyAndValue = 100 * value;
  keyAndValue = keyAndValue + key;
  Serial1.write(keyAndValue);     //写入二进制数据到串口
  //Serial1.write(keyAndValue);
}


void loop() {
//读取按键
  for (short index = 0; index < keyMax; index++)
  {
    int currentButtonState = !digitalRead(ButtonMap[index]);
    if (currentButtonState != lastButtonState[index])
    {
      lastButtonState[index] = currentButtonState;
      sendList(index);
    } 
  }

//发送队列
  for (short index = 0; index < keyMax; index++)
  {
    short times = lastButtonStateAck[index];
    if(times>0){
      times=times-1;
      short now_value = lastButtonState[index];
      sendRow(index, now_value);
    }
  }
  
//接收响应
  if (Serial1.available() > 0) {
    char inChar = Serial1.read();
    if (inChar != 255) {
      if ( inChar <= 200 ) {
        short key = 0;
        short Value = 0;
        if (inChar >= 100) {
          Value = 1;
          key = inChar - 100;
        } else {
          Value = 0;
          key = inChar;
        }
        if (key >= 0 && key < keyMax ) {
            sendListClear(key);
        }
      }
    }
  }


}
