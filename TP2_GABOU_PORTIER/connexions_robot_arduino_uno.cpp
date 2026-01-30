#include "connexions_robot_arduino_uno.h"

void init_pin(void){    
  pinMode(PIN_LED, OUTPUT); // initialize digital pin LED_BUILTIN as an output.  
  pinMode(PIN_BP, INPUT); // make the pushbutton's pin an input.

  pinMode(PIN_TENSION_REDUITE_BATTERIE, INPUT); // Met le pin de tension reduite de la batterie en entrée
  
  // Gestion capteurs
  pinMode(PIN_IR1, INPUT);
  pinMode(PIN_IR2, INPUT);
  pinMode(PIN_IR3, INPUT);
  pinMode(PIN_IR4, INPUT);
  pinMode(PIN_IR5, INPUT);
  pinMode(PIN_IR6, INPUT);

  // Gestion moteurs
    // Moteur droit
  pinMode(PIN_M_DROIT_A, OUTPUT);
  pinMode(PIN_M_DROIT_R, OUTPUT);
    // Moteur gauche
  pinMode(PIN_M_GAUCHE_A, OUTPUT);
  pinMode(PIN_M_GAUCHE_R, OUTPUT);

  // à compléter

}
