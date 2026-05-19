/*************************************************
   ROBOT ESP32 + TB6612 + HC-SR04 + CNY70 + SHARP
*************************************************/

// ========= TB6612 =========

// Motor A
#define PWMA 18
#define AIN1 16
#define AIN2 17

// Motor B
#define PWMB 5
#define BIN1 26
#define BIN2 25

// Standby
#define STBY 4


// ========= HC-SR04 =========

#define TRIG_PIN 21
#define ECHO_PIN 19


// ========= SENSORES SHARP =========

// Frente
#define SHARP_FRONT 39

// Izquierda
#define SHARP_LEFT 34

// Derecha
#define SHARP_RIGHT 35


// ========= SENSORES CNY70 =========

// Abajo
#define CNY70_DOWN 36

// Frente
#define CNY70_FRONT 23


// ========= PWM =========

#define PWM_FREQ 1000
#define PWM_RESOLUTION 8

#define CHANNEL_A 0
#define CHANNEL_B 1


// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(115200);

  // ===== TB6612 =====
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  pinMode(STBY, OUTPUT);

  digitalWrite(STBY, HIGH);

  // PWM
  ledcSetup(CHANNEL_A, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(PWMA, CHANNEL_A);

  ledcSetup(CHANNEL_B, PWM_FREQ, PWM_RESOLUTION);
  ledcAttachPin(PWMB, CHANNEL_B);

  // ===== HC-SR04 =====
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // ===== Sensores =====
  pinMode(CNY70_DOWN, INPUT);
  pinMode(CNY70_FRONT, INPUT);

  pinMode(SHARP_FRONT, INPUT);
  pinMode(SHARP_LEFT, INPUT);
  pinMode(SHARP_RIGHT, INPUT);

  Serial.println("Sistema iniciado");
}


// =====================================================
// LOOP
// =====================================================

void loop() {

  long distancia = medirDistancia();

  int sharpFront = analogRead(SHARP_FRONT);
  int sharpLeft  = analogRead(SHARP_LEFT);
  int sharpRight = analogRead(SHARP_RIGHT);

  int cnyDown  = analogRead(CNY70_DOWN);
  int cnyFront = analogRead(CNY70_FRONT);

  // ===== Mostrar datos =====
  Serial.println("==========");

  Serial.print("Ultrasonico: ");
  Serial.print(distancia);
  Serial.println(" cm");

  Serial.print("Sharp Frente: ");
  Serial.println(sharpFront);

  Serial.print("Sharp Izq: ");
  Serial.println(sharpLeft);

  Serial.print("Sharp Der: ");
  Serial.println(sharpRight);

  Serial.print("CNY70 Abajo: ");
  Serial.println(cnyDown);

  Serial.print("CNY70 Frente: ");
  Serial.println(cnyFront);


  // =================================================
  // LOGICA DEL ROBOT
  // =================================================

  // Si detecta obstáculo cerca
  if (distancia < 15 && distancia > 0) {

    detenerMotores();
    delay(300);

    retroceder(180);
    delay(500);

    girarDerecha(180);
    delay(500);
  }

  // Si el CNY70 de abajo detecta borde
  else if (cnyDown < 1500) {

    detenerMotores();
    delay(200);

    retroceder(180);
    delay(500);

    girarIzquierda(180);
    delay(500);
  }

  // Si sensor frontal detecta algo
  else if (sharpFront > 2500 || cnyFront > 2000) {

    detenerMotores();
    delay(200);

    girarDerecha(180);
    delay(400);
  }

  // Movimiento normal
  else {

    avanzar(180);
  }

  delay(50);
}


// =====================================================
// FUNCIONES MOTORES
// =====================================================

void avanzar(int velocidad) {

  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);

  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);

  ledcWrite(CHANNEL_A, velocidad);
  ledcWrite(CHANNEL_B, velocidad);
}

void retroceder(int velocidad) {

  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);

  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);

  ledcWrite(CHANNEL_A, velocidad);
  ledcWrite(CHANNEL_B, velocidad);
}

void girarDerecha(int velocidad) {

  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);

  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);

  ledcWrite(CHANNEL_A, velocidad);
  ledcWrite(CHANNEL_B, velocidad);
}

void girarIzquierda(int velocidad) {

  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);

  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);

  ledcWrite(CHANNEL_A, velocidad);
  ledcWrite(CHANNEL_B, velocidad);
}

void detenerMotores() {

  ledcWrite(CHANNEL_A, 0);
  ledcWrite(CHANNEL_B, 0);
}


// =====================================================
// HC-SR04
// =====================================================

long medirDistancia() {

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duracion = pulseIn(ECHO_PIN, HIGH, 30000);

  long distancia = duracion * 0.034 / 2;

  return distancia;
}
