//   17/11/2013
//input array and the goal node are insered manually, code calculate path using flood fill algorithm
//calling the (go to point) function the robot moves to the specified node 
//(Made In Alex team) 
//https://www.facebook.com/made.in.alex.MIA 


#define NUM_SENSORS             8  // number of sensors used
#define NUM_SAMPLES_PER_SENSOR  4  // average 4 analog samples per sensor reading
#define INTERSECTION_CONDITION (sensorValues[0] <900 &&sensorValues[7]<200) || (sensorValues[1]<200&&sensorValues[7]<200) || (sensorValues[0]<200 &&sensorValues[6]<200 )|| (sensorValues[1]<200 &&sensorValues[6]<200 ) 
#define NODE_CONDITION  ((sensorValues[2] >500 &&sensorValues[3] >500 )||(sensorValues[3] >500 && sensorValues[4] > 500))
#include <QTRSensors.h>
#include <PID_v1.h>
#include <floodfill.h>

    #define motorL_s 6
    #define motorL_1 2  
    #define motorL_2 3
    #define motorR_s 7 
    #define motorR_1 4
    #define motorR_2 5
    
   
class MOTOR {
public:
  void attach( byte, byte, byte);
  void write(int);
  void stop();


private:
  byte en;
  byte in1;
  byte in2;
  boolean state;
  boolean pstate;

}
motorR,motorL;

QTRSensorsAnalog qtra((unsigned char[]) {0, 1, 2, 3, 4, 5,6,7}, NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, QTR_NO_EMITTER_PIN); //creating new object of QTR class 
unsigned int sensorValues[NUM_SENSORS];//array that contain the values of the reading of the sensors
double Setpoint, Input, Output; //global variables used in PID control  .015,.0002,0.0035,
PID myPID(&Input, &Output, &Setpoint,.05,0.0002,0.007, DIRECT); //creating object of PID class
unsigned int minimum[8] = {752 ,746 , 643,582 ,627 ,696 ,603 ,699};
unsigned int maximum[8] = {1007 ,999, 1001 ,996 ,999 ,1005, 1000, 1005};

unsigned int minimum1[8] = { 114 ,47 ,56, 47 ,50, 96, 106, 397  };        

unsigned int maximum1[8] = {983 ,971, 977 ,970 ,970 ,983 ,983 ,996 };  

//22,24,26,28,30,32,34,36 leds pins
int minMoves =0;
IJ output_array[16];
int t,m;
int goGoal=0;
int gG,i=90;
int x=0;
int y = 0;
int counter=0;
int OutR = 0;
int OutL = 0;
int sensorValue[8];
int x_axis=0;
int y_axis=-1;
int flag_flood=0;
int inputarr[4][5]={{0,0,0,0,0},
		    {0,-1,0,-1,0},
                    {0,0,0,-1,0},
                    {0,0,0,0,0}};
                    
int left_inc=0,right_inc=0;

void setup()
{
   Serial.begin(57600);
    Serial.println("hopa bsmallah");
    myPID.SetMode(AUTOMATIC);
    qtra.calibratedMinimumOn = minimum;
    qtra.calibratedMaximumOn = maximum;
    motorR.attach(motorR_s,motorR_1,motorR_2);
    motorL.attach(motorL_s,motorL_1,motorL_2);
    myPID.SetOutputLimits(-60,60);
    Setpoint = 3650; // banner field
   // field 5shb kant 3550
    myPID.SetSampleTime(70);
    while(goGoal == 0) 
      goGoal = track_line();        
    goGoal =0; 
    
  floodfill test;
  test.initialize(inputarr,output_array);
  minMoves = test.compute(3,3,0,0); 

  for(int h = 0 ; h < minMoves ; h++)   
    gotopoint(output_array[h].i , output_array[h].j , output_array[h+1].i , output_array[h+1].j);   
}

void loop()
{  
  //dry_run();
 // track_line(); 
  
//
         
}

void MOTOR::attach( byte MOen, byte MO1, byte MO2 )
{
  MOTOR::en=MOen;
  MOTOR::in1=MO1;
  MOTOR::in2=MO2;

  pinMode(MOen,OUTPUT);
  pinMode(MO1,OUTPUT);
  pinMode(MO2,OUTPUT);
  pstate=0;
  state=0;
}

void MOTOR::write(int MOspeed)
{       
  if(MOspeed >0)
  { 
    MOTOR::state=1;
    analogWrite(en,MOspeed);
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
  }
      else if(MOspeed<0)
      { 
        MOTOR::state=0;
        analogWrite(en,abs(MOspeed));
        digitalWrite(in1,LOW);
        digitalWrite(in2,HIGH);
      }
      else
      {
        digitalWrite(in1,HIGH);
        digitalWrite(in2,HIGH);
        analogWrite(en,255);
      }
}

void MOTOR::stop()
{ 
  analogWrite(en,255);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,HIGH);
} 

boolean track_line()
{
  boolean rg3=0;
   unsigned int position = qtra.readLine(sensorValues,QTR_EMITTERS_ON,1);
   Input = position;
   myPID.Compute();  
   if(abs(Output)<7)
  {
   motorR.write(120);
   motorL.write(160);
  }
     else
     {
        OutR = 120 - Output;
        OutL = 140+ Output;
        motorR.write(OutR);
        motorL.write(OutL);
      }
   qtra.calibratedMinimumOn = minimum1;//minimum1;
   qtra.calibratedMaximumOn = maximum1;//maximum1;
   qtra.readLine(sensorValues,QTR_EMITTERS_ON,1);
   
  if(INTERSECTION_CONDITION)    
  {        //Serial.println(bNodes);
  
    if(x==0)
    {  
     x=1; 
     counter++;
     rg3=1;
       if (flag_flood==0)
         Upgrade();
     
     if(NODE_CONDITION)
    {  
      inputarr[x_axis][y_axis]=-1;
      motorR.stop();
      motorL.stop();
      delay(1000);
      
        if( y == 0)
        {   //Serial.println("node"); 
          motorR.stop();
          motorL.stop();
          y =  1;
          delay(1000);
        }
            else
            {
              y = 0 ;
              //Serial.println("node"); 
            }
    }
       // inputarr[x_axis][y_axis] = rg3;
      Serial.print("X:: ");
      Serial.print(x_axis);
      Serial.print(" Y:: ");
      Serial.print(y_axis);
      Serial.println("");
      Serial.print("left increment:  ");
      Serial.print(left_inc);
      Serial.print(" right increment:  ");
      Serial.print(right_inc);
      Serial.println("");
      Serial.print("Counter:   ");
      Serial.print(counter);
      Serial.println("");

          for(t=0;t<4;t++)
          {
            for(m=0;m<5;m++)
            { 
            Serial.print(inputarr[t][m]);
            Serial.print(" ");
            }
          Serial.println(""); 
          }  
     Serial.println("");     
     Serial.println("new reading");

      motorR.stop();
      motorL.stop();
    }
   
   motorR.write(120);
   motorL.write(160);
   delay(40);
    }
        else
        {
        x=0;
        rg3=0;
        qtra.calibratedMinimumOn = minimum;
        qtra.calibratedMaximumOn = maximum;
        }
        return rg3;
}

void Upgrade()
{
      if(right_inc==0)
      y_axis++;
      
      if(right_inc==1)
      x_axis++;
      
      if(right_inc==2&&left_inc==0)
      y_axis--;
      
      if(right_inc==2&&left_inc==1)
      x_axis++;
      
      if(right_inc==2&&left_inc==2)
      y_axis++;
      
      if(right_inc==3&&left_inc==2)
      x_axis++;
      
      if(right_inc==4&&left_inc==2)
      y_axis--;
}

void rotateL()
{
//attachInterrupt(2,rotate_count,RISING)
motorR.stop();
motorL.stop();
left_inc++;
motorR.write(100);
motorL.write(-120);
delay(800);
motorR.stop();
motorL.stop();
}

void rotateR()
{
//attachInterrupt(2,rotate_count,RISING)
motorR.stop();
motorL.stop();
right_inc++;
motorR.write(-120);
motorL.write(100);
delay(800);
motorR.stop();
motorL.stop();
}

void dry_run()
{  
  if((counter==5||counter==7||counter==19||counter==21))
    {
      rotateR();
      counter++;
    }
  
if((counter==12||counter==14||counter==36||counter==38||counter==26||counter==27))
    {
    rotateL();
    if(counter==26){
    motorR.stop();
  motorL.stop();
  delay(500);
  //flood_fill();
  flag_flood=1;     
    
    }
    counter++;
    }
  
 if(counter==31||counter==32||counter==30||counter==29)
    { 
    motorR.write(OutR+80);
    motorL.write(OutL+80);
     digitalWrite(22,HIGH);
    }
       
if(counter==40||counter==33)
{

 motorR.stop();
  motorL.stop();
  delay(500);
  digitalWrite(22,HIGH);
  digitalWrite(24,HIGH);
  digitalWrite(26,HIGH);
  digitalWrite(28,HIGH);
  digitalWrite(30,HIGH);
  digitalWrite(32,HIGH);
  digitalWrite(34,HIGH);
  digitalWrite(36,HIGH);
  }
} 

/*
void flood_fill(){

int minMoves =0;

IJ output_array[16];
 int i,j;
floodfill test;
  test.initialize(inputarr,output_array);
  minMoves = test.compute(3,4,0,0);
  
  for( i = 0 ; i < 4 ; i ++)
    for(  j = 0 ; j < 5 ;j++)
	{
	  //Serial.print (setw(4));
          Serial.print (inputarr[i][j]);
	  if(j == 4)
	  Serial.println("");
	}
	Serial.println("MIAFTW");
	for(int i = 0 ; i <= minMoves ; i ++)
	{
          //print (setw(2));
          Serial.print((int)(output_array[i].i));
          //Serial.print(setw(2));
          Serial.print((int)(output_array[i].j));
          Serial.println("");
	}

}
*/

      void gotopoint(int x1,int y1,int x2,int y2)
    {
        if(x2>x1)
        {

    if(orientation()==180)
    {
    rotateR();
    rotateR(); 

    }
    if(orientation()==270)
    {
    rotateL();

    }
    if (orientation()==90)
    {
    rotateR();
    }
    for(gG=x1;gG<x2;gG++){
 
   while(goGoal == 0)
{   
goGoal = track_line();
}
goGoal =0;
    }

    x1=x2;

    }
    if(y2>y1)
    {
    if(orientation()==0)
    {
    rotateL();
    }
    if(orientation()==180)
    {
    rotateR();
    }
    if(orientation()==270)
    {
    rotateL();
    rotateL(); 
    }

    for(gG=y1;gG<y2;gG++)
       {
            while(goGoal == 0)
{   
goGoal = track_line();
}
goGoal =0;
}
    y1=y2;
    }
                 if(y2<y1)
                {
     if(orientation()==0)
    {
    rotateR();
    }
    if(orientation()==90)
    {
    rotateL();
    rotateL();
    }
    if(orientation()==180)
    {
    rotateL();
    }
    for(gG=y2;gG<y1;gG++)
                            {
                              
                                 while(goGoal == 0)
{   
goGoal = track_line();
}
goGoal =0;
                            }

    y1=y2;              }
         if(x2<x1)
        {
           if(orientation()==0)
    {
        rotateL();
    rotateL();
    }
    if(orientation()==90)
    {
    rotateL();
    }
    if(orientation()==270)
    {
    rotateR();

    }
      for(gG=x2;gG<x1;gG++)
                    {
                         while(goGoal == 0)
{   
goGoal = track_line();
}
goGoal = 0;
                    }
    x1=x2;
    }
    motorR.stop();
    motorL.stop();

	}



int orientation()
{
 
int wegha = right_inc - left_inc;
int zawya ;  // a7a ma3reftaksh ana keda
if (wegha>=0)
{
 if (wegha%4==0) //up
 zawya=90;
 
 if (wegha%4==1) //right
 zawya=0;
 
 if (wegha%4==2) //down
 zawya=270;
 
 if (wegha%4==3) //left
 zawya=180;
}

if (wegha<0)
{
if (abs (wegha)%4==0) //up
zawya=90;

if (abs (wegha)%4==1)  // left
zawya=180;

if (abs (wegha)%4==2)  // down
zawya=270;

if (abs (wegha)%4==3)  //right
zawya=0;
}
  
return (zawya);

//following section is added for the function to be compatible with the cube finding function coded by Ahmed Yousef
//if zawya==90
//{vertical=true;
//up=true;
//}
//
//if zawya==0
//{vertical=false;
//right=true;
//}
//
//if zawya==270
//{vertical=true;
//dowan=true;
//}
//
//if zawya==180
//{vertical=false;
//left=true;
//}

}
