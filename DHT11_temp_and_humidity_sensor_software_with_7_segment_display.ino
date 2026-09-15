/*
# temp and humitity sensor using DHT11 sensor with a switched circuit i.e. one terminal of switch has to be in GND and the another in pin 12
connection:
data terminal of DHT11(labled s) into pin 2
one press or the button logs temp and humidity once

5161BS 7 segment display

PIN_3 = B
PIN_4 = A
PIN_5 = F
PIN_6 = G
PIN_7 = C
PIN_8 = D
PIN_9 = E

##install DHT11 by DhrubaSaha 
https://github.com/dhrubasaha08/DHT11

*/


#include<DHT11.h>

int startingPin = 3;
int finalPin = 9;
int buttonStatusPin = 12;
int DHT11SPin = 2;
int slicedNumberArray[2];
int delayTime = 1000; //in ms

DHT11 dht11(DHT11SPin);


void setup() {
  pinMode(buttonStatusPin,INPUT_PULLUP);
  for (int pin = startingPin; pin <= finalPin; pin++) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }

  Serial.begin(9600);//for output on the serialmonitor
}

void loop() {
  int buttonStatus = digitalRead(buttonStatusPin);
  if(buttonStatus ==  0){
    readFromDHT11();
  }
  
}

void readFromDHT11(){
  int temp = dht11.readTemperature();
  int hum = dht11.readHumidity();

   if ((temp != DHT11::ERROR_TIMEOUT && temp != DHT11::ERROR_CHECKSUM) && (hum != DHT11::ERROR_TIMEOUT && hum != DHT11::ERROR_CHECKSUM)) {
    Serial.print("\n");
    Serial.print("Temperature: ");
    Serial.print(temp);
  
    Serial.print("\n");

    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print("%");

    displayNumber(temp);
    delay(delayTime);
    displayNumber(hum);

} else {

  Serial.println(DHT11::getErrorString(temp));


}
}

void displayDigit(int number){
  int numArray[10][7] = {
                          {3, 4, 5, 7, 8, 9},       // 0
                          {3, 7},                   // 1
                          {3, 4, 6, 8, 9},          // 2
                          {3, 4, 6, 7, 8},          // 3
                          {3, 5, 6, 7},             // 4
                          {4, 5, 6, 7, 8},          // 5
                          {4, 5, 6, 7, 8, 9},       // 6
                          {3, 4, 7},                // 7
                          {3, 4, 5, 6, 7, 8, 9},    // 8
                          {3, 4, 5, 6, 7, 8}        // 9
                        };
  int arrayLength = sizeof(numArray[number]) / sizeof(numArray[number][0]);
  for(int pinIndex = 0; pinIndex < arrayLength ; pinIndex++){
    digitalWrite(numArray[number][pinIndex],LOW);
  }
}

void displayReset(){
  for (int pin = startingPin; pin <= finalPin; pin++) {
    digitalWrite(pin, HIGH);
  }
}

void sliceDigit(int number){
  int oncePlaceDigit = number % 10;
  int tensPlaceDigit = number / 10;
  slicedNumberArray[0] = tensPlaceDigit;
  slicedNumberArray[1] = oncePlaceDigit;
}

void displayNumber(int number){
  sliceDigit(number);
  for(int i = 0; i < 2 ; i++){
    displayDigit(slicedNumberArray[i]);
    delay(delayTime);
    displayReset();
    delay(delayTime/5);
  }
  memset(slicedNumberArray, 0, sizeof(slicedNumberArray));
}
