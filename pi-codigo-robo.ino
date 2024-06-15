#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>

#define I2C_ADDRESS 0x27

#define RED_LED1 6
#define RED_LED2 5
#define GREEN_LED1 3
#define GREEN_LED2 4

#define MOTOR_PIN 7

#define BUTTON_PIN 2

#define MESSAGE_PEACE "Paz"
#define MESSAGE_WAR "Guerra"

#define In1 9
#define In2 10
#define ENA 11


LiquidCrystal_I2C lcd(I2C_ADDRESS, 16, 2);

int receiver_pin = 8;
IRrecv irrecv(receiver_pin);
decode_results results;

bool motorState = false;
bool modeWar = false;

int SPEED = 210;

void setup() {
  pinMode(RED_LED1, OUTPUT);
  pinMode(RED_LED2, OUTPUT);
  pinMode(GREEN_LED1, OUTPUT);
  pinMode(GREEN_LED2, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(ENA, OUTPUT);

  lcd.init();
  lcd.backlight();

  updateMode(); // Chama a função updateMode() para configurar o sistema no modo inicial

  irrecv.enableIRIn();
}

void loop() {\image.png
  if (irrecv.decode(&results)) {
    if (results.value == 0xFFA25D) { // Código do botão no controle remoto
      modeWar = !modeWar;
      updateMode();
    }
    irrecv.resume();
  }

  if (digitalRead(BUTTON_PIN) == LOW) {
    modeWar = !modeWar;
    updateMode();
    delay(200); // Debouncing
  }
}

void updateMode() {
  if (modeWar) {
    digitalWrite(RED_LED1, LOW);
    digitalWrite(RED_LED2, LOW);
    digitalWrite(GREEN_LED1, HIGH);
    digitalWrite(GREEN_LED2, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(MESSAGE_WAR);

    // Ligar o motor
    digitalWrite(In1, HIGH);
    digitalWrite(In2, LOW);
    analogWrite(ENA, SPEED);
  } else {
    digitalWrite(RED_LED1, HIGH);
    digitalWrite(RED_LED2, HIGH);
    digitalWrite(GREEN_LED1, LOW);
    digitalWrite(GREEN_LED2, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(MESSAGE_PEACE);

    // Desligar o motor
    digitalWrite(In1, LOW);
    digitalWrite(In2, LOW);
    analogWrite(ENA, 0);
  }
}
