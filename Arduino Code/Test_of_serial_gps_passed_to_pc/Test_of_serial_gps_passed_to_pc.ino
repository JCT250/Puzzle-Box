/* Created by Mikal Hart */

#include <SoftwareSerial.h>

/* The basic objects needed */
SoftwareSerial nss(4, 3);
void setup()
{
  /* establish a debug session with a host computer */
  Serial.begin(4800);

  /* establish communications with the GPS module */
  nss.begin(4800);
}

/* The Arduino loop() function */
void loop()
{
  if(nss.available()>0)
    Serial.write(nss.read());
} 


