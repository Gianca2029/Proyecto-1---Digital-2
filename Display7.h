#ifndef _DISPLAY7_H_
#define _DISPLAY7_H_

#include <Arduino.h>
#include <stdint.h>

extern uint8_t pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinP;

//Función para la configuración del Display
void configurarDisplay(uint8_t led_A, uint8_t led_B, uint8_t led_C, uint8_t led_D, uint8_t led_E, uint8_t led_F, uint8_t led_G, uint8_t led_P);

//Función para mostrar el número en el display
void mostrar_num(uint8_t numero);

//Función para mostrar el punto decimal
void mostrar_punto(uint8_t punto);

#endif // _DISPLAY7_H_
