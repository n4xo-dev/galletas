// C++ code
//

#include <Servo.h>
#include <IRremote.h>

Servo myservo;		// servo variables
int servoPin = 5;

//Letters from "a" to "g" are related to 7-segment
int a = 13;
int b = 12;
int c = 11;
int d = 10;
int e = 9;
int f = 8;
int g = 4;

// IR variables and constants
bool isEnabled = false;		// Boolean for system state
int RECV_PIN = 3;			// Receving digital pin
IRrecv irrecv(RECV_PIN); 	// Initialize sensor
decode_results results;		// Variable for received code

// Counter variables and constants
int packageCount = 0;				// Count of cookies
const int packageGoal = 7;			// Goal of cookies per package
const int cookieThreshold = 130;	// Threshold distance to consider a cookie has been detected

void setup()
{
  pinMode(a, OUTPUT); // This stablish the digital outputs...
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);
  
  myservo.attach(servoPin);
  myservo.write(0);
  Serial.begin(9600);
  irrecv.enableIRIn();	// Start the receiver
  
  display(-1);		// System starts at state "disabled"
}

  


//https://www.tinkercad.com/things/bwx3HVeX1Jn
int sonarReading = 0;	// sonar variables
int echoPin = 6;
int triggerPin = 7;
long readUltrasonicDistance(int triggerPin, int echoPin) // sonar function
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}


void takeCookie()
{
  myservo.write(90); //Moves the servo
  delay(1000); 
  myservo.write(0); //Returns servo to starting position

}

/*
    IR MODULE
    Source: https://www.tinkercad.com/things/0n2VuaSIEZV
    Description: Checks if any IR signal has been sent.
	If the ON/OFF button is detected the state of the system changes.
*/ 
void checkIRSignal()
{
  if (irrecv.decode(&results)) {		// Receives button code
      Serial.println(results.value, DEC); // Shows code through terminal
  	if (results.value == 16580863){		// Check if ON/OFF button was pressed
      isEnabled = !isEnabled;			// System state changes
      display(-1);						// Display E for error
      Serial.print("System state:");
      Serial.println(isEnabled);
  	}
  	irrecv.resume(); 					// Reset sensor
  }
  delay(100);
}

/*
    7 SEGMENTS MODULE
    Source: https://www.tinkercad.com/things/lm0lS38Mq42
    Description: Writes the given segments to the output pins.
*/ 
void seg7(int a, int b, int c, int d, int e, int f, int g)
{

  // 7-segment variables assignation
  digitalWrite(4,g);
  digitalWrite(8,f);
  digitalWrite(9,e);
  digitalWrite(10,d);
  digitalWrite(11,c);
  digitalWrite(12,b);
  digitalWrite(13,a); 
}

/*
	DISPLAY MODULE
    Source: https://www.tinkercad.com/things/lm0lS38Mq42
    Description: Translates number to 7 segments and displays it.
    Inspired from source, not copied.
*/
void display(int n)
{
  switch(n){
    case 0:
      seg7(1,1,1,1,1,1,0);
      break;
    case 1:
      seg7(0,1,1,0,0,0,0);
      break;
    case 2:
      seg7(1,1,0,1,1,0,1);
      break;
    case 3:
      seg7(1,1,1,1,0,0,1);
      break;
    case 4:
      seg7(0,1,1,0,0,1,1);
      break;
    case 5:
      seg7(1,0,1,1,0,1,1);
      break;
    case 6:
      seg7(1,0,1,1,1,1,1);
      break;
    case 7:
      seg7(1,1,1,0,0,0,0);
      break;
    case 8:
      seg7(1,1,1,1,1,1,1);
      break;
    case 9:
      seg7(1,1,1,1,0,1,1);
      break;
    default:
      seg7(1,0,0,1,1,1,1);	// ERROR
      break;
  }
}


/*
	COUNTER MODULE
    Source: own
    Description: Checks if signal from sonar has detected a cookie.
	If so, it increases the count of cookies and when it reaches the
    given package amount, it activates the servo packaging function.
    It also sends the count to display.
*/ 
void counter()
{
  if (sonarReading < cookieThreshold){
    packageCount++;
    display(packageCount);
    Serial.print("Count: ");
    Serial.println(packageCount);
    if(packageCount >= packageGoal){
      takeCookie();
      packageCount = 0;	// Reset count
      Serial.println("Count has been reset");
    }
  }
}


//https://www.tinkercad.com/things/lm0lS38Mq42

void loop()
{
  checkIRSignal();
  if(isEnabled){
    delay(1000);	// Wait for 1000 millisecond(s)
    sonarReading = 0.01723 * readUltrasonicDistance(triggerPin,echoPin);
    counter();
    Serial.print("Sonar: ");
    Serial.println(sonarReading);
  }
}
