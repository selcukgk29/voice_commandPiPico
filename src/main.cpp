#include <Arduino.h>
#include <I2S.h>

// Pin Tanımlamaları
const int I2S_BCLK = 18;  // Bit Clock (SCK)
const int I2S_LRCK = 19;  // Word Select (WS)
const int I2S_DIN = 20;   // Serial Data (SD)

// I2S nesnesi (sadece INPUT modu ile başlat)
I2S i2s(INPUT);

// Buffer
int32_t sample_l;
int32_t sample_r;

void setup() {
    Serial.begin(921600);
    while (!Serial) delay(10);
    Serial.println("INMP441 I2S Mikrofon Test");

    pinMode(LED_BUILTIN, OUTPUT);

    // Pin ayarları
    i2s.setBCLK(I2S_BCLK);
    i2s.setDATA(I2S_DIN);

    // I2S Konfigürasyonu
    i2s.setBitsPerSample(32);     // INMP441 32-bit
    i2s.setFrequency(16000);      // 16kHz örnekleme
    i2s.setBuffers(8, 1024);      // 8 buffer, her biri 1024 örnek

    // I2S başlat
    if (!i2s.begin()) {
        Serial.println("I2S başlatılamadı!");
        while (1) {
            digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
            delay(100);
        }
    }

    Serial.println("I2S başlatıldı");
    digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
    if (i2s.available()) {
        // Stereo veri oku
        if (i2s.read32(&sample_l, &sample_r)) {
            // Sol kanalı kullan (INMP441'de L/R pini GND'ye bağlı)
            // 32-bit veriyi 16-bit'e normalize et
            int16_t sample = (sample_l >> 16)*4;
            
            // Seri porta gönder
            Serial.println(sample);
            
            // LED'i ses seviyesine göre yanıp söndür
            digitalWrite(LED_BUILTIN, abs(sample) > 1000 ? HIGH : LOW);
        }
    }
}