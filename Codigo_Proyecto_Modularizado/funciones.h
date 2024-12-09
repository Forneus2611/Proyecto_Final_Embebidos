#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <Wire.h>
#include <LCDI2C_Multilingual.h>
#include <Servo.h>
#include <Arduino.h>

// Definición de pines 
#define clk 7
#define dt  10
#define btn A3
#define extra_led 5
#define SERVO_PIN 9

// Declaraciones de variables globales
extern String opciones[];
extern int max_opciones;
extern int state_clk_old;
extern int state_btn_old;
extern int count;
extern Servo myServo;
