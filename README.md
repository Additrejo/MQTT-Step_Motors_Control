# 
Control de motores paso a paso con protocolo MQTT monitoreado por un Dashboard de control en Node-Red

## Proyecto: MQTT Step Motors Control

<!--------------------- PROYECTO DE LABORATORIO -------------------------->
<!--![Proyecto](https://img.shields.io/badge/Proyecto-NANOSWAI-blueviolet?style=for-the-badge)-->
[![Proyecto](https://img.shields.io/badge/Proyecto-MQTT-darkorange)](https://github.com/search?q=topic:at-iot)  
[![Estatus Activo](https://img.shields.io/badge/Estatus-activo-2EA44F)](https://github.com/search?q=topic:at-activo)  
[![Electrónica](https://img.shields.io/badge/ÁREA-ELECTRÓNICA-007EC6)](https://github.com/search?q=topic:at-electronics) [![IoT](https://img.shields.io/badge/IoT-E05D44)](https://github.com/search?q=topic:at-iot) [![Área](https://img.shields.io/badge/Robotics-FE7D37)](https://github.com/search?q=topic:at-robotics) 

<!-------------------- ETIQUETAS DE ESTATUS DEL PROYECTO ------------------------->

<!-- [![LINX Pausa](https://img.shields.io/badge/STATUS-PAUSA-E3B341)](https://github.com/search?q=topic:linx-pausa) -->
<!--[![LINX Completado](https://img.shields.io/badge/STATUS-COMPLETADO-50FA7B)](https://github.com/search?q=topic:linx-completado) --> 
<!--[![LINX Archivado](https://img.shields.io/badge/STATUS-ARCHIVADO-656D76)](https://github.com/search?q=topic:linx-archivado) -->


<!--------------------- ETIQUETAS DE ÁREA DEL PROYECTO -------------------------->


 <!-------------------- Imagen del proyecto ------------------------->
[Imagen]

---
<!-------------------- DESCRIPCIÓN Y OBJETIVOS DEL PROYECTO ------------------------->


## 🎯 Descripción y objetivos

###  **Objetivo Principal:**  
Controlar motores a pasos usando protocolo MQTT a través de un dashboard.  
###  **Objetivos Secundarios:**
 * Control de giro y velocidad.
 * Controlar hasta 4 motores.
 * Gráficar datos.


## Tecnologías: 
 * MQTT.
   
---

## 📊 Gestión del Proyecto

Toda la gestión de tareas, sprints y seguimiento de issues se maneja a través de nuestro tablero de GitHub Projects.

**➡️ [Ir al Tablero del Proyecto](httpsEscribe-aqui-el-enlace-a-tu-GitHub-Project)**

---

## 📜 Tabla de Contenidos

* [🎯 Descripción y objetivos](#-descripción-y-ojetivos)
* [Tecnologías](#-Tecnologías)
* [📊 Gestión del Proyecto](#-gestión-del-proyecto)
* [🛠️ Stack Tecnológico y Componentes](#-stack-tecnológico-y-componentes)
* [📁 Estructura del Repositorio](#-estructura-del-repositorio)
* [🚀 Instalación y Puesta en Marcha](#-instalación-y-puesta-en-marcha)
* [💡 Uso y Operación](#-uso-y-operación)
* [📚 Documentación Adicional](#-documentación-adicional)
* [👥 Equipo y Responsables](#-equipo-y-responsables)
* [⚖️ Licencia](#-licencia)

---

## 🛠️ Stack Tecnológico y Componentes

### Software
* **Firmware (IDE):**
  - [VS Code + PlatformIO](https://code.visualstudio.com/)
  - [Arduino IDE](https://www.arduino.cc/en/software)
* **Acceso remoto):** 
  - [RealVNC - Acceso remoto Raspberry pi 5.](https://www.realvnc.com/es/?lai_vid=BbW02Kbn5fga&lai_sr=0-4&lai_sl=l&lai_p=1&lai_na=611310)
  - [Node-RED (Dashboard) - Panel de visualzación de datos y control.](https://www.https://nodered.org/)


### Versión  
* **Python:** Python 3.x

### Hardware y Componentes Clave

* **Microcontroladores:**
  - [ESP32 30 Pines ESP WROOM 32](https://documentation.espressif.com/esp32_datasheet_en.pdf),
  - [Raspberry Pi 5](https://pip-assets.raspberrypi.com/categories/892-raspberry-pi-5/documents/RP-008348-DS-3-raspberry-pi-5-product-brief.pdf?disposition=inline) 
* **Actuadores:**
  - [Motor 28BYJ-48](https://uelectronics.com/wp-content/uploads/2017/08/KS0208-keyestudio-ULN2003-Stepper-Motor-Driver.pdf)
* **Drivers:**
  - [Controlador ULN2003](https://uelectronics.com/wp-content/uploads/2017/08/KS0208-keyestudio-ULN2003-Stepper-Motor-Driver.pdf)
* **Potencia:**
  - Eliminador externo. (Para 4 motores es obligatorio usar una fuente externa potente, ya que cada motor puede consumir picos de hasta 200-250mA).
* **Conexiones:**
  - Cables Dupont (hembra-hembra y hembra-macho) para realizar las conexiones.
  


---

## 📁 Estructura del Repositorio

[esp32_ws](https://github.com/LINX-ICN-UNAM/Colmena_2_Rover/tree/main/esp32_ws): Esta carpeta contiene los test así como el programa de la versión V2.  
[img](https://github.com/LINX-ICN-UNAM/Colmena_2_Rover/tree/main/img): Esta carpeta contiene las imagenes del repositorio.  
[raspberry_ws](https://github.com/LINX-ICN-UNAM/Colmena_2_Rover/tree/main/raspberry_ws): Esta carpeta contiene los códigos necesarios para ejecutar los programas en Raspberry pi 5.  
[Dasboard](): Esta carpeta contiene la programación del dashboard.

---

## 🚀 Instalación y Puesta en Marcha

### 1. Circuito.

   **1.1 Conexión entre ESP32 y ULN2003 (para un motor):**
   La comunicación es muy directa. La ESP32 envía las señales de control al driver ULN2003, que a su vez alimenta las bobinas del motor. La conexión típica es la siguiente.

| ESP32 Devkit               | ULN2003 (Módulo Driver) | Cable del Motor 28BYJ-48          |
|----------------------------|--------------------------|-----------------------------------|
| GPIO 14                    | IN1                      |                                   |
| GPIO 27                    | IN2                      |                                   |
| GPIO 26                    | IN3                      |                                   |
| GPIO 32                    | IN4                      |                                   |
| GND                        | GND                      |                                   |
| 5V (de fuente externa)     | VCC (5V)                 |                                   |
|                            | A, B, C, D               | Azul, Rosa, Amarillo, Naranja (conectar en orden) |
|                            | VCC (5V)                 | Rojo (alimentación del motor)     |


<img width="701" height="487" alt="image" src="https://github.com/user-attachments/assets/32e52889-6a3e-4d7a-aeba-bb6f7d1dfcc1" />


 **Puntos críticos**  
 
**1.2 Alimentación:** El ULN2003 y el motor necesitan 5V. No intentes alimentarlos desde el pin de 3.3V de la ESP32, no funcionará.

**1.3 Conexión a Tierra (GND):** Es fundamental conectar el GND de la ESP32 con el GND del ULN2003. Esto crea una referencia de voltaje común para que las señales se interpreten correctamente.

**1.4 Múltiples Motores:** Para conectar 4 motores, repetirás este esquema. La ESP32 tiene suficientes GPIOs. Por ejemplo, podrías usar:  

- Motor 1: GPIOs 14, 27, 26, 32

- Motor 2: GPIOs 33, 25, 13, 12

- Motor 3: GPIOs 19, 18, 5, 17

- Motor 4: GPIOs 16, 4, 2, 15  
(Asegúrate de no usar GPIOs reservados como 0, 3, 1 para evitar problemas).

### 2. Programar la ESP32 (Arduino IDE)

Vamos a utilizar el Arduino IDE para programar la ESP32. Si no lo tienes listo para ESP32, primero debes instalar el soporte para estas placas siguiendo una guía rápida.

**Instalación de librerías necesarias:**

 2.1 Abre el Arduino IDE.  
 2.2 Ve a Herramientas -> Gestor de librerías.  
 2.3 Busca e instala PubSubClient de Nick O‘Leary. Esta librería maneja la comunicación MQTT.  
 2.4 La librería Stepper para controlar el motor ya viene incluida por defecto con el Arduino IDE.  

 **Código para la ESP32:**  
 Crea un nuevo sketch y pega el siguiente código. Recuerda modificar las líneas marcadas con tus datos de Wi-Fi y la IP de tu Raspberry Pi.

   ```C++
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
   ```

**Descripción del código:**
- **Conexión Wi-Fi y MQTT:** La ESP32 se conecta a tu red y al broker MQTT de la Raspberry Pi .
  
- **Función callback:** Es el corazón de la comunicación. Se ejecuta automáticamente cuando llega un mensaje al tópico motores/control. Ahí analizamos el mensaje (ej. "1:ON") y cambiamos el estado de las variables que controlan los motores (motor1Estado, motor2Estado, etc.).

- **Control de Motores:** En el loop(), si la variable de estado de un motor es true, se ejecuta motor.step(1). La librería Stepper se encarga de enviar la secuencia correcta de pulsos a los pines GPIO para que el motor gire un paso. La velocidad se controla con motor.setSpeed(RPM).

- **Escalabilidad:** El código está preparado para que añadas fácilmente los motores 3 y 4, solo necesitas definir sus pines, crear el objeto Stepper, y añadir su lógica en el loop() y el callback.

### 3. Configurar el Broker MQTT en Raspberry Pi 5

La Raspberry Pi 5 actuará como tu servidor central (broker MQTT). Usaremos Mosquitto, un broker de código abierto muy popular .

 **3.1 Abre una terminal en tu Raspberry Pi.**
 **3.2 Actualiza el sistema e instala Mosquitto:**

  ```bash
    sudo apt update
    sudo apt upgrade -y
    sudo apt install -y mosquitto mosquitto-clients
   ```

  **3.3 Habilitar el servicio para que arranque automáticamente:**
  ```bash
sudo systemctl enable mosquitto.service
   ```
  **3.4 Verificar que Mosquitto está corriendo:**
  ```bash
sudo systemctl status mosquitto
   ```
Deberías ver algo como active (running).

**3.5 Configuración básica:** 
Por defecto, Mosquitto escucha en el puerto 1883 y permite conexiones anónimas desde tu red local. Para un proyecto casero, esto es suficiente. Si quieres añadir seguridad, puedes editar el archivo de configuración más adelante.

¡Listo! Ya tienes un broker MQTT funcionando.


### 4. Diseñar el Dashboard en Node-RED 
Node-RED es una herramienta visual para conectar dispositivos y servicios. Viene preinstalado en Raspberry Pi OS, pero si no lo tienes, puedes instalarlo.

  **4.1 Iniciar Node-RED:**
  En la terminal de la Raspberry Pi, ejecuta:  
   ```bash
node-red-start
   ``` 
**4.2 Acceder a Node-RED:**  
Abre un navegador web en cualquier ordenador de la misma red y escribe la dirección IP de tu Raspberry Pi seguida del puerto 1880.   
Por ejemplo:  http://192.168.1.X:1880.

**4.3 Instalar nodos del dashboard**  
- Ve al menú (esquina superior derecha) y selecciona "Manage palette".
- Ve a la pestaña "Install".
- Busca @flowfuse/node-red-dashboard y haz clic en Install.


**4.3 Creando el flujo (Flow):**
Vas a construir el flujo que conecta la interfaz de usuario con el broker MQTT. Sigue estos pasos y replica la estructura de la imagen de ejemplo que te proporciono más abajo.

**Nodos de Interfaz de Usuario (UI):**   
Desde la paleta de nodos, arrastra los siguientes elementos del grupo dashboard:

- Cinco nodos button (botón): Para los botones de "Prender Motor 1", "Apagar Motor 1", "Prender Todos", "Apagar Todos", etc.
- Un nodo slider (deslizador): Para controlar la velocidad (RPM) de los motores.
- Un nodo text (texto): Para mostrar el estado o la velocidad actual.

**Nodos MQTT:**  
Del grupo network, arrastra un nodo mqtt out. Este será el que publique los comandos en el broker.

**Conectar y Configurar:**  
- Haz doble clic en el nodo mqtt out para configurarlo.
- En "Server", añade una nueva conexión MQTT. Pon la dirección IP de tu Raspberry Pi (192.168.1.X) y el puerto 1883.
- En "Topic", escribe motores/control (el mismo tópico que la ESP32 está escuchando).
- Conecta la salida de cada botón y del slider a la entrada de este nodo mqtt out.
- Configura cada nodo button para que, al presionarse, envíe el mensaje adecuado.
- Botón "Prender Motor 1": En la propiedad Payload, escribe 1:ON.
- Botón "Apagar Motor 1": Payload = 1:OFF.
- Botón "Prender Todos": Payload = ALL:ON.
- Botón "Apagar Todos": Payload = ALL:OFF.

- Configura el nodo slider para que tenga un rango (ej. 1 a 20) y que al moverse, su Payload sea un texto como SPEED: seguido del valor. Esto se puede hacer con un nodo function entre el slider y el nodo mqtt out. El código de la función sería: msg.payload = "SPEED:" + msg.payload; return msg;.

<img width="1364" height="642" alt="image" src="https://github.com/user-attachments/assets/b9f00bea-f892-4297-b538-e5d6ca0c13f8" />

**Crear el Dashboard:**

- Haz clic en el icono de la barra lateral con dos cuadrados (Dashboard). 
- Crea un nuevo grupo (ej. "Control de Motores") dentro de una nueva pestaña (ej. "Motores"). 
- Asigna cada nodo ui_button y ui_slider a este grupo en su configuración.

¡Prueba! Presiona "Prender Motor 1". Deberías ver el motor girar y en el Serial Monitor de la ESP32 aparecer "Motor 1 Encendido". Luego, prueba a cambiar la velocidad con el slider.

Ahora, para ver tu dashboard, abre un navegador y ve a http://192.168.1.X:1880/ui (usando la IP de tu Raspberry Pi).

## 💡 Uso y Operación

Cómo ejecutar el proyecto una vez instalado.

- Sube el código a tu ESP32 desde el Arduino IDE.
- Abre el Serial Monitor (115200 baudios) para verificar que la ESP32 se conecta al Wi-Fi y al broker MQTT.
- En Node-RED, haz clic en el botón Deploy (rojo, en la esquina superior derecha) para guardar y activar tu flujo.
- Abre un navegador y ve a http://192.168.1.X:1880/ui (usando la IP de tu Raspberry Pi).
- Prueba que los botones interactuen con los motores.
---

## 📚 Documentación Adicional

Enlaces a documentación más detallada, que no encaja en el README.

* [Hoja de Datos - Sensor XYZ](https://...link-al-pdf)
* [Reporte Final de Simulación Estructural](https://...link-al-reporte-en-drive-o-wiki)
* [Diagrama de Flujo del Firmware (Miro/Lucidchart)](https://...link-al-diagrama)
* [Wiki del Proyecto](https://github.com/tu-organizacion/tu-repo/wiki) (¡Altamente recomendado usar el Wiki de GitHub!)



 

---

## 👥 Equipo y Responsables

| Nombre | Rol en el Proyecto | GitHub |
| :--- | :--- | :--- |
| Add Trejo | Desarrollador del proyecto | [@additrejo](https://https://github.com/Additrejo) |

---

## ⚖️ Licencia

Este proyecto está bajo la licencia [Nombre de la Licencia, ej. MIT, CERN-OHL-P, o "Uso Interno Exclusivo del Laboratorio"].
