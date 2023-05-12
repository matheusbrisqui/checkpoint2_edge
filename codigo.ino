#include <LiquidCrystal.h>

const int LDR = A0;
const int verde_LED_PIN = 2;
const int amarelo_LED_PIN = 3;
const int vermelho_LED_PIN = 4;
const int buzzer_PIN = 5;
const int TMP = A1;
const int Potenciometro = A2;

// Define os pinos do display LCD
const int rsPin = 12;
const int enPin = 11;
const int d4Pin = 9;
const int d5Pin = 8;
const int d6Pin = 7;
const int d7Pin = 6;

float temperature;
int humidity;
int lightIntensity;
float temperatureSum = 0;
float humiditySum = 0;
int readingCount = 0;
const int numReadings = 5;

LiquidCrystal lcd(rsPin, enPin, d4Pin, d5Pin, d6Pin, d7Pin); // Inicializa o LCD

enum DisplayMode {
  DisplayMode_Light,
  DisplayMode_Temperature,
  DisplayMode_Humidity
};

DisplayMode currentDisplayMode = DisplayMode_Light;
unsigned long displayStartTime = 0;
const unsigned long displayDuration = 3000; // 3 segundos

void setup() {
  pinMode(verde_LED_PIN, OUTPUT);
  pinMode(amarelo_LED_PIN, OUTPUT);
  pinMode(vermelho_LED_PIN, OUTPUT);
  pinMode(buzzer_PIN, OUTPUT);

  lcd.begin(16, 2); // Inicializa o LCD com 16 colunas e 2 linhas
  lcd.clear();
}

void loop() {
  // Leitura da temperatura
  temperature = analogRead(TMP) * 0.48828125; // Conversão para graus Celsius

  // Leitura da umidade
  humidity = analogRead(Potenciometro) / 10;

  // Leitura da intensidade de luz
  lightIntensity = analogRead(LDR);

  // Soma das leituras dos sensores
  temperatureSum += temperature;
  humiditySum += humidity;
  readingCount++;

  // Verifica se atingiu o número mínimo de leituras
  if (readingCount >= numReadings) {
    // Calcula a média das leituras
    float averageTemperature = temperatureSum / numReadings;
    float averageHumidity = humiditySum / numReadings;

    // Reinicia as variáveis de soma e contagem
    temperatureSum = 0;
    humiditySum = 0;
    readingCount = 0;

    // Verifica o estado da luz ambiente
    if (currentDisplayMode == DisplayMode_Light) {
      if (lightIntensity <= 800) { // Ambiente escuro
        digitalWrite(verde_LED_PIN, HIGH);
        digitalWrite(amarelo_LED_PIN, LOW);
        digitalWrite(vermelho_LED_PIN, LOW);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Ambiente escuro  ");
      } else if (lightIntensity > 800 && lightIntensity < 850) { // Ambiente a meia luz
        digitalWrite(verde_LED_PIN, LOW);
        digitalWrite(amarelo_LED_PIN, HIGH);
        digitalWrite(vermelho_LED_PIN, LOW);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Ambiente ");
        lcd.setCursor(0, 2);
        lcd.print("Meia Luz   ");
      } else { // Ambiente totalmente iluminado
        digitalWrite(verde_LED_PIN, LOW);
        digitalWrite(amarelo_LED_PIN, LOW);
        digitalWrite(vermelho_LED_PIN, HIGH);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Ambiente   ");
        lcd.setCursor(0, 1);
        lcd.print("Ambiente muito   ");
        lcd.setCursor(0, 2);
        lcd.print("Muito claro   ");
      }
      displayStartTime = millis();
      currentDisplayMode = DisplayMode_Temperature;
    }

    // Verifica se passaram 3 segundos para exibir a temperatura
    if (currentDisplayMode == DisplayMode_Temperature && millis() - displayStartTime >= displayDuration) {
      if (averageTemperature >= 10 && averageTemperature <= 15) {
        lcd.setCursor(0, 0);
        lcd.print("Temperatura OK   ");
        lcd.setCursor(0, 1);
        lcd.print("Temp: ");
        lcd.print(averageTemperature);
        lcd.print("C");
        digitalWrite(amarelo_LED_PIN, LOW);
        digitalWrite(buzzer_PIN, LOW);
      } else if (averageTemperature > 15) {
        lcd.setCursor(0, 0);
        lcd.print("Temp. Alta       ");
        lcd.setCursor(0, 1);
        lcd.print("Temp: ");
        lcd.print(averageTemperature);
        lcd.print("C");
        digitalWrite(amarelo_LED_PIN, HIGH);
        digitalWrite(buzzer_PIN, HIGH);
      } else if (averageTemperature < 10) {
        lcd.setCursor(0, 0);
        lcd.print("Temp. Baixa      ");
        lcd.setCursor(0, 1);
        lcd.print("Temp: ");
        lcd.print(averageTemperature);
        lcd.print("C");
        digitalWrite(amarelo_LED_PIN, HIGH);
        digitalWrite(buzzer_PIN, HIGH);
      }
      displayStartTime = millis();
      currentDisplayMode = DisplayMode_Humidity;
    }

    // Verifica se passaram 3 segundos para exibir a umidade
    if (currentDisplayMode == DisplayMode_Humidity && millis() - displayStartTime >= displayDuration) {
      if (averageHumidity >= 50 && averageHumidity <= 70) {
        lcd.setCursor(0, 0);
        lcd.print("Umidade OK       ");
        lcd.setCursor(0, 1);
        lcd.print("Umidade: ");
        lcd.print(averageHumidity);
        lcd.print("%");
        digitalWrite(vermelho_LED_PIN, LOW);
        digitalWrite(buzzer_PIN, LOW);
      } else if (averageHumidity > 70) {
        lcd.setCursor(0, 0);
        lcd.print("Umidade Alta     ");
        lcd.setCursor(0, 1);
        lcd.print("Umidade: ");
        lcd.print(averageHumidity);
        lcd.print("%");
        digitalWrite(vermelho_LED_PIN, HIGH);
        digitalWrite(buzzer_PIN, HIGH);
      } else if (averageHumidity < 50) {
        lcd.setCursor(0, 0);
        lcd.print("Umidade Baixa    ");
        lcd.setCursor(0, 1);
        lcd.print("Umidade: ");
        lcd.print(averageHumidity);
        lcd.print("%");
        digitalWrite(vermelho_LED_PIN, HIGH);
        digitalWrite(buzzer_PIN, HIGH);
      }
      displayStartTime = millis();
      currentDisplayMode = DisplayMode_Light;
    }

    // Aguarda 5 segundos antes da próxima leitura
    delay(5000);
  }
}
