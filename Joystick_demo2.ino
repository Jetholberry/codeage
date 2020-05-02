int vrx = A0;
int vry = A1;
int led = 12;
int xPosition;
int yPosition;

void setup() {
  Serial.begin(9600);
  pinMode(vrx, INPUT);
  pinMode(vry, INPUT);
  digitalWrite(led, OUTPUT);
  digitalWrite(led, LOW);

}

void loop() {
  xPosition = analogRead(vrx);
  yPosition = analogRead(vry);
  delay(200);
  Serial.print("X: ");
  Serial.println(xPosition);
  Serial.print("Y: ");
  Serial.println(yPosition);
  Serial.println(". . .");
  
//works until this point*

  if (analogRead(xPosition == 0);
      digitalWrite(led, HIGH));

} else {
  digitalWrite(led, LOW);
}
delay(1000);


}
