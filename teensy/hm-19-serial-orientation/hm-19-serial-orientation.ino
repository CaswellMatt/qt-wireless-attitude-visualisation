#define HWSERIAL Serial1

void setup() {
  Serial.begin(9600);
  HWSERIAL.begin(230400);
}
float angle = 0;
void loop() {

  if (Serial.available()) {
    int i = 0;
    const int count = Serial.available();
    char buffer[count];
    while (Serial.available() > 0) {
      delay(20);
      buffer[i] = Serial.read();
      i++;
    }


    HWSERIAL.write(buffer, count - 1);
    HWSERIAL.flush();
    Serial.flush();
  }


  if (HWSERIAL.available()) {
    int i = 0;
    const int count = HWSERIAL.available();
    char buffer[count];
    
    while (HWSERIAL.available() > 0) {
      delay(20);
      buffer[i] = HWSERIAL.read();
      i++;
    }
  
    Serial.println(buffer);
    HWSERIAL.flush();
    Serial.flush();
  }

  angle += 1;
  angle = int(angle) % 360;
  float angleInRadians = angle * M_PI / 180;
  float sinOfHalfAngle = sin(angleInRadians / 2);
  float q1 = cos(angleInRadians / 2);
  float q2 = (sqrt(2)/2) * sinOfHalfAngle;
  float q3 = (sqrt(2)/2) * sinOfHalfAngle;
  float q4 = 0 * sinOfHalfAngle;
  
  HWSERIAL.print(q1);HWSERIAL.print(" ");
  HWSERIAL.print(q2);HWSERIAL.print(" ");
  HWSERIAL.print(q3);HWSERIAL.print(" ");
  HWSERIAL.print(q4);

  delay(100);

}
