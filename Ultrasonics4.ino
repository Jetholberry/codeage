#define echoPin 11
#define trigPin 12
#define red 7
#define green 6 
float duration, distance;

void setup() {
Serial.begin(9600);
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
pinMode(red, OUTPUT);
pinMode(green, OUTPUT);

}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  Serial.println(distance);
  distance= duration*0.0343/2;
  Serial.print("distance: ");
  if (distance>= 200 || distance <= 2) {
    Serial.println("  Yeah you OK");
    digitalWrite(green, HIGH);
    digitalWrite(red, LOW);
    delay(100);
    
  }
  else {
    Serial.println("TOO CLOSE MOFO");
    digitalWrite(red, HIGH);
    digitalWrite(green, LOW);
    delay(100);
  }
  delay(100);
  
}
