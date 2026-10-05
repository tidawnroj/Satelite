#pragma once

/*
  ReactionWheel.h — Reusable Reaction Wheel Subsystem
  SunSeek v1.3 Satellite Training Platform — Workshop T01
  
  Subsystem: ADCS Reaction Wheel Actuator
  Scope: Reaction Mode (Signed Command -100 to +100 %)
*/

#include <Arduino.h>
#include "RW_Config.h"

enum RWMode {
  RW_MODE_REACTION,
  RW_MODE_MOMENTUM
};

enum RWState {
  RW_STOPPED,
  RW_REACTION_DRIVE,
  RW_MOMENTUM_TARGET
};

static RWMode  _rwMode = RW_MODE_REACTION;
static RWState _rwState = RW_STOPPED;
static int     _rwMotorCommand = 0;
static int     _rwCurrentBias = 0;

// แปลงเปอร์เซ็นต์คำสั่ง (0-100%) เป็นค่า PWM (0-255)
inline int _rwPercentToPWM(int percent) {
  percent = constrain(percent, 0, 100);
  return map(percent, 0, 100, 0, 255);
}

// ขับมอเตอร์ผ่าน TB6612FNG (AIN1, AIN2, PWMA)
inline void _rwDrive(int command) {
  command = constrain(command, -100, 100);
  _rwMotorCommand = command;

  int pwm = _rwPercentToPWM(abs(command));

  if (command == 0) {
    // โหมดหยุด / Coasting
    digitalWrite(RW_PIN_AIN1, LOW);
    digitalWrite(RW_PIN_AIN2, LOW);
    analogWrite(RW_PIN_PWMA, 0);
  } else if (command > 0) {
    // หมุนไปข้างหน้า (Forward / Positive)
    digitalWrite(RW_PIN_AIN1, HIGH);
    digitalWrite(RW_PIN_AIN2, LOW);
    analogWrite(RW_PIN_PWMA, pwm);
  } else {
    // หมุนกลับทิศ (Reverse / Negative)
    digitalWrite(RW_PIN_AIN1, LOW);
    digitalWrite(RW_PIN_AIN2, HIGH);
    analogWrite(RW_PIN_PWMA, pwm);
  }
}

// เริ่มต้นฮาร์ดแวร์ Reaction Wheel
inline void rwBegin() {
  pinMode(RW_PIN_PWMA, OUTPUT);
  pinMode(RW_PIN_AIN1, OUTPUT);
  pinMode(RW_PIN_AIN2, OUTPUT);
  _rwDrive(0);
  _rwState = RW_STOPPED;
  _rwMode = RW_MODE_REACTION;
}

// วนรอบการทำงาน (สำหรับขยายต่อใน T02)
inline void rwUpdate() {
  // ใน T01 เป็น direct reaction drive ไม่ต้องการ non-blocking timer
}

// สั่งงานใน Reaction Mode (-100 ถึง +100)
inline void rwSetReactionCommand(int command) {
  _rwDrive(command);
  _rwState = (command == 0) ? RW_STOPPED : RW_REACTION_DRIVE;
}

// สั่งหยุดมอเตอร์ทันที
inline void rwStop() {
  _rwDrive(0);
  _rwState = RW_STOPPED;
}

// ตั้งค่า Bias (มีไว้เพื่อรองรับ Subsystem Interface ของ T02)
inline bool rwSetBias(int bias) {
  if (bias < 0 || bias > 100) return false;
  _rwCurrentBias = bias;
  return true;
}

// ฟังก์ชันอ่านค่าสถานะ
inline RWMode rwGetMode() { return _rwMode; }
inline RWState rwGetState() { return _rwState; }
inline int rwGetMotorCommand() { return _rwMotorCommand; }
inline int rwGetCurrentBias() { return _rwCurrentBias; }

inline String rwStateText() {
  switch (_rwState) {
    case RW_STOPPED:        return "STOPPED";
    case RW_REACTION_DRIVE: return "REACTION_DRIVE";
    case RW_MOMENTUM_TARGET:return "MOMENTUM_TARGET";
  }
  return "UNKNOWN";
}
