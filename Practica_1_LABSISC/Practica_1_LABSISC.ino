#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Configurar la dirección del LCD (prueba con 0x27 o 0x3F si no funciona)
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int FOTO = A3;
const int numMuestras = 10;  // Número de muestras para el promedio
int valoresADC[numMuestras]; // Array para almacenar las lecturas
int indice = 0;              // Índice para recorrer el array
long suma = 0;               // Variable para la suma de muestras
int promedioADC = 0;         // Valor promedio del ADC
int porcentual = 0;
int valoresLed = 0;
const int LED = 3; 

void setup() {
    lcd.init();  // Inicializar LCD
    Serial.begin(9600);
    analogReference(EXTERNAL);
    lcd.backlight();  // Encender la retroiluminación
    pinMode(FOTO, INPUT);
    pinMode(LED, OUTPUT);
    lcd.setCursor(0, 0);
    lcd.print("Lectura ADC:");
    delay(10);

    // Inicializar array de muestras con valores iniciales
    for (int i = 0; i < numMuestras; i++) {
        valoresADC[i] = analogRead(FOTO);
        suma += valoresADC[i];
    }
}

void loop() {
    // Restar el valor más antiguo de la suma
    suma -= valoresADC[indice];

    // Leer un nuevo valor del ADC
    valoresADC[indice] = analogRead(FOTO);

    // Sumar el nuevo valor a la suma
    suma += valoresADC[indice];

    // Calcular el promedio
    promedioADC = suma / numMuestras;

    // Convertir el promedio a porcentaje y asegurar que no supere 100
    porcentual = constrain(map(promedioADC, 10, 400, 0, 100), 0, 100);
    
    // Invertir la relación de brillo del LED (Menos luz = Más brillo)
    valoresLed = map(promedioADC, 14, 426, 255, 0);
    
    // Si el porcentaje es mayor a 90, reducir brillo en lugar de apagar abruptamente
    if (porcentual > 90) {
        valoresLed = 0;  // Brillo muy bajo pero visible
    }

    // Mostrar datos en el LCD
    lcd.setCursor(0, 1);
    lcd.print("Valor: ");
    lcd.print("    "); // Borra valores anteriores
    lcd.setCursor(7, 1);
    lcd.print(porcentual);
    lcd.print("%");

    // Aplicar el brillo al LED
    analogWrite(LED, valoresLed);
    Serial.println(promedioADC);

    // Mover índice circularmente
    indice = (indice + 1) % numMuestras;

    delay(100); // Retardo para hacer más legible la actualización
}
