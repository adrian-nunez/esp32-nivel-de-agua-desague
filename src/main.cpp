#include <Arduino.h>

const int pinesDisponibles[] = {4, 13, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33}; // 12 disponibles

unsigned int tiempoAnterior;
const unsigned long int lapso = 3 * 60 * 60000; // 3 horas                                                                                          * 60000
// const unsigned long int lapso = 10000; // minutos * 60000

// Rellenado del táper de tomates
unsigned int tiempoAnteriorTomates;
const unsigned long int lapsoTomates = 1 * 60 * 60000; // 1 hora

bool succion1 = false; // Es la más cercana a la caja de control. La que termina de desaguar al final.
const int bomba1 = 4;
const int sensor1 = 13;
int nivel1 = 1;

bool succion2 = false;
const int bomba2 = 16;
const int sensor2 = 17;
int nivel2 = 1;

bool succion3 = false;
const int bomba3 = 18;
const int sensor3 = 19;
int nivel3 = 1;

bool succion4 = false;
const int bomba4 = 21;
const int sensor4 = 22;
int nivel4 = 1;

// Se van a usar el 5 y 6 para bombear agua los tanques. El 5 para las torres debe hacerlo al terminar el drenaje.
bool succion5 = false;
const int bomba5 = 23;  // torre
const int sensor5 = 25; // torre
int nivel5 = 1;

bool succion6 = false;
const int bomba6 = 26;  // tomates
const int sensor6 = 27; // tomates
int nivel6 = 1;

void rellenar(int sensor, int bomba);

void setup()
{
  tiempoAnterior = millis();
  tiempoAnteriorTomates = millis();
  Serial.begin(115200);

  pinMode(sensor1, INPUT_PULLDOWN);
  pinMode(bomba1, OUTPUT);

  pinMode(sensor2, INPUT_PULLDOWN);
  pinMode(bomba2, OUTPUT);

  pinMode(sensor3, INPUT_PULLDOWN);
  pinMode(bomba3, OUTPUT);

  pinMode(sensor4, INPUT_PULLDOWN);
  pinMode(bomba4, OUTPUT);

  pinMode(sensor5, INPUT_PULLDOWN);
  pinMode(bomba5, OUTPUT);

  pinMode(sensor6, INPUT_PULLDOWN);
  pinMode(bomba6, OUTPUT);
}

void loop()
{

  // Obtener valores de niveles
  nivel1 = digitalRead(sensor1);
  nivel2 = digitalRead(sensor2);
  nivel3 = digitalRead(sensor3);
  nivel4 = digitalRead(sensor4);
  nivel5 = digitalRead(sensor5);
  nivel6 = digitalRead(sensor6);

  if (millis() - tiempoAnterior > lapso)
  {
    tiempoAnterior = millis();
    Serial.println("Ciclo");
    /* Bomba 1 */
    if (!succion1 && nivel1 == 0)
    {
      succion1 = true;
      digitalWrite(bomba1, LOW);
      Serial.print("bomba 1 ");
      Serial.print(sensor1);
      Serial.print(' ');
      Serial.println(bomba1);
    }

    /* Bomba 2 */
    if (!succion2 && nivel2 == 0)
    {
      succion2 = true;
      digitalWrite(bomba2, LOW);
      Serial.print("bomba 2 ");
      Serial.print(sensor2);
      Serial.print(' ');
      Serial.println(bomba2);
    }

    /* Bomba 3 */
    if (!succion3 && nivel3 == 0)
    {
      succion3 = true;
      digitalWrite(bomba3, LOW);
      Serial.print("bomba 3 ");
      Serial.print(sensor3);
      Serial.print(' ');
      Serial.println(bomba3);
    }

    /* Bomba 4 */
    if (!succion4 && nivel4 == 0)
    {
      succion4 = true;
      digitalWrite(bomba4, LOW);
    }

    /* Bomba 5 */
    if (!succion5 && nivel5 == 0)
    {
      succion5 = true;
      digitalWrite(bomba5, LOW);
    }

    /* Bomba 6 */
    if (!succion6 && nivel6 == 0)
    {
      succion6 = true;
      digitalWrite(bomba6, LOW);
    }
  }

  /* Apagar Bomba 1 */
  if (succion1 = true && nivel1 == 1)
  {
    succion1 = false;
    digitalWrite(bomba1, HIGH);

    // Después de apagar la bomba 1 se debe rellenar el táper:
    rellenar(25, 23);
  }

  if (nivel1 == 1)
  {
    succion1 = false;
    digitalWrite(bomba1, HIGH);
  }

  /* Apagar Bomba 2 */
  if (succion2 = true && nivel2 == 1)
  {
    succion2 = false;
    digitalWrite(bomba2, HIGH);
  }

  if (nivel2 == 1)
  {
    succion2 = false;
    digitalWrite(bomba2, HIGH);
  }

  /* Apagar Bomba 3 */
  if (succion3 = true && nivel3 == 1)
  {
    succion3 = false;
    digitalWrite(bomba3, HIGH);
  }

  if (nivel3 == 1)
  {
    succion3 = false;
    digitalWrite(bomba3, HIGH);
  }

  /* Apagar Bomba 4 */
  if (succion4 = true && nivel4 == 1)
  {
    succion4 = false;
    digitalWrite(bomba4, HIGH);
  }

  if (nivel4 == 1)
  {
    succion4 = false;
    digitalWrite(bomba4, HIGH);
  }

  /* Apagar Bomba 5 */
  if (succion5 = true && nivel5 == 1)
  {
    succion5 = false;
    digitalWrite(bomba5, HIGH);
  }

  if (nivel5 == 1)
  {
    succion5 = false;
    digitalWrite(bomba5, HIGH);
  }

  /* Apagar Bomba 6 */
  if (succion6 = true && nivel6 == 1)
  {
    succion6 = false;
    digitalWrite(bomba6, HIGH);
  }

  if (nivel6 == 1)
  {
    succion6 = false;
    digitalWrite(bomba6, HIGH);
  }

  // Rellenar táper de tomates

  if (millis() - tiempoAnteriorTomates > lapsoTomates)
  {
    tiempoAnteriorTomates = millis();
    rellenar(27, 26);
  }

  // Serial.println("Sensor 25: " + String(digitalRead(25)));
  // Serial.println("Sensor 27: " + String(digitalRead(27)));
}

void rellenar(int sensor, int bomba)
{
  while (digitalRead(sensor) == 1)
  {
    digitalWrite(bomba, LOW);
  }
  digitalWrite(bomba, HIGH);
}