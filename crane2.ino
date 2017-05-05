//pin numbers
#define crane 8
#define rope  9
#define s0 2
#define s1 3
#define s2 4
#define s3 5
#define out 6
#define rswitch 10
#define lswitch 11
//constents
int right = 30;
int left = -30;
int up = 100;
int down = -100;
int freez = 0;
int dropdis = 3500;
int highus = 1500;
int lowus = 1000;
int movedis = 500;
//variables
char dir;
unsigned long times;
int red, green, blue;
enum position {leftlim, mid, rightlim};
position pos, prevpos;
String input;
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setup() {
 Serial.begin(9600);
  pinMode(lswitch, INPUT);
  pinMode(rswitch, INPUT);
  sensorInit();
  pinMode(crane,OUTPUT);
  pinMode(rope,OUTPUT);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void loop() {
//reciving a message
  if (Serial.available() > 0){
    input = Serial.readString();
  }
//--------------------------------------------------
//going left
  if (input == "lg"){
   if(pos != leftlim) 
    move(crane,left);
   else if(pos != prevpos)
   Serial.print("|You'v reached the left edge|");
  }
//--------------------------------------------------
//going rigth
  else if (input == "rg"){
   if(pos != rightlim)
    move(crane,right);
   else if(pos != prevpos)
   Serial.print("|You'v reached the right edge|");
  }
//--------------------------------------------------
//going up
  else if (input == "ug"){
    move(rope,up);
  }
//--------------------------------------------------
//going down
  else if (input == "dg"){
    move(rope,down);
  }
//--------------------------------------------------
// scan for color
  else if (input == "scan"){
    if(color() != "")
    Serial.print("|" + color() + "|");
   input = "";
  }
//--------------------------------------------------
//looking for a cube
  else if (input == "red" || input == "green" || input == "blue"){
    find(input);
  }
//--------------------------------------------------
//freez all motor functions
  else {
    move(rope,freez);
    move(crane,freez);
  }
//--------------------------------------------------
//keep track of crane's position
  if(digitalRead(lswitch))
    pos = leftlim;
  else if(digitalRead(rswitch))
    pos = rightlim;
  else
  pos = mid;

  prevpos = pos;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void sensorInit(){
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);
  // Setting frequency-scaling to 20%
  digitalWrite(s0,HIGH);
  digitalWrite(s1,LOW);
}
//find the color
String color(){
digitalWrite(s2, LOW);  
  digitalWrite(s3, LOW);  
  //count OUT, pRed, RED  
  red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
  digitalWrite(s3, HIGH);  
  //count OUT, pBLUE, BLUE  
  blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
  digitalWrite(s2, HIGH);  
  //count OUT, pGreen, GREEN  
  green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH); 
  if(red < blue && red < green && red < 100)
    return "red";
  if(blue < red && blue < green && blue < 100)
    return "blue";
  if(green < red && green < blue && green < 100)
    return "green";
  else
    return "";
}
//find a specific color
void find(String cube){
  while(color() != cube && !digitalRead(lswitch)){
    dir = 'l';
    move(crane,left); 
  }
  while(color() != cube && !digitalRead(rswitch)){
    dir = 'r';
    move(crane,right);
  }
  times = millis();
  if (dir == 'l'){
    Serial.println(dir);
    while(millis() < times + movedis){
   move(crane,left); }
  }
  if (dir == 'r'){
    Serial.println(dir);
    while(millis() < times + movedis){
   move(crane,right); }
  }
  delay(500);
  drop();
  dir = 's';
  input = "s";
}
//dropping the rope to cube hight and lift it up
void drop(){
  times = millis();
  while(millis() < times + dropdis){
   move(rope,down); }
  delay(500);
  times = millis();
  while(millis() < times + dropdis + 200)
   move(rope,up); 
}
//motors control
void move (int pin, int speed){
  highus = 5*speed + 1500;
  digitalWrite(pin, HIGH);
  delayMicroseconds(highus);
  digitalWrite(pin, LOW);
  delayMicroseconds(lowus);
}

