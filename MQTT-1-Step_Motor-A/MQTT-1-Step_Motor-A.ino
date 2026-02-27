#include <WiFi.h>
#include <PubSubClient.h>
#include <Stepper.h>

// --- CONFIGURACIÓN DE RED ---
const char* ssid = "SterenC";
const char* password = "unodostrescuatro";

// --- CONFIGURACIÓN MQTT ---
const char* mqtt_server = "192.168.1.229";
const int mqtt_port = 1883;
const char* mqtt_topic_command = "motores/control";
const char* mqtt_topic_rpm_base = "motores/rpm/"; // Base para tópicos de RPM

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
    Serial.printf("⚠️ Motor %d: Velocidad %d RPM excede el máximo (%d RPM). Limitando a %d RPM.\n", 
                  motorNum, requestedSpeed, MAX_RPM, MAX_RPM);
    return MAX_RPM;
  }
  return requestedSpeed;
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
  Serial.println("\n✅ WiFi conectada");
  Serial.print("📡 IP: ");
  Serial.println(WiFi.localIP());
  Serial.printf("⚙️ Límite máximo de velocidad: %d RPM\n", MAX_RPM);
}

void callback(char* topic, byte* message, unsigned int length) {
  String messageTemp;
  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }
  Serial.printf("📨 Comando recibido: %s\n", messageTemp.c_str());

  // Comandos individuales para cada motor (ON/OFF)
  if (messageTemp.startsWith("1:")) {
    motor1Estado = (messageTemp.substring(2) == "ON");
    Serial.printf("🔧 Motor 1 %s\n", motor1Estado ? "✅ ON" : "⬛ OFF");
  } 
  else if (messageTemp.startsWith("2:")) {
    motor2Estado = (messageTemp.substring(2) == "ON");
    Serial.printf("🔧 Motor 2 %s\n", motor2Estado ? "✅ ON" : "⬛ OFF");
  }
  else if (messageTemp.startsWith("3:")) {
    motor3Estado = (messageTemp.substring(2) == "ON");
    Serial.printf("🔧 Motor 3 %s\n", motor3Estado ? "✅ ON" : "⬛ OFF");
  }
  else if (messageTemp.startsWith("4:")) {
    motor4Estado = (messageTemp.substring(2) == "ON");
    Serial.printf("🔧 Motor 4 %s\n", motor4Estado ? "✅ ON" : "⬛ OFF");
  }
  // Comandos de velocidad INDIVIDUAL para cada motor
  else if (messageTemp.startsWith("SPEED1:")) {
    int requestedSpeed = messageTemp.substring(7).toInt();
    motor1Speed = validateSpeed(requestedSpeed, 1);
    motor1.setSpeed(motor1Speed);
    Serial.printf("⚡ Motor 1 velocidad: %d RPM\n", motor1Speed);
  }
  else if (messageTemp.startsWith("SPEED2:")) {
    int requestedSpeed = messageTemp.substring(7).toInt();
    motor2Speed = validateSpeed(requestedSpeed, 2);
    motor2.setSpeed(motor2Speed);
    Serial.printf("⚡ Motor 2 velocidad: %d RPM\n", motor2Speed);
  }
  else if (messageTemp.startsWith("SPEED3:")) {
    int requestedSpeed = messageTemp.substring(7).toInt();
    motor3Speed = validateSpeed(requestedSpeed, 3);
    motor3.setSpeed(motor3Speed);
    Serial.printf("⚡ Motor 3 velocidad: %d RPM\n", motor3Speed);
  }
  else if (messageTemp.startsWith("SPEED4:")) {
    int requestedSpeed = messageTemp.substring(7).toInt();
    motor4Speed = validateSpeed(requestedSpeed, 4);
    motor4.setSpeed(motor4Speed);
    Serial.printf("⚡ Motor 4 velocidad: %d RPM\n", motor4Speed);
  }
  // Comandos globales
  else if (messageTemp == "ALL:ON") {
    motor1Estado = true;
    motor2Estado = true;
    motor3Estado = true;
    motor4Estado = true;
    Serial.println("🔛 Todos los motores ENCENDIDOS");
  } 
  else if (messageTemp == "ALL:OFF") {
    motor1Estado = false;
    motor2Estado = false;
    motor3Estado = false;
    motor4Estado = false;
    Serial.println("🔛 Todos los motores APAGADOS");
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("🔄 Conectando MQTT...");
    String clientId = "ESP32Stepper-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("✅ conectada");
      client.subscribe(mqtt_topic_command);
    } else {
      Serial.print("❌ falló, rc=");
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
    
    // Publicar RPM de cada motor
    if (client.connected()) {
      String topic1 = String(mqtt_topic_rpm_base) + "1";
      client.publish(topic1.c_str(), String(rpm1).c_str());
      
      String topic2 = String(mqtt_topic_rpm_base) + "2";
      client.publish(topic2.c_str(), String(rpm2).c_str());
      
      String topic3 = String(mqtt_topic_rpm_base) + "3";
      client.publish(topic3.c_str(), String(rpm3).c_str());
      
      String topic4 = String(mqtt_topic_rpm_base) + "4";
      client.publish(topic4.c_str(), String(rpm4).c_str());
      
      Serial.printf("📊 RPM - M1: %.2f, M2: %.2f, M3: %.2f, M4: %.2f\n", 
                    rpm1, rpm2, rpm3, rpm4);
    }
    
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
  Serial.println("\n\n🚀 Iniciando sistema de control de 4 motores...");
  Serial.printf("🔧 Motor 28BYJ-48 - Máx: %d RPM\n", MAX_RPM);
  
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  
  // Establecer velocidades iniciales
  motor1.setSpeed(motor1Speed);
  motor2.setSpeed(motor2Speed);
  motor3.setSpeed(motor3Speed);
  motor4.setSpeed(motor4Speed);
  
  Serial.println("✅ Sistema listo!");
  Serial.println("Comandos disponibles:");
  Serial.println("  - 1:ON, 1:OFF, 2:ON, 2:OFF, etc.");
  Serial.println("  - SPEED1:15, SPEED2:8, SPEED3:12, SPEED4:5");
  Serial.println("  - ALL:ON, ALL:OFF");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Control de motores con sus velocidades individuales
  if (motor1Estado) {
    motor1.step(1);
    motor1StepCount++;
  }
  if (motor2Estado) {
    motor2.step(1);
    motor2StepCount++;
  }
  if (motor3Estado) {
    motor3.step(1);
    motor3StepCount++;
  }
  if (motor4Estado) {
    motor4.step(1);
    motor4StepCount++;
  }
  
  // Publicar RPM cada segundo
  publishRPM();
  
  delay(1);
}