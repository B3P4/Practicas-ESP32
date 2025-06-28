# Practica simulación Smart House MQTT
### Proyecto conjunto Milena Cardoso y Benicio Pavón
Esta práctica está separada en dos códigos, que representan un controlador de luces (Subscriber) y un panel inteligente para controlar el primero (Publisher).

## Panel inteligente
Consta de cuatro switches, que pueden prender o apagar luces de sus respectivos cuartos, y un LDR que apaga y prende todas las luces según un umbral de luz. Cada switch y LDR tienen un `topic` respectivo donde mandan `ON` u `OFF`.

## Controlador de luces
Para simular la iluminación de una casa se usan 4 LEDs (con lógica negativa para mejor consumo del ESP32). El controlador se suscribe a los 5 `topics` y enciende o apaga las luces dependiendo del mensaje.