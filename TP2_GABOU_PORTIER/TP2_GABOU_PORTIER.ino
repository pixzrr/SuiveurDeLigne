/*TP2
  PORTIER Gabriel
  GABOU Abel

  Projet Suiveur de Piste
  Robot TP2/1
  Date dernière modification : 09/04/2026 - Ajout de commentaires et structuration finale
*/

// ********************************************************************************************************************************************* //
// ********* Section 1 : Presets  ************************************************************************************************************** //
// ********************************************************************************************************************************************* //

// ──────────── Partie 1 : Inclusion des fichiers auxiliaires ──────────── //

#include "connexions_robot_arduino_uno.h"
#include "fonctions_test.h"
#include "actions_elementaires.h"
#include "actions_complexes.h"
#include "SonarVDA.h"

// ──────────── Partie 2 : Définition des constantes ───────────────── //

//Réglages selon l'exécution souhaitée
#define DEBEUG  0     // mettre 1 si le robot reste relié au PC (affichage CONSOLE)
                      // SINON 0
#define TEST    0     // mettre à 1 pour lancer un des tests
                      // SINON 0
  // Sélectionner un test :
  #define TEST_LED  0
  #define TEST_UART 0
  #define TEST_BATTERIE 0
  #define TEST_BP   0
  #define TEST_attendre_START 0
  #define TEST_attendre_START_en_boucle 0
  #define TEST_IR   0
  #define TEST_SONAR 1
  #define TEST_MOTEURS 0

//Paramètres de réglages
#define SEUIL_PRESENT 15  // distance en cm en dessous de laquelle un objet sera détecté
#define SEUIL_ABSENT  20  // distance en cm au dessus de laquelle un objet sera plus détecté


// ──────────── Partie 3 : Paramétrage des variables globales──────────── //

int mode = MODE_NORMAL; // variable définie dans actions_complexes.h

SonarVDA sonar; //Céation d'un objet "sonar" de classe "SonarVDA"
volatile bool obstacle_proche = false; //variable faisant le lien avec les fonction d'interruption

//Prototypes :
void test(void);
void obstacle_present(void);//Programme de gestion d'un obstacle
void obstacle_absent(void);//Programme de gestion de la disparition de l'obstacle

// ********************************************************************************************************************************************* //







// ********************************************************************************************************************************************* //
// ********* Section 2 : APPLICATIONS : setup() et loop()  ************************************************************************************* //
// ********************************************************************************************************************************************* //

// ──────────── Partie 1 : Setup ───────────────────────────── //

// the setup routine runs once when you press reset:
void setup() {  
  if(DEBEUG){Serial.begin(9600);}// initialize serial communication at 9600 bits per second:
  init_pin();
  
  if(TEST){test();}

  // Setup sonar
  sonar.set_trigger(SEUIL_PRESENT, SEUIL_ABSENT, obstacle_present, obstacle_absent); 
  sonar.start();
}

// ──────────── Partie 2 : Loop ───────────────────────────── //

void loop() {  

  // ----------- Vérifier boutton start ----------- //

  // Setup boutton start
  static bool verif_boutton_start = 0;
  
  if (verif_boutton_start == 0) {
    attendre_START();
    while (digitalRead(PIN_BP) == 1) {} // attendre que le bouton soit relaché pour éviter de gacher la fonction d'arret
    verif_boutton_start = 1;
  }
  

  // ---------------- Gestion Sonar ----------------- //

  if (obstacle_proche == true) {
    analogWrite(PIN_M_GAUCHE_A, 0);
    analogWrite(PIN_M_GAUCHE_R, 0);
    analogWrite(PIN_M_DROIT_A, 0);
    analogWrite(PIN_M_DROIT_R, 0);
    while (obstacle_proche == true) {}
  }
  

// ------------------ On met des conditions avec mode pour éviter que le robot fasse un 360 ou suive la courbe pendant un raccourci par exemple ------------------- //

  if (mode ==  MODE_NORMAL) suivre_courbure();

  raccourci();
  
  if (mode != MODE_RACCOURCI) stop_360();


  //  ----------- Si tout est blanc, on s'arrête par précaution  ou  Si l'utilisateur appuie sur le boutton encore une fois, le robot s'arrête ----------- //
    
    if ((digitalRead(PIN_IR2) == 1 && digitalRead(PIN_IR3) == 1 && digitalRead(PIN_IR4) == 1 && digitalRead(PIN_IR5) == 1) || digitalRead(PIN_BP) == 1 && verif_boutton_start == 1) {
      analogWrite(PIN_M_GAUCHE_A, 0);
      analogWrite(PIN_M_GAUCHE_R, 0);
      analogWrite(PIN_M_DROIT_A, 0);
      analogWrite(PIN_M_DROIT_R, 0);
      verif_boutton_start = 0;
    }
}

// ********************************************************************************************************************************************* //






// ********************************************************************************************************************************************* //
// ********* Section 3 : APPLICATIONS : test() et instructions liées aux interruptions  ******************************************************** //
// ********************************************************************************************************************************************* //

// ──────────── Partie 1 : Fonction test ──────────────────────── //

// Lancement de fonctions de tests 
//NB : constantes à régler en début de code selon tests désirées
void test(void){
  if(TEST_LED){test_led_en_boucle();}
  if(TEST_UART && DEBEUG){test_uart_en_boucle();}
  if(TEST_BATTERIE && DEBEUG){affiche_tention_batterie();}
  if(TEST_BP && DEBEUG){test_bouton_poussoir_en_boucle();}
  if(TEST_attendre_START && DEBEUG){ attendre_START();}
  if(TEST_attendre_START_en_boucle && DEBEUG){ test_attendre_START_en_boucle();}
  if(TEST_IR && DEBEUG){ test_capteurs_IR();}
  if(TEST_SONAR && DEBEUG){test_sonar_en_boucle(sonar, SEUIL_PRESENT, SEUIL_ABSENT, obstacle_present, obstacle_absent);}
  if(TEST_MOTEURS && DEBEUG){test_moteurs();}
}

// ──────────── Partie 2 : Instructions d'interruption ──────────────── //

//Fonction exécutée à l'apparition d'un obstacle
void obstacle_present(void){
   obstacle_proche = true;
   Serial.println(" Objet present");   
}

//Fonction exécutée à la disparition d'un obstacle
void obstacle_absent(void){
   obstacle_proche = false;
   Serial.println(" Objet absent");
}

// ********************************************************************************************************************************************* //
