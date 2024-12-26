//----------------------------Proyecto Instrumentacion Electrica (Receptor)---
#include <RH_ASK.h>		// incluye libreria RadioHead.h
#include <SPI.h> 		// incluye libreria SPI necesaria por RadioHead.h
RH_ASK rf_driver(4000);		// crea objeto para modulacion por ASK
const int AZUL = 2;
const int ROJO = 3;
const int AMARILLO = 4;
const int VERDE = 5;
const int MOTOR_AA = 6; //MOTOR A
const int MOTOR_AB = 7;
const int MOTOR_BA = 8; //MOTOR B
const int MOTOR_BB = 9;
 
void setup(){
    Serial.begin(9600);
    pinMode(AZUL, OUTPUT);
    pinMode(ROJO, OUTPUT);
    pinMode(AMARILLO, OUTPUT);
    pinMode(VERDE, OUTPUT);
    pinMode(MOTOR_AA, OUTPUT);
    pinMode(MOTOR_AB, OUTPUT);
    pinMode(MOTOR_BA, OUTPUT);
    pinMode(MOTOR_BB, OUTPUT);
    rf_driver.init();		// inicializa objeto con valores por defecto
  }
 
void loop(){
    uint8_t buf[1];			// espacio para mensaje recibido de 1 caracter
    uint8_t buflen = sizeof(buf);	// longitud de buffer
    
    if (rf_driver.recv(buf, &buflen))	{
      if((char)buf[0]== '1' ){
            digitalWrite(AZUL, HIGH);
            digitalWrite(MOTOR_AA, HIGH);
            digitalWrite(MOTOR_AB, LOW);
            digitalWrite(MOTOR_BA, HIGH);
            digitalWrite(MOTOR_BB, LOW);
            delay(1000); 
        }						 
        else if((char)buf[0]=='2'){
            digitalWrite(ROJO, HIGH);
            digitalWrite(MOTOR_AA, LOW);
            digitalWrite(MOTOR_AB, HIGH);
            digitalWrite(MOTOR_BA, LOW);
            digitalWrite(MOTOR_BB, HIGH);
            delay(1000);   
        }       
        
        else if((char)buf[0]=='3'){
            digitalWrite(AMARILLO, HIGH);
            digitalWrite(MOTOR_AA, HIGH);
            digitalWrite(MOTOR_AB, LOW);
            digitalWrite(MOTOR_BA, LOW);
            digitalWrite(MOTOR_BB, HIGH);
            delay(1000);   
        }        
       
        else if((char)buf[0]=='4'){
            digitalWrite(VERDE, HIGH);
            digitalWrite(MOTOR_AA, LOW);
            digitalWrite(MOTOR_AB, HIGH);
            digitalWrite(MOTOR_BA, HIGH);
            digitalWrite(MOTOR_BB, LOW);
            delay(1000);     
        }  
    }
  digitalWrite(MOTOR_AA, LOW);  
  digitalWrite(MOTOR_AB, LOW);
  digitalWrite(MOTOR_BA, LOW);
  digitalWrite(MOTOR_BB, LOW);
  digitalWrite(AZUL, LOW);
  digitalWrite(ROJO, LOW);
  digitalWrite(AMARILLO, LOW);
  digitalWrite(VERDE, LOW);
  delay(10);  
}
