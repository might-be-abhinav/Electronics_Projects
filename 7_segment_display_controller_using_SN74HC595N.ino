/*
CIRCUIT EXPLAINATION:

5v from arduino UNO goes to the positive power rail and gnd to the negative power rail.
A 1k resistor connects the 5v to the VCC of SN74HC595N shift register and similarly SR's gnd connects to the -ve power rail. 
The 8 output pins Qa to Qh connect to a through g and decimal segments of the 7 segment display.
The displays common ground connects to the -ve powerrail through 1k resistor connected in series with a 5 k potentiometer which can be used as a brightness control.
(The 1k resistor can be replaced with a smaller one : plz do not skip this resistor : to increase max brightness and the 5k potentiometer can be replaced with a larger 
one to reduce the min brightness : this one can be skipped);

shiftRegisterBitsCorrespondingSegment = {a,b,c,d,e,f,g,decimal}                         

     --- a ---
    |         |
    f         b
    |         |
     --- g ---
    |         |
    e         c
    |         |
     --- d ---    . <------decimal



p.s.1 OE pin always LOW and srclr always high qA to a pin of the display,Qb to pin b and so on with pin h being the decimal pin
p.s.2 for the display, segment pins are the +ve terminal and the common pin is the ground. when high is applied to +ve segment terminal, the segment glows
*/

int dataPin = 6;//reads the current input
int srclkPin = 10;//moves the current input(HIGH:1/LOW:0) to the register
int rclkPin = 8;//sends output based on the entire register state


int digitPin = 2; // this pin holds the cathode pin of the 7 segment display



int number[11][8] = {
  //1 indicates that the corresponding segment is on
  {1,1,1,1,1,1,0,0},//0
  {0,1,1,0,0,0,0,0},//1
  {1,1,0,1,1,0,1,0},//2
  {1,1,1,1,0,0,1,0},//3
  {0,1,1,0,0,1,1,0},//4
  {1,0,1,1,0,1,1,0},//5
  {1,0,1,1,1,1,1,0},//6
  {1,1,1,0,0,0,0,0},//7
  {1,1,1,1,1,1,1,0},//8
  {1,1,1,1,0,1,1,0},//9
  {0,0,0,0,0,0,0,1}//.
};

void setup() {
  Serial.begin(9600);
  
  pinMode(dataPin,OUTPUT);
  pinMode(srclkPin,OUTPUT);
  pinMode(rclkPin,OUTPUT);


  digitalWrite(dataPin,LOW);
  digitalWrite(srclkPin,LOW);
  digitalWrite(rclkPin,LOW);

  pinMode(digitPin, OUTPUT);
  digitalWrite(digitPin, HIGH);

}

void loop() {
  int i = 0;
  for(i = 0; i < 11; i++){
    makeDisplayCompatibleAndSendToDisplay(digitPin,number[i]);  //currently acts as a 11 second timer
    delay(1000);
  }
  
}

void makeDisplayCompatibleAndSendToDisplay(int digitPin,int displayInstruction[8]){

  digitalWrite(digitPin,LOW);
  resetRegister();
  int registerStatus[8] = {0,0,0,0,0,0,0,0};
  for(int index = 0 ; index < 8 ; index++){
    if(displayInstruction[7-index] == 1){
      registerStatus[index] = 1;
      digitalWrite(dataPin,HIGH);
      
    }else{
      digitalWrite(dataPin,LOW);
      
    }
    digitalPulse(srclkPin);
  }
  digitalPulse(rclkPin);
}

void digitalPulse(int pin){
  digitalWrite(pin,HIGH);
  digitalWrite(pin,LOW);
}

void resetRegister(){
  for(int i = 0; i < 8; i++) {
    digitalWrite(dataPin, LOW);
    digitalPulse(srclkPin);
  }
  digitalPulse(rclkPin);
}