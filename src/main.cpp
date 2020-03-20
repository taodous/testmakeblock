#include <Arduino.h>
#include <MeMCore.h>
#include <SoftwareSerial.h>
#include <MeIR.h>
#include <stdio.h>      
#include <stdlib.h>  
#include <time.h>



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
 
//Sera utilisé pour faire un random 0 ou 1 pour choisir le sens de demi-tour
int sens;



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


  // --------------------------------
  // -- TELECOMMANDE ----------------
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
  }
//--------------------- FIN TELECOMMANDE


  //Affichage sur la matrice LED de la distance en cm
  ledMx.showNum(ultraSensor.distanceCm());
 
  //Mise a jour de la vitesse des moteurs

  //Mise a jour a chaque passage dans loop de la vitesse des moteurs
  motor1.run(-motorSpeed);
  motor2.run(motorSpeed);

  //Demi-tour pendant 1000ms si on detecte un obstacle a moins de 40
  if (ultraSensor.distanceCm() < 40 )
  {
    //nombre aleatoire 0 ou 1
    sens = rand()%2;
    if (sens == 0){
      motor1.run(motorSpeed);
      motor2.run(motorSpeed);

    }
    else {
      motor1.run(-motorSpeed);
      motor2.run(-motorSpeed);
    }

    //motor1.run(motorSpeed);
    //motor2.run(motorSpeed);
    //ledMx.drawStr(0,7,">>>");
    //delay(750);
    //motor1.run(-motorSpeed);
    //motor2.run(motorSpeed);


    //On va demarrer une boucle tant que on est a moins de 40 cm
    //On prends un chrono avant le demarrage pour pouvoir mesurer le temps
    unsigned long int timeZero = millis();
    //timer contient maintenant le temps actuel (TOP CHRONO)

    while (ultraSensor.distanceCm()<40)
    {

      double duration = (millis() - timeZero) / 1000;
      Serial.println("Duration: ");
      Serial.println(duration);

      if (sens == 0){
        ledMx.drawStr(0,7,">>>"); 
      }
      else {
        ledMx.drawStr(0,7,"<<<");
      }

      //Si on est face a un obstacle pendant moins de 5 secondes
      if (duration < 5 )
      {
        if (sens == 0){
          //Demi-tour sens 0
          motor1.run(motorSpeed);
          motor2.run(motorSpeed);
        }
        else {
          //Demi-tour sens 1
          motor1.run(-motorSpeed);
          motor2.run(-motorSpeed);
        }
      }
      else //Si on est face a un obstacle depuis plus ou egal de 5 secondes
      {
        //Marche arriere pendant deux secondes
        motor1.run(motorSpeed);
        motor2.run(-motorSpeed);
        delay(2000);
        motorSpeed = 0;
        break ;
      }
    }//Fin while si on est a moins de 40 cm
  }//Fin if on est moins de 40 cm
}//Fin loop

  
  

