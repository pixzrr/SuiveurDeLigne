/* Ressources utilisées :
  Timers 0 8bits :        > Moteur 1 :
    OC0A = PD6 =  D6  IO6   > Cmd_1A
    OC0B = PD5 =  D5  IO5   > Cmd_1B
  Timers 1 16bits:        > Sonar :
    ICP1 = PB0 =  D8  IO8   > US_echo
    OC1A = PB1 =  D9  IO9
    OC1B = PB2 =  D10 SS    > US_trig
  Timers 2 8bits:         > Moteur 2 :
    OC2A = PB3 =  D11 MOSI  > Cmd_2A
    OC2B = PD3 =  D3  IO3   > Cmd_2B
*/

#ifndef connexions_robot_arduino_uno_h
  #define connexions_robot_arduino_uno_h

  #include <Arduino.h>

// ──────────── Partie 1 : LED, BP, Batterie ───────────────────── //

  #define PIN_LED 13 // LED
  #define PIN_BP 12 //Bouton poussoir
  #define PIN_TENSION_REDUITE_BATTERIE A4 // Tension réduite batterie

  #define PIN_BATT A123456 //Image de la tension de la Batterie

// ──────────── Partie 2 : Capteurs IR ──────────────────────── //

  // Capteurs IR de gauche à droite, vue derrière le robot
  #define PIN_IR1 4
  #define PIN_IR2 A3
  #define PIN_IR3 A2
  #define PIN_IR4 A1
  #define PIN_IR5 A0
  #define PIN_IR6 7

// ──────────── Partie 3 : Moteurs ────────────────────────── //

    // Moteur droit
  #define PIN_M_DROIT_A 5
  #define PIN_M_DROIT_R 6
  
    //Moteur gauche
  #define PIN_M_GAUCHE_A 3
  #define PIN_M_GAUCHE_R 11

  // Puissances moteurs
    // Lent
  #define RAPIDE 255
    // Rapide
    #define LENT 180




  void init_pin(void);

#endif
