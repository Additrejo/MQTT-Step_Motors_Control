#include <WiFi.h>
#include <PubSubClient.h>
#include <Stepper.h>

// --- CONFIGURACIÓN DE RED ---
const char* ssid = "SterenC";
const char* password = "unodostrescuatro";

// --- CONFIGURACIÓN MQTT ---
const char* mqtt_server = "192.168.1.229";
const int mqtt_port = 1883;

// TÓPICOS INDIVIDUALES PARA CADA MOTOR
const char* mqtt_topic_motor1 = "obc1/esp1/motor1/control";
const char* mqtt_topic_motor2 = "obc1/esp1/motor2/control";
const char* mqtt_topic_motor3 = "obc1/esp1/motor3/control";
const char* mqtt_topic_motor4 = "obc1/esp1/motor4/control";

// TÓPICO PARA CONTROL GENERAL (TODOS LOS MOTORES)
const char* mqtt_topic_traction = "obc1/esp1/traction/control";

// TÓPICOS PARA PUBLICAR RPM
const char* mqtt_topic_rpm_base = "obc1/esp1/rpm/"; // Base para tópicos de RPM

// --- CONFIGURACIÓN DE MOTORES ---
const int stepsPerRevolution = 4096; // Pasos por revolución del 28BYJ-48
const int MAX_RPM = 20; // Límite máximo físico del motor

// Pines Motor 1
#define M1_IN1 14
#define M1_IN2 27
#define M1_IN3 26
#define M1_IN4 32
Stepper motor1(stepsPerRevolution, M1_IN1, M1_IN3, M1_IN2, M1_IN4);

// Pines Motor 2
#define M2_IN1 33
#define M2_IN2 25
#define M2_IN3 13
#define M2_IN4 12
Stepper motor2(stepsPerRevolution, M2_IN1, M2_IN3, M2_IN2, M2_IN4);

// Pines Motor 3
#define M3_IN1 19
#define M3_IN2 18
#define M3_IN3 5
#define M3_IN4 17
Stepper motor3(stepsPerRevolution, M3_IN1, M3_IN3, M3_IN2, M3_IN4);

// Pines Motor 4
#define M4_IN1 16
#define M4_IN2 4
#define M4_IN3 2
#define M4_IN4 15
Stepper motor4(stepsPerRevolution, M4_IN1, M4_IN3, M4_IN2, M4_IN4);

// --- VARIABLES DE ESTADO Y VELOCIDAD INDIVIDUAL ---
bool motor1Estado = false;
bool motor2Estado = false;
bool motor3Estado = false;
bool motor4Estado = false;

int motor1Speed = 10; // RPM individual para motor 1
int motor2Speed = 10; // RPM individual para motor 2
int motor3Speed = 10; // RPM individual para motor 3
int motor4Speed = 10; // RPM individual para motor 4

// Variables para step divisor
int motor1StepDiv = 1;
int motor2StepDiv = 1;
int motor3StepDiv = 1;
int motor4StepDiv = 1;

// --- VARIABLES PARA CÁLCULO DE RPM REALES ---
unsigned long lastRpmPublish = 0;
const unsigned long rpmPublishInterval = 1000; // Publicar RPM cada 1 segundo

// Contadores de pasos para cada motor
volatile int motor1StepCount = 0;
volatile int motor2StepCount = 0;
volatile int motor3StepCount = 0;
volatile int motor4StepCount = 0;

// --- CLIENTES WIFI Y MQTT ---
WiFiClient espClient;
PubSubClient client(espClient);

// --- FUNCIÓN PARA VALIDAR Y LIMITAR VELOCIDAD ---
int validateSpeed(int requestedSpeed, int motorNum) {
  if (requestedSpeed < 0) return 0;
  if (requestedSpeed > MAX_RPM) {
    Serial.printf("Motor %d: Velocidad %d RPM excede el máximo (%d RPM). Limitando a %d RPM.\n", 
                  motorNum, requestedSpeed, MAX_RPM, MAX_RPM);
    return MAX_RPM;
  }
  return requestedSpeed;
}

// --- FUNCIÓN PARA PROCESAR COMANDOS INDIVIDUALES ---
void processMotorCommand(int motorNum, String command) {
  // Eliminar espacios en blanco
  command.trim();
  
  if (command == "pwr:on") {
    switch(motorNum) {
      case 1: motor1Estado = true; Serial.printf("Motor 1 ON\n"); break;
      case 2: motor2Estado = true; Serial.printf("Motor 2 ON\n"); break;
      case 3: motor3Estado = true; Serial.printf("Motor 3 ON\n"); break;
      case 4: motor4Estado = true; Serial.printf("Motor 4 ON\n"); break;
    }
  }
  else if (command == "pwr:off") {
    switch(motorNum) {
      case 1: motor1Estado = false; Serial.printf("Motor 1 OFF\n"); break;
      case 2: motor2Estado = false; Serial.printf("Motor 2 OFF\n"); break;
      case 3: motor3Estado = false; Serial.printf("Motor 3 OFF\n"); break;
      case 4: motor4Estado = false; Serial.printf("Motor 4 OFF\n"); break;
    }
  }
  else if (command.startsWith("pwm:freq:")) {
    int freq = command.substring(9).toInt();
    // Convertir frecuencia (Hz) a RPM aproximado
    // Para 28BYJ-48: RPM ≈ (frecuencia * 60) / 4096
    int rpm = (freq * 60) / stepsPerRevolution;
    int validatedRpm = validateSpeed(rpm, motorNum);
    
    // DEBUG: Mostrar información detallada
    Serial.println("=== DEBUG MOTOR INDIVIDUAL ===");
    Serial.printf("Motor: %d\n", motorNum);
    Serial.printf("Comando: %s\n", command.c_str());
    Serial.printf("Frecuencia: %d Hz\n", freq);
    Serial.printf("RPM calculado: %d\n", rpm);
    Serial.printf("RPM validado: %d\n", validatedRpm);
    
    switch(motorNum) {
      case 1: 
        Serial.printf("Motor1Speed ANTES: %d\n", motor1Speed);
        motor1Speed = validatedRpm;
        motor1.setSpeed(motor1Speed);
        Serial.printf("Motor1Speed DESPUES: %d\n", motor1Speed);
        break;
      case 2:
        Serial.printf("Motor2Speed ANTES: %d\n", motor2Speed);
        motor2Speed = validatedRpm;
        motor2.setSpeed(motor2Speed);
        Serial.printf("Motor2Speed DESPUES: %d\n", motor2Speed);
        break;
      case 3:
        Serial.printf("Motor3Speed ANTES: %d\n", motor3Speed);
        motor3Speed = validatedRpm;
        motor3.setSpeed(motor3Speed);
        Serial.printf("Motor3Speed DESPUES: %d\n", motor3Speed);
        break;
      case 4:
        Serial.printf("Motor4Speed ANTES: %d\n", motor4Speed);
        motor4Speed = validatedRpm;
        motor4.setSpeed(motor4Speed);
        Serial.printf("Motor4Speed DESPUES: %d\n", motor4Speed);
        break;
    }
    Serial.println("=============================");
  }
  else if (command.startsWith("step:div:")) {
    int divisor = command.substring(9).toInt();
    if (divisor < 1) divisor = 1;
    
    switch(motorNum) {
      case 1: 
        motor1StepDiv = divisor;
        Serial.printf("Motor 1 step divisor: %d\n", divisor);
        break;
      case 2:
        motor2StepDiv = divisor;
        Serial.printf("Motor 2 step divisor: %d\n", divisor);
        break;
      case 3:
        motor3StepDiv = divisor;
        Serial.printf("Motor 3 step divisor: %d\n", divisor);
        break;
      case 4:
        motor4StepDiv = divisor;
        Serial.printf("Motor 4 step divisor: %d\n", divisor);
        break;
    }
  }
  else {
    Serial.printf("Comando no reconocido para motor %d: %s\n", motorNum, command.c_str());
  }
}

// --- FUNCIÓN PARA PROCESAR COMANDOS GLOBALES (TRACTION) ---
void processTractionCommand(String command) {
  command.trim();
  
  if (command == "pwr:on") {
    motor1Estado = true;
    motor2Estado = true;
    motor3Estado = true;
    motor4Estado = true;
    Serial.println("TODOS los motores ENCENDIDOS");
  }
  else if (command == "pwr:off") {
    motor1Estado = false;
    motor2Estado = false;
    motor3Estado = false;
    motor4Estado = false;
    Serial.println("TODOS los motores APAGADOS");
  }
  else if (command.startsWith("pwm:freq:")) {
    int freq = command.substring(9).toInt();
    int rpm = (freq * 60) / stepsPerRevolution;
    int validRpm = validateSpeed(rpm, 0); // 0 = todos los motores
    
    // DEBUG: Mostrar información detallada
    Serial.println("=== DEBUG TRACTION (TODOS) ===");
    Serial.printf("Comando: %s\n", command.c_str());
    Serial.printf("Frecuencia: %d Hz\n", freq);
    Serial.printf("RPM calculado: %d\n", rpm);
    Serial.printf("RPM validado: %d\n", validRpm);
    
    // Aplicar la MISMA velocidad a TODOS los motores
    motor1Speed = validRpm;
    motor2Speed = validRpm;
    motor3Speed = validRpm;
    motor4Speed = validRpm;
    
    motor1.setSpeed(motor1Speed);
    motor2.setSpeed(motor2Speed);
    motor3.setSpeed(motor3Speed);
    motor4.setSpeed(motor4Speed);
    
    Serial.printf("Velocidades actualizadas - M1:%d M2:%d M3:%d M4:%d\n", 
                  motor1Speed, motor2Speed, motor3Speed, motor4Speed);
    Serial.println("==============================");
  }
  else if (command.startsWith("step:div:")) {
    int divisor = command.substring(9).toInt();
    if (divisor < 1) divisor = 1;
    
    motor1StepDiv = divisor;
    motor2StepDiv = divisor;
    motor3StepDiv = divisor;
    motor4StepDiv = divisor;
    
    Serial.printf("TODOS los motores step divisor: %d\n", divisor);
  }
  else {
    Serial.printf("Comando no reconocido para control general: %s\n", command.c_str());
  }
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectada");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.printf("Limite maximo de velocidad: %d RPM\n", MAX_RPM);
}

void callback(char* topic, byte* message, unsigned int length) {
  String messageTemp;
  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }
  
  Serial.println("--- NUEVO MENSAJE MQTT ---");
  Serial.printf("Topic: %s\n", topic);
  Serial.printf("Mensaje: %s\n", messageTemp.c_str());

  // Determinar qué motor según el tópico
  if (strcmp(topic, mqtt_topic_motor1) == 0) {
    processMotorCommand(1, messageTemp);
  }
  else if (strcmp(topic, mqtt_topic_motor2) == 0) {
    processMotorCommand(2, messageTemp);
  }
  else if (strcmp(topic, mqtt_topic_motor3) == 0) {
    processMotorCommand(3, messageTemp);
  }
  else if (strcmp(topic, mqtt_topic_motor4) == 0) {
    processMotorCommand(4, messageTemp);
  }
  else if (strcmp(topic, mqtt_topic_traction) == 0) {
    processTractionCommand(messageTemp);
  }
  else {
    Serial.printf("Topic no reconocido: %s\n", topic);
  }
  Serial.println("-------------------------");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando MQTT...");
    String clientId = "ESP32Stepper-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("conectada");
      
      // Suscribirse a los 5 tópicos (4 motores + 1 general)
      client.subscribe(mqtt_topic_motor1);
      client.subscribe(mqtt_topic_motor2);
      client.subscribe(mqtt_topic_motor3);
      client.subscribe(mqtt_topic_motor4);
      client.subscribe(mqtt_topic_traction);
      
      Serial.println("Suscrito a:");
      Serial.printf(" - %s\n", mqtt_topic_motor1);
      Serial.printf(" - %s\n", mqtt_topic_motor2);
      Serial.printf(" - %s\n", mqtt_topic_motor3);
      Serial.printf(" - %s\n", mqtt_topic_motor4);
      Serial.printf(" - %s\n", mqtt_topic_traction);
    } else {
      Serial.print("fallo, rc=");
      Serial.print(client.state());
      Serial.println(" reintentando en 5s");
      delay(5000);
    }
  }
}

void publishRPM() {
  static unsigned long lastTime = 0;
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - lastTime;
  
  if (elapsedTime >= rpmPublishInterval) {
    float seconds = elapsedTime / 1000.0;
    
    // Calcular RPM para cada motor
    float rpm1 = (motor1StepCount / seconds) * 60.0 / stepsPerRevolution;
    float rpm2 = (motor2StepCount / seconds) * 60.0 / stepsPerRevolution;
    float rpm3 = (motor3StepCount / seconds) * 60.0 / stepsPerRevolution;
    float rpm4 = (motor4StepCount / seconds) * 60.0 / stepsPerRevolution;
    
    // Mostrar contadores y RPM calculadas
    Serial.println("--- REPORTE RPM ---");
    Serial.printf("CONTADORES - M1:%d M2:%d M3:%d M4:%d\n", 
                  motor1StepCount, motor2StepCount, motor3StepCount, motor4StepCount);
    Serial.printf("RPM CALCULADAS - M1:%.2f M2:%.2f M3:%.2f M4:%.2f\n", 
                  rpm1, rpm2, rpm3, rpm4);
    
    // Publicar RPM de cada motor
    if (client.connected()) {
      String topic1 = String(mqtt_topic_rpm_base) + "motor1";
      client.publish(topic1.c_str(), String(rpm1).c_str());
      
      String topic2 = String(mqtt_topic_rpm_base) + "motor2";
      client.publish(topic2.c_str(), String(rpm2).c_str());
      
      String topic3 = String(mqtt_topic_rpm_base) + "motor3";
      client.publish(topic3.c_str(), String(rpm3).c_str());
      
      String topic4 = String(mqtt_topic_rpm_base) + "motor4";
      client.publish(topic4.c_str(), String(rpm4).c_str());
      
      Serial.println("RPM publicados en MQTT");
    }
    Serial.println("------------------");
    
    // Reiniciar contadores
    motor1StepCount = 0;
    motor2StepCount = 0;
    motor3StepCount = 0;
    motor4StepCount = 0;
    lastTime = currentTime;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n\n=== INICIANDO SISTEMA DE CONTROL DE 4 MOTORES ===");
  Serial.printf("Motor 28BYJ-48 - Maximo: %d RPM\n", MAX_RPM);
  
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  
  // Establecer velocidades iniciales
  motor1.setSpeed(motor1Speed);
  motor2.setSpeed(motor2Speed);
  motor3.setSpeed(motor3Speed);
  motor4.setSpeed(motor4Speed);
  
  Serial.println("Sistema listo!");
  Serial.println("\nComandos disponibles:");
  Serial.println(" pwr:on   - Encender motor(es)");
  Serial.println(" pwr:off  - Apagar motor(es)");
  Serial.println(" pwm:freq:[valor] - Frecuencia en Hz");
  Serial.println(" step:div:[valor] - Divisor de pasos");
  Serial.println("================================\n");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Control de motores con sus velocidades individuales
  if (motor1Estado) {
    motor1.step(motor1StepDiv);
    motor1StepCount += motor1StepDiv;
  }
  if (motor2Estado) {
    motor2.step(motor2StepDiv);
    motor2StepCount += motor2StepDiv;
  }
  if (motor3Estado) {
    motor3.step(motor3StepDiv);
    motor3StepCount += motor3StepDiv;
  }
  if (motor4Estado) {
    motor4.step(motor4StepDiv);
    motor4StepCount += motor4StepDiv;
  }
  
  // Publicar RPM cada segundo
  publishRPM();
  
  delay(1);
}