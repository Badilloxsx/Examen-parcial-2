#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define ANCHO 128
#define ALTO 64

Adafruit_SSD1306 display(ANCHO, ALTO, &Wire, -1);

// Pines
int pinLDR = A0;
int pinPot = A1;
int pinLED = 9;
int btnModo = 2;
int btnReset = 3;

// Variables
bool modoAuto = true;
int brillo = 0;

void setup() {
  Serial.begin(9600);

  pinMode(pinLED, OUTPUT);
  pinMode(btnModo, INPUT);
  pinMode(btnReset, INPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
}

void loop() {
  int valorLDR = analogRead(pinLDR);
  int valorPot = analogRead(pinPot);

  float voltaje = valorLDR * (5.0 / 1023.0);

  // BOTÓN CAMBIAR MODO
  if (digitalRead(btnModo) == HIGH) {
    modoAuto = !modoAuto;
    delay(300);
  }

  // BOTÓN RESET
  if (digitalRead(btnReset) == HIGH) {
    brillo = 128;
    delay(300);
  }

  // MODOS
  if (modoAuto) {
    brillo = map(valorLDR, 0, 1023, 255, 0);
  } else {
    brillo = map(valorPot, 0, 1023, 0, 255);
  }

  analogWrite(pinLED, brillo);

  // OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.print("Modo: ");
  display.println(modoAuto ? "AUTO" : "MANUAL");

  display.setCursor(0, 10);
  display.print("LDR: ");
  display.println(valorLDR);

  display.setCursor(0, 20);
  display.print("Volt: ");
  display.println(voltaje);

  display.setCursor(0, 30);
  display.print("Brillo: ");
  display.print(map(brillo, 0, 255, 0, 100));
  display.println("%");

  display.display();

  // SERIAL
  Serial.print("[");
  Serial.print(modoAuto ? "AUTO" : "MANUAL");
  Serial.print("] LDR: ");
  Serial.print(valorLDR);
  Serial.print(" | V: ");
  Serial.print(voltaje);
  Serial.print(" | Brillo: ");
  Serial.print(map(brillo, 0, 255, 0, 100));
  Serial.println("%");

  delay(200);
}