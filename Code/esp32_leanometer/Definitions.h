//#define SERIAL_DEBUG
#ifdef SERIAL_DEBUG
//  #define SERIAL_MPU
#endif

#define USER_BUTTON 16
#define BOOT_BUTTON 0

#define BRIGHTNESS_PIN 13
#define PWM_CHAN 0
#define PWM_FREQ 1000
#define PWM_RES 8
#define LDR 32

#define SHIFT_REG_DATA 25
#define SHIFT_REG_CLOCK 26
#define SHIFT_REG_LATCH 27

#define I2C_SDA 21
#define I2C_SCL 22
#define I2C_FREQ 400000

#define IMU_INT 4

#define VBAT_PIN 33

#define SD_MOSI 23
#define SD_MISO 19
#define SD_SCK 18
#define SD_SS 5
#define SD_DET 17

#define LDR_MIN_THRES 200
#define LDR_MAX_THRES 900
//#define CA_7SDU
#define CC_7SDU

#define PREF_NAMESPACE "Leanometer_data"
#define LOG_PATH "/logs/"
#define RESET_LOG_PATH "/resetLog.txt"

/// OTA 
//#define SSID "Leanometer_AP"
// #define PASSWORD "yoyoyoyo"
