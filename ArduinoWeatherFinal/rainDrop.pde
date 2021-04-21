class rainDrop {

 float x = random(width);
 float y = random (-200, - 100);
 float yspeed = random(10, 20);
 
 void fall () {
  y = y + yspeed; 

  if (y > height) {
    y = random(-200, -100);
  }

 }
 
 void show() {
  stroke(0, 255, 255);
  line(x, y, x, y+10);
 }
}
