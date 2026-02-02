#include "actions_complexes.h"

void suivre_courbure(void) {
  int mGaucheIR2_A, mGaucheIR2_R, mDroitIR5_A, mDroitIR5_R;
  
  float vitesseIR2 = analogRead(PIN_IR2)*1.02-663; // 500 -153
  float vitesseIR5 = analogRead(PIN_IR5)*1.02-663; // 700 51

  // Bloquer les valeurs entre -255 et 255
  if (vitesseIR2 < -255) vitesseIR2 = -255;
  if (vitesseIR2 > 255) vitesseIR2 = 255;
  if (vitesseIR5 < -255) vitesseIR5 = -255;
  if (vitesseIR5 > 255) vitesseIR5 = 255;
  
  int erreur = vitesseIR2 - vitesseIR5; // Si <0, il faut tourner à gauche, si = 0, tout droit, si >0 doit tourner à droite

  vitesseIR2 = 255 - erreur * 0.5; // car 255 - (-255) = 510, et on veut 255 max, donc on fait un prduit en croix : 255/510 = 0,5
  vitesseIR5 = 255 + erreur * 0.5; // pareil ici, si on a 255+255 ça marche pas
  
  // Test valeur positive/negative IR2
  if (vitesseIR2 >=0) {
    mGaucheIR2_A = vitesseIR2;
    mGaucheIR2_R = 0;
  } else {
    mGaucheIR2_A = 0;
    mGaucheIR2_R = -vitesseIR2;
   }
   // Test valeur positive/negative IR5
  if (vitesseIR5 >=0) {
    mDroitIR5_A = vitesseIR5;
    mDroitIR5_R = 0;
  } else {
    mDroitIR5_A = 0;
    mDroitIR5_R = -vitesseIR5;
   }

   analogWrite(PIN_M_GAUCHE_A, mGaucheIR2_A);
   analogWrite(PIN_M_GAUCHE_R, mGaucheIR2_R);
   analogWrite(PIN_M_DROIT_A, mDroitIR5_A);
   analogWrite(PIN_M_DROIT_R, mDroitIR5_R);
}
