#include "fonctions_test.h"
  
void test_led_en_boucle(void){
  while(1){
    digitalWrite(PIN_LED, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(PERIODE_BLINK/2);                      // wait in ms
    digitalWrite(PIN_LED, LOW);   // turn the LED off by making the voltage LOW
    delay(PERIODE_BLINK/2);                      // wait in ms
  }
}

//Fonction de test de la liaison série UART entre la carte Arduino et le PC
void test_uart_en_boucle(void){
  Serial.println("Bonjour");
  delay(2000);
  while(1){
    Serial.println("Utile pour debugger");
    if (digitalRead(PIN_LED) == HIGH) digitalWrite(PIN_LED, LOW);
    else digitalWrite(PIN_LED,HIGH);
    delay(500);
  }
}

//Fonction de....................
void test_bouton_poussoir_en_boucle(void){
  Serial.println("Test lecture BP");
  delay(2000);
  bool buttonState;

while (1) {
  do {
    buttonState = digitalRead(PIN_BP);
  }
  while(buttonState != 1);
   buttonState = digitalRead(PIN_BP); 
   Serial.println("BP appuyé"); // print out the state of the button
   digitalWrite(PIN_LED, HIGH);
   delay(100);

  do {
    buttonState = digitalRead(PIN_BP);
  }
  while(buttonState != 0);
    buttonState = digitalRead(PIN_BP); 
    Serial.println("BP relaché"); // print out the state of the button
    digitalWrite(PIN_LED, LOW);
    delay(100); 
  } 
}

//Fonction de..........
void test_attendre_START_en_boucle(void){
  Serial.println("Test attendre START");  
  unsigned char i=0;
  delay(2000);
  while(1){
    attendre_START();
    i++;
    Serial.print("Bouton START appuyé "); 
    Serial.print(i); 
    Serial.println(" fois"); 
  }
}

//Fonction de..........

// Noir = 0, Blanc = 1
void test_capteurs_IR_logique(void) {
  Serial.println("Capteur1(D1):  Capteur2(AN3): Capteur3(AN2):  Capteur4(AN1):  Capteur5(AN0):  Capteur6(D0):");
  bool choix = 0;
  while(1) {
    while (choix == 0) {
      if (digitalRead(PIN_BP) == 1) choix = 1;
      Serial.print(digitalRead(PIN_IR1));
      Serial.print("    ");
      Serial.print(digitalRead(PIN_IR2));
      Serial.print("    ");
      Serial.print(digitalRead(PIN_IR3));
      Serial.print("    ");
      Serial.print(digitalRead(PIN_IR4));
      Serial.print("    ");
      Serial.print(digitalRead(PIN_IR5));
      Serial.print("    ");
      Serial.println(digitalRead(PIN_IR6));

      delay(1000);
    }
    while (choix == 1) {
      if (digitalRead(PIN_BP) == 1) choix = 0;
      Serial.print(analogRead(PIN_IR1));
      Serial.print("    ");
      Serial.print(analogRead(PIN_IR2));
      Serial.print("    ");
      Serial.print(analogRead(PIN_IR3));
      Serial.print("    ");
      Serial.print(analogRead(PIN_IR4));
      Serial.print("    ");
      Serial.print(analogRead(PIN_IR5));
      Serial.print("    ");
      Serial.println(analogRead(PIN_IR6));

      delay(1000);
    }
  }
}

void test_moteurs(void) {
  while (1) {
  attendre_START();

// Moteur droit
  analogWrite(PIN_M_DROIT_A, RAPIDE);
  analogWrite(PIN_M_DROIT_R, 0);
  delay(500);
  analogWrite(PIN_M_DROIT_A, 0);

  analogWrite(PIN_M_DROIT_R, RAPIDE);
  analogWrite(PIN_M_DROIT_A, 0);
  delay(500);
  analogWrite(PIN_M_DROIT_R, 0);

  // Moteur gauche
  analogWrite(PIN_M_GAUCHE_A, RAPIDE);
  analogWrite(PIN_M_GAUCHE_R, 0);
  delay(500);
  analogWrite(PIN_M_GAUCHE_A, 0);

  analogWrite(PIN_M_GAUCHE_R, RAPIDE);
  analogWrite(PIN_M_GAUCHE_A, 0);
  delay(500);
  analogWrite(PIN_M_GAUCHE_R, 0);

  delay(1500);

  // Avancer / Reculer / tourner à gauche / tourner à droite
  analogWrite(PIN_M_DROIT_A, RAPIDE);
  analogWrite(PIN_M_GAUCHE_A, RAPIDE);
  analogWrite(PIN_M_GAUCHE_R, 0);
  analogWrite(PIN_M_DROIT_R, 0);
  delay(1000);
  analogWrite(PIN_M_DROIT_A, 0);
  analogWrite(PIN_M_GAUCHE_A, 0);
  
  analogWrite(PIN_M_DROIT_R, RAPIDE);
  analogWrite(PIN_M_GAUCHE_R, RAPIDE);
  analogWrite(PIN_M_GAUCHE_A, 0);
  analogWrite(PIN_M_DROIT_A, 0);
  delay(1000);
  analogWrite(PIN_M_DROIT_R, 0);
  analogWrite(PIN_M_GAUCHE_R, 0);
  
  }
}


//Fonction de test de la détection d'obstacle
//Cette fonction appelle les fonctions : obstacle_present() et obstacle_absent()
void test_sonar_en_boucle(SonarVDA sonar, int nb_cm_min, int nb_cm_max, void (*userFunc)(void), void (*userFunc2)(void)){
  Serial.println("Test du programme SONAR"); // print fonctionne aussi mais println permet de sauter une ligne à la fin
  sonar.set_trigger(nb_cm_min, nb_cm_max, userFunc, userFunc2); 
	 //Si un objet vient d'être détecté à moins de SEUIL_PRESENT cm alors la fonction "obj_present()" est appelée
	 //Si l'objet n'est plus détecté au dela de SEUIL_ABSENT cm alors la fonction "obj_absent()" est appelée
  sonar.start(); // On démarre les mesures d'obstacle
  while(1){
    Serial.println(sonar.get_last_measure());//on affiche la dernière mesure faite par le sonar
    delay(1000);//pause d'1 seconde
    if(digitalRead(PIN_BP)==1) { //Test de l'arrêt et de la remise en route du sonar
        sonar.stop();
        do{;
        }while(digitalRead(PIN_BP)==1);
        sonar.start();
    }
  }
}
