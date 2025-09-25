#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Define nRF24L01 pins
#define CE_PIN 4  // GPIO4 (D4)
#define CSN_PIN 5 // GPIO5 (D5)

// Create an RF24 object
RF24 radio(CE_PIN, CSN_PIN);

// Define the address (must match on both transmitter and receiver)
const uint64_t address = 0xF0F0F0F0E1LL;
float data[2];
unsigned long lastReceiveTime = 0;

void setup() {
  delay(5000);
  Serial.begin(115200);

  while (!Serial) delay(5000);  // Wait for serial to initialize
  Serial.println("\nESP32 NRF24L01 Receiver");
  
  Serial.println("Initializing radio...");
  if (!radio.begin()) {
    Serial.println("Radio hardware not responding!");
    while (1) {
      delay(300);
    }
  }
  
  Serial.println("Radio initialized successfully!");
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW, 0);
  radio.setDataRate(RF24_1MBPS);
  radio.printDetails();
  radio.startListening();  // Start listening right away
  lastReceiveTime = millis();
}

void loop() {
  if (radio.available()) {
    radio.read(&data, sizeof(data));
    lastReceiveTime = millis();
    
    // Validate data before displaying
    if (isnan(data[0]) || isnan(data[1])) {
      Serial.println("Received invalid data");
    } else {
      Serial.println("\n--------- RECEIVED TEMPERATURE ---------");
      Serial.print("Temperature: ");
      Serial.print(data[0]);
      Serial.println(" °C");
      Serial.print("Humidity: ");
      Serial.print(data[1]);
      Serial.println(" %");
    }
  }
  
  // Show timeout if no data received for a long time
  if (millis() - lastReceiveTime > 20000 && lastReceiveTime > 0) {
    Serial.println("No data received for 1 minute");
    lastReceiveTime = millis();  // Reset to avoid repeated messages
  }
}
