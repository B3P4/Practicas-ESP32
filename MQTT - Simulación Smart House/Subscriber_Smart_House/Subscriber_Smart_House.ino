#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;

// Creamos un cliente de MQTT con la conexión a internet via wifi
PubSubClient client(espClient);

//Vacio para seguridad
String internet = "";
String password = "";
const char* userName = "";
const char* topicCocina = "";
const char* topicLiving = "";
const char* topicDormitorio = "";
const char* topicBaño = "";
const char* topicTodas = "";


String surname = "";

void callback(char* topic, byte* payload, unsigned int length) {
  // Iteramos el payload
  String msg;

  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }
  Serial.println(msg);

  if (String(topic) == String(topicCocina)) {
    if (msg == "ON") {
      digitalWrite(19, LOW);
    }

    else if (msg == "OFF") {
      digitalWrite(19, HIGH);
    }
  }

  if (String(topic) == String(topicLiving)) {
    if (msg == "ON") {
      digitalWrite(21, LOW);
    }

    else if (msg == "OFF") {
      digitalWrite(21, HIGH);
    }
  }


  if (String(topic) == String(topicDormitorio)) {
    if (msg == "ON") {
      digitalWrite(22, LOW);
    }

    else if (msg == "OFF") {
      digitalWrite(22, HIGH);
    }
  }

  if (String(topic) == String(topicBaño)) {
    if (msg == "ON") {
      digitalWrite(23, LOW);
    }

    else if (msg == "OFF") {
      digitalWrite(23, HIGH);
    }
  }
  if (String(topic) == String(topicTodas)) {
    if (msg == "ON") {
      digitalWrite(19, LOW);
      digitalWrite(21, LOW);
      digitalWrite(22, LOW);
      digitalWrite(23, LOW);
    }

    else if (msg == "OFF") {
      digitalWrite(19, HIGH);
      digitalWrite(21, HIGH);
      digitalWrite(22, HIGH);
      digitalWrite(23, HIGH);
    }
  }
}

void setup() {

  Serial.begin(115200);
  pinMode(19, OUTPUT);
  pinMode(21, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);

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
  client.setServer("", );  // Vacio por seguridad
  client.connect(userName);                     // devuelve un booleano indicando si se conecto correctamente
  client.subscribe(topicLiving);
  client.subscribe(topicCocina);
  client.subscribe(topicDormitorio);
  client.subscribe(topicBaño);
  client.subscribe(topicTodas);
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
}