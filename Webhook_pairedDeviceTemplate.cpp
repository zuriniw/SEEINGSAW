bool doBlink = false;

// We will be using D2 to control our LED
int ledPin = D2;

// Our button wired to D0
int sensorPin = D3;

void setup()
{

  // For input, we define the
  // pushbutton as an input-pullup
  // this uses an internal pullup resistor
  // to manage consistent reads from the device

  pinMode( sensorPin , INPUT_PULLUP); // sets pin as input

  // We also want to use the LED

  pinMode( ledPin , OUTPUT ); // sets pin as output
	
	// blink the LED when the setup is complete
	blinkLED( 3, ledPin );
  
  Particle.subscribe( "blinkLED", handleActivateLED );

}

void loop()
{
   // find out if the button is pushed
   // or not by reading from it.
   int sensorState = digitalRead( buttonPin );

  // remember that we have wired the pushbutton to
  // ground and are using a pulldown resistor
  // that means, when the button is pushed,
  // we will get a LOW signal
  // when the button is not pushed we'll get a HIGH

  // let's use that to set our LED on or off

  if( sensorState == LOW )
  {
    // turn the LED On
    digitalWrite( ledPin, HIGH);
    Particle.publish( "doPairedPublish" );
		
  }else{
    // otherwise
    // turn the LED Off
    digitalWrite( ledPin, LOW);

  }
		delay( 1000 );



    if( doBlink == true ){
        blinkLED( 6, ledPin );
        doBlink = false;
    }


}

void blinkLED( int times, int pin ){
    
    for( int i = 0; i < times ; i++ ){
        digitalWrite( pin, HIGH );
        delay( 500 );
        digitalWrite( pin, LOW );
        delay( 500 );
    }
    
}

void handleActivateLED( const char *event, const char *data)
{
   doBlink = true;
}






