
/*#include <Arduino.h>
#include <ESP32Servo.h>

Servo esc;
const int escPin = 18; // Signal ESC
const int potPin = 34; // Broche analogique du potentiomètre (ADC1)

// Plage ESC standard en microsecondes
const int escMin = 1000;
const int escMax = 2000;

void setup()
{
  Serial.begin(115200);

  esc.setPeriodHertz(50); // Fréquence de fonctionnement ESC RC (50 Hz)
  esc.attach(escPin, escMin, escMax);

  // Délai d'initialisation ESC
  esc.writeMicroseconds(escMin);
  Serial.println("Initialisation ESC...");
  delay(1000);
}

void loop()
{
  // Lecture du potentiomètre (résultat entre 0 et 4095 sur ESP32)
  int potValue = analogRead(potPin);
  potValue = 1000;
  // Conversion en microsecondes pour ESC
  int escValue = map(potValue, 0, 4095, escMin, escMax);

  // Conversion en pourcentage (0 à 100 %)
  int percentValue = map(escValue, escMin, escMax, 0, 180);

  // Envoi du signal PWM à l’ESC
  // esc.writeMicroseconds(escValue);
  // esc.write(percentValue);

  // Affichage série
  Serial.print("Pot: ");
  Serial.print(potValue);
  Serial.print(" → PWM: ");
  Serial.print(escValue);
  Serial.print(" µs → ");
  Serial.print(percentValue);
  Serial.println(" %");

}
*/

// #include <Arduino.h>
// #define PIN_BRUSHLESS_MOTOR 18
// #define CHANNEL_0 0
// #define RESOLUTION_BIT 8
// #define FREQUENCE 50 // en Hz
// int i = 0;
// void setup()
// {
//   // Initialisation du Serial
//   Serial.begin(115200);

//   // Brushlessmotor
//   pinMode(PIN_BRUSHLESS_MOTOR, OUTPUT);

//   ledcSetup(CHANNEL_0, FREQUENCE, RESOLUTION_BIT);
//   Serial.printf("Mise au minimum du manche a gaz \n");
//   ledcWrite(CHANNEL_0, 0);

//     Serial.println("Mise au min. des gaz 00");
//   ledcWrite(CHANNEL_0, 0);
//   delay(5000); // Laisse le temps à l'ESC de calibrer le min

//   Serial.println("Mise au max. des gaz 255");
//   ledcWrite(CHANNEL_0, 255);
//   delay(2000);

//   Serial.println("Retour au min. des gaz 0");
//   ledcWrite(CHANNEL_0, 0);
//   delay(2000);

//   Serial.println("Calibrage terminé, prêt à fonctionner");

//   delay(10000);
// }

// void loop()
// {

//   // if (i > 250)
//   // {
//   //   i = 0;
//   // }
//   // ledcWrite(CHANNEL_0, i++);
// }

#include <Arduino.h>

#define PIN_BRUSHLESS_MOTOR 18
#define CHANNEL_0 0
#define RESOLUTION_BITS 16 // Obtenir un control fin est nettement meilleur pour le moteur
#define FREQ_HZ 50
#define PWM_MAX ((1 << RESOLUTION_BITS) - 1) // Revient a faire  2^{RESOLUTION_BITS} - 1

// Convertit un pourcentage (0 à 100) en valeur de rapport cyclique pour le PWM
uint32_t pourcentageEnRapportCyclique(float pourcentage)
{
  pourcentage = constrain(pourcentage, 0, 100);
  pourcentage = (pourcentage / 100.0) * PWM_MAX;
  return pourcentage;
}
void move_brushless(int rapport_cyclique)
{
  ledcWrite(CHANNEL_0, pourcentageEnRapportCyclique(rapport_cyclique)); // 5% (équivaut environ à 1000µs)
}

void init_esc(bool _active_sequence)
{
  if (_active_sequence)
  {
    Serial.println("Mise au min des gaz (0%)");
    move_brushless(5);
    delay(5000);

    Serial.println("Mise au max des gaz (10%)");
    move_brushless(10); // 10% (~2000µs)
    delay(2000);

    Serial.println("Retour au min des gaz (5%)");
    move_brushless(5); // 5%
    delay(2000);

    Serial.println("Calibrage terminé !");
  }
}

void setup()
{
  Serial.begin(115200);
  ledcSetup(CHANNEL_0, FREQ_HZ, RESOLUTION_BITS);
  ledcAttachPin(PIN_BRUSHLESS_MOTOR, CHANNEL_0);
  init_esc(true);
}

void loop()
{
  // Tu pourras ajouter ton code ici pour varier entre 1000 et 2000 µs
  // move_brushless(50);
  // delay(5000);
  // move_brushless(5);
  // delay(5000);
}
