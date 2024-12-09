#include "funciones.h"

// Inicialización de variables globales
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
LCDI2C_Generic lcd(0x27, 16, 2); // Pantalla LCD

void inicializarSistema() {
    Serial.begin(9600);
    lcd.init();
    lcd.backlight();

    pinMode(extra_led, OUTPUT);
    pinMode(clk, INPUT);
    pinMode(dt, INPUT);
    pinMode(btn, INPUT_PULLUP);

    myServo.attach(SERVO_PIN);

    state_clk_old = digitalRead(clk);
    state_btn_old = digitalRead(btn);
    mostrarMenu();
}
void gestionarMenu() {
    int state_btn = digitalRead(btn);

    encoder();

    if (state_btn_old == HIGH && state_btn == LOW) {
        runOption();
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

        if (count < 0) count = max_opciones - 1;
        if (count >= max_opciones) count = 0;

        mostrarMenu();
    }

    delay(5);
    state_clk_old = state_clk;
}

