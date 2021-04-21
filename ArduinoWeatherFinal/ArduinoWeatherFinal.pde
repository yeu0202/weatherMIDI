// This example code is in the public domain.
import processing.serial.*;
Serial myPort;      // The serial port
int[] serialInArray = new int[6]; // Where we'll put what we receive
int serialCount = 0;     // A count of how many bytes we receive
boolean firstContact = false;  // Whether we've heard from the
          // microcontroller




PGraphics brightness;

PImage img;

int rainArraySize = 100;
rainDrop[] drops = new rainDrop[rainArraySize];


// serial variables
int windSpeed = 0;
int tempIn = 125;
int tempOut = 125;
int humidity = 125;
int lightLevel = 125;
int airPressure = 125;

int tempComb = 0;


// convolution matrices
float [][] matrix_1 = {{ -0.5, -0.5, -0.5},
                     { -0.5, 5, -0.5},
                     {-0.5, -0.5, -0.5}};
//float [][] matrix0 = {{ -1, -1, -1},
//                     { -1, 9, -1},
//                     {-1, -1, -1}};
float [][] matrix1 = {{ 0, 0, 0},
                     { 0, 1, 0},
                     { 0, 0, 0}};
//float [][] matrix2 = {{ 1/9, 1/9, 1/9},
//                     { 1/9, 1/9, 1/9},
//                     { 1/9, 1/9, 1/9}};
float [][] matrix3 = {{ 0.1111, 0.1111, 0.1111},
                     { 0.1111, 0.1111, 0.1111},
                     { 0.1111, 0.1111, 0.1111}};
int matrixSize = 3;



void setup() {
  size(720, 720);  // 738 536
  img = loadImage("testimage.png");

  brightness = createGraphics(width, height);

  for (int i = 0; i < drops.length; i++)
  {
    drops [i] = new rainDrop();
  }
  strokeWeight(3);
  textSize(20);
  fill(0, 0, 0);
  
  
  // serial stuff
  // Print a list of the serial ports, for debugging purposes:
  println(Serial.list());
  // I know that the first port in the serial list on my mac
  // is always my FTDI adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  String portName = Serial.list()[2];
  myPort = new Serial(this, portName, 9600);
}



/**
windSpeed = posterize
temp = tint
humidity = rain
lightLevel = brightness
airPressure = blur
**/
void draw() {
  //int varying = round(map(mouseX, 0, width, 0, 255));
  
  // collect sensor information here
  //windSpeed = 0;
  //tempIn = 127;
  //tempOut = 127;
  tempComb = (tempIn + tempOut)/2;
  //humidity = 180;
  //lightLevel = 127;
  //airPressure = 255;
  
   
  
  
  // draw image
  brightness.beginDraw();

  loadPixels();
  img.loadPixels();


  for (int x = 0; x < img.width; x++ ) {
    for (int y = 0; y < img.height; y++ ) {

      int loc = x + y*img.width;

      float r = red  (img.pixels[loc]);
      float g = green(img.pixels[loc]);
      float b = blue (img.pixels[loc]);
 
      // brightness
      float adjustBrightness = map(lightLevel, 0, 255, 0.2, 1.8); 
      r *= adjustBrightness;
      g *= adjustBrightness;
      b *= adjustBrightness;
      
      // tint
      float adjustTint = map(tempComb, 0, 255, 0, 2);
      r *= adjustTint;
      
      // don't go out of bounds
      r = constrain(r, 0, 255); 
      g = constrain(g, 0, 255);
      b = constrain(b, 0, 255);

      color c = color(r, g, b);
      pixels[loc] = c;
    }
  }
  
  // blur
  int blurLevel = int(map(airPressure, 0, 255, 0, 3));
  if(blurLevel == 0){
    for (int x = 0; x < img.width; x++ ) {
      for (int y = 0; y < img.height; y++ ) {
  
        int loc = x + y*img.width;
        color c = convolution(x, y, matrix3, matrixSize, img);
        pixels[loc] = c;
      }
    }
  }
  else if (blurLevel == 1){
    // do nothing
    for (int x = 0; x < img.width; x++ ) {
      for (int y = 0; y < img.height; y++ ) {
  
        int loc = x + y*img.width;
        color c = convolution(x, y, matrix1, matrixSize, img);
        pixels[loc] = c;
      }
    }
  }
  else{ // blurLevel == 2
    for (int x = 0; x < img.width; x++ ) {
      for (int y = 0; y < img.height; y++ ) {
  
        int loc = x + y*img.width;
        color c = convolution(x, y, matrix_1, matrixSize, img);
        pixels[loc] = c;
      }
    }
  }
  



  updatePixels();
  brightness.endDraw();
  
  float windLog = log(windSpeed+1); // use log to get a semi-linear scaling
  float posterizeLevel = map(windLog, 0, 5.6, 25, 3);
  filter(POSTERIZE, posterizeLevel);
  
  
   int adjustRain = int(map(humidity, 0, 255, 0, rainArraySize));
   for (int i = 0; i< adjustRain; i++){
      drops[i].fall();
      drops[i].show();
   }
   
  text("windSpeed = " + windSpeed, 10, 25); 
  text("tempIn = " + tempIn, 10, 50); 
  text("tempOut = " + tempOut, 10, 75); 
  text("humidity = " + humidity, 10, 100); 
  text("lightLevel = " + lightLevel, 10, 125); 
  text("airPressure = " + airPressure, 10, 150);
  
  
  text("fps: " + frameRate, 10, 175);
}


color convolution(int x, int y, float[][] matrix, int matrixsize, PImage img) {
  float rtotal = 0.0;
  float gtotal = 0.0;
  float btotal = 0.0;
  int offset = matrixsize / 2;
  // Loop through convolution matrix
  for (int i = 0; i < matrixsize; i++){
    for (int j= 0; j < matrixsize; j++){
      // What pixel are we testing
      int xloc = x+i-offset;
      int yloc = y+j-offset;
      int loc = xloc + img.width*yloc;
      // Make sure we have not walked off the edge of the pixel array
      loc = constrain(loc,0,pixels.length-1);
      // Calculate the convolution
      // We sum all the neighboring pixels multiplied by the values in the convolution matrix.
      rtotal += (red(pixels[loc]) * matrix[i][j]);
      gtotal += (green(pixels[loc]) * matrix[i][j]);
      btotal += (blue(pixels[loc]) * matrix[i][j]);
    }
  }
  // Make sure RGB is within range
  rtotal = constrain(rtotal,0,255);
  gtotal = constrain(gtotal,0,255);
  btotal = constrain(btotal,0,255);
  // Return the resulting color
  return color(rtotal,gtotal,btotal);
}



void serialEvent(Serial myPort) {
  // read a byte from the serial port:
  int inByte = myPort.read();
  // if this is the first byte received, and it's an A,
  // clear the serial buffer and note that you've
  // had first contact from the microcontroller.
  // Otherwise, add the incoming byte to the array:
  //println("aaaaaaa"); // debugging message
  if (firstContact == false) {
    if (inByte == 'A') {
      myPort.clear();   // clear the serial port buffer
      firstContact = true;  // you've had first contact from the microcontroller
      myPort.write('A');  // ask for more
    }
  }
  else {
    // Add the latest byte from the serial port to array:
    serialInArray[serialCount] = inByte;
    serialCount++;
    // If we have     \/ this number + 1 bytes:
    if (serialCount > 5 ) {
      windSpeed = serialInArray[0];
      tempIn = serialInArray[1];
      tempOut = serialInArray[2];
      humidity = serialInArray[3];
      lightLevel = serialInArray[4];
      airPressure = serialInArray[5];
      // Send a capital A to request new sensor readings:
      myPort.write('A');
      // Reset serialCount:
      serialCount = 0;
    }
  }
}



//// key press to change values
//void keyPressed() {
//  if (key == 'q' || key == 'Q') {
//    windSpeed += 10;
//    windSpeed = constrain(windSpeed, 0, 255);
//  }
//  if (key == 'a' || key == 'A') {
//    windSpeed -= 10;
//    windSpeed = constrain(windSpeed, 0, 255);
//  }
  
//  if (key == 'w' || key == 'W') {
//    tempIn += 10;
//    tempIn = constrain(tempIn, 0, 255);
//  }
//  if (key == 's' || key == 'S') {
//    tempIn -= 10;
//    tempIn = constrain(tempIn, 0, 255);
//  }
  
//  if (key == 'e' || key == 'E') {
//    tempOut += 10;
//    tempOut = constrain(tempOut, 0, 255);
//  }
//  if (key == 'd' || key == 'D') {
//    tempOut -= 10;
//    tempOut = constrain(tempOut, 0, 255);
//  }
  
//  if (key == 'r' || key == 'R') {
//    humidity += 10;
//    humidity = constrain(humidity, 0, 255);
//  }
//  if (key == 'f' || key == 'F') {
//    humidity -= 10;
//    humidity = constrain(humidity, 0, 255);
//  }
  
//  if (key == 't' || key == 'T') {
//    lightLevel += 10;
//    lightLevel = constrain(lightLevel, 0, 255);
//  }
//  if (key == 'g' || key == 'G') {
//    lightLevel -= 10;
//    lightLevel = constrain(lightLevel, 0, 255);
//  }
  
//  if (key == 'y' || key == 'Y') {
//    airPressure += 10;
//    airPressure = constrain(airPressure, 0, 255);
//  }
//  if (key == 'h' || key == 'H') {
//    airPressure -= 10;
//    airPressure = constrain(airPressure, 0, 255);
//  }

//  tempComb = (tempIn + tempOut)/2;
//}
