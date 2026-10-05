/*
  SunSeek_Training_Firmware_T01.ino
  SunSeek v1.3 Satellite Training Platform — Workshop T01
  
  Reaction Wheel Bring-up & Characterization (Reaction Mode Only)
  Thailand Young Satellite Challenge
  
  Subsystem: ADCS — Reaction Wheel Actuator
  Firmware: SunSeek Training Firmware T01 v1.0
  Baud Rate: 115200 (Line Ending: Newline หรือ Both NL & CR)
*/

#include "RW_Config.h"
#include "ReactionWheel.h"

String inputBuffer = "";

void processCommand(String cmd) {
  cmd.trim();
  if (cmd.length() == 0) return;

  if (cmd.equalsIgnoreCase("STOP")) {
    rwStop();
    Serial.println("ACK,STOP");
    Serial.println("STATUS: Motor stopped.");
  }
  else if (cmd.startsWith("RW,") || cmd.startsWith("rw,")) {
    String valStr = cmd.substring(3);
    valStr.trim();
    int val = valStr.toInt();

    // ตรวจสอบช่วงคำสั่ง (-100 ถึง +100)
    if (val < -100 || val > 100) {
      Serial.println("ERR,OUT_OF_RANGE: Command must be between -100 and +100");
      return;
    }

    rwSetReactionCommand(val);
    Serial.print("ACK,RW,");
    Serial.println(val);
    Serial.print("STATUS: Motor set to ");
    Serial.print(val);
    Serial.print("% (PWM: ");
    Serial.print(_rwPercentToPWM(abs(val)));
    Serial.println("/255)");
  }
  else if (cmd.equalsIgnoreCase("STATUS")) {
    Serial.println("--- REACTION WHEEL STATUS ---");
    Serial.print("State: ");
    Serial.println(rwStateText());
    Serial.print("Command: ");
    Serial.print(rwGetMotorCommand());
    Serial.println("%");
    Serial.print("Current Bias: ");
    Serial.print(rwGetCurrentBias());
    Serial.println("%");
    Serial.println("-----------------------------");
  }
  else if (cmd.startsWith("BIAS,") || cmd.startsWith("bias,")) {
    // คำสั่ง BIAS สงวนไว้สำหรับความเข้ากันได้กับ Workshop T02
    String valStr = cmd.substring(5);
    valStr.trim();
    int biasVal = valStr.toInt();
    if (rwSetBias(biasVal)) {
      Serial.print("ACK,BIAS,");
      Serial.println(biasVal);
    } else {
      Serial.println("ERR,INVALID_BIAS");
    }
  }
  else if (cmd.equalsIgnoreCase("PING")) {
    Serial.println("PONG");
  }
  else {
    Serial.print("ERR,UNKNOWN_COMMAND: ");
    Serial.println(cmd);
    Serial.println("Available commands: RW,<value> | STOP | STATUS | PING");
  }
}

void setup() {
  Serial.begin(115200);
  
  // รอการเชื่อมต่อ Serial เล็กน้อย
  delay(1000);

  // เริ่มต้นโมดูล Reaction Wheel
  rwBegin();

  // แสดง Banner ตามข้อกำหนดของ Workshop T01
  Serial.println();
  Serial.println("============================================");
  Serial.println("    SUNSEEK TRAINING FIRMWARE — T01");
  Serial.println("============================================");
  Serial.println("Subsystem: ADCS Reaction Wheel Bring-up");
  Serial.println("Platform: SunSeek v1.3 (ESP32-S3)");
  Serial.println("Baud Rate: 115200 bps | Line Ending: Newline");
  Serial.println("Ready for commands. Type RW,20 / RW,-20 / STOP / STATUS");
  Serial.println();
}

void loop() {
  // รับคำสั่งผ่าน USB Serial
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    if (c == '\n' || c == '\r') {
      if (inputBuffer.length() > 0) {
        processCommand(inputBuffer);
        inputBuffer = "";
      }
    } else {
      inputBuffer += c;
    }
  }

  // อัปเดต subsystem
  rwUpdate();
}
