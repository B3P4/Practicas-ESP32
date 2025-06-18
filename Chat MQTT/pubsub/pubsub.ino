#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;

// Creamos un cliente de MQTT con la conexión a internet via wifi
PubSubClient client(espClient);

//Vacio para seguridad
String internet = "";
String password = "";
const char* userName = "";
const char* topicSubscribed = "";
const char* topicPublisher = "";
String surname = "";

void callback(char* topic, byte* payload, unsigned int length) {
  // Iteramos el payload
  String receivedMsg;

  for (int i = 0; i < length; i++) {
    receivedMsg += (char)payload[i];
  }
  Serial.println(receivedMsg);
}


void setup() {

  Serial.begin(115200);
  // Conectarse a una red por nombre + contraseña
  WiFi.begin(internet, password);
  Serial.print("Conectando");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  // ok
  Serial.println("\nSuccess");

  // Realizamos la conexión con el servidor
  client.setServer("", );    // Vacio por seguridad
  client.connect(userName);  // devuelve un booleano indicando si se conecto correctamente
  client.subscribe(topicSubscribed);
  Serial.print("Conectando a broker");

  while (!client.connected()) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nSuccess");

  client.setCallback(callback);
}
void loop() {
  client.loop();

  if (Serial.available()) {
    if (Serial.available()) {
      String msg = Serial.readStringUntil('\n');
      msg = surname + msg;
      client.publish(topicPublisher, msg.c_str());
    }
  }
}