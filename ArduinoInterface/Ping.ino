
int ping[] = {7,8,9,10};

void setup() {
  Serial.begin(9600);
}

void loop() {

  for(int i=0; i < 4; i++){
  int cm = sensor(i);
  Serial.print(cm);
  Serial.print(" cm---");
  Serial.print(i+1);
  Serial.println();

  if(cm<30){
    Serial.print(i+1);
    Serial.write(" 1");
  }
  else{
    Serial.print(i+1);
    Serial.write(" 0"); 
  }
   Serial.println();

  delay(1000);
   }
}

unsigned long sensor(int index){
  unsigned long echo;
  long cm;
  pinMode(ping[index], OUTPUT);
  digitalWrite(ping[index], LOW);
  delayMicroseconds(2);
  digitalWrite(ping[index], HIGH);
  delayMicroseconds(5);
  digitalWrite(ping[index], LOW);
  pinMode(ping[index], INPUT);
  echo = pulseIn(ping[index], HIGH);
  cm = microsecondsToCentimeters(echo);
  return cm;
}
long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
