#include <SPI.h>
#include "Wire.h"
#include <RF24.h>
#include <DHT.h>
#include "RTClib.h"
#include <Adafruit_Sensor.h>

// I2C pins for DS3231
#define SDA_PIN 2
#define SCL_PIN 3

// NRF24L01 pins
#define CE_PIN 17
#define CSN_PIN 14
#define MISO_PIN 0
#define MOSI_PIN 7
#define SCK_PIN 6

// DHT22 Sensor pins
#define DHT_PIN 15
#define DHT_TYPE DHT22

RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

RF24 radio(CE_PIN, CSN_PIN);
const uint64_t address = 0xF0F0F0F0E1LL;

DHT dht(DHT_PIN, DHT_TYPE);
unsigned long previousMillis = 0;
const long interval = 20000;

float data[2];

// Function Headers
void initializeNRF24L01Radio();
void setupRTCModule();
void readTemperatureAndHumidity();

void setup() {
  Serial.begin(115200);
  initializeNRF24L01Radio();
  setupRTCModule();

  delay(1500);
  dht.begin();
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    readTemperatureAndHumidity();
    DateTime now = rtc.now();
    Serial.println("------------------------");
    Serial.print("Current Temperature: ");
    Serial.println(data[0]);
    Serial.println();
    Serial.print("Current Humidity: ");
    Serial.println(data[1]);
    Serial.println("------------------------");
    Serial.println("Getting Current Time...");

    bool success = radio.write(&data, sizeof(data));

    Serial.println("------------------------");
    if (success) {
      Serial.println("SUCCESS");
    } else {
      Serial.println("FAILED");
    }
  }
}

void initializeNRF24L01Radio() {
  while (!Serial) delay(100);

  Serial.println("Initializing SPI and radio...");

  // Explicitly set the SPI pins
  SPI.setRX(MISO_PIN);
  SPI.setTX(MOSI_PIN);
  SPI.setSCK(SCK_PIN);
  SPI.begin();

  delay(100);  // Give SPI time to initialize

  if (!radio.begin()) {
    Serial.println("Radio hardware not responding!");
    while (1) {
      delay(1000);
    }
  }

  Serial.println("Radio initialized successfully");
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW, 0);
  radio.setDataRate(RF24_1MBPS);
  radio.stopListening();
}

void setupRTCModule() {
  if(!rtc.begin()) {
    Serial.println("Could not find or start RTC!");
    Serial.flush();
    while(1) delay(10);
  }

  if(rtc.lostPower()) {
    Serial.println("RTC lost power, setting the  time");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void readTemperatureAndHumidity() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (!isnan(t)) {
    data[0] = t;
  }

  if (!isnan(h)) {
    data[1] = h;
  }
}