// Pines de control para los motores
const int input1 = 33;  // IN1 (Motor A)
const int input2 = 25;  // IN2 (Motor A)
const int input3 = 26;  // IN3 (Motor B)
const int input4 = 27;  // IN4 (Motor B)

// Pines para PWM
const int enableA = 14;  // Enable A para el Motor A
const int enableB = 12;  // Enable B para el Motor B

// Pines del sensor HC-SR04
const int trigPin = 15;  // Pin de Trigger
const int echoPin = 16;  // Pin de Echo

// Matriz de movimientos
// {input1, input2, input3, input4, velA, velB}
int moves[6][6] = {
  {0, 1, 0, 1, 255, 255},  // 0 - Avanzar hacia adelante
  {0, 1, 1, 0, 200, 200},  // 1 - Girar a la izquierda
  {1, 0, 0, 1, 200, 200},  // 2 - Girar a la derecha
  {1, 0, 0, 1, 150, 255},  // 3 - Curva a la izquierda
  {0, 1, 1, 0, 255, 150},  // 4 - Curva a la derecha
  {0, 0, 0, 0, 0,   0}     // 5 - Detener
};

// Función para mover el robot
void move(int moveType) {
  digitalWrite(input1, moves[moveType][0]);
  digitalWrite(input2, moves[moveType][1]);
  digitalWrite(input3, moves[moveType][2]);
  digitalWrite(input4, moves[moveType][3]);
  
  // Asignar la velocidad a ambos motores usando analogWrite
  analogWrite(enableA, moves[moveType][4]); // Velocidad motor A
  analogWrite(enableB, moves[moveType][5]); // Velocidad motor B
}

// Función para medir distancia con el sensor HC-SR04
float measureDistance() {
  // Generar el pulso en el pin de Trigger
  digitalWrite(trigPin, LOW); // Asegurar que el Trigger está en LOW
  delayMicroseconds(2); // Esperar 2 microsegundos
  digitalWrite(trigPin, HIGH); // Enviar pulso HIGH por 10 microsegundos
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW); // Regresar el Trigger a LOW

  // Leer el tiempo que tarda el pulso en volver (duración)
  long duration = pulseIn(echoPin, HIGH);

  // Calcular la distancia en centímetros
  float distance = duration * 0.0343 / 2;

  return distance;
}

void setup() {
  // Inicializar el puerto serial
  Serial.begin(115200);

  // Configurar pines como salida para motores
  pinMode(input1, OUTPUT);
  pinMode(input2, OUTPUT);
  pinMode(input3, OUTPUT);
  pinMode(input4, OUTPUT);
  
  // Configurar pines PWM para habilitar motores
  pinMode(enableA, OUTPUT);
  pinMode(enableB, OUTPUT);

  // Configurar pines como entrada o salida para el sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Inicializar los motores apagados
  move(3);  // Detener los motores
}

void loop() {
  // Medir distancia con el sensor ultrasónico
  float distance = measureDistance();
  
  // Mostrar la distancia en el monitor serial
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Si el objeto está a menos de 15 cm, detener motores y girar
  if (distance <= 15) {
    Serial.println("¡Objeto detectado a menos de 15 cm! Girando.");
    move(3);  // Detener motores
    delay(500);  // Pausar brevemente
    
    // Elegir aleatoriamente girar a la izquierda o derecha
    if (random(0, 2) == 0) {
      move(1);  // Girar a la izquierda
      Serial.println("Girando a la izquierda.");
    } else {
      move(2);  // Girar a la derecha
      Serial.println("Girando a la derecha.");
    }

    delay(1000);  // Girar durante 1 segundo
  } else {
    // Si no hay obstáculos, avanzar hacia adelante
    move(0);  // Avanza hacia adelante a velocidad máxima
    Serial.println("Avanzando.");
  }

  delay(100); // Esperar brevemente antes de volver a medir la distancia
}
