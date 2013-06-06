#include <SoftwareSerial.h> //Software Serial Port
#define RxD 2
#define TxD 3

SoftwareSerial blueToothSerial(RxD,TxD);

int pinI1=8;//define I1 interface
int pinI2=11;//define I2 interface 
int speedpinA=9;//enable motor A
int pinI3=12;//define I3 interface 
int pinI4=13;//define I4 interface 
int speedpinB=10;//enable motor B
int spead =127;//define the spead of motor
int RF_RX_PIN = 2;

void setup()
{
  pinMode(pinI1,OUTPUT);
  pinMode(pinI2,OUTPUT);
  pinMode(speedpinA,OUTPUT);
  pinMode(pinI3,OUTPUT);
  pinMode(pinI4,OUTPUT);
  pinMode(speedpinB,OUTPUT);

  Serial.begin(9600);
  Serial.println("BT Setup Start");
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  setupBlueToothConnection(); 
  Serial.println("BT Setup Finished");

  left(100);
  delay(250);
  stopTurn(250);

  right(100);
  delay(250);
  stopTurn(250);
  Serial.println("Ready");
}

void setupBlueToothConnection()
{
  blueToothSerial.begin(38400); //Set BluetoothBee BaudRate to default baud rate 38400
  blueToothSerial.print("\r\n+STWMOD=0\r\n"); //set the bluetooth work in slave mode
  blueToothSerial.print("\r\n+STNA=SeeedBTSlave\r\n"); //set the bluetooth name as "SeeedBTSlave"
  blueToothSerial.print("\r\n+STPIN=0000\r\n");//Set SLAVE pincode"0000"
  blueToothSerial.print("\r\n+STOAUT=1\r\n"); // Permit Paired device to connect me
  blueToothSerial.print("\r\n+STAUTO=0\r\n"); // Auto-connection should be forbidden here
  delay(2000); // This delay is required.
  blueToothSerial.print("\r\n+INQ=1\r\n"); //make the slave bluetooth inquirable 
  Serial.println("The slave bluetooth is inquirable!");
  delay(2000); // This delay is required.
  blueToothSerial.flush();
  blueToothSerial.write("Hi");
}

void stopTurn(int delayTime)//
{
  digitalWrite(speedpinA,LOW);// Unenble the pin, to stop the motor. this should be done to avid damaging the motor. 
  delay(delayTime);
}

void stopAcceleration(int delayTime)//
{
  digitalWrite(speedpinB,LOW);// Unenble the pin, to stop the motor. this should be done to avid damaging the motor. 
  delay(delayTime);
}

void stopAll(int delayTime){
  stopTurn(delayTime);
  stopAcceleration(delayTime);
}

String message = "";

void loop(){

  while (blueToothSerial.available() > 0) {
    char inChar = blueToothSerial.read();
    if (inChar != 'x') {
      // convert the incoming byte to a char 
      // and add it to the string:
      message += (char)inChar; 
    }
    // if you get a newline, print the string,
    // then the string's value:
    else {
      Serial.print("Command=[");
      Serial.print(message);
      Serial.println("]");
      control(message);
      // clear the string for new input
      message = ""; 
    }
  }

}

void control(String command){

  command.toUpperCase();
  String motorStr = command.substring(0,1) + "";
  String directionStr = command.substring(1,2)+ "";
  String speedStr = command.substring(2,5) + "";

  Serial.print ( "Motor=" );
  Serial.println ( motorStr );

  Serial.print ( "Direction=" );
  Serial.println ( directionStr );

  Serial.print ( "Speed=" );
  Serial.println ( speedStr );

  int speedVal = 0;
  speedVal = speedStr.toInt();

  if (motorStr == "S"){
    if (directionStr == "L"){
      left(speedVal);
    }
    if (directionStr == "R"){
      right(speedVal);
    }
  }
  if (motorStr == "D"){
    if (directionStr == "F"){
      forward(speedVal);
    }
    if (directionStr == "B"){
      reverse(speedVal);
    }
  }
  if (motorStr == "H"){
     stopAcceleration(100);
  }
  if (motorStr == "T"){
     stopTurn(100);
  }


}

// mspeed is % load (0-100)
void left(int mspeed){
  mspeed=map(mspeed,0,100,0,255);//scale 0-100 to 0-255
  analogWrite(speedpinA,mspeed);//set speed
  digitalWrite(pinI2,LOW);//set direction
  digitalWrite(pinI1,HIGH);
}

void right(int mspeed){
  mspeed=map(mspeed,0,100,0,255);
  analogWrite(speedpinA,mspeed);
  digitalWrite(pinI2,HIGH);
  digitalWrite(pinI1,LOW);
}

void forward(int mspeed){
  mspeed=map(mspeed,0,100,0,255);
  analogWrite(speedpinB,mspeed);
  digitalWrite(pinI4,HIGH);
  digitalWrite(pinI3,LOW);
}

void reverse(int mspeed){
  mspeed=map(mspeed,0,100,0,255);
  analogWrite(speedpinB,mspeed);
  digitalWrite(pinI4,LOW);
  digitalWrite(pinI3,HIGH);
}





























