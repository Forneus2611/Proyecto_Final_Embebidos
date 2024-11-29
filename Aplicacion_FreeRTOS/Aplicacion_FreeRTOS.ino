#include <Wire.h>
#include <LCDI2C_Multilingual.h>
#include <Servo.h>
#include <Arduino_FreeRTOS.h>

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
int count = 0;
Servo myServo;

// Variables para el encoder
int state_clk_old;
int state_btn_old;

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

  // Crear las tareas
  xTaskCreate(tareaEncoder, "Encoder", 128, NULL, 1, NULL);
  xTaskCreate(tareaLCD, "LCD", 128, NULL, 1, NULL);
  
  vTaskStartScheduler();  // Iniciar el planificador de FreeRTOS
}

void loop() {
  // El loop principal queda vacío; FreeRTOS maneja las tareas.
}

void tareaEncoder(void *pvParameters) {
  while (true) {
    int state_clk = digitalRead(clk);
    int state_dt = digitalRead(dt);
    int state_btn = digitalRead(btn);

    if (state_clk_old == HIGH && state_clk == LOW) {
      if (state_dt == LOW) {
        count--;
      } else {
        count++;
      }

      if (count < 0) count = max_opciones - 1;
      if (count >= max_opciones) count = 0;
    }

    if (state_btn_old == HIGH && state_btn == LOW) {
      run_option();  // Ejecutar la opción seleccionada
    }

    state_clk_old = state_clk;
    state_btn_old = state_btn;

    vTaskDelay(pdMS_TO_TICKS(5));  // Evitar rebotes
  }
}

void tareaLCD(void *pvParameters) {
  while (true) {
    mostrar_menu();
    vTaskDelay(pdMS_TO_TICKS(70));  // Actualizar el LCD periódicamente
  }
}

void mostrar_menu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Menu:");
  lcd.setCursor(0, 1);
  lcd.print(opciones[count]);  // Mostrar la opción seleccionada
}

void run_option() {
  switch (count) {
    case 0: moverServo(30); break;
    case 1: moverServo(12); break;
    case 2: moverServo(18); break;
    case 3: moverServo(80); break;
    case 4: moverServo(15); break;
    case 5: moverServo(45); break;
    case 6: moverServo(36); break;
    case 7: encenderLED5s(); break;
    case 8: parpadeo(); break;
    default: break;
  }
}

void moverServo(int angulo) {
  myServo.write(angulo);
  vTaskDelay(pdMS_TO_TICKS(1000));
  myServo.write(0);
  vTaskDelay(pdMS_TO_TICKS(1000));
}

void encenderLED5s() {
  digitalWrite(extra_led, HIGH);
  vTaskDelay(pdMS_TO_TICKS(5000));
  digitalWrite(extra_led, LOW);
}

void parpadeo() {
  unsigned long start_time = millis();
  while (millis() - start_time < 5000) {
    digitalWrite(extra_led, HIGH);
    vTaskDelay(pdMS_TO_TICKS(1000));
    digitalWrite(extra_led, LOW);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}