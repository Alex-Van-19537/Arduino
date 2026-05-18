// /*
//  * Запис на данни в MIFARE Classic 1K карта
//  * Записваме "Skoda Fabia 1.9" в Блок 4
//  */

// #include <SPI.h>
// #include <MFRC522.h>

// #define RST_PIN         9
// #define SS_PIN          10

// MFRC522 mfrc522(SS_PIN, RST_PIN);
// MFRC522::MIFARE_Key key;

// void setup() {
//   Serial.begin(9600);
//   SPI.begin();
//   mfrc522.PCD_Init();

//   // Подготвяме стандартния ключ (FF FF FF FF FF FF)
//   for (byte i = 0; i < 6; i++) {
//     key.keyByte[i] = 0xFF;
//   }

//   Serial.println(F("Доближи картата, за да запишем данни..."));
// }

// void loop() {
//   // Търсене на карта
//   if ( ! mfrc522.PICC_IsNewCardPresent()) return;
//   if ( ! mfrc522.PICC_ReadCardSerial()) return;

//   Serial.print(F("Карта открита: "));
  
//   // ИЗБОР НА БЛОК: Пишем в блок 4 (това е първият блок на Сектор 1)
//   byte blockAddr = 4;
//   byte dataBlock[] = {
//     // 16 байта данни. Ако текстът е по-къс, допълваме с интервали
//     'S', 'k', 'o', 'd', 'a', ' ', 'F', 'a', 'b', 'i', 'a', ' ', '1', '.', '9', ' '
//   };
  
//   MFRC522::StatusCode status;

//   // 1. Удостоверяване (Authenticate) - трябва да се представим на картата
//   status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockAddr, &key, &(mfrc522.uid));
//   if (status != MFRC522::STATUS_OK) {
//     Serial.print(F("Грешка при Auth: "));
//     Serial.println(mfrc522.GetStatusCodeName(status));
//     return;
//   }

//   // 2. Записване на данните (Write)
//   status = mfrc522.MIFARE_Write(blockAddr, dataBlock, 16);
//   if (status != MFRC522::STATUS_OK) {
//     Serial.print(F("Грешка при запис: "));
//     Serial.println(mfrc522.GetStatusCodeName(status));
//   } else {
//     Serial.println(F("УСПЕШЕН ЗАПИС в Блок 4!"));
//     Serial.println(F("Сега качи кода за четене (Dump), за да видиш резултата."));
//   }

//   // Спираме картата, за да не пише отново и отново
//   mfrc522.PICC_HaltA();
//   mfrc522.PCD_StopCrypto1();
//   delay(2000);
// }




/*
 * Четене на пълно съдържание на RFID таг с RC522
 * Изисква библиотека MFRC522 от Miguel Balboa
 */

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Създаване на инстанция на MFRC522

void setup() {
    Serial.begin(9600);     // Инициализиране на серийната връзка
    while (!Serial);        // Изчакване на порта (само за Leonardo/Native USB)
    SPI.begin();            // Инициализиране на SPI шината
    mfrc522.PCD_Init();     // Инициализиране на модула MFRC522
    delay(4);               // Малко изчакване за стабилност
    mfrc522.PCD_DumpVersionToSerial(); // Показва версията на фърмуера (да видим дали четеца работи)
    Serial.println(F("Сканирай PICC (чип/карта), за да видиш съдържанието..."));
}

void loop() {
    // 1. Търсене на нови карти
    if ( ! mfrc522.PICC_IsNewCardPresent()) {
        return;
    }

    // 2. Избор на една от картите
    if ( ! mfrc522.PICC_ReadCardSerial()) {
        return;
    }

    // 3. Извеждане на цялата информация в Serial Monitor
    // Тази функция автоматично пробва стандартни ключове (Key A/B = FF FF FF FF FF FF)
    // и показва всички сектори в HEX формат.
    mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
    
    // Спираме криптираната комуникация след прочитането
    // Ако искаш да четеш непрекъснато, махни долните редове, но ще спами конзолата
    /* * За да не чете една и съща карта 100 пъти в секунда, 
     * можеш да добавиш delay или да спреш цикъла до махане на картата.
     */
    delay(2000); 
}