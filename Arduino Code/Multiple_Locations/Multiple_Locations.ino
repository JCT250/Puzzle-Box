/* Based upon the work by Mikal Hart.
Multiple Location version of the puzzle box.
Adapted by James Taylor 2012-2013
*/

#include <PWMServo.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>

void PowerOff();

/*Hardware variable setup*/
static const int pololu_switch_off = 12; // Pololu switch control
static const int servo_control = 9; // Servo control
static const int dial1_input = 0;
static const int dial2_input = 1;
//static const int LED_pin = 11; // The button LED


/* These values should be adjusted according to your needs - Software variable setup*/
static const int CLOSED_ANGLE = 70; // degrees
static const int OPEN_ANGLE = 130; // degrees
static const float DEST1_LATITUDE = -36.779267;
static const float DEST1_LONGITUDE = 174.656980;
static const float DEST2_LATITUDE = -36.90018;
static const float DEST2_LONGITUDE = 174.783188;
static const float DEST3_LATITUDE = -36.828837;
static const float DEST3_LONGITUDE = 174.812144;
static const float DEST4_LATITUDE = -36.822858;
static const float DEST4_LONGITUDE = 174.750806;
static const int RADIUSL = 1000; // meters for dest 1,2,3
static const int RADIUSS = 500; //meters for dest 4
static const int array_size = 64; //change this to change size of array
int curr_array_pos = 0;
int dial1_array[array_size];
int dial2_array[array_size];
int val_accepted = 0; //value to exit from code enterance loop and proceed to latch unlocking in back door
int dial1_val; //used to store current average value of dial input 1
int dial2_val; //used to store current average value of dial input 2
long dial1_total;
long dial2_total;

static const int correct_dial1_val = 809; //correct value of dial 1
static const int correct_dial2_val = 596; //correct value of dial 2



/* Fixed values should not need changing */
static const int DEF_ATTEMPT_MAX = 50;
static const int EEPROM_OFFSET = 100;

/* The basic objects needed */
SoftwareSerial nss(3, 4);
LiquidCrystal lcd(10, 11, 5, 6, 7, 8);
TinyGPS tinygps; 
int attempt_counter;
PWMServo servo;

/* A helper function to display messages of a specified duration */
void Msg(LiquidCrystal &lcd, const char *top, const char *bottom, unsigned long del)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(top);
  lcd.setCursor(0, 1);
  lcd.print(bottom);
  delay(del);
}

//void Deg(LiquidCrystal &lcd, int c, int r)
//{
//lcd.setCursor(c,r);
//lcd.print((char)223);
//} 

/* The Arduino setup() function */
void setup()
{
  /* attach servo motor */
  servo.attach(servo_control);

  /* establish a debug session with a host computer */
  Serial.begin(57600);
  Serial.println("Serial Comms EST");
  
  /* establish communications with the GPS module */
  nss.begin(9600);
  Serial.println("GPS Comms EST");
  
  /* establish communication with LCD */
  lcd.begin(16, 2); // this for an 16x2 LCD -- adjust as needed 

  /* Make sure Pololu switch pin is OUTPUT and LOW */
  pinMode(pololu_switch_off, OUTPUT);
  digitalWrite(pololu_switch_off, LOW);

  /* make sure motorized latch is closed */
  servo.write(CLOSED_ANGLE); 
  Serial.println("Lock Closed");
  
  /* read the attempt counter from the EEPROM */
  attempt_counter = EEPROM.read(EEPROM_OFFSET);

  if (attempt_counter == 0xFF) // brand new EEPROM?
    attempt_counter = 0;

  /* increment it with each run */
  //++attempt_counter; //Uncomment to enable incrementation of attempt counter with each run

  /* Greeting */
  Msg(lcd, " Welcome to the", "   Puzzle Box", 3000);

  /* Game over? */
  if (attempt_counter >= DEF_ATTEMPT_MAX)
  {
    Msg(lcd, "Sorry! No more", "attemps allowed", 2000);
    PowerOff();
  }

  /* Print out the attempt counter */
  Msg(lcd, "This is attempt", " ", 1000);
  lcd.setCursor(0, 1);
  lcd.print(attempt_counter);
  lcd.print(" of "); 
  lcd.print(DEF_ATTEMPT_MAX);
  delay(3000);

  /* Save the new attempt counter */
  EEPROM.write(EEPROM_OFFSET, attempt_counter);

  Msg(lcd, "Seeking", "Signal..", 0);
  Serial.println("Entering loop");
}

/* The Arduino loop() function */
void loop()
{
  /* Has a valid NMEA sentence been parsed? */
  if(nss.available() && tinygps.encode(nss.read()))
  {
    float lat, lon;
    unsigned long fix_age;

    /* Have we established our location? */
    tinygps.f_get_position(&lat, &lon, &fix_age);
    if (fix_age != TinyGPS::GPS_INVALID_AGE)
    {
      /* Calculate the distance to all four destinations */
      float distance_meters1 = TinyGPS::distance_between(lat, lon, DEST1_LATITUDE, DEST1_LONGITUDE);
      float distance_meters2 = TinyGPS::distance_between(lat, lon, DEST2_LATITUDE, DEST2_LONGITUDE);
      float distance_meters3 = TinyGPS::distance_between(lat, lon, DEST3_LATITUDE, DEST3_LONGITUDE);
      float distance_meters4 = TinyGPS::distance_between(lat, lon, DEST4_LATITUDE, DEST4_LONGITUDE);   

      /* Are we close enough to any of the destinations? */
      if (distance_meters1 <= RADIUSL)
      { //Message for DEST1 goes here
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("36.779267 S");
        //Deg(lcd,9,0);
        lcd.setCursor(0,1);
        lcd.print("174.656980 E");
        //Deg(lcd,10,1);
        delay(10000);
        lcd.setCursor(0,2);
        lcd.print("XXXXOOOOXXXXOOOO");
        lcd.setCursor(0,3);
        lcd.print("XXXXOOOOXXXXOOOO");
        delay(5000);
      }

      else if (distance_meters2 <= RADIUSL)
      { //Message for DEST2 goes here
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("36.900118 S");
        //Deg(lcd,9,0);
        lcd.setCursor(0,1);
        lcd.print("174.783188 E");
        //Deg(lcd,10,1);
        delay(10000);
        lcd.setCursor(0,2);
        lcd.print("XXXXOOOOXXXXOOOO");
        lcd.setCursor(0,3);
        lcd.print("XXXXOOOOXXXXOOOO");
        delay(5000);
      }

      else if (distance_meters3 <= RADIUSL)
      { //Message for DEST3 goes here
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("36.828837 S");
        //Deg(lcd,9,0);
        lcd.setCursor(0,1);
        lcd.print("174.812144 E");
        //Deg(lcd,10,1);
        delay(10000);
        lcd.setCursor(0,2);
        lcd.print("XXXXOOOOXXXXOOOO");
        lcd.setCursor(0,3);
        lcd.print("XXXXOOOOXXXXOOOO");
        delay(5000);
      }

      else if(distance_meters4 <=RADIUSS)
      { //Message for DEST4 goes here
        lcd.clear();
        lcd.setCursor(3,0);
        lcd.print("Problem Solved");
        lcd.setCursor(3,1);
        lcd.print("Access Granted");
        delay(3000);
        servo.write(OPEN_ANGLE);
        PowerOff(); //Shutdown after opening    
      }

      /* If not then Print the distance. */
      else 
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Distance to A");
        lcd.setCursor(0, 1);
        lcd.print((int)(distance_meters1));
        lcd.print(" m.");
        delay(4000);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Distance to B");
        lcd.setCursor(0, 1);
        lcd.print((int)(distance_meters2));
        lcd.print(" m.");
        delay(4000);

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Distance to C");
        lcd.setCursor(0, 1);
        lcd.print((int)(distance_meters3));
        lcd.print(" m.");
        //delay(2000);   Not needed as next command preceeded with a delay of 4 seconds 

        //Then after either displaying coords and clue or distance screen show the following message and Shutdown  
        delay(4000);
        Msg(lcd, "Access", "Denied!", 2000);
      }

      PowerOff();
    }
  }

  /* Turn off after 5 minutes - Program should only reach this point if no GPS signal received. 
   Looks to be designed to prevent the Box from staying on if the PowerOff function 
   call above is not called because program never enters the IF statement dependant on a valid NMEA signal
   and never enters the IF statement dependant on a GPS fix*/

  if (millis() >= 300000)
    PowerOff();
}

/* Called to shut off the system using the Pololu switch */
void PowerOff()
{
  Serial.println("Powering Off");
  Msg(lcd, "Powering", "Off!", 2000);
  lcd.clear(); 

  /* Bring Pololu switch control pin HIGH to turn off */
  digitalWrite(pololu_switch_off, HIGH);

  /* This is the back door.  If we get here, then the battery power */
  /* is being bypassed by the USB port.  We'll wait a couple of */
  /* minutes and then ask for the right code. If corrent then grant access. */
  delay(120000);

  while(val_accepted < 500)
  {
    // delay(500);
    if(curr_array_pos == array_size) //If array position is invlaid because it has incremented above array size (note array = zero indexed) then reset to zero
  {
    curr_array_pos = 0;
  }

  dial1_total = dial1_total - dial1_array[curr_array_pos]; //subtract the old value in the dial 1 current array position from the dial 1 total
  dial2_total = dial2_total - dial2_array[curr_array_pos];
  
  dial1_array[curr_array_pos] = analogRead(dial1_input); //take value from dial 1 analog input pin and store in current position in dial 1 array
  dial2_array[curr_array_pos] = analogRead(dial2_input); //take value from dial 2 analog input pin and store in current position in dial 2 array
  
  dial1_total = dial1_total + dial1_array[curr_array_pos];
  dial2_total = dial2_total + dial2_array[curr_array_pos];
  
  dial1_val = dial1_total / array_size;
  dial2_val = dial2_total / array_size;
  
    lcd.setCursor(0,0);
    lcd.print(dial1_val);
    lcd.setCursor(0,1);
    lcd.print(dial2_val);
    lcd.setCursor(5,0);
    lcd.print(dial1_total);
    lcd.setCursor(5,1);
    lcd.print(curr_array_pos);
    
    
    if(dial1_val == correct_dial1_val && dial2_val == correct_dial2_val)
     {
       val_accepted ++;
     }
     else
     {
       val_accepted = 0;
     }
     
     curr_array_pos ++; //increment array position
     
  }
  
  lcd.clear();
 
  servo.write(OPEN_ANGLE); // and open the box 

  /* Reset the attempt counter */
  EEPROM.write(EEPROM_OFFSET, 0); 

  /* Leave the latch open for 10 seconds */
  delay(10000); 

  /* And then seal it back up */
  servo.write(CLOSED_ANGLE); 

  /* Exit the program for real */
  exit(1);
} 



