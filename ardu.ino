#include <Ultrasonic.h>

#define pino_trigger_mi 9
#define pino_trigger_lambda 10
#define pino_echo_mi 12
#define pino_echo_lambda 13

Ultrasonic sensor_mi(pino_trigger_mi, pino_echo_mi);
Ultrasonic sensor_lambda(pino_trigger_lambda, pino_echo_lambda);

float distancia_mi;
float distancia_lambda;
float vcve = 0.5;
float vcfa = -0.5;
float vcic = 0.5;
float vcpa = -0.5;

int paraAnalisador(float mi, float lambda) {
  mi = mi / 100;
  lambda = lambda / 100;
  float Gce = mi - lambda;
  float Gin = (mi + lambda) - 1;
  int estado = 0;
  float modulo_Gce;
  float modulo_Gin;
  
  if (Gce < 0)
    modulo_Gce = Gce * (-1);
  else
    modulo_Gce = Gce;
  
  if (Gin < 0)
    modulo_Gin = Gin * (-1);
  else
    modulo_Gin = Gin;

  if (Gce >= vcve)
    estado = 1;
  else if (Gce <= vcfa)
    estado = 2;
  else if (Gin >= vcic)
    estado = 3;
  else if (Gin <= vcpa)
    estado = 4;
  else if ((Gce >= 0) && (Gce < vcve) && (Gin >= 0) && (Gin < vcic) && (Gce >= Gin))
    estado = 5;
  else if ((Gce >= 0) && (Gce < vcve) && (Gin >= 0) && (Gin < vcic) && (Gce < Gin))
    estado = 6;
  else if ((Gce >= 0) && (Gce < vcve) && (Gin > vcpa) && (Gin <= 0) && (Gce >= modulo_Gin))
    estado = 7;
  else if ((Gce >= 0) && (Gce < vcve) && (Gin > vcpa) && (Gin <= 0) && (Gce < modulo_Gin))
    estado = 8;
  else if ((Gce > vcfa) && (Gce <= 0) && (Gin > vcpa) && (Gin <= 0) && (modulo_Gce >= modulo_Gin))
    estado = 9;
  else if ((Gce > vcfa) && (Gce <= 0) && (Gin > vcpa) && (Gce < Gin) && (Gin <= 0))
    estado = 10;
  else if ((Gce > vcfa) && (Gce <= 0) && (Gin >= 0) && (Gin < vcic) && (Gce >= Gin))
    estado = 11;
  else if ((Gce <= 0) && (Gce < vcfa) && (Gin >= 0) && (Gin < vcic) && (Gce < Gin))
    estado = 12;

  return estado;
}

int in1 = 7;
int in2 = 6;
int in3 = 5;
int in4 = 4;

float ajusteDistancia(Ultrasonic sensor) {
  float cmMsec;
  long microsec = sensor.read();
  cmMsec = microsec;
  
  if (cmMsec > 120)
    cmMsec = 120;
  else if (cmMsec < 5)
    cmMsec = 5;
  
  return cmMsec;
}

void para() {
  digitalWrite(in2, LOW);
  digitalWrite(in1, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void anda() {
  digitalWrite(in1, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in2, HIGH);
  digitalWrite(in4, LOW);
}

void re() {
  digitalWrite(in1, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in4, HIGH);
}

void direita() {
  digitalWrite(in1, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in4, LOW);
}

void esquerda() {
  digitalWrite(in1, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in4, LOW);
}

void esquerda_f() {
  digitalWrite(in1, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in4, LOW);
}

void direita_f() {
  digitalWrite(in1, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in4, HIGH);
}

void setup() {
  Serial.begin(9600);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void loop() {
  distancia_mi = map(ajusteDistancia(sensor_mi), 10, 120, 0, 100);
  distancia_lambda = map(ajusteDistancia(sensor_lambda), 10, 120, 100, 0);

  int estado = paraAnalisador(distancia_mi, distancia_lambda);
  Serial.println(String("Distancia-mi : ") + distancia_mi + String("| Distancia-lambda : ") + distancia_lambda + String("|Estado : ") + estado);
  if (estado == 1) {
    anda();
  }
  else if (estado == 2) {
    re();
  }
  else if (estado == 3) {
    esquerda();
  }
  else if (estado == 4) {
    direita();
  }
  else if (estado == 5) {
    direita_f();
  }
  else if (estado == 6) {
    direita();
  }
  else if (estado == 7) {
    esquerda_f();
  }
  else if (estado == 8) {
    esquerda();
  }
  else if (estado == 9) {
    para();
    esquerda_f();
    delay(500);
  }
  else if (estado == 10) {
    para();
    esquerda();
  }
  else if (estado == 11) {
    para();
    direita_f();
    delay(500);
  }
  else if (estado == 12) {
    para();
    direita();
  }
}
