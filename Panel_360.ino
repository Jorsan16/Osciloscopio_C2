#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// LDRs
const int LDR_TOP_LEFT = A0;
const int LDR_TOP_RIGHT = A1;
const int LDR_BOTTOM_LEFT = A2;
const int LDR_BOTTOM_RIGHT = A3;
const int LIGHT_THRESHOLD = 10;

// Servos 360°
Servo servo_horizontal;
Servo servo_vertical;

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
  lcd.init();
  lcd.backlight();

  // Servos
  servo_vertical.attach(5);
  servo_horizontal.attach(6);
  servo_vertical.write(90);   // Detenido
  servo_horizontal.write(90); // Detenido

  // Botones
  pinMode(MANUAL_MODE_BUTTON, INPUT_PULLUP);
  pinMode(AUTO_MODE_BUTTON, INPUT_PULLUP);
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(BUTTON_3, INPUT_PULLUP);
  pinMode(BUTTON_4, INPUT_PULLUP);

  // LCD inicial
  lcd.setCursor(0, 0);
  lcd.print("Modo: AUTO");
  lcd.setCursor(0, 1);
  lcd.print("Servos 360");
}

void loop() {
  if (digitalRead(MANUAL_MODE_BUTTON) == LOW) {
    delay(200);
    manualMode = true;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Modo: MANUAL");
    lcd.setCursor(0, 1);
    lcd.print("Servos 360");
    delay(500);
  }

  if (digitalRead(AUTO_MODE_BUTTON) == LOW) {
    delay(200);
    manualMode = false;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Modo: AUTO");
    lcd.setCursor(0, 1);
    lcd.print("Servos 360");
    delay(500);
  }

  if (manualMode) {
    checkManualButtons();
  } else {
    int ldr_tl_value = analogRead(LDR_TOP_LEFT);
    int ldr_tr_value = analogRead(LDR_TOP_RIGHT);
    int ldr_bl_value = analogRead(LDR_BOTTOM_LEFT);
    int ldr_br_value = analogRead(LDR_BOTTOM_RIGHT);

    int avg_top = (ldr_tl_value + ldr_tr_value) / 2;
    int avg_bottom = (ldr_bl_value + ldr_br_value) / 2;
    int avg_left = (ldr_tl_value + ldr_bl_value) / 2;
    int avg_right = (ldr_tr_value + ldr_br_value) / 2;

    moveServos360(avg_top, avg_bottom, avg_left, avg_right);
  }

  if ((millis() - lastTime) > threshold) {
    lastTime = millis();
    float current_measured = medirCorriente(CURRENT_SENSOR, SENSIBILITY, CURRENT_SAMPLES);
    Serial.print("Corriente: ");
    Serial.println(current_measured);
  }

  delay(30);
}

void moveServos360(int avg_top, int avg_bottom, int avg_left, int avg_right) {
  // Vertical
  if ((avg_top - avg_bottom) > LIGHT_THRESHOLD) {
    servo_vertical.write(180); // gira hacia arriba
  } else if ((avg_bottom - avg_top) > LIGHT_THRESHOLD) {
    servo_vertical.write(0);   // gira hacia abajo
  } else {
    servo_vertical.write(90);  // detener
  }

  // Horizontal
  if ((avg_left - avg_right) > LIGHT_THRESHOLD) {
    servo_horizontal.write(180); // gira hacia la izquierda
  } else if ((avg_right - avg_left) > LIGHT_THRESHOLD) {
    servo_horizontal.write(0);   // gira hacia la derecha
  } else {
    servo_horizontal.write(90);  // detener
  }
}

float medirCorriente(int current_sensor, float sensibility, int samples) {
  float sensor_read = 0;
  for (int i = 0; i < samples; i++) {
    sensor_read += analogRead(current_sensor);
  }
  sensor_read /= samples;
  sensor_read *= (5.0 / 1023.0);
  return (sensor_read - 2.5) / sensibility;
}

void checkManualButtons() {
  if (digitalRead(BUTTON_1) == LOW) {
    servo_vertical.write(180);
    servo_horizontal.write(90);
    delay(500);
    stopServos();
  } else if (digitalRead(BUTTON_2) == LOW) {
    servo_vertical.write(0);
    servo_horizontal.write(90);
    delay(500);
    stopServos();
  } else if (digitalRead(BUTTON_3) == LOW) {
    servo_vertical.write(90);
    servo_horizontal.write(180);
    delay(500);
    stopServos();
  } else if (digitalRead(BUTTON_4) == LOW) {
    servo_vertical.write(90);
    servo_horizontal.write(0);
    delay(500);
    stopServos();
  }
}

void stopServos() {
  servo_vertical.write(90);
  servo_horizontal.write(90);
}
