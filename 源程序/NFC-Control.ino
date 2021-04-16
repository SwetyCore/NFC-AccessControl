#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9

int Pin0 = 4;
int Pin1 = 5;
int Pin2 = 6;
int Pin3 = 7;
int _step = 0;

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key;

// Init array that will store new NUID
byte nuidPICC[4] = {1，23，24，342};

void setup() {
  Serial.begin(9600);
  pinMode(Pin0, OUTPUT);
  pinMode(Pin1, OUTPUT);
  pinMode(Pin2, OUTPUT);
  pinMode(Pin3, OUTPUT);
  pinMode(8,INPUT);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  Serial.println(F("此代码扫描MIFARE Classsic NUID。"));
  Serial.print(F("使用以下键："));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
}

void loop() {
  if(digitalRead(8)==HIGH)
  {
    openDoor(true);
    delay(500);
    openDoor(false);
  }
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("您的标签的类型不是MIFARE Classic。"));
    return;
  }

  if (rfid.uid.uidByte[0] != nuidPICC[0] ||
      rfid.uid.uidByte[1] != nuidPICC[1] ||
      rfid.uid.uidByte[2] != nuidPICC[2] ||
      rfid.uid.uidByte[3] != nuidPICC[3] ) {
    // Serial.println(F("已检测到新卡。"));

    // // Store NUID into nuidPICC array
    // for (byte i = 0; i < 4; i++) {
    //   nuidPICC[i] = rfid.uid.uidByte[i];
    // }

    // Serial.println(F("The NUID tag is:"));
    // Serial.print(F("In hex: "));
    // printHex(rfid.uid.uidByte, rfid.uid.size);
    // Serial.println();
    // Serial.print(F("In dec: "));
    // printDec(rfid.uid.uidByte, rfid.uid.size);
    // Serial.println();
    Serial.println(F("nuid不匹配！"));
  }
  else {
    Serial.println(F("正确的卡。"));
    openDoor(true);
    delay(500);
    openDoor(false);
  }


  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}
void openDoor(boolean dir) {
  int _step = 0;
  for (int i = 0; i <= 2300; i++)
  {
    // 驱动步进电机开门
    // boolean dir=true;//正反转
    switch (_step) {
      case 0:
        Serial.println("0");
        digitalWrite(Pin0, HIGH);
        digitalWrite(Pin1, LOW);
        digitalWrite(Pin2, LOW);
        digitalWrite(Pin3, LOW);
        break;
      case 1:
        Serial.println("1");
        digitalWrite(Pin0, LOW);
        digitalWrite(Pin1, HIGH);
        digitalWrite(Pin2, LOW);
        digitalWrite(Pin3, LOW);
        break;
      case 2:
        Serial.println("2");
        digitalWrite(Pin0, LOW);
        digitalWrite(Pin1, LOW);
        digitalWrite(Pin2, HIGH);
        digitalWrite(Pin3, LOW);
        break;
      case 3:
        Serial.println("3");
        digitalWrite(Pin0, LOW);
        digitalWrite(Pin1, LOW);
        digitalWrite(Pin2, LOW);
        digitalWrite(Pin3, HIGH);
        break;
      default:
        digitalWrite(Pin0, LOW);
        digitalWrite(Pin1, LOW);
        digitalWrite(Pin2, LOW);
        digitalWrite(Pin3, LOW);
        break;
    }

    if (dir) {
      _step++;
    } else {
      _step--;
    }

    if (_step > 3) {
      _step = 0;
    }

    if (_step < 0) {
      _step = 3;
    }
  }
  delay(0);

  digitalWrite(Pin0, LOW);
  digitalWrite(Pin1, LOW);
  digitalWrite(Pin2, LOW);
  digitalWrite(Pin3, LOW);
}


/**
   Helper routine to dump a byte array as hex values to Serial.
*/
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

/**
   Helper routine to dump a byte array as dec values to Serial.
*/
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}
