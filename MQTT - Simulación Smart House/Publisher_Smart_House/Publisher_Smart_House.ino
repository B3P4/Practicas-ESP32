#include <WiFi.h>
#include <PubSubClient.h>

#define ldr 14
#define living 2
#define dormitorio 21
#define toilette 5
#define cocina 18

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

//Flags para saber si enviar on/off
bool flagCocina = 0;
bool flagLiving = 0;
bool flagBaño = 0;
bool flagDormitorio = 0;

bool flagLDR = 1; //Este flag tambien evita que se envie infinitamente ON/OFF


void setup() {

  Serial.begin(115200);
  pinMode(cocina, INPUT);
  pinMode(living, INPUT);
  pinMode(dormitorio, INPUT);
  pinMode(toilette, INPUT);
  pinMode(ldr, INPUT);
  // Conectarse a una red por nombre + contraseña
  WiFi.begin(internet, password);
  Serial.print("Conectando");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nSuccess");

  // Realizamos la conexión con el servidor
  client.setServer("", );  // Vacio por seguridad
  client.connect(userName);                    
  Serial.print("Conectando a broker");

  while (!client.connected()) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nSuccess");
}
void loop() {

  if (digitalRead(cocina) == 0) {
    if (flagCocina) {
      client.publish(topicCocina, "OFF");
      Serial.println("Cocina OFF");
    } else {
      client.publish(topicCocina, "ON");
      Serial.println("Cocina ON");
    }
    flagCocina = !flagCocina;
    delay(600);  //Para evitar al ruido del pulsador
  }
  if (digitalRead(living) == 0) {
    if (flagLiving) {
      client.publish(topicLiving, "OFF");
      Serial.println("Living OFF");
    } else {
      client.publish(topicLiving, "ON");
      Serial.println("Living ON");
    }
    flagLiving = !flagLiving;
    delay(600);  //Para evitar al ruido del pulsador
  }
  if (digitalRead(toilette) == 0) {
    if (flagBaño) {
      client.publish(topicBaño, "OFF");
      Serial.println("Baño OFF");
    } else {
      client.publish(topicBaño, "ON");
      Serial.println("Baño ON");
    }
    flagBaño = !flagBaño;
    delay(600);  //Para evitar al ruido del pulsador
  }
  if (digitalRead(dormitorio) == 0) {
    if (flagDormitorio) {
      client.publish(topicDormitorio, "OFF");
      Serial.println("Dormitorio OFF");
    } else {
      client.publish(topicDormitorio, "ON");
      Serial.println("Dormitorio ON");
    }
    flagDormitorio = !flagDormitorio;
    delay(600);  //Para evitar al ruido del pulsador
  }

  if (analogRead(ldr) > 2500 && flagLDR) { //Cuando se supere un umbral de oscuridad se prenden todas las luces
    client.publish(topicTodas, "ON");
    Serial.println("Todas ON");

    flagLDR = 0;

    flagCocina = 1;
    flagDormitorio = 1;
    flagBaño = 1;
    flagLiving = 1;
  }
  if (analogRead(ldr) < 2400 && !flagLDR) { //Cuando se supere un umbral de luz se apagan todas las luces
    client.publish(topicTodas, "OFF");
    Serial.println("Todas OFF");

    flagLDR = 1;

    flagCocina = 0;
    flagDormitorio = 0;
    flagBaño = 0;
    flagLiving = 0;
  }
}