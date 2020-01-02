#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
int getFingerprintIDez();
SoftwareSerial mySerial(2,3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
void setup() {
  // put your setup code here, to run once:
  while(!Serial);
  Serial.begin(9600);
  Serial.println("Adfruit finger detect test");
  finger.begin(57600);
  if(finger.verifyPassword()){
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor : (");
    while(1);
  }
  Serial.println("Waiting for valid finger...");
}

void loop() {
  // put your main code here, to run repeatedly:
  getFingerprintIDez();
  delay(50);
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
    Serial.println("Image taken");
    break;
    case FINGERPRINT_NOFINGER:
    Serial.println("No finger detected");
    return p;
    case FINGERPRINT_PACKETRECIEVEERR:
    Serial.println("Communication error");
    return p;
    case FINGERPRINT_IMAGEFAIL:
    Serial.println("Imageing error");
    return p;
    default:
    Serial.println("Unknown error");
    return p;
  }
  // OK success!
  p = finger.image2Tz();
  switch(p) {
    case FINGERPRINT_OK:
    Serial.println("Image converted");
    break;
    case FINGERPRINT_IMAGEMESS:
    Serial.println("Image too messy");
    return p;
    case FINGERPRINT_PACKETRECIEVEERR:
    Serial.println("Communication error");
    return p;
    case FINGERPRINT_FEATUREFAIL:
    Serial.println("Could not find fingerprint features");
    return p;
    case FINGERPRINT_INVALIDIMAGE:
    Serial.println("Could not find fingerprint features");
    return p;
    default:
    Serial.println("Unknown error");
    return p;
  }
  // OK converted!
  p = finger.fingerFastSearch();
  if(p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if ( p == FINGERPRINT_NOTFOUND) {
    Serial.println("Communication error");
    return p;
  } else if ( p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }
  //found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
}
//return -1 if failed, otherwise returns ID #
int get FingerprintDez(){
  uint8_t p = finger.getImage();
  if(p != FINGERPRINT_OK) return -1;
  p = finger.Image2Tz();
  if (p != FINGERPRINT_OK) return -1;
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) return -1;
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
  }
