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

void suivre_courbure(void) {
  int vitesseIR2, vitesseIR5; // on s'en fou du float, on est pas à ça près
  float rapport = 7.97;
  vitesseIR2 = analogRead(PIN_IR2)/rapport;
  vitesseIR5 = analogRead(PIN_IR2)/rapport;

  analogWrite(PIN_M_GAUCHE_A
}
