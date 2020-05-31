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

short isDebug = 0;
short keyMax = 20;
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
                   20, 0,                  // Button Count, Hat Switch Count
                   true, true, false,     // X and Y, but no Z Axis
                   false, false, false,   // No Rx, Ry, or Rz
                   false, false,          // No rudder or throttle
                   false, false, false);  // No accelerator, brake, or steering

void setup() {
  // Initialize Button Pins

  // Initialize Joystick Library
  Joystick.begin();

  //透传模块设置
  int d9 = 9;
  pinMode(d9, OUTPUT);
  digitalWrite(d9, HIGH); //高电平透传

  if (isDebug > 0) {

    Serial.begin(9600); //初始化串口并设置波特率为9600 串口1pc通信专用
    Serial.println("osh");
  }
  Serial1.begin(9600); //初始化串口并设置波特率为9600 串口2 板载使用 连接无线透传模块，默认9600
  Serial1.println("osh");

  //初始化按键
  for (short index = 0; index < keyMax; index++)
  {
    Joystick.setButton(index, 0);
  }
}

// Last state of the buttons
int lastButtonState[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


//总结
//很容易丢包，在nrf24l01电源两端并联一个100uf左右的电容，能有效解决。四天得来的经验送你了。
//模块瞬间用电量大，有可能供电不足模块瞬间断电导致模块工作异常，用电容存储电量保证模块电源不间断
//1，电源必须在它的电压符合范围之内，不能接5V，会烧掉，某宝有1117稳压模块比较好。
//2，供电电源波纹必须在80mv以内，就是波动不能超过0.08V，某宝有1117稳压模块，并联一个100UF的点解电容和105的瓷片电容。


void loop() {

  if (Serial1.available() > 0) {
    char inChar = Serial1.read();
    //响应
    Serial1.write(inChar);

    if (isDebug > 0) {
      Serial.write(inChar);
    }
    //解析按键
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
        if (key >= 0 && key <= 19 ) {

          int lastValue = lastButtonState[key];
          if (lastValue != Value) {
            lastButtonState[key] = Value;
            Joystick.setButton(key, Value);
          }
        }

      }
    }
  }




}
