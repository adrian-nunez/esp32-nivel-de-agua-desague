#include <Arduino.h>

// PINES DISPONIBLES EN EL ESP32
const int pinesDisponibles[] = {4, 13, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33}; // 12 disponibles

int vueltas;
// 33 salida para solenoide
// 34 entrada para solenoide (sensor de nivel del aceitunero)
int salidaSolenoideAceitunero = 33;
int sensorNivelAceitunero = 12;

// DURACIÓN DEL CICLO EN EL QUE SE LLENA EL ACEITUNERO
const unsigned long int lapsoAceitunero = 3 * 60 * 60000; // 3 horas
// const unsigned long int lapsoAceitunero = 10000; // 3 horas
unsigned int tiempoAnteriorAceitunero;

// DURACIÓN DEL CICLO PARA EL VACIADO DE LAS TORRES
unsigned int tiempoAnterior;
const unsigned long int lapso = 2 * 60 * 60000; // 3 horas         // ahora cada 2 horas                                                                                 * 60000
// const unsigned long int lapso = 6000; // 1 min

// CONTROL DEL LLENADO DE TAPER DE TORRES
bool tocaLlenarTaperTorres;

// DURACIÓN DEL CICLO PARA EL LLENADO DEL TÁPER DE TOMATES
unsigned int tiempoAnteriorTomates;
const unsigned long int lapsoTomates = 1 * 60 * 60000; // 1 hora
unsigned int pausa1;                                   // pausa periódica para el vaciado de la torre 1 para que la bomba descanse de a ratos
// const unsigned long int lapsoTomates = 30000; // 30 seg
bool cicloNuevo = false;
bool cicloNuevoAceitunero = false;

bool primerCiclo = true; // Recién arranca

// DEFINICIÓN DE QUÉ PINES SON PARA CUÁLES BOMBAS Y SENSORES:
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

int nivelAceitunero = 1;

// void rellenarTorres();
// void rellenarTomates();
void rellenarAceitunero();
// void vaciarTorre1();

void setup()
{
  vueltas = 0;

  bool tocaLlenarTaperTorres = false;

  tiempoAnterior = millis();
  tiempoAnteriorTomates = millis();
  tiempoAnteriorAceitunero = millis();
  pausa1 = millis();
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

  pinMode(sensorNivelAceitunero, INPUT_PULLDOWN);
  pinMode(salidaSolenoideAceitunero, OUTPUT);
  digitalWrite(salidaSolenoideAceitunero, HIGH); // HIGH SIGNIFICA APAGADO PORQUE EL RELAY ES NC
  delay(1500);
  Serial.println("**************");
  Serial.println(nivel1);
  Serial.println("**************");
}

void loop()
{
  nivelAceitunero = digitalRead(sensorNivelAceitunero);
  // RELLENAR ACEITUNERO
  // if (digitalRead(sensorNivelAceitunero) == 0)
  // {
  //   digitalWrite(salidaSolenoideAceitunero, HIGH);
  //   cicloNuevoAceitunero = false;
  //   tiempoAnteriorAceitunero = millis();
  // }

  if (((millis() - tiempoAnteriorAceitunero > lapsoAceitunero) || primerCiclo) && nivelAceitunero == 1)
  {
    cicloNuevoAceitunero = true;
    tiempoAnteriorAceitunero = millis();
    // rellenarAceitunero();
    if (nivelAceitunero == 1)
    {
      digitalWrite(salidaSolenoideAceitunero, LOW);
    }
  }

  if (nivelAceitunero == 0)
  {
    cicloNuevoAceitunero = false;
    digitalWrite(salidaSolenoideAceitunero, HIGH);
    return;
  }

  // Obtener valores de niveles
  nivel1 = digitalRead(sensor1);
  nivel2 = digitalRead(sensor2);
  nivel3 = digitalRead(sensor3);
  nivel4 = digitalRead(sensor4);
  nivel5 = digitalRead(sensor5);
  nivel6 = digitalRead(sensor6);

  // VACIADO DE LAS TORRES
  if (millis() - tiempoAnterior > lapso || primerCiclo)
  {
    vueltas++;
    primerCiclo = false;
    tiempoAnterior = millis();
    cicloNuevo = true;
    Serial.print("Ciclo nuevo ");
    Serial.println(vueltas);
    /* Bomba 1 */
    if (!succion1 && nivel1 == 0)
    {
      succion1 = true;
      digitalWrite(bomba1, LOW);
      // vaciarTorre1();
    }
    if (nivel1 == 1)
    {
      succion1 = false;
      digitalWrite(bomba1, HIGH);
      Serial.println("apagar bomba 1");
    }

    /* Bomba 2 */
    if (!succion2 && nivel2 == 0)
    {
      succion2 = true;
      digitalWrite(bomba2, LOW);
    }
    if (nivel2 == 1)
    {
      succion2 = false;
      digitalWrite(bomba2, HIGH);
      Serial.println("apagar bomba 2");
    }

    /* Bomba 3 */
    if (!succion3 && nivel3 == 0)
    {
      succion3 = true;
      digitalWrite(bomba3, LOW);
    }
    if (nivel3 == 1)
    {
      succion3 = false;
      digitalWrite(bomba3, HIGH);
      Serial.println("apagar bomba 3");
    }

    /* Bomba 4 */
    if (!succion4 && nivel4 == 0)
    {
      succion4 = true;
      digitalWrite(bomba4, LOW);
    }
    if (nivel4 == 1)
    {
      succion4 = false;
      digitalWrite(bomba4, HIGH);
      Serial.println("apagar bomba 4");
    }
  }

  /* Apagar Bomba 1 */

  if (nivel5 == 0) // Este sensor marca 1 cuando el nivel está abajo
  {
    tocaLlenarTaperTorres = false;
    cicloNuevo = false;
    digitalWrite(bomba5, HIGH);
  }
  if (succion1 = true && nivel1 == 1)
  {
    succion1 = false;
    digitalWrite(bomba1, HIGH);

    // Después de apagar la bomba 1 se debe rellenar el táper de torres:

    if (nivel5 == 1 && cicloNuevo)
    {
      tocaLlenarTaperTorres = true;
    }
    if (tocaLlenarTaperTorres)
    {
      digitalWrite(bomba5, LOW);
    }
    else
    {
      digitalWrite(bomba5, HIGH);
    }
  }

  if (nivel1 == 1) // Apagar la bomba 1 si el sensor de nivel está en 1
  {
    succion1 = false;
    digitalWrite(bomba1, HIGH); // Apagar la bomba 1
  }

  /* Apagar Bomba 5 */
  if (nivel5 == 1)
  {
    succion5 = false;
    digitalWrite(bomba5, HIGH);
  }

  /* Apagar Bomba 6 */
  // if (nivel6 == 1)
  // {
  //   succion6 = false;
  //   digitalWrite(bomba6, HIGH);
  // }

  // Rellenar táper de tomates

  if ((millis() - tiempoAnteriorTomates > lapsoTomates)) // Este sensor marca 1 cuando el nivel está abajo
  {
    tiempoAnteriorTomates = millis();
    if (nivel6 == 1)
      digitalWrite(bomba6, LOW);
  }
  if (nivel6 != 1)
  {
    digitalWrite(bomba6, HIGH);
  }
}

// ABAJO SE DEFINEN LAS FUNCIONES:

// void rellenarTorres()
// {
//   while (tocaLlenarTaperTorres) // Este sensor marca 1 cuando el nivel está abajo
//   {
//     digitalWrite(bomba5, LOW);
//     if (digitalRead(sensor5) != 1)
//       tocaLlenarTaperTorres = false;
//   }

//   digitalWrite(bomba5, HIGH);
//   succion5 = false;
//   cicloNuevo = false;
//   return;
// }

// void rellenarTomates()
// {
//   while (digitalRead(sensor6) == 1)
//   {
//     digitalWrite(bomba6, LOW);
//   }
//   digitalWrite(bomba6, HIGH);
//   return;
// }

void rellenarAceitunero()
{
  while (cicloNuevoAceitunero)
  {
    digitalWrite(salidaSolenoideAceitunero, LOW);
    if (digitalRead(sensorNivelAceitunero) == 0)
    {
      cicloNuevoAceitunero = false;
      digitalWrite(salidaSolenoideAceitunero, HIGH);
      return;
    }
  }
  return;
}

// void vaciarTorre1()
// {
//   while (succion1)
//   {
//     Serial.println("entra al while");
//     if (millis() - pausa1 < 30000) // primero debe funcionar 30 segundos
//     {
//       digitalWrite(bomba1, LOW);
//       Serial.println("menos de 30 seg");
//     }
//     if ((millis() - pausa1) >= 30000 && (millis() - pausa1) <= 35000) // pausa de 5 segundos para que descanse la bomba
//     {
//       digitalWrite(bomba1, HIGH);
//       Serial.println("entre 30 seg y 35 seg");
//     }
//     if (millis() - pausa1 > 35000) // resetear el contador de pausa1
//     {
//       pausa1 = millis();
//       digitalWrite(bomba1, LOW);
//       Serial.println("más de 35 seg - reset");
//     }
//   }
// }