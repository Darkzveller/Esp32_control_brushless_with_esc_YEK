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
