#include "actions_complexes.h"

void suivre_courbure(void) {
  float vitesseIR2 = analogRead(PIN_IR2)*1.02-663;
  float vitesseIR5 = analogRead(PIN_IR5)*1.02-663;
  float AvancerIR2, ReculerIR2, AvancerIR5, ReculerIR5;

  // Test IR2
  if (vitesseIR2 >=0) {
    AvancerIR2 = vitesseIR2;
    ReculerIR2 = 0;
  } else {
    AvancerIR2 = 0;
    ReculerIR2 = -vitesseIR2;
   }
   // Test IR5
   if (vitesseIR5 >=0) {
    AvancerIR5 = vitesseIR5;
    ReculerIR5 = 0;
  } else {
    AvancerIR5 = 0;
    ReculerIR5 = -vitesseIR5;
   }
   analogWrite(PIN_M_GAUCHE_A, AvancerIR2);
   analogWrite(PIN_M_GAUCHE_R, ReculerIR2);
   analogWrite(PIN_M_DROIT_A, AvancerIR5);
   analogWrite(PIN_M_DROIT_R, ReculerIR5);
}
