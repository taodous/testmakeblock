#include <Arduino.h>
#include <MeMCore.h>
#include <SoftwareSerial.h>
#include <MeIR.h>

MeDCMotor motor1(M1);
MeDCMotor motor2(M2);
MeLEDMatrix ledMx(PORT_4);
// detecteur de distance
MeUltrasonicSensor ultraSensor(PORT_3); 
//telecommande 
MeIR ir = MeIR();

//vitesse du moteur
uint8_t speed = 250;

//vitesse du moteur
uint8_t motorSpeed = 0;

//variable utilisee pour lire le code de la touche
uint32_t value;



void setup()
{
  //Initialisation de la matrice LED
  ledMx.setBrightness(6);
  ledMx.setColorIndex(5);

  //Initialisation du port serie pour pouvoir afficher des infos dans le serial monitor
  Serial.begin(9600);

  //Initialisation du recepteur infra rouge de la telecommande
  ir.begin();
  
  //On commence moteurs eteints
  motorSpeed = 0;
  

}


void loop()
{

  //si une touche de la telecomamnde est pressée
  if (ir.decode()) {
    //recuperation de la touche
    value = ir.value;
    //Affichage de la valeur du code de la touche pressée
    Serial.println(value);
  //Si la touche 0 est pressee, arret des moteurs 
  if (value == 3910598400)
    motorSpeed = 0;

  //Si la touche 1 est pressee, demarage des moteurs 
  if (value == 4077715200)
    motorSpeed = speed;
  //si la touche fleche haut est pressee, augmenter la vitesse de 10 MAIS la bloquer a 250
  if (value == 3208707840){
    speed = speed + 10;
    if (speed > 250)
      speed = 250;
    motorSpeed = speed;
  }
  //si la touche fleche bas est pressee, reduire la vitesse de 10 MAIS la bloquer a 10
  if (value == 3860463360){
    speed = speed - 10;
    if (speed < 0)
      speed = 0;
    motorSpeed = speed;
  }
  
    motor2.run(motorSpeed);
  } 

  //Affichage sur la matrice LED de la distance en cm
  ledMx.showNum(ultraSensor.distanceCm());
 
  //Mise a jour de la vitesse des moteurs
  motor1.run(-motorSpeed);
  motor2.run(motorSpeed);

  //Demi-tour pendant 1000ms si on detecte un obstacle a moins de 40
  if (ultraSensor.distanceCm() < 40 )
  {
    motor1.run(motorSpeed);
    motor2.run(motorSpeed);
    //motor1.run(motorSpeed);
    //motor2.run(motorSpeed);
    //ledMx.drawStr(0,7,">>>");
    //delay(750);
    //motor1.run(-motorSpeed);
    //motor2.run(motorSpeed);

  while (ultraSensor.distanceCm()<40)//si la distance est inferieure a 40 cm alors tourner 
  {
       ledMx.drawStr(0,7,">>>");
  }

  }
   }   

  
  

