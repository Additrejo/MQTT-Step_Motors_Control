

   
#  **3.2 Actualiza el sistema e instala Mosquitto:**
   sudo apt update
   sudo apt upgrade -y
   sudo apt install -y mosquitto mosquitto-clients
	
#  **3.3 Habilitar el servicio para que arranque automáticamente:**

   sudo systemctl enable mosquitto.service
   
# **3.4 Verificar que Mosquitto está corriendo:**

   sudo systemctl status mosquitto
   
# **4.1 Iniciar Node-RED:**

   node-red-start
   