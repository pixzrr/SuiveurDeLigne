#include "actions_complexes.h"

// ********************************************************************************************************************************************* //
// ********* Section 1 : Suivre Courbure  ****************************************************************************************************** //
// ********************************************************************************************************************************************* //

void suivre_courbure(void) {

// ──────────── Partie 1 : Fonction affine ─────────────────────────────────────── //
  
  int mGaucheIR2_A, mGaucheIR2_R, mDroitIR5_A, mDroitIR5_R;

  int Vmin = 70;
  int Vmax = 850;
  float a = 510.0 / (Vmax-Vmin);
  float b = -255.0-(a*Vmin);
  
  float vitesseIR2 = analogRead(PIN_IR2)*a-b; // a=0.56 ; b=294.2 ||| a=0.62 ; b=-298.4  ||| a=0.654 ; b=-301
  float vitesseIR5 = analogRead(PIN_IR5)*a-b; // a=0.6 ; b=-297

// ──────────── Partie 2 : Calcul de l'écart ────────────────────────────────────── //
  
  int erreur = vitesseIR5 - vitesseIR2; // Si <0, il faut tourner à gauche, si = 0, tout droit, si >0 doit tourner à droite

  vitesseIR2 = 255 - erreur*0.5; // car 255 - (-255) = 510 (ou -510)
  vitesseIR5 = 255 + erreur*0.5;

// ──────────── Partie 3 : Transcription vers les moteurs ────────────────────────────── //

  // Bloquer les valeurs entre -255 et 255
  if (vitesseIR2 < -255) vitesseIR2 = -255;
  if (vitesseIR2 > 255) vitesseIR2 = 255;
  if (vitesseIR5 < -255) vitesseIR5 = -255;
  if (vitesseIR5 > 255) vitesseIR5 = 255;
  
  // Test valeur positive/négative IR2
  if (vitesseIR2 >=0) {
    mGaucheIR2_A = vitesseIR2;
    mGaucheIR2_R = 0;
  } else {
    mGaucheIR2_A = 0;
    mGaucheIR2_R = -vitesseIR2;
   }
   // Test valeur positive/négative IR5
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

// ********************************************************************************************************************************************* //









// ********************************************************************************************************************************************* //
// ********* Section 2 : Stop 360  ************************************************************************************************************* //
// ********************************************************************************************************************************************* //

void stop_360(void) {

// ──────────── Partie 1 : Détection de la signalétique ──────────────────────────────── //

  int compteur_lignes_IR1 = 0;
  
  if (analogRead(PIN_IR6) <= 120  && digitalRead(PIN_IR1) == 1 && analogRead(PIN_IR5) <= 120 && digitalRead(PIN_IR2) == 1) { // on test tous les capteurs car parfois quand le robot tourne la ligne touche les capteurs les plus éloignés
    delay(20); // Ajuster le délai (en gros les deux if() imbriqués c'est juste dire au robot "attends d'avoir confirmation que c'es bien la ligne)
    if (analogRead(PIN_IR6) <= 120 && digitalRead(PIN_IR1) == 1 && analogRead(PIN_IR5) <= 120 && digitalRead(PIN_IR2) == 1) {

// ──────────── Partie 2 : 360 - incrémentation du compteur ───────────────────────────── //
      
      while (compteur_lignes_IR1 < 2 ) {
        
        digitalWrite(PIN_LED, LOW);
        
        while (digitalRead(PIN_IR1) == 1) {
          analogWrite(PIN_M_GAUCHE_A, 0);
          analogWrite(PIN_M_GAUCHE_R, 255);
          analogWrite(PIN_M_DROIT_A, 255);
          analogWrite(PIN_M_DROIT_R, 0);
        }
        

        while (digitalRead(PIN_IR1) == 0) {}

        
        digitalWrite(PIN_LED, HIGH);
        compteur_lignes_IR1++;
      }

// ──────────── Partie 3 : Recalibrage sur piste ─────────────────────────────────── //
      
      digitalWrite(PIN_LED, LOW);

      while (digitalRead(PIN_IR4) == 1 || digitalRead(PIN_IR2) == 1) {
          analogWrite(PIN_M_GAUCHE_A, 0);
          analogWrite(PIN_M_GAUCHE_R, 255);
          analogWrite(PIN_M_DROIT_A, 255);
          analogWrite(PIN_M_DROIT_R, 0);
        }

// ──────────── Partie 4 : Sortie de la section "stop sign" ──────────────────────────── //
      
      while (digitalRead(PIN_IR6) == 0) {
      suivre_courbure();
      }
    }
  }
}

// ********************************************************************************************************************************************* //









// ********************************************************************************************************************************************* //
// ********* Section 3 : Raccourci  ************************************************************************************************************ //
// ********************************************************************************************************************************************* //

void raccourci(void) {

// ──────────── Partie 1 : Détection de la signalétique ──────────────────────────────── //

  static int etape = -1;

  if (digitalRead(PIN_IR6) == 1 && digitalRead(PIN_IR5) == 1 && analogRead(PIN_IR2) <= 120 && etape == -1) {
    delay(20);
    if (digitalRead(PIN_IR6) == 1 && digitalRead(PIN_IR5) == 1 && analogRead(PIN_IR2) <= 120 && etape == -1) {
        
      mode = MODE_RACCOURCI;
      etape = 0;
    }
  }
  
// ──────────── Partie 2 : Etape 0 - Calibration sur le chemin du raccourci ──────────────────── //

  switch (etape) {
    case 0:
      suivre_courbure();
        while (digitalRead(PIN_IR2) == 0) {}                    // on attend de sortir de la signalétique
        while (digitalRead(PIN_IR2) == 1) suivre_courbure();    // on suit la piste tant que le chemin du raccourci n'est pas détecté
        delay(150);                                             // quand le chemin est reconnu, on avance un peu afin d'aligner les moteurs au chemin
        analogWrite(PIN_M_GAUCHE_A, 0);
        analogWrite(PIN_M_GAUCHE_R, 155);                       // on évite de tourner trop fort (problème moteur)
        analogWrite(PIN_M_DROIT_A, 255);
        analogWrite(PIN_M_DROIT_R, 0);
        while (digitalRead(PIN_IR2) == 1) {}                    // on continue de tourner tant que IR2 détecte du blanc (donc tant que nous ne sommes pas alignés au raccourci)
        etape = 1;
      break;

// ──────────── Partie 3 : Etape 1 - Suivi de courbe jusqu'à fin de raccourci puis sortie de raccourci ──── //

    case 1:
      suivre_courbure();
      if (digitalRead(PIN_IR5) == 0 && digitalRead(PIN_IR2) == 0) { // on attend de détecter la fin du raccourci                      
        delay(150);                                                 // on avance encore un peu pour aligner les moteurs à la piste
        analogWrite(PIN_M_GAUCHE_A, 0);
        analogWrite(PIN_M_GAUCHE_R, 155);
        analogWrite(PIN_M_DROIT_A, 255);
        analogWrite(PIN_M_DROIT_R, 0);
        while (digitalRead(PIN_IR2) == 1) {}                        // On tourne tant que IR2 ne détecte pas la piste principale (on ne prend pas IR5 car on prend en compte le drift)
        etape = 2;
      }
      break;

// ──────────── Partie 4 : Etape 2 - Passage en mode normal ───────────────────────────── //

    case 2:
      suivre_courbure();
      mode = MODE_NORMAL;
      break;
  }
}

// ********************************************************************************************************************************************* //
