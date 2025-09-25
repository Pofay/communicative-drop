# Communicative Drop

Tinkering the following components:

- [RPI Pico](https://www.raspberrypi.com/products/raspberry-pi-pico/)
- [ESP32 Devkit v1](https://www.espressif.com/en/products/devkits)
- [RTC DS3231](https://www.makerlab-electronics.com/products/ds3231-rtc-high-precision-real-time-clock-module?srsltid=AfmBOoo8SvmCNn5BWOvWejlR6Go2UPRrDRzZv-OZ2H1SBogYBUiQVOiF)
- [DHT22](https://www.adafruit.com/product/385)
- [NR24L01 with antenna](https://www.makerlab-electronics.com/products/2-4ghz-nrf24l01-module-pa-lna-sma-antenna-1km?_pos=2&_psq=nrf&_ss=e&_v=1.0)

Goal is to have the following work:

```
+--------+       Radio (NRF24L01)         +--------+         WiFi           +---------------------+
|  Pico  |------------------------------>| ESP32  |----------------------->| Java Server (Quarkus)|
|        |                               |        |                        |      (MQTT Server)   |
| DS3231 |                               |NRF24L01|                        +---------------------+
| DHT22  |                               |        |
+--------+                               +--------+
```

So far my ESP32 + NRF24L01 is having problems receiving from the Pico
