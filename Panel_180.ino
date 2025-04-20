#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// LDRs
const int LDR_TOP_LEFT = A0;
const int LDR_TOP_RIGHT = A1;
const int LDR_BOTTOM_LEFT = A2;
const int LDR_BOTTOM_RIGHT = A3;
const int LIGHT_THRESHOLD = 10;

// Servos
Servo servo_horizontal;
Servo servo_vertical;
int pos_sh = 90;
int pos_sv = 90;
const int UPPER_LIMIT_POS = 160;
const int LOWER_LIMIT_POS = 20;

// Sensor de corriente
const int CURRENT_SENSOR = A6;
const float SENSIBILITY = 0.185;
const int CURRENT_SAMPLES = 10;

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Tiempo
unsigned long lastTime = 0;
unsigned long threshold = 2000;

// Botones
const int MANUAL_MODE_BUTTON = 7;
const int AUTO_MODE_BUTTON = 12;
const int BUTTON_1 = 8;
const int BUTTON_2 = 9;
const int BUTTON_3 = 10;
const int BUTTON_4 = 11;

bool manualMode = false;

void setup() {
  Serial.begin(115200);
  delay(500);
  lcd.init();
  delay(2000);
  lcd.backlight();
  
  // Servos
  servo_vertical.attach(5);
  servo_horizontal.attach(6);
  servo_vertical.write(pos_sv);
  servo_horizontal.write(pos_sh);

  // Botones
  pinMode(MANUAL_MODE_BUTTON, INPUT_PULLUP);
  pinMode(AUTO_MODE_BUTTON, INPUT_PULLUP);
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(BUTTON_3, INPUT_PULLUP);
  pinMode(BUTTON_4, INPUT_PULLUP);

  // Mostrar estado inicial
  lcd.setCursor(0, 0);
  lcd.print("Modo: AUTO");
  lcd.setCursor(0, 1);
  lcd.print("V:");
  lcd.print(pos_sv);
  lcd.print(" H:");
  lcd.print(pos_sh);
}

void loop() {
  // Cambiar a modo manual
  if (digitalRead(MANUAL_MODE_BUTTON) == LOW) {
    delay(200);
    manualMode = true;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Modo: MANUAL");
    lcd.setCursor(0, 1);
    lcd.print("V:");
    lcd.print(pos_sv);
    lcd.print(" H:");
    lcd.print(pos_sh);
    delay(500);
  }

  // Cambiar a modo automático
  if (digitalRead(AUTO_MODE_BUTTON) == LOW) {
    delay(200);
    manualMode = false;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Modo: AUTO");
    lcd.setCursor(0, 1);
    lcd.print("V:");a
    lcd.print(pos_sv);
    lcd.print(" H:");
    lcd.print(pos_sh);
    delay(500);
  }

  // Modo manual o automático
  if (manualMode) {
    checkManualButtons();
  } else {
    int ldr_tl_value = analogRead(LDR_TOP_LEFT);
    int ldr_tr_value = analogRead(LDR_TOP_RIGHT);
    int ldr_bl_value = analogRead(LDR_BOTTOM_LEFT);
    int ldr_br_value = analogRead(LDR_BOTTOM_RIGHT);

    int average_top = (ldr_tl_value + ldr_tr_value) / 2;
    int average_bottom = (ldr_bl_value + ldr_br_value) / 2;
    int average_left = (ldr_tl_value + ldr_bl_value) / 2;
    int average_right = (ldr_tr_value + ldr_br_value) / 2;

    moveSolarTracker(average_top, average_bottom, average_left, average_right);
  }

  // Mostrar corriente cada cierto tiempo
  if ((millis() - lastTime) > threshold) {
    lastTime = millis();
    float current_measured = medirCorriente(CURRENT_SENSOR, SENSIBILITY, CURRENT_SAMPLES);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(manualMode ? "Modo: MANUAL" : "Modo: AUTO");
    lcd.setCursor(0, 1);
    lcd.print("V:");
    lcd.print(pos_sv);
    lcd.print(" H:");
    lcd.print(pos_sh);
    Serial.print("Corriente: ");
    Serial.println(current_measured);
  }

  delay(30);
}

void moveSolarTracker(int average_top, int average_bottom, int average_left, int average_right) {
  if ((average_top - average_bottom) > LIGHT_THRESHOLD && pos_sv < UPPER_LIMIT_POS) {
    pos_sv++;
    servo_vertical.write(pos_sv);
  } else if ((average_bottom - average_top) > LIGHT_THRESHOLD && pos_sv > LOWER_LIMIT_POS) {
    pos_sv--;
    servo_vertical.write(pos_sv);
  }

  if ((average_left - average_right) > LIGHT_THRESHOLD && pos_sh < UPPER_LIMIT_POS) {
    pos_sh++;
    servo_horizontal.write(pos_sh);
  } else if ((average_right - average_left) > LIGHT_THRESHOLD && pos_sh > LOWER_LIMIT_POS) {
    pos_sh--;
    servo_horizontal.write(pos_sh);
  }
}

float medirCorriente(int current_sensor, float sensibility, int samples) {
  float sensor_read = 0;
  for (int i = 0; i < samples; i++) {
    sensor_read += analogRead(CURRENT_SENSOR);
  }
  sensor_read = sensor_read / samples;
  sensor_read = sensor_read * (5.0 / 1023.0);
  return (sensor_read - 2.5) / sensibility;
}

void checkManualButtons() {
  if (digitalRead(BUTTON_1) == LOW) {
    pos_sv = 160;
    pos_sh = 90;
    servo_vertical.write(pos_sv);
    servo_horizontal.write(pos_sh);
    updateLCDPosition();
    delay(500);
  } else if (digitalRead(BUTTON_2) == LOW) {
    pos_sv = 20;
    pos_sh = 90;
    servo_vertical.write(pos_sv);
    servo_horizontal.write(pos_sh);
    updateLCDPosition();
    delay(500);
  } else if (digitalRead(BUTTON_3) == LOW) {
    pos_sv = 90;
    pos_sh = 160;
    servo_vertical.write(pos_sv);
    servo_horizontal.write(pos_sh);
    updateLCDPosition();
    delay(500);
  } else if (digitalRead(BUTTON_4) == LOW) {
    pos_sv = 90;
    pos_sh = 20;
    servo_vertical.write(pos_sv);
    servo_horizontal.write(pos_sh);
    updateLCDPosition();
    delay(500);
  }
}

void updateLCDPosition() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(manualMode ? "Modo: MANUAL" : "Modo: AUTO");
  lcd.setCursor(0, 1);
  lcd.print("V:");
  lcd.print(pos_sv);
  lcd.print(" H:");
  lcd.print(pos_sh);
}
