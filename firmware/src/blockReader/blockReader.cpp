#include "blockReader.h"
#include <iomanip>
#include <sstream> // Add this line to include the <sstream> header

//* ------
// *             MFRC522
// *             Reader/PCD   ESP32
// * Signal      Pin          Pin
// * --------------------------------------
// * RST/Reset   RST          21
// * SPI SS      SDA(SS)      5
// * SPI MOSI    MOSI         23
// * SPI MISO    MISO         19
// * SPI SCK     SCK          18
//------ESP32 RFID setting ----------------

#define SS_PIN 5
#define RST_PIN 22

MFRC522 rfid(SS_PIN, RST_PIN);

std::string id_model = "";

void byteCaster(byte buffer[]) {
  std::stringstream ss;

  for (byte i = 0; i < 4; i++) {
    if (buffer[i] == 0) {
      id_model += "00";
    } else {
      ss << std::hex << std::setw(2) << std::setfill('0')
         << static_cast<int>(buffer[i]);
      id_model += ss.str();
      ss.str(""); // cleaning it
    }
  }
}

void idPrinting() { Serial.println(id_model.c_str()); }

void resetId() { id_model = ""; }

void readPage(int page) {
  byte buffer[18]; // Buffer to hold read data (4 bytes per page)
  byte size = sizeof(buffer);

  // For reading the blocks
  MFRC522::StatusCode status =
      rfid.MIFARE_Read(page, buffer, &size); // best line 2024

  if (status != MFRC522::STATUS_OK) {
    Serial.print("Error reading page ");
    Serial.print(page);
    Serial.print(": ");
    Serial.println(rfid.GetStatusCodeName(status));
    return;
  }

  byteCaster(buffer);
}

std::string getModelId() { return id_model; }

void readingRC522() {
  id_model = "";
  // Read only pages 21 and 22 (model ID inform in )
  readPage(21);
  delay(10); // Small delay for readability
  readPage(22);
  delay(10); // Small delay for readability
}
