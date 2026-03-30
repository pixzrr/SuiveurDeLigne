#ifndef actions_complexes_h
  #define actions_complexes_h

  #include <Arduino.h>
  #include "connexions_robot_arduino_uno.h"

  // Gestion des modes
#define MODE_NORMAL 0
#define MODE_360 1
#define MODE_RACCOURCI 2

extern int mode; // Variable globale de gestion des modes
  
void suivre_courbure(void);
void stop_sign_360v1(void);
void stop_360(void);
void raccourci(void);
void raccourci_etats(void);

void raccourcii(void);

#endif
