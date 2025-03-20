#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Configuración de la pantalla LCD I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pines de los botones
const int btn_30 = 10;
const int btn_60 = 11;
const int btn_90 = 12;
const int btn_power = 7;

// Pines de control del motor en el L298N
const int motorENA = 3;  // PWM de velocidad
const int motorIN1 = 9;  // Dirección
const int motorIN2 = 8;  // Dirección

// Pin del potenciómetro y Servo
const int potPin = A0;
Servo myServo;

// Variables de control
int velocidad = 0;
bool motorEncendido = false;  // Estado del motor
bool lastBtnPowerState = HIGH; // Estado anterior del botón de apagado

// Variables para filtro de potenciómetro
int lastPotValue = 0;  // Último valor leído
int filteredPotValue = 0;  // Valor filtrado
int smoothingFactor = 4;  // Factor de suavizado (cuanto más alto, más suave)

void setup() {
    // Configurar botones como entrada con resistencias pull-up
    pinMode(btn_30, INPUT_PULLUP);
    pinMode(btn_60, INPUT_PULLUP);
    pinMode(btn_90, INPUT_PULLUP);
    pinMode(btn_power, INPUT_PULLUP);
    
    // Configurar pines del motor como salida
    pinMode(motorENA, OUTPUT);
    pinMode(motorIN1, OUTPUT);
    pinMode(motorIN2, OUTPUT);

    // Configurar motor en apagado al inicio
    digitalWrite(motorIN1, LOW);
    digitalWrite(motorIN2, LOW);
    analogWrite(motorENA, 0);

    // Iniciar la pantalla LCD
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Motor: OFF");
    
    // Iniciar Servo
    myServo.attach(6);  // Puedes cambiar el pin si es necesario
}

void loop() {
    // Control del botón de encendido/apagado
    bool currentBtnPowerState = digitalRead(btn_power); // Leer el estado actual del botón de apagado
    if (currentBtnPowerState == LOW && lastBtnPowerState == HIGH) {
        motorEncendido = !motorEncendido;  // Cambia el estado (ON/OFF)
        delay(200); // Pequeño debounce
    }
    lastBtnPowerState = currentBtnPowerState; // Actualizar el estado anterior del botón

    // Control de velocidad (encienden automáticamente el motor si estaba apagado)
    if (digitalRead(btn_30) == LOW) {
        velocidad = 76; // 30% de 255
        motorEncendido = true;
        actualizarIndicadores();
    }

    if (digitalRead(btn_60) == LOW) {
        velocidad = 153; // 60% de 255
        motorEncendido = true;
        actualizarIndicadores();
    }

    if (digitalRead(btn_90) == LOW) {
        velocidad = 230; // 90% de 255
        motorEncendido = true;
        actualizarIndicadores();
    }

    // Control del motor con el L298N
    if (motorEncendido) {
        digitalWrite(motorIN1, HIGH);  // Avance
        digitalWrite(motorIN2, LOW);
        analogWrite(motorENA, velocidad);
    } else {
        digitalWrite(motorIN1, LOW); // Apagar motor
        digitalWrite(motorIN2, LOW); // Asegurarse de que el motor no reciba energía
        analogWrite(motorENA, 0); // Detener el PWM
        lcd.setCursor(0, 0);
        lcd.print("Motor: OFF");
    }

    // Filtrar y suavizar la lectura del potenciómetro
    int potValue = analogRead(potPin);
    // Aplicar un filtro suave (promedio con el valor anterior)
    filteredPotValue = (filteredPotValue * smoothingFactor + potValue) / (smoothingFactor + 1);

    // Control del Servo con el potenciómetro (movimiento suave)
    int servoPos = map(filteredPotValue, 0, 1023, 0, 180);
    myServo.write(servoPos);
    
    delay(10); // Pequeño retardo para estabilidad
}

// Función para actualizar la pantalla LCD
void actualizarIndicadores() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Vel: ");
    lcd.print((velocidad * 100) / 255);
    lcd.print("%");
}
