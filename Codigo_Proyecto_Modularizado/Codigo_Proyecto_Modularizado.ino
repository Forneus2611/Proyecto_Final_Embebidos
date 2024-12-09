#include <Wire.h>
#include <LCDI2C_Multilingual.h>
#include <Servo.h>

// Definir los pines
#define clk 7
#define dt  10
#define btn A3
#define extra_led 5  // Nuevo LED en el pin 5
#define SERVO_PIN 9

// Inicializar LCD I2C
LCDI2C_Generic lcd(0x27, 16, 2);  // Dirección 0x27, dimensiones 16x2

// Variables para el menú y el encoder
String opciones[] = {
  "Mover 60 Grados", "Mover 120 grados", "Mover 180 grados", 
  "Mover 90 Grados", "Mover 150 grados", "Mover 45 grados", 
  "Mover 360 Grados", "Encender LED 5s", "Parpadeo 1(s)"
};
int max_opciones = sizeof(opciones) / sizeof(opciones[0]);
int state_clk_old;
int state_btn_old;
int count = 0;
Servo myServo;

void setup() {
  Serial.begin(9600);
  
  lcd.init();         // Inicializar la pantalla LCD
  lcd.backlight();    // Activar la retroiluminación
  
  pinMode(extra_led, OUTPUT);  // Configurar el LED
  pinMode(clk, INPUT);
  pinMode(dt, INPUT);
  pinMode(btn, INPUT_PULLUP);
  myServo.attach(SERVO_PIN);   // Pin del servomotor

  state_clk_old = digitalRead(clk);
  state_btn_old = digitalRead(btn);
  mostrar_menu();
}

void loop() {
  int state_btn = digitalRead(btn);
  
  encoder(); // Verifica el estado del encoder

  if (state_btn_old == HIGH && state_btn == LOW) {
    run_option();  // Ejecutar la opción seleccionada
  }

  state_btn_old = state_btn;
}

void encoder() {
  int state_clk = digitalRead(clk);
  int state_dt = digitalRead(dt);

  if (state_clk_old == HIGH && state_clk == LOW) {
    if (state_dt == LOW) {
      count--;
    } else {
      count++;
    }

    // Asegura que el índice esté dentro del rango
    if (count < 0) count = max_opciones - 1;
    if (count >= max_opciones) count = 0;
    
    mostrar_menu();  // Actualiza el menú
  }

  delay(5);  // Reducir el ruido del encoder
  state_clk_old = state_clk;
}

void mostrar_menu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Menu:");
  lcd.setCursor(0, 1);
  lcd.print(opciones[count]);  // Mostrar la opción seleccionada
}

void run_option() {
  // Ejecutar las funciones según la opción seleccionada
  switch (count) {
    case 0:
      moverServo(30);  // Opción 1: Mover a 60 grados
      break;
    case 1:
      moverServo(12);  // Opción 2: Mover a 120 grados
      break;
    case 2:
      moverServo(18);  // Opción 3: Mover a 180 grados
      break;
    case 3:
      moverServo(80);  // Opción 4: Mover a 90 grados
      break;
    case 4:
      moverServo(15);  // Opción 5: Mover a 150 grados
      break;
    case 5:
      moverServo(45);  // Opción 6: Mover a 45 grados
      break;
    case 6:
      moverServo(36);  // Opción 7: Mover a 0 grados
      break;
    case 7:
      encenderLED5s();  // Opción 8: Encender LED 5 segundos
      break;
    case 8:
      parpadeo();  // Opción 9: Parpadeo del LED a 50 Hz
      break;
    default:
      break;
  }
  mostrar_menu();  // Volver al menú
}

void moverServo(int angulo) {
  myServo.write(angulo);  // Mover el servomotor al ángulo deseado
  delay(1000);  // Esperar para que el servo complete el movimiento
  myServo.write(0);  // Regresar el servo a la posición inicial (0 grados)
  delay(1000);  // Esperar antes de permitir otro movimiento
}

void encenderLED5s() {
  digitalWrite(extra_led, HIGH);  // Encender el LED
  delay(5000);                    // Esperar 5 segundos
  digitalWrite(extra_led, LOW);   // Apagar el LED
}

void parpadeo() {
  unsigned long start_time = millis();
  while (millis() - start_time < 5000) {  // Mantener el parpadeo durante 5 segundos
    digitalWrite(extra_led, HIGH);
    delay(1000);  // Encendido durante 1000 s
    digitalWrite(extra_led, LOW);
    delay(1000);  // Apagado durante 1000 s
  }
}