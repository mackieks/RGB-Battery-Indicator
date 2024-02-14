// RGB-Battery-Indicator Arduino example

#define LED_RED 0		// pin PB0. In MicroCore, this is Arduino Pin 0
#define LED_GREEN 1	// pin PB1. In MicroCore, this is Arduino Pin 1
#define LED_BLUE 2	// pin PB2. In MicroCore, this is Arduino Pin 2

#define batteryPin A3   // pin PB3. In MicroCore, this is Arduino Pin 3 (Pin A3 for analog stuff)

// MicroCore Pinout: https://github.com/MCUdude/MicroCore?tab=readme-ov-file#pinout


void setup(){ // runs once when the microcontroller powers up

    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);


    pinMode(batteryPin, INPUT);  // this is the pin we will measure battery voltage on.

    // The microcontroller is powered from the battery (VCC). It uses VCC as the reference voltage for its analog measurements.
    // So it doesn't have a fixed reference to compare the battery voltage to!
    // If you try to measure VCC / 2 via a voltage divider, it will always register as exactly half of the reference voltage (VCC)
    // even as the battery voltage changes, which is useless.

    // Luckily, AVR microcontrollers have an internal 1.1V reference voltage for these situations (kind of like an internal LDO regulator)
    // Normally you do the following to set it up:

    // analogReference(INTERNAL1V1);

    // ...but MicroCore doesn't have this function implemented, so you have to write the register directly:

    ADMUX |= _BV(REFS0); // sets the internal reference bit. Table 14-2 in the Attiny13A datasheet.


    // turn off all LEDs (set their pins low)
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, LOW);

}

void loop(){ // runs after setup, and loops forever
	
    // measure battery voltage! Add up 5 readings, then 

    uint16_t batteryVoltage = 0;

    for (uint8_t x = 0; x < 5; x++) { // loop 5 times

        batteryVoltage += analogRead(batteryPin);
        delay(5); // 5 milliseconds

    }

    // done looping. divide the 5 summed readings by 5 to get the average reading
    batteryVoltage = batteryVoltage / 5;

    // analogRead() produces a result between 0 and 1023. 0 = GND and 1023 = VREF 
    // VREF is the 1.1v internal reference, in this case.

    // The actual voltage thresholds depend on the voltage divider used to divide the battery voltage.
    // I don't know what the voltage divider setup for InsideGadget's board is but here's an example:
    // If VBAT/VCC = 4V and your voltage divider is a 10k to GND and a 40k to VBAT/VCC, then the ADC will measure 0.8V. 4*(10k/50k)
    // 0.8V / 1.1V * 1023 = 744. So analogRead will return a value of 744.

    // now light up the LEDs based on the battery voltage reading!

    // Green
    if (batteryVoltage >= (650)) { // 3.8v
        digitalWrite(LED_GREEN, LOW); // turn on green
        digitalWrite(LED_RED, HIGH); // turn off red
        digitalWrite(LED_BLUE, HIGH); // turn off blue
    }
        
    // Blue
    else if (batteryVoltage >= (580)) { // 3.3v
        digitalWrite(LED_GREEN, HIGH); // turn off green
        digitalWrite(LED_RED, HIGH); // turn off red
        digitalWrite(LED_BLUE, LOW); // turn on blue
    }
    
    // Red
    else {
        digitalWrite(LED_GREEN, HIGH); // turn off green
        digitalWrite(LED_RED, LOW); // turn on red
        digitalWrite(LED_BLUE, HIGH); // turn off blue
    }


    delay(1000); // wait 1 second

    // jump back to the start of void loop()

}