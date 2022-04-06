#define echoPin 11
#define trigPin 10

long duration;
int distance;

void setup() {
  Serial.begin(9600);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(100);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  //  Convert to cm
  distance = duration / 58.2;
//  Serial.println("ss");
  Serial.println(duration);
  delay(100);
}
