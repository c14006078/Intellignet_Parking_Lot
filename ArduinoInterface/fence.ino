#include <Servo.h>

// servo接線 紅: 5v  黃:pin9 橘:GND 

Servo myservo;

int redpin = 11; // select the pin for the red LED

int bluepin =10; // select the pin for the blueLED

int val; // 255 - red    0 - green

int value = 0; // 旋轉角度

const int pingPin = 12; // for ultrasonic

unsigned int duration, cm;

boolean check = false ;



void setup() {

 pinMode(redpin, OUTPUT);

 pinMode(bluepin, OUTPUT);

 Serial.begin(9600);


 // set red

 val = 255 ;

 analogWrite(11, val);

 analogWrite(10, 255-val);

 // set fence

 value = 20 ;

 myservo.attach(9); // Servo 接在 pin 9

 myservo.write(value);  

}

void loop()

{

  //check distance

  pinMode(pingPin, OUTPUT);

  digitalWrite(pingPin, LOW);        // Ensure pin is low

  delayMicroseconds(2);

  digitalWrite(pingPin, HIGH);       // Start ranging

  delayMicroseconds(5);              //   with 5 microsecond burst

  digitalWrite(pingPin, LOW);        // End ranging

  pinMode(pingPin, INPUT);           // Set pin to INPUT

  duration = pulseIn(pingPin, HIGH); // Read echo pulse

  cm = duration / 29 / 2 ;   // Convert to cm

  //Serial.println(cm);            // Display result

  delay(200);                 // Short delay

  

  // set state

  if( cm < 10 && check == false) {

    check = true ;

    //Serial.println("Notify central center to recognize");

    Serial.print('c') ;  

    delay(1000);

  }



  // check if receive success -> open gate

  else if(check == true && Serial.available()) {

      char state = Serial.read() ;

      if(state =='y'){   // open

        //Serial.println("Pass is ON.");

        analogWrite(11, 0);

        analogWrite(10, 100);

        value = 100 ;

        myservo.write(value);  

        delay(500);

        val = 0 ; 

      }

  }

  

  //if gate is opening 

  else if(check == true && value == 100 && cm < 10) {

     //Serial.println("Gate is opening."); 

  }

  

  // open gate -> close gate

  else if( check == true && cm > 10 && value == 100 ){ // close

        analogWrite(11, 255);

        analogWrite(10, 0);

        value = 20 ;

        myservo.write(value);  

        delay(500);

        val = 255;

        //Serial.println("Car is leaving");

        check = false ;

  }



  // reset when car leaved before recognization success

  else if( check == true && cm > 10 && value == 20 ){

    check = false ;    

    //Serial.println("Reset");

  }

  

  // Waitting for recognization

  else if (check == true  && cm < 10 ){

      //Serial.println("Waitting for recognization");

  }

  

  // Waitting for car

  else{

      //Serial.println("Waitting for car");

  }

  
          Serial.write('n') ;  
          delay(500);


  

}
