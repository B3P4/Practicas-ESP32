# Práctica chat MQTT

Esta práctica consiste en implementar un sistema de chat básico utilizando comunicación MQTT entre ESP32s conectados a través de WiFi.

## Etapas

### Conexión al servidor

El primer paso es conectarse a un **broker** con las funciones `setServer` y `connect`.

### Subscriber

Suscribirse significa conectarse a un topic y esperar recibir mensajes. Para esto se usan las funciones `subscribe(topic)`, `loop()` y `setCallback()`

### Publisher

Publicar significa enviar mensajes a un topic específico. Para esto se usa la funcion `publish(topic, message)`