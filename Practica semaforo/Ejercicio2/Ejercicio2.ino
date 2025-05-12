#define verde 14
#define amarillo 12
#define rojo 13


void semaforo(void *parameter) {
  while (1) {
    digitalWrite(rojo, LOW);
    digitalWrite(amarillo, HIGH);
    vTaskDelay(pdMS_TO_TICKS(1000));

    digitalWrite(amarillo, LOW);
    digitalWrite(verde, HIGH);
    vTaskDelay(pdMS_TO_TICKS(4000));

    digitalWrite(verde, LOW);
    digitalWrite(amarillo, HIGH);
    vTaskDelay(pdMS_TO_TICKS(1000));

    digitalWrite(amarillo, LOW);
    digitalWrite(rojo, HIGH);
    vTaskDelay(pdMS_TO_TICKS(4000));
  }
}

void setup() {
  Serial.begin(115200);  // Inicializamos el puerto serie
  delay(100);            // Esperamos un poco para que el Serial esté listo

  pinMode(verde, OUTPUT);
  pinMode(amarillo, OUTPUT);
  pinMode(rojo, OUTPUT);

  digitalWrite(rojo, HIGH);
  delay(4000);

  xTaskCreatePinnedToCore(
    semaforo,      // Función que ejecuta la tarea
    "Task_Core2",  // Nombre de la tarea
    4096,          // Tamaño del stack en palabras
    NULL,          // Parámetro que se le pasa a la tarea
    1,             // Prioridad de la tarea
    NULL,          // Handle (opcional, no lo usamos aquí)
    0              // Core donde queremos que corra la tarea (0 o 1)
  );
}

void loop() {
}
