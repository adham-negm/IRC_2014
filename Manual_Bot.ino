
#include <PS2X_lib.h>  //for v1.6
PS2X ps2x;
/*
5 3 6 9
4 2 7 8
*/
#define motor2en 5
#define motor1en 3
#define motor4en 6
#define motor3en 9

#define motor2_1 4

#define motor1_1 2


#define motor4_1 7


#define motor3_1 8




int error = 0; 
byte type = 0;
byte vibrate = 0;



boolean lfs,rfs,rrs,lrs;
int lf,lr,rr,rf;
int x,y,rotate;
boolean mode =1 ;
boolean notmode=0;


class MOTORM {
  public:
  void attach( byte, byte);
  void write(boolean,byte);
  void stop();
  
  private:
    byte enm;
   byte in1m;
   boolean statem;
   boolean pstatem;
   
     
}motor1,motor2,motor3,motor4;




void setup(){
 
   Serial.begin(57600);
 error = ps2x.config_gamepad(12,11,13,10, true, false);   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
   
   if(error == 0){   Serial.println("Found Controller, configured successful");
   Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
  Serial.println("holding L1 or R1 will print out the analog stick values.");
  Serial.println("Go to www.billporter.info for updates and to report bugs.");
 }
   else if(error == 1)
   Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
   Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");
   
  else if(error == 3)
   Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
   
   //Serial.print(ps2x.Analog(1), HEX);
   
   type = ps2x.readType(); 
     switch(type) {
       case 0:
        Serial.println("Unknown Controller type");
       break;
       case 1:
        Serial.println("DualShock Controller Found");
       break;
       case 2:
         Serial.println("GuitarHero Controller Found");
       break;
     }


motor1.attach(motor1en,motor1_1);
motor2.attach(motor2en,motor2_1);
motor3.attach(motor3en,motor3_1);
motor4.attach(motor4en,motor4_1);


}

void loop()
{
 
   ps2x.read_gamepad();           
  if(ps2x.ButtonPressed(PSB_SELECT))
  {
    mode=!mode;
    notmode=!notmode;
  }
      
     if(ps2x.Button(PSB_PAD_UP)) { 
       int UP = map(ps2x.Button(PSB_PAD_UP),0,255,50,150);      
       motor1.write(mode,200);
       motor2.write(mode,150);
       motor3.write(mode,150);
       motor4.write(mode,150); 
       delay(25);     
     }
     
      if(ps2x.Button(PSB_PAD_LEFT))
       {
        int RIGHT = map(ps2x.Button(PSB_PAD_LEFT),0,255,100,200);      
     
        motor1.write(mode,150);
        motor2.write(notmode,100);
        motor3.write(notmode,100);
        motor4.write(mode,100);
        delay(25);
      }
      
      if(ps2x.Button(PSB_PAD_RIGHT)){
        
            int LEFT = map(ps2x.Button(PSB_PAD_RIGHT),0,255,50,150);  
         motor1.write(notmode,200);
        motor2.write(mode,150);
        motor3.write(mode,150);
        motor4.write(notmode,150);
       delay(25);
       
      }
      if(ps2x.Button(PSB_PAD_DOWN)){
        
        int DOWN= map(ps2x.Button(PSB_PAD_DOWN),0,255,50,150);  
       motor1.write(notmode,150);
       motor2.write(notmode,100);
       motor3.write(notmode,100);
       motor4.write(notmode,100);
       delay(25);
   }   
  if(ps2x.Button(PSB_BLUE))
  {
    motor1.stop();
    motor2.stop();
    motor3.stop();
    motor4.stop();
  }

    if(ps2x.Button(PSB_L1)) // print stick values if either is TRUE
    {
      
  
   x= ps2x.Analog(PSS_LX)-128;
   y= (255-ps2x.Analog(PSS_LY))-127;
  
   rotate=(ps2x.Analog(PSS_RX))-128;

   lf=y+x+rotate;
if(lf >=0 )   lfs= 1 ; else  lfs =0;

   lr=y-x+rotate;
   if(lr >=0 )   lrs= 1 ;else  lrs =0;
   rf=y-x-rotate;
if(rf >=0 )   rfs= 1 ;else  rfs =0;
   rr=y+x-rotate;
if(rr >=0 )   rrs= 1 ;else rrs =0;

  // normalize wheel throttles
  int maximum = max(max(abs(lf), abs(rf)), max(abs(lr), abs(rr)));
  if (maximum > 127) {
    lf = (lf / maximum) * 127;
    rf = (rf / maximum) * 127;
    lr = (lr / maximum) * 127;
    rr = (rr / maximum) * 127;
      

  }
   if( rf ==0 && lf==0 && lr== 0 && rr==0)
   {

     motor1.stop();
     motor2.stop();
     motor3.stop();
     motor4.stop();
     

    
   }
   else {
        
     if(lf !=0)
     {
  lf=abs(lf)+127;
     }
  if(rf !=0)
  {
  rf=abs(rf)+127;
  }
  if(lr !=0)
  {
  lr=abs(lr)+127;
  }
  if(rr != 0)
  {
  rr=abs(rr)+127;
  }
  
  motor2.write(rfs,abs(rf));
  motor4.write(rrs,abs(rr));
  motor1.write(lfs,abs(lf));
  motor3.write(lrs,abs(lr));
  
     
   }
  
 
  /*
        Serial.print("Stick Values:");
        Serial.print(rfs, DEC); //Left stick, Y axis. Other options: LX, RY, RX  
        Serial.print(",");
        Serial.print(rrs, DEC); 
        Serial.print(",");
        Serial.print(lfs, DEC); 
        Serial.print(",");
        Serial.println(lrs, DEC); 
        
        Serial.print("Stick Values:");
        Serial.print(rf, DEC); //Left stick, Y axis. Other options: LX, RY, RX  
        Serial.print(",");
        Serial.print(rr, DEC); 
        Serial.print(",");
        Serial.print(lf, DEC); 
        Serial.print(",");
        Serial.println(lr, DEC);

 */
      
     delay(20);
    } 

      
      
    delay(50);
 }
 
  



void MOTORM::attach( byte MOen, byte MO1 )
{
  
 MOTORM::enm=MOen;
 MOTORM::in1m=MO1;
 
 
  pinMode(MOen,OUTPUT);
  pinMode(MO1,OUTPUT);
  pstatem=0;
  statem=0;
  
}



void MOTORM::write(boolean MOdir,byte MOspeed)
{ 
  
      MOTORM::statem=MOdir ;
      
      if(statem!=pstatem)
      {
  analogWrite(enm,0);
  digitalWrite(in1m,HIGH);
  
  delay(20);
  //Serial.println("here");
      }
      
 MOspeed=constrain(MOspeed,0,254);
  analogWrite(enm,MOspeed);
  digitalWrite(in1m,MOdir);
 
  pstatem=statem ;
}

void MOTORM::stop()
{
  analogWrite(enm,0);
  digitalWrite(in1m,LOW);
 
}

