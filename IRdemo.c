/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <IRremote.h>

int RECV_PIN = 10;

IRrecv irrecv(RECV_PIN); // Inicializar sensor

decode_results results;	// Variable para codigo de boton

void setup()
{
  Serial.begin(9600);
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
  pinMode(13,OUTPUT);
}


void loop() {
  if (irrecv.decode(&results)) {	// Recibe codigo de boton
    Serial.println(results.value, DEC);
    if (results.value == 16580863){
     digitalWrite(13,HIGH); 
    }
    irrecv.resume(); // Reinicia sensor
  }
  delay(100);
}
