#ifndef fonction_test_h
  #define fonction_test_h

  #include <Arduino.h>  
  #include "connexions_robot_arduino_uno.h"
  #include "actions_elementaires.h"
  #include "actions_complexes.h"
  #include "SonarVDA.h"

  //Paramètres de réglage des tests : 
  #define PERIODE_BLINK 1000 //période de clignotement

  //Prototypes :
  void test_led_en_boucle(void);
  void test_uart_en_boucle(void);
  void test_bouton_poussoir_en_boucle(void);
  void test_attendre_START_en_boucle(void);
  void test_sonar_en_boucle(SonarVDA sonar, int nb_cm_min, int nb_cm_max, void (*userFunc)(void), void (*userFunc2)(void));
  void test_capteurs_IR_logique(void);
  void test_moteurs(void);

#endif
