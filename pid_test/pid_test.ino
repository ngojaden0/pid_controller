#include <math.h>
#include "readPinFast.h"
#include "enc_R.h"
#include "mot_R.h"

float kp = 300;
float kd = 0;
float ki = 100;
float e, dedt, eintegral = 0, eprev = 0;
float target = 0.0;
float u, pwr;
long currT, prevT;
float deltaT, encR, old_encR = 0, angular_vel_encR=0;

void setup() 
{
  Serial.begin(9600);
  motorRSetup();
  encRSetup();
}

void loop() 
{
  currT = micros();
  deltaT = ((float)(currT-prevT))/1.0e6;
  prevT = currT;

  encR = getEncR();
  angular_vel_encR = abs(encR-old_encR)/deltaT/2048.0*2*M_PI;
  if(encR < old_encR)
    angular_vel_encR = -1*angular_vel_encR;

  e = angular_vel_encR - target;
  dedt = (e-eprev)/deltaT;
  eintegral = eintegral + e*deltaT;

  u = kp*e + kd*dedt + ki*eintegral;
  
  pwr = fabs(u);
  pwr = constrain(pwr, 0, 255);

  if(u > 0)
  {
    digitalWrite(motAdir, HIGH);
  }
  else if(u < 0)
  {
    digitalWrite(motAdir, LOW);
  }
  analogWrite(motApwm, pwr);
  eprev = e;
  old_encR = encR;


  Serial.print(target);
  Serial.print(" ");
  Serial.print(angular_vel_encR);
  //Serial.print(" ");
  //Serial.print(pwr);
  Serial.println();
}
