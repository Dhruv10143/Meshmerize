#define ir1 4
#define ir2 A0
#define ir3 A1
#define ir4 A2
#define ir5 A3
#define ir6 A4
#define ir7 A5
#define ir8 3
#define stops 7
int timee;

int n=0;

int ir[8];                      // ir[0] -> rightfar
int irr[8];
//// out 4

#define ML1 11
#define ML2 10

#define MR1 9
#define MR2 6

int leapTime = 100;
int v=210;
int node[100];
int error = 0, prev_error = 0;
float Kp = 18, Kd = 8; // KP=14, kD=6
// float kp1= 18 kd1=10;
int weights[8] = { -10, -7, -5, -1, 1, 5, 7, 10};
int avg_error = 0, sensors = 0, output = 0, leftmot = 0, rightmot = 0;
unsigned long t;

void setup() {
  // put your setup code here, to run once
  for(int i=0;i<100;i++)
     node[i]=0;
  pinMode(ir1, INPUT);
  pinMode(ir2, INPUT);
  pinMode(ir3, INPUT);
  pinMode(ir4, INPUT);
  pinMode(ir5, INPUT);
  pinMode(ir6, INPUT);
  pinMode(ir7, INPUT);
  pinMode(ir8, INPUT);
  pinMode(stops, INPUT);
  
  pinMode(ML1, OUTPUT);
  pinMode(ML2, OUTPUT);
  pinMode(MR1, OUTPUT);
  pinMode(MR2, OUTPUT);
  pinMode(12,OUTPUT);

 Serial.begin(9600);
  analogWrite(ML1, 0);
  analogWrite(ML2, 0);
  analogWrite(MR1, 0);
  analogWrite(MR2, 0);
  delay(1000);
}

void loop() {

  readSensors();  
  
  if ((ir[0] && ir[1] && ir[2]) || (ir[5] && ir[7]) || 
      (ir[0] && ir[1] && ir[2] && ir[3] && ir[4] && ir[7]) ||
      (!ir[0] && !ir[1] &&  !ir[2] && !ir[3] && !ir[4] && !ir[5] &&  !ir[6] && !ir[7]))
  {
    // Serial.println("going for LSR");
    for(int i=0; i<8 ; i++)
       irr[i] = 0;
    t = millis();
    while (millis() - t < 60) {
      readSensors();
      for (int i = 0; i < 8; i++)
      {
        if (irr[i] == 0)
          irr[i] = ir[i]; 
      }
      normStraight();
    }
      for(int i=7; i>=0; i--)
      {
        Serial.print(ir[i]);
        Serial.print("   ");
        }
        Serial.println("");
    LSR();
  }
  else {
    straight();
  }

}


void readSensors() {
  ir[0] = !digitalRead(ir1);
  ir[1] = !digitalRead(ir2);
  ir[2] = !digitalRead(ir3);
  ir[3] = !digitalRead(ir4);
  ir[4] = !digitalRead(ir5);
  ir[5] = !digitalRead(ir6);
  ir[6] = !digitalRead(ir7);
  ir[7] = !digitalRead(ir8);
}

int check(){
  if(node[n-1] == 2 && node[n-2] == 3 && node[n-3] == 0)   //lbr=b
  {
    n=n-3;
    node[n] = 3;
    Serial.println("B1");
    n++;
    return 1;
  }
  else if(node[n-1] == 1 && node[n-2] == 3 && node[n-3] == 0) //lbs=r
  {
    n=n-3;
    node[n] = 2;
    Serial.println("R2");
    n++;
    return 1;
  }
  else if(node[n-1] == 0 && node[n-2] == 3 && node[n-3] == 2) //rbl=b
  {
    n=n-3;
    node[n] = 3;
    Serial.println("B3");
    n++;
    return 1;
  }
  else if(node[n-1] == 0 && node[n-2] == 3 && node[n-3] == 1) //sbl=r
  {
    n=n-3;
    node[n] = 2;
    Serial.println("R4");
    n++;
    return 1;
  }
  else if(node[n-1] == 1 && node[n-2] == 3 && node[n-3] == 1) //sbs=b
  {
    n=n-3;
    node[n] = 3;
    Serial.println("B5");
    n++;
    return 1;
  }
  else if(node[n-1] == 0 && node[n-2] == 3 && node[n-3] == 0) //lbl=s
  {
    n=n-3;
    node[n] = 1;
    Serial.println("S6"); 
    n++;
    return 1;
  }
  else
    return 0;
    Serial.println("storing failed");
}
int z=0;
void maze_solver(){
  v=200;
    readSensors();  
  
  if ((ir[0] && ir[1] && ir[2]) || (ir[5] && ir[7]) || 
      (ir[0] && ir[1] && ir[2] && ir[3] && ir[4] && ir[7]) ||
      (!ir[0] && !ir[1] &&  !ir[2] && !ir[3] && !ir[4] && !ir[5] &&  !ir[6] && !ir[7]))
  {
    // Serial.println("going for LSR");
    for(int i=0; i<8 ; i++)
       irr[i] = 0;
    t = millis();
    while (millis() - t < 60) {
      readSensors();
      for (int i = 0; i < 8; i++)
      {
        if (irr[i] == 0)
          irr[i] = ir[i];
      }
//      Serial.println("GG");
      normStraight();
    }

    //  LSR1();
    if(!digitalRead(ir8) && !digitalRead(ir7) && !digitalRead(ir6)){
      donereplay();
    }
    if(!digitalRead(ir8) && !digitalRead(ir7) && !digitalRead(ir6) && !digitalRead(ir4) && !digitalRead(ir3) && !digitalRead(ir1)){
      donereplay();
    }

    if(node[z]==0){
      t=millis();
      while(millis() - t <  8)
        speed_reduced();
      t = millis();
      while(millis()-t<50)
        //reverse();
      turnLeft();

    }
    else if(node[z]==2){
      t=millis();
      while(millis() - t <  8)
        speed_reduced();
              t = millis();
      while(millis()-t<50)
        //reverse();
      turnRight();
    }
    else{
      t = millis();
      while(millis() - t <50)
      straight();
    }
    z++;

  }
  //   // else {
  // else if(node[z]==1){
  //     z++;
  //    int t =millis();
  //    while(millis() - t < 100)
  //     straight();
  // }
  else{
    straight();
  }
  // }
  maze_solver();
}

void donereplay(){
  analogWrite(ML1, 0);  
  analogWrite(ML2, 0);
  analogWrite(MR1, 0);
  analogWrite(MR2, 0);
  digitalWrite(12,HIGH);
  donereplay();
}

void LSR() {  
  if (irr[7] && irr[6] && irr[5])
  { 
    t = millis();
//    done();
    while (millis() - t < 2){
      speed_reduced();
//      done();
    }
//    done();
//  t = millis();
//  while(millis() - t <90){
//    reverse();
//  }
//done();
    if (!digitalRead(ir4)==1 && !digitalRead(ir5)==1 && !digitalRead(ir6)==1 && !digitalRead(ir7)==1){
      done();
      delay(10000);
      digitalWrite(12,LOW);
      maze_solver();
//      Serial.println("going left and done");
    }

      node[n]=0;
      n++;
      if(n >= 3)
        check();
      Serial.println("L");
    turnLeft();
//    done();
  }

  else if (irr[0] || irr[1]) {    
//    Serial.println("11");
    if (!digitalRead(ir1)==1 || !digitalRead(ir2)==1 || !digitalRead(ir3)==1 ||
    !digitalRead(ir4)==1 || !digitalRead(ir5)==1 || !digitalRead(ir6)==1 || !digitalRead(ir7)==1 || !digitalRead(ir8)==1) {
//      Serial.println("22");
      // while(!digitalRead(ir1)==1 || !digitalRead(ir2)==1 || !digitalRead(ir3)==1)
      //   straight();

      t = millis();
      while(millis() - t <  3)
        speed_reduced();


    if ( digitalRead(ir2)==1 && digitalRead(ir3)==1 && digitalRead(ir4)==1 &&  digitalRead(ir6)==1 && digitalRead(ir7)==1)
        {
//                t = millis();
//      while(millis()-t<90){
//        reverse();
//      }
          node[n]=2;
          n++;
          if (n >= 3)
          check();
          Serial.println("R1");
          turnRight();
        }
      else{
      node[n]=1;
      n++;
      if(n >= 3)
        check();
      Serial.println("S");
      }
    }
    else {
      t=millis();
      while (millis() - t < 3)
        speed_reduced();
//      t = millis();
//      while(millis()-t<90){
//        reverse();
//      }
      node[n] = 2;
      n++;
      if(n >= 3)
        check();
      Serial.println("R");
      turnRight();
    }
  }
  else if (!irr[0] && !irr[1] &&  !irr[2] && !irr[3] && !irr[4] && !irr[5] &&  !irr[6] && !irr[7]) {
    node[n]=3;
    n++;
    if(n >= 3)
        check();
    t=millis();
    while (millis() - t < 3 ){
        speed_reduced(); 
    }
    
    Serial.println("B");
    turnRight();
  }
  v=200;
}

void done() {
  for(int i=0; i<n; i++){
    Serial.print(node[i]);
    Serial.print(" ");
  }
  Serial.print("######COMPLETE#######");
  analogWrite(ML1, 0);
  analogWrite(ML2, 0);
  analogWrite(MR1, 0);
  analogWrite(MR2, 0);
  digitalWrite(12,HIGH);
  delay(5000);
}


void straight() {
  prev_error = avg_error;
  for (int i = 0; i < 8; i++)
  {
    if (ir[i])
      sensors++;
    error += ir[i] * weights[7 - i];
  }
  avg_error = error / sensors;
  sensors = 0; error = 0;
  output = Kp * avg_error + Kd * (avg_error - prev_error);

  leftmot = v + output;
  rightmot = v - output;
  leftmot = constrain(leftmot, 0, 255);
  rightmot = constrain(rightmot, 0, 255);
  analogWrite(ML1, leftmot);
  analogWrite(ML2, LOW);
  analogWrite(MR1, rightmot);
  analogWrite(MR2, LOW);
  output = 0;
}

void turnLeft() {
  int m = 10;
  while ( digitalRead(ir5)==0 || digitalRead(ir4)==0 )
  {
    if(m>130)
      m=130;
    analogWrite(ML1, 0);
    analogWrite(ML2, m);
    analogWrite(MR1, m);
    analogWrite(MR2, 0);
    m+=20;
    delay(10);
  }

  while (digitalRead(ir5)==1 && digitalRead(ir4)==1 ) {  
    if(m>130)
      m=130;      
    analogWrite(ML1, 0);
    analogWrite(ML2, m);
    analogWrite(MR1, m);
    analogWrite(MR2, 0);
    m+=20;
    delay(10);
  }
  analogWrite(ML1, 130);  
  analogWrite(ML2, 0);
  analogWrite(MR1, 0);
  analogWrite(MR2, 130);
  delay(40);
  //done();
  while(millis() - t < 10){
       v=0;
    speed_increased();
  }
  v=200;
//  done();

}


void turnRight() {
  int m =10;
  while (digitalRead(ir5)==0 || digitalRead(ir4)==0) {
        if(m>130)
      m=130; 
//    Serial.println("1st");
    analogWrite(ML1, m);
    analogWrite(ML2, 0);
    analogWrite(MR1, 0);
    analogWrite(MR2, m);
    m+=20;
    delay(8);
  }

  while ( digitalRead(ir4)==1 && digitalRead(ir5)==1 /*&& digitalRead(ir2)*/) {
        if(m>130)
      m=130; 
//    Serial.println("2nd");
    analogWrite(ML1, m);
    analogWrite(ML2, 0);
    analogWrite(MR1, 0);
    analogWrite(MR2, m);
    m+=20;
    delay(10);
  }
  analogWrite(ML1, 0);
  analogWrite(ML2, 130);
  analogWrite(MR1, 130);
  analogWrite(MR2, 0);
  delay(45);
  int t = millis();

  while(millis() - t < 10){
    v=0;
    speed_increased();
  }
  v=200;
//  done();
}

void turnAround() {
  int m=10;
while (digitalRead(ir5)==1 && digitalRead(ir4)==1 /*&& digitalRead(ir6) && digitalRead(ir7)*/) {
      if(m>130)
      m=130; 
//    Serial.println("1st");
    analogWrite(ML1, m);
    analogWrite(ML2, 0);
    analogWrite(MR1, 0);
    analogWrite(MR2, m);
    m+=10;
    delay(10);
  }

  analogWrite(ML1, 0);  
  analogWrite(ML2, 130);
  analogWrite(MR1, 130);
  analogWrite(MR2, 0);
  delay(42);

  int t = millis();
  while(millis() - t < 20){
        v=0;
    speed_increased();
  }
  v=200;
}
void speed_reduced(){
  while(v>=0){
    v-=60;
    straight();
    delay(0.1);
}
//  v=200;
}
void speed_increased(){
  while(v<=200){
    v+=75;
    straight();
    delay(3);
  }
    v=0;
}
 
void stopVehicle(){
    analogWrite(ML1, 0);  
  analogWrite(ML2, 0);
  analogWrite(MR1, 0);
  analogWrite(MR2, 0);
}
void normStraight(){
    analogWrite(ML1, 200);
  analogWrite(ML2, LOW);
  analogWrite(MR1, 200);
  analogWrite(MR2, LOW);
  
}
void reverse(){
  analogWrite(ML1, 0);  
  analogWrite(ML2, 200);
  analogWrite(MR1, 0);
  analogWrite(MR2, 200);
  
}
