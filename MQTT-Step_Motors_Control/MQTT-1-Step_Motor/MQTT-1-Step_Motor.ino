 #include <WiFi.h>
#include <PubSubClient.h>
#include <Stepper.h>

// --- CONFIGURACIÓN DE RED ---
const char* ssid = "TU_SSID"; // <-- CAMBIA POR TU NOMBRE DE RED
const char* password = "TU_PASSWORD"; // <-- CAMBIA POR TU PASSWORD

// --- CONFIGURACIÓN MQTT (RASPBERRY PI) ---
const char* mqtt_server = "192.168.1.X"; // <-- CAMBIA POR LA IP DE TU RASPBERRY PI 5
const int mqtt_port = 1883;
const char* mqtt_topic_command = "motores/control"; // Tópico para recibir comandos

// --- CONFIGURACIÓN DE LOS MOTORES ---
// Número de pasos por revolución para el motor 28BYJ-48 (con gearbox)
// Modo Full-step: 2048 pasos/rev. Modo Half-step: 4096 pasos/rev (más suave)
const int stepsPerRevolution = 4096; 

// Definir los pines para cada motor (IN1, IN2, IN3, IN4)
// Motor 1
#define M1_IN1 14
#define M1_IN2 27
#define M1_IN3 26
#define M1_IN4 32
Stepper motor1(stepsPerRevolution, M1_IN1, M1_IN3, M1_IN2, M1_IN4); // Nota: el orden puede variar según la librería. Si gira al revés, intercambia los dos últimos pines.

// Motor 2 (EJEMPLO - Conecta los pines que hayas elegido)
#define M2_IN1 33
#define M2_IN2 25
#define M2_IN3 13
#define M2_IN4 12
Stepper motor2(stepsPerRevolution, M2_IN1, M2_IN3, M2_IN2, M2_IN4);

// --- ESTADOS DE LOS MOTORES ---
bool motor1Estado = false;
bool motor2Estado = false;
// Añadir para motor3 y motor4...
int motorSpeed = 10; // Velocidad por defecto (RPM)

// --- CLIENTES WIFI Y MQTT ---
WiFiClient espClient;
PubSubClient client(espClient);

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

Serial.println("");
Serial.println("WiFi conectada");
Serial.print("IP address: ");
Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
Serial.print("Mensaje recibido en tópico: ");
Serial.print(topic);
Serial.print(". Mensaje: ");

String messageTemp;
for (int i = 0; i < length; i++) {
 Serial.print((char)message[i]);
 messageTemp += (char)message[i];
}
Serial.println();

// Procesar comandos recibidos
// Ejemplo de formato esperado: "1:ON", "2:OFF", "SPEED:15", "ALL:ON"
if (messageTemp.startsWith("1:")) {
 if (messageTemp.substring(2) == "ON") {
   motor1Estado = true;
   Serial.println("Motor 1 Encendido");
 } else if (messageTemp.substring(2) == "OFF") {
   motor1Estado = false;
   Serial.println("Motor 1 Apagado");
 }
} else if (messageTemp.startsWith("2:")) {
 if (messageTemp.substring(2) == "ON") {
   motor2Estado = true;
   Serial.println("Motor 2 Encendido");
 } else if (messageTemp.substring(2) == "OFF") {
   motor2Estado = false;
   Serial.println("Motor 2 Apagado");
 }
} else if (messageTemp.startsWith("SPEED:")) {
 motorSpeed = messageTemp.substring(6).toInt();
 motor1.setSpeed(motorSpeed);
 motor2.setSpeed(motorSpeed);
 Serial.printf("Velocidad ajustada a %d RPM\n", motorSpeed);
} else if (messageTemp == "ALL:ON") {
 motor1Estado = true;
 motor2Estado = true;
 Serial.println("Todos los motores Encendidos");
} else if (messageTemp == "ALL:OFF") {
 motor1Estado = false;
 motor2Estado = false;
 Serial.println("Todos los motores Apagados");
}
}

void reconnect() {
while (!client.connected()) {
 Serial.print("Intentando conexión MQTT...");
 String clientId = "ESP32Stepper-";
 clientId += String(random(0xffff), HEX);
 if (client.connect(clientId.c_str())) {
   Serial.println("conectada");
   client.subscribe(mqtt_topic_command);
 } else {
   Serial.print("falló, rc=");
   Serial.print(client.state());
   Serial.println(" intentando de nuevo en 5 segundos");
   delay(5000);
 }
}
}

void setup() {
Serial.begin(115200);
setup_wifi();
client.setServer(mqtt_server, mqtt_port);
client.setCallback(callback);

// Velocidad inicial
motor1.setSpeed(motorSpeed);
motor2.setSpeed(motorSpeed);
// ... para motor3 y 4
}

void loop() {
if (!client.connected()) {
 reconnect();
}
client.loop();

// Lógica de control de motores
if (motor1Estado) {
 motor1.step(1); // Dar un paso
}
if (motor2Estado) {
 motor2.step(1); // Dar un paso
}
// Añadir para motor3 y motor4...

delay(1); // Pequeña pausa para estabilidad. La velocidad real la controla setSpeed()
}