#pragma once

/*
  RW_Config.h — Hardware Pins & Characterization Parameters
  SunSeek v1.3 Satellite Training Platform — Workshop T01
  
  Team Parameter Configuration:
  บันทึกและปรับแต่งค่าพารามิเตอร์ที่ได้จากการทดลองจริงใน Workshop T01
  เพื่อนำไปใช้ต่อ (Carry Forward) ใน Workshop T02-T04
*/

// Hardware Pin Configuration (SunSeek v1.3 ESP32-S3 -> TB6612FNG Driver)
#define RW_PIN_PWMA   1    // GPIO01: PWM Speed Control
#define RW_PIN_AIN1   38   // GPIO38: Direction Control 1
#define RW_PIN_AIN2   39   // GPIO39: Direction Control 2

// Characterization Parameters (กรอกผลลัพธ์จากการทดลอง T01 ของทีม)
// ค่าเหล่านี้จะได้จากการทดลอง Experiment 2 และ Experiment 4
#define RW_MIN_START_PWM       15    // Minimum Start PWM (%) ที่ล้อเริ่มหมุนจากหยุดนิ่ง (ตัวอย่างเริ่มต้น: ปรับตามผลจริง)
#define RW_MIN_STABLE_PWM      10    // Minimum Stable PWM (%) ค่าต่ำสุดที่ยังรักษาการหมุนได้อย่างต่อเนื่อง
#define RW_RECOMMENDED_MIN     20    // Recommended Minimum Operating PWM (%)
#define RW_RECOMMENDED_MAX     80    // Recommended Maximum Operating PWM (%)
#define RW_DEFAULT_BIAS        40    // Default Bias สำหรับ T02

// Direction Convention
// บันทึกทิศทางการหมุนของล้อตาม convention ของทีม เช่น CW (ตามเข็ม) หรือ CCW (ทวนเข็ม)
#define RW_POSITIVE_DIR_DESC   "CW (Clockwise looking from top)"
#define RW_NEGATIVE_DIR_DESC   "CCW (Counter-Clockwise looking from top)"
