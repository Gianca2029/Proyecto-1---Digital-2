//Giancarlo Rosa - 22709
//Electrónica Digital 2
//Proyecto 1

//Librerías

#include <Arduino.h>
#include "display7.h"
#include "config.h"

//Variables globales

const int lm35_pin = 34;  
const int Boton_pin = 5;
int Boton_state = HIGH;
float temperaturaC = 0.0; 

const int led_v = 15;  
const int led_a = 2;  
const int led_r = 4;  

int digito1;
int digito2;
int digito3;

AdafruitIO_Feed *temperature = io.feed("temperatura-proyecto-1-digital-2");
AdafruitIO_Feed *servo_feed = io.feed("servo-proyecto-1-digital-2");

// variables para el millis

int lastBoton_state = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long currentMillis;

//Servo 

#define Servo_pin 19
#define canalPWM 1
#define frequency 50
#define resolution 12

// Leds display

#define led_A 12
#define led_B 32
#define led_C 25
#define led_D 27
#define led_E 14
#define led_F 13
#define led_G 33
#define led_P 26

// Displays 

#define display1 22
#define display2 21
#define display3 18

//funciones prototipo

void initPWM(void);
void moverServo(int ang);
void temp_display(void);


void setup() {

  Serial.begin(115200);

  // para que podamos ir viendo como va la conexión

  Serial.print("Conectando a Adafruit");

  io.connect();

  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  //Si ya se conectó

  Serial.println();
  Serial.print(io.statusText()); 

  pinMode(lm35_pin, INPUT);
  pinMode(Boton_pin, INPUT_PULLUP); 

  pinMode(led_v, OUTPUT);
  pinMode(led_a, OUTPUT);
  pinMode(led_r, OUTPUT);

  digitalWrite(led_v, LOW);
  digitalWrite(led_a, LOW);
  digitalWrite(led_r, LOW); 

  initPWM();

  configurarDisplay(led_A, led_B, led_C, led_D, led_E, led_F, led_G, led_P); 
  pinMode(display1, OUTPUT);
  pinMode(display2, OUTPUT);
  pinMode(display3, OUTPUT);

  digitalWrite(display1,LOW);
  digitalWrite(display2,LOW);
  digitalWrite(display3,LOW);


}

void loop() {

  Boton_state = digitalRead(Boton_pin);

  unsigned long currentMillis = millis();
  temp_display();


  if (Boton_state == LOW) {  // Si el botón está presionado porque está en pullup
    io.run();
    int analogValue = analogRead(lm35_pin);
    
    // Convertir la lectura analógica a temperatura en grados Celsius

    float voltaje = analogValue * (3.3 / 4095.0); // Conversión a voltaje con regla de 3 (3.3V referencia, ADC de 12 bits)
    temperaturaC = voltaje * 100.0;        // LM35 entrega 10mV/°C y se le suma 13 para corrección del sensor chino

    int temp_entera = (temperaturaC * 10);
    digito1 = temp_entera / 100;
    digito2 = (temp_entera / 10) % 10;
    digito3 = temp_entera % 10;

    
    Serial.print("Temperatura: ");
    Serial.print(temperaturaC);
    Serial.println(" ºC");

    if (temperaturaC < 15.0) {
        digitalWrite(led_v, HIGH);
        digitalWrite(led_a, LOW);
        digitalWrite(led_r, LOW); 
        moverServo(180);
        servo_feed-> save(45);
        
    } else if (temperaturaC >= 15.0 && temperaturaC <= 17.0) {
        digitalWrite(led_v, LOW);
        digitalWrite(led_a, HIGH);
        digitalWrite(led_r, LOW); 
        moverServo(90);
        servo_feed-> save(90);

    } else if (temperaturaC > 18.0) {
        digitalWrite(led_v, LOW);
        digitalWrite(led_a, LOW);
        digitalWrite(led_r, HIGH);
        moverServo(0);
        servo_feed-> save(135);

        temperature-> save(temperaturaC);
    }
    
  lastBoton_state = Boton_state;

  temp_display();
  }
}


void initPWM(void){
    ledcSetup(canalPWM, frequency, resolution);
    ledcAttachPin(Servo_pin, canalPWM);
    ledcWrite(canalPWM, 0); 
}

void moverServo(int ang) {
  int duty = map(ang, 0, 180, 205, 410);
  ledcWrite(canalPWM, duty);
}

void temp_display(void) {
  //Dìgito 1 
  digitalWrite(display1, HIGH);
  mostrar_num(digito1);
  mostrar_punto(0);
  delay(5); //delay para que si se encienda un ratito antes de apagarse
  digitalWrite(display1, LOW);
  
  //Dìgito 2
  digitalWrite(display2, HIGH);
  mostrar_num(digito2);
  mostrar_punto(1);
  delay(5); //delay para que si se encienda un ratito antes de apagarse
  digitalWrite(display2, LOW);

  //Dìgito 3
  digitalWrite(display3, HIGH);
  mostrar_num(digito3);
  mostrar_punto(0);
  delay(5); //delay para que si se encienda un ratito antes de apagarse
  digitalWrite(display3, LOW);

}
