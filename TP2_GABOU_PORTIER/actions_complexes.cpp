#include "actions_complexes.h"

void suivre_courbure(void) {
  int mGaucheIR2_A, mGaucheIR2_R, mDroitIR5_A, mDroitIR5_R;

  int Vmin = 70;
  int Vmax = 850;
  float a = 510.0 / (Vmax-Vmin);
  float b = -255.0-(a*Vmin);
  
  float vitesseIR2 = analogRead(PIN_IR2)*a-b; // a=0.56 ; b=294.2 ||| a=0.62 ; b=-298.4  ||| a=0.654 ; b=-301
  float vitesseIR5 = analogRead(PIN_IR5)*a-b; // a=0.6 ; b=-297
  
  int erreur = vitesseIR5 - vitesseIR2; // Si <0, il faut tourner à gauche, si = 0, tout droit, si >0 doit tourner à droite

  vitesseIR2 = 255 - erreur*0.5; // car 255 - (-255) = 510, et on veut 255 max, donc on fait un prduit en croix : 255/510 = 0,5
  vitesseIR5 = 255 + erreur*0.5;

  // Bloquer les valeurs entre -255 et 255
  if (vitesseIR2 < -255) vitesseIR2 = -255;
  if (vitesseIR2 > 255) vitesseIR2 = 255;
  if (vitesseIR5 < -255) vitesseIR5 = -255;
  if (vitesseIR5 > 255) vitesseIR5 = 255;
  
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






void stop_sign_360v1(void) {
  int compteur_lignes_IR6 = 0;
  int compteur_lignes_IR1 = 0;
  if (digitalRead(PIN_IR6) == 0 && digitalRead(PIN_IR1) == 1 && digitalRead(PIN_IR2) == 1) {
    delay(20); // Ajuster le délai (en gros les deux if() imbriqués c'est juste dire au robot "attends d'avoir confirmation que c'es bien la ligne)
    if (digitalRead(PIN_IR6) == 0 && digitalRead(PIN_IR1) == 1 && digitalRead(PIN_IR2) == 1) {
      do {
      analogWrite(PIN_M_GAUCHE_A, 255);
      analogWrite(PIN_M_GAUCHE_R, 0);
      analogWrite(PIN_M_DROIT_A, 0);
      analogWrite(PIN_M_DROIT_R, 255);
      if (digitalRead(PIN_IR6) == 0) {
        compteur_lignes_IR6++;
        compteur_lignes_IR1++;
      }
      while (digitalRead(PIN_IR6) == 0) { // Tant qu'on détecte encore du noir, on tourne juste sans compter
        analogWrite(PIN_M_GAUCHE_A, 255);
        analogWrite(PIN_M_GAUCHE_R, 0);
        analogWrite(PIN_M_DROIT_A, 0);
        analogWrite(PIN_M_DROIT_R, 255);
        }
      } while (compteur_lignes_IR6 < 3 && compteur_lignes_IR1 < 4);
  
      while (digitalRead(PIN_IR5) == 0) {}
    }
  }
}


void stop_360(void) {

  int compteur_lignes_IR6 = 0;
  int compteur_lignes_IR1 = 0;
  
  if (analogRead(PIN_IR6) <= 120  && digitalRead(PIN_IR1) == 1 && analogRead(PIN_IR5) <= 120 && digitalRead(PIN_IR2) == 1) { // on test tous les capteurs car parfois quand le robot tourne la ligne touche les capteurs les plus éloignés
    delay(20); // Ajuster le délai (en gros les deux if() imbriqués c'est juste dire au robot "attends d'avoir confirmation que c'es bien la ligne)
    if (analogRead(PIN_IR6) <= 120 && digitalRead(PIN_IR1) == 1 && analogRead(PIN_IR5) <= 120 && digitalRead(PIN_IR2) == 1) {
      while (compteur_lignes_IR1 < 2 ) {
        
        digitalWrite(PIN_LED, LOW);
        
        while (digitalRead(PIN_IR1) == 1) {
          analogWrite(PIN_M_GAUCHE_A, 0);
          analogWrite(PIN_M_GAUCHE_R, 255);
          analogWrite(PIN_M_DROIT_A, 255);
          analogWrite(PIN_M_DROIT_R, 0);
        }
        

        while (digitalRead(PIN_IR1) == 0) {
          analogWrite(PIN_M_GAUCHE_A, 0);
          analogWrite(PIN_M_GAUCHE_R, 255);
          analogWrite(PIN_M_DROIT_A, 255);
          analogWrite(PIN_M_DROIT_R, 0);
        }

        
        digitalWrite(PIN_LED, HIGH);
        compteur_lignes_IR1++;
      }
      
      digitalWrite(PIN_LED, LOW);

      while (digitalRead(PIN_IR1) == 1 || digitalRead(PIN_IR2) == 1) {
          analogWrite(PIN_M_GAUCHE_A, 0);
          analogWrite(PIN_M_GAUCHE_R, 255);
          analogWrite(PIN_M_DROIT_A, 255);
          analogWrite(PIN_M_DROIT_R, 0);
        }
      
      while (digitalRead(PIN_IR1) == 0) {
      suivre_courbure();
      }
      mode = MODE_NORMAL;
    }
  }
}




void raccourci(void) {
  static int etape = -1;
  int compteur_IR5 = 0;

  if (digitalRead(PIN_IR6) == 1 && digitalRead(PIN_IR5) == 1 && analogRead(PIN_IR2) <= 120 && etape == -1) {
    digitalWrite(PIN_LED, HIGH);
    delay(20);
    if (digitalRead(PIN_IR6) == 1 && digitalRead(PIN_IR5) == 1 && analogRead(PIN_IR2) <= 120 && etape == -1) {
      while (analogRead(PIN_IR2) <= 120) {
        analogWrite(PIN_M_GAUCHE_A, 200);
        analogWrite(PIN_M_GAUCHE_R, 0);
        analogWrite(PIN_M_DROIT_A, 200);
        analogWrite(PIN_M_DROIT_R, 0);
      }
        
      mode = MODE_RACCOURCI;
      etape = 0;
    }
  }

  switch (etape) {
    case 0:
      suivre_courbure();
      
      if (digitalRead(PIN_IR2) == 0) {
        while (digitalRead(PIN_IR2) == 0) {}
        analogWrite(PIN_M_GAUCHE_A, 0);
        analogWrite(PIN_M_GAUCHE_R, 0);
        analogWrite(PIN_M_DROIT_A, 255);
        analogWrite(PIN_M_DROIT_R, 0);
        delay(350);
        compteur_IR5 = 0;
        etape = 1;
      }
      break;

    case 1:
      suivre_courbure();
      if (digitalRead(PIN_IR5) == 0 && digitalRead(PIN_IR2) == 0) {
        while (digitalRead(PIN_IR2) == 0) {}
        delay(200);
        analogWrite(PIN_M_GAUCHE_A, 0);
        analogWrite(PIN_M_GAUCHE_R, 255);
        analogWrite(PIN_M_DROIT_A, 255);
        analogWrite(PIN_M_DROIT_R, 0);
        delay(350);
        etape = 2;
      }
      break;

    case 2:
      suivre_courbure();
      mode = MODE_NORMAL;
      break;
  }
}





void raccourci_etats(void) { // Version de la fonction raccourci avec une machine à états

  static int etape = -1;
  
  if (digitalRead(PIN_IR6) == 1 && digitalRead(PIN_IR5) == 1 && analogRead(PIN_IR2) <= 120 && etape == -1) {
    digitalWrite(PIN_LED, HIGH);
    delay(20);
    if (digitalRead(PIN_IR6) == 1 && digitalRead(PIN_IR5) == 1 && analogRead(PIN_IR2) <= 120 && etape == -1) {
      etape = 0;
      mode = MODE_RACCOURCI;
    }
  }


      switch (etape) {
        case -1:
          break;
          
        case 0:
          suivre_courbure();
          if (digitalRead(PIN_IR2) == 0) etape = 1;
          break;

        case 1:
          suivre_courbure();
          if (digitalRead(PIN_IR2) == 1) etape = 2;
          break;

        case 2:
          suivre_courbure();
          if (digitalRead(PIN_IR2) == 0) {
            etape = 3;
          }
          break;

        case 3:
          analogWrite(PIN_M_GAUCHE_A, 0);
          analogWrite(PIN_M_GAUCHE_R, 255);
          analogWrite(PIN_M_DROIT_A, 255);
          analogWrite(PIN_M_DROIT_R, 0);
          if (digitalRead(PIN_IR2) != 0) etape = 4;
          break;

        case 4:
          suivre_courbure();
          if (digitalRead(PIN_IR1) == 0 && digitalRead(PIN_IR6) == 0) etape = 5;
          break;

        case 5:
          analogWrite(PIN_M_GAUCHE_A, 255);
          analogWrite(PIN_M_GAUCHE_R, 0);
          analogWrite(PIN_M_DROIT_A, 255);
          analogWrite(PIN_M_DROIT_R, 0);
          if (digitalRead(PIN_IR1) == 1 && digitalRead(PIN_IR6) == 1) etape = 6;
          break;

        case 6:
          analogWrite(PIN_M_GAUCHE_A, 0);
          analogWrite(PIN_M_GAUCHE_R, 255);
          analogWrite(PIN_M_DROIT_A, 255);
          analogWrite(PIN_M_DROIT_R, 0);
          if (digitalRead(PIN_IR5) == 0) etape = -1;
          mode = MODE_NORMAL;
          break;

      default:
          analogWrite(PIN_M_GAUCHE_A, 0);
          analogWrite(PIN_M_GAUCHE_R, 0);
          analogWrite(PIN_M_DROIT_A, 0);
          analogWrite(PIN_M_DROIT_R, 0);
          etape = -1;
          break;
      }
}
