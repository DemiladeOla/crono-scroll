import processing.serial.*; 
Serial myPort;

int numOfImages = 10;
float distance = 0;
float smoothDistance = 0;
float smoothIndicatorPos = 0;
int lineUnit = 0;
int currentIndex = numOfImages;
PFont fontFamily;
PImage[] images = new PImage[numOfImages];
Artwork[] artworks = new Artwork[numOfImages];
String[] dates = {"600 B.C", "510 B.C", "500 B.C", "490 B.C", "400 B.C", "300 B.C", "275 B.C", "250 B.C", "200 B.C", "100 A.D"};


void setup() {
  imageMode(CENTER);
  
  printArray(Serial.list());
  size(displayWidth, displayHeight);
  
  String portName = Serial.list()[2];
  myPort = new Serial(this, portName, 9600);
  myPort.bufferUntil('\n');
  
  for (int i=0; i < numOfImages; i++) {
    images[i] = loadImage(i+1 + ".jpg");
    artworks[i] = new Artwork(images[i]);
  }
  lineUnit = height/(numOfImages + 1);
  fontFamily = createFont("Druk", 88);
  textFont(fontFamily);
}

void draw() {
  background(200);
  fill(100);
  noStroke();
  drawTimeline();
  smoothDistance = constrain(lerp(smoothDistance, distance, 0.1f), 200, 1000);
  int activeIndex = floor(map(smoothDistance, 200, 1000, 0, 9));
  showArtwork(activeIndex);
  fill(255, 100, 50);
  float indicatorPos = map(smoothDistance, 200, 1000, lineUnit, height-lineUnit);
  smoothIndicatorPos = lerp(smoothIndicatorPos, indicatorPos, 0.1f);
  rect(0, smoothIndicatorPos - (lineUnit/3), 88, 4);
  fill(50);
  text(dates[activeIndex], 128, smoothIndicatorPos);
}

void showArtwork(int index) {
  int prevIndex = index >= numOfImages - 1 ? 0 : index + 1;
  artworks[prevIndex].hide();
  int nextIndex = index - 1 >= 0 ? index - 1 : numOfImages - 1;
  artworks[nextIndex].prepare();
  artworks[index].display();
}

void drawTimeline() {
  for (int i=0; i < numOfImages; i++) {
    rect(0, lineUnit*(i+1), 40, 4); 
  }
}

void serialEvent(Serial myPort) {
  String myString = myPort.readStringUntil('\n');
  if(myString != null){
    distance = PApplet.parseFloat(myString);
  }
}
class Artwork {
  PImage image;
  float x = width/2;
  float y = height/2.1f;
  float prepX = x + width/3.5f;
  float holdX = prepX;
  float holdX2 = width;
  float hideX = x - width/3.5f;
  float holdHideX = x;
  float alpha = 60;
  float holdAlpha = alpha;
  float holdAlpha2 = 255;
  
  Artwork(PImage image){
    this.image = image;
    this.image.resize(width/3, 0);
  }
  
  void prepare(){
    tint(255, alpha);
    this.holdX2 = lerp(this.holdX2, this.prepX, 0.12f);
    image(this.image, this.holdX2, this.y);
    
    this.holdX = prepX;
    this.holdAlpha = this.alpha;
  }
  
  void display(){
    this.holdAlpha = lerp(this.holdAlpha, 255, 0.1f);
    tint(255, this.holdAlpha);
    this.holdX = lerp(this.holdX, this.x, 0.1f);
    image(this.image, this.holdX, this.y);
    
    this.holdHideX = this.x;
    this.holdAlpha2 = 255;
  }
  
  void hide(){
    this.holdAlpha2 = lerp(this.holdAlpha2, 20, 0.08f);
    tint(255, this.holdAlpha2); 
    this.holdHideX = lerp(this.holdHideX, this.hideX, 0.08f);
    image(this.image, this.holdHideX, this.y);
    
    this.holdX2 = width;
  }
}

