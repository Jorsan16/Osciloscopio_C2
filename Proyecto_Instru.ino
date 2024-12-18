//----------------------------Proyecto Instrumentacion Electrica (Emisor)-----------------------------------


//--------------------- Inicializacion de Librerias --------------------------------------------------
#include <RH_ASK.h>		// incluye libreria RadioHead.h
#include <SPI.h> 		// incluye libreria SPI necesaria por RadioHead.h
RH_ASK rf_driver;


//-----------------------Rangos de la lectura ----------------------------------------
const int RS_1 = 300; //Movimiento Adelante
const int RI_1 = 100;

const int RS_2 = 600; //Movimiento Atras
const int RI_2 = 400;

const int RS_3 = 800; //Movimiento Derecha
const int RI_3 = 700;

const int RS_4 = 1000; //Movimiento Izquierda
const int RI_4 = 900;


//----------------------------- Variables y Pines a usar ---------------------------------------------
const int AZUL = 2;
const int ROJO = 3;
const int AMARILLO = 4;
const int VERDE = 5;

const int emgPin = A0; // Pin analógico conectado al sensor EMG
const int numSamples = 10; // Número de muestras para la media móvil
int samples[numSamples]; // Array para almacenar las muestras
int sampleIndex = 0;
int total = 0;
int smoothedValue = 0;
//--------------------------------------------------------------------------------------

void setup() {
  rf_driver.init();
  Serial.begin(9600); // Inicia la comunicación serial
  pinMode(AZUL, OUTPUT);
  pinMode(ROJO, OUTPUT);
  pinMode(AMARILLO, OUTPUT);
  pinMode(VERDE, OUTPUT);
  pinMode(emgPin, INPUT);

  for (int i = 0; i < numSamples; i++) {
    samples[i] = 0;
  }
}
//-------------------------------------------------------------------------------------------
void loop() {
  // Leer el valor del sensor
  int sensorValue = analogRead(emgPin);

  // Filtro de media móvil
  total = total - samples[sampleIndex]; // Restar la muestra antigua
  samples[sampleIndex] = sensorValue;   // Reemplazar con nueva muestra
  total = total + samples[sampleIndex]; // Sumar la nueva muestra

  // Incrementar índice circular
  sampleIndex = (sampleIndex + 1) % numSamples;

  // Calcular el promedio
  smoothedValue = total / numSamples;

  if(smoothedValue >= RI_1 && smoothedValue <= RS_1){
    const char *msg = "1";			// carga numero 1 en mensaje a enviar
    rf_driver.send((uint8_t *)msg, strlen(msg));	// envia el mensaje
    rf_driver.waitPacketSent();			// espera al envio correcto del mensaje
    digitalWrite(AZUL, HIGH);
    Serial.println("Mueve hacia Adelante");
  }

    else if(smoothedValue >= RI_2 && smoothedValue <= RS_2){
    digitalWrite(ROJO, HIGH);
    Serial.println("Mueve hacia Atras");
    const char *msg = "2";			// carga numero 1 en mensaje a enviar
    rf_driver.send((uint8_t *)msg, strlen(msg));	// envia el mensaje
    rf_driver.waitPacketSent();			// espera al envio correcto del mensaje
  }

    else if(smoothedValue >= RI_3 && smoothedValue <= RS_3){
    digitalWrite(AMARILLO, HIGH);
    Serial.println("Mueve hacia Derecha");
    const char *msg = "3";			// carga numero 1 en mensaje a enviar
    rf_driver.send((uint8_t *)msg, strlen(msg));	// envia el mensaje
    rf_driver.waitPacketSent();			// espera al envio correcto del mensaje
  }

    else if(smoothedValue >= RI_4 && smoothedValue <= RS_4){
    digitalWrite(VERDE, HIGH);
    Serial.println("Mueve hacia Izquierda");
    const char *msg = "4";			// carga numero 1 en mensaje a enviar
    rf_driver.send((uint8_t *)msg, strlen(msg));	// envia el mensaje
    rf_driver.waitPacketSent();			// espera al envio correcto del mensaje
  }

  digitalWrite(AZUL, LOW);
  digitalWrite(ROJO, LOW);
  digitalWrite(AMARILLO, LOW);
  digitalWrite(VERDE, LOW);
  delayMicroseconds(1000); 
}
