#include <Arduino.h>
#include <MeMCore.h>
#include <SoftwareSerial.h>
#include <MeIR.h>

MeDCMotor motor1(M1);
MeDCMotor motor2(M2);
MeLEDMatrix ledMx(PORT_4);
MeUltrasonicSensor ultraSensor(PORT_3); 

MeIR ir = MeIR();

MeLightSensor lightSensor(PORT_6);
int value = 0;      // a variable for the lightSensor's value

uint8_t motorSpeed = 255;
uint8_t motorSpeed2 = 20;




void setup()
{
  //Initialisation de la matrice LED
  ledMx.setBrightness(6);
  ledMx.setColorIndex(1);

  //Initialisation du port serie pour pouvoir afficher des infos dans le serial monitor
  Serial.begin(9600);

  //Initialisation du recepteur infra rouge de la telecommande
  ir.begin();
  
  //On commence moteurs eteints
  motorSpeed = 0;
  motorSpeed2 = 0;

}


void loop()
{

  //si une touche de la telecomamnde est pressée
  if (ir.decode()) {
    uint32_t value = ir.value;
    //Affichage de la valeur du code de la touche pressée
    Serial.println(value,HEX);
  }

  //Affichage sur la matrice LED de la distance en cm
  ledMx.showNum(ultraSensor.distanceCm());
 
  //Mise a jour de la vitesse des moteurs
  motor1.run(-motorSpeed);
  motor2.run(motorSpeed);


  //Demi-tour pendant 1000ms si on detecte un obstacle a moins de 40
  if (ultraSensor.distanceCm() < 40 )
  {
    motor1.run(-motorSpeed);
    motor2.run(motorSpeed);
    ledMx.drawStr(0,7,"<<<");
    delay(1000);
    motor1.run(-motorSpeed);
    motor2.run(motorSpeed2);
  } 
    

}
