 import processing.serial.*;
Serial myPort;

int numOfImages = 10;
float distance = 0;
float smoothDistance = 0;
PImage[] images = new PImage[numOfImages];
Artwork[] artworks = new Artwork[numOfImages];
String[] dates = {"600 B.C", "510 B.C", "500 B.C", "490 B.C", "400 B.C", "300 B.C", "275 B.C", "250 B.C", "200 B.C", "100 A.D"};
int lineUnit;


void setup() {
  imageMode(CENTER);
  size(displayWidth, displayHeight);
  printArray(Serial.list());
  
  String portName = Serial.list()[2];
  myPort = new Serial(this, portName, 9600);
  myPort.bufferUntil('\n');
  
  for (int i=0; i < numOfImages; i++) {
    images[i] = loadImage(i+1 + ".jpg");
    artworks[i] = new Artwork(images[i]);
  }
  lineUnit = height/(numOfImages + 1);
}

void draw() {
  background(200);
  fill(100);
  noStroke();
  drawTimeline();
  smoothDistance = constrain(lerp(smoothDistance, distance, 0.1), 200, 1000);
  int activeIndex = round(map(smoothDistance, 200, 1000, 0, 9));
  showArtwork(activeIndex);
  fill(255, 100, 50);
  float indicatorPos = map(smoothDistance, 200, 1000, lineUnit, height-lineUnit);
  rect(0, indicatorPos, 80, 4);
  fill(50);
  textSize(32);
  text(dates[activeIndex], 120, indicatorPos);
}

void showArtwork(int index) {
  int nextIndex = index - 1 >= 0 ? index - 1 : numOfImages - 1;
  artworks[nextIndex].prepare();
  artworks[index].display();
}

void drawTimeline() {
  for (int i=0; i < numOfImages; i++) {
    rect(0, lineUnit*(i+1), 24, 4); 
  }
}

void serialEvent(Serial myPort) {
  String myString = myPort.readStringUntil('\n');
  if(myString != null){
    distance = float(myString);
  }
}
class Artwork {
  PImage image;
  float x = width/2;
  float y = height/2.1;
  
  Artwork(PImage image){
    this.image = image;
    this.image.resize(width/3, 0);
  }
  
  void prepare(){
    tint(255, 60);
    image(this.image, this.x + width/4, this.y);
    tint(255, 255);
  }
  
  void display(){
    image(this.image, this.x, this.y);
  }
}
