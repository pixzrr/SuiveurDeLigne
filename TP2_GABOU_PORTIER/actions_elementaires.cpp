#include "actions_elementaires.h"

void attendre_START(void){
  bool buttonState;
  //vérifier ou attendre que le bouton ne soit pas appuyé
  do{
    buttonState = digitalRead(PIN_BP);
  }while(buttonState != 0);
  Serial.println("En attente...");
  do{
    buttonState = digitalRead(PIN_BP);
  }while(buttonState != 1);
  Serial.println("Début");
}

void affiche_tention_batterie(void){
  float valeur = analogRead(PIN_TENSION_REDUITE_BATTERIE)*5.0/1023.0;
  float t9v6 = valeur*2.8;
  Serial.println(analogRead(PIN_TENSION_REDUITE_BATTERIE));
  Serial.println("Tension reduite : ");
  Serial.println(valeur,2);
  Serial.println("Tension 9V6 : ");
  Serial.println(t9v6);
  
}
