//----------------------------Proyecto Instrumentacion Electrica (Receptor)---

#include <RH_ASK.h>		// incluye libreria RadioHead.h
#include <SPI.h> 		// incluye libreria SPI necesaria por RadioHead.h

RH_ASK rf_driver;		// crea objeto para modulacion por ASK

const int AZUL = 2;
const int ROJO = 3;
const int AMARILLO = 4;
const int VERDE = 5;
 
void setup(){
    pinMode(AZUL, OUTPUT);
    pinMode(ROJO, OUTPUT);
    pinMode(AMARILLO, OUTPUT);
    pinMode(VERDE, OUTPUT);
    rf_driver.init();		// inicializa objeto con valores por defecto
}
 
void loop(){
    uint8_t buf[1];			// espacio para mensaje recibido de 1 caracter
    uint8_t buflen = sizeof(buf);	// longitud de buffer
    
    if (rf_driver.recv(buf, &buflen))	{
      if((char)buf[0]== '1' ){
            digitalWrite(AZUL, HIGH);
            delayMicroseconds(1000); 
        }						 
        else if((char)buf[0]=='2'){
            digitalWrite(ROJO, HIGH);
            delayMicroseconds(1000);   
        }       
        
        else if((char)buf[0]=='3'){
            digitalWrite(AMARILLO, HIGH);
            delayMicroseconds(1000);   
        }        
       
        else if((char)buf[0]=='4'){
            digitalWrite(VERDE, HIGH);
            delayMicroseconds(1000);     
        }  
    }

  digitalWrite(AZUL, LOW);
  digitalWrite(ROJO, LOW);
  digitalWrite(AMARILLO, LOW);
  digitalWrite(VERDE, LOW);
  delay(10);  


}