SemaphoreHandle_t pista[3];  // Hago un vector de semaforos, uno para cada pista

QueueHandle_t listaEspera;
QueueHandle_t canalComunicacion[5];

int recibirPistaVacia() {
  int pistaVacia;
  for (int i = 0; i < 3; i++) {
    if (xSemaphoreTake(pista[i], 1) == pdTRUE) {
      xSemaphoreGive(pista[i]);
      pistaVacia = i;
      break;
    }
  }

  return pistaVacia;
}

void avion1(void *parameter) {
  int id = 0;
  int aterrizaje;
  while (1) {
    vTaskDelay(pdMS_TO_TICKS(random(3000, 6001)));
    Serial.printf("Avion %d: Solicitando permiso para aterrizar\n", id + 1);
    xQueueSend(listaEspera, &id, portMAX_DELAY);

    xQueueReceive(canalComunicacion[id], &aterrizaje, portMAX_DELAY);
    xSemaphoreTake(pista[aterrizaje], portMAX_DELAY);
    Serial.printf("Avion %d: Aterrizando en la pista %d\n", id + 1, aterrizaje + 1);
    vTaskDelay(pdMS_TO_TICKS(random(5000, 10001)));

    xSemaphoreGive(pista[aterrizaje]);
    Serial.printf("Avion %d: Pista %d liberada\n", id + 1, aterrizaje + 1);
  }
}

void avion2(void *parameter) {
  int id = 1;
  int aterrizaje;
  while (1) {
    vTaskDelay(pdMS_TO_TICKS(random(3000, 6001)));
    Serial.printf("Avion %d: Solicitando permiso para aterrizar\n", id + 1);
    xQueueSend(listaEspera, &id, portMAX_DELAY);

    xQueueReceive(canalComunicacion[id], &aterrizaje, portMAX_DELAY);
    xSemaphoreTake(pista[aterrizaje], portMAX_DELAY);
    Serial.printf("Avion %d: Aterrizando en la pista %d\n", id + 1, aterrizaje + 1);
    vTaskDelay(pdMS_TO_TICKS(random(5000, 10001)));

    xSemaphoreGive(pista[aterrizaje]);
    Serial.printf("Avion %d: Pista %d liberada\n", id + 1, aterrizaje + 1);
  }
}

void avion3(void *parameter) {
  int id = 2;
  int aterrizaje;
  while (1) {
    vTaskDelay(pdMS_TO_TICKS(random(3000, 6001)));
    Serial.printf("Avion %d: Solicitando permiso para aterrizar\n", id + 1);
    xQueueSend(listaEspera, &id, portMAX_DELAY);

    xQueueReceive(canalComunicacion[id], &aterrizaje, portMAX_DELAY);
    xSemaphoreTake(pista[aterrizaje], portMAX_DELAY);
    Serial.printf("Avion %d: Aterrizando en la pista %d\n", id + 1, aterrizaje + 1);
    vTaskDelay(pdMS_TO_TICKS(random(5000, 10001)));

    xSemaphoreGive(pista[aterrizaje]);
    Serial.printf("Avion %d: Pista %d liberada\n", id + 1, aterrizaje + 1);
  }
}

void avion4(void *parameter) {
  int id = 3;
  int aterrizaje;
  while (1) {
    vTaskDelay(pdMS_TO_TICKS(random(3000, 6001)));
    Serial.printf("Avion %d: Solicitando permiso para aterrizar\n", id + 1);
    xQueueSend(listaEspera, &id, portMAX_DELAY);

    xQueueReceive(canalComunicacion[id], &aterrizaje, portMAX_DELAY);
    xSemaphoreTake(pista[aterrizaje], portMAX_DELAY);
    Serial.printf("Avion %d: Aterrizando en la pista %d\n", id + 1, aterrizaje + 1);
    vTaskDelay(pdMS_TO_TICKS(random(5000, 10001)));

    xSemaphoreGive(pista[aterrizaje]);
    Serial.printf("Avion %d: Pista %d liberada\n", id + 1, aterrizaje + 1);
  }
}

void avion5(void *parameter) {
  int id = 4;
  int aterrizaje;
  while (1) {
    vTaskDelay(pdMS_TO_TICKS(random(3000, 6001)));
    Serial.printf("Avion %d: Solicitando permiso para aterrizar\n", id + 1);
    xQueueSend(listaEspera, &id, portMAX_DELAY);

    xQueueReceive(canalComunicacion[id], &aterrizaje, portMAX_DELAY);
    xSemaphoreTake(pista[aterrizaje], portMAX_DELAY);
    Serial.printf("Avion %d: Aterrizando en la pista %d\n", id + 1, aterrizaje + 1);
    vTaskDelay(pdMS_TO_TICKS(random(5000, 10001)));

    xSemaphoreGive(pista[aterrizaje]);
    Serial.printf("Avion %d: Pista %d liberada\n", id + 1, aterrizaje + 1);
  }
}

void torre(void *parameter) {
  int id;
  int pista;
  while (1) {
    xQueueReceive(listaEspera, &id, portMAX_DELAY);
    pista = recibirPistaVacia();
    xQueueSend(canalComunicacion[id], &pista, portMAX_DELAY);
    Serial.printf("Torre de control: Permiso concedido para el avión %d para aterrizar en la pista %d\n", id + 1, pista + 1);
  }
}

void setup() {
  Serial.begin(115200);

  pista[0] = xSemaphoreCreateMutex();
  pista[1] = xSemaphoreCreateMutex();
  pista[2] = xSemaphoreCreateMutex();

  listaEspera = xQueueCreate(5, sizeof(int));

  canalComunicacion[0] = xQueueCreate(1, sizeof(int));
  canalComunicacion[1] = xQueueCreate(1, sizeof(int));
  canalComunicacion[2] = xQueueCreate(1, sizeof(int));
  canalComunicacion[3] = xQueueCreate(1, sizeof(int));
  canalComunicacion[4] = xQueueCreate(1, sizeof(int));

  xTaskCreatePinnedToCore(
    torre,    // Referencia a la funcion que vamos a ejecutar
    "torre",  // Nombre para la funcion, sirve solamente para propositos de debugging
    4096,     // Tamaño del stack la tarea
    NULL,     // Parametro que recibe la funcion que le vamos a pasar
    1,        // Prioridad de la tarea
    NULL,     // no es importante
    1         // El core donde queremos que corra la task (0/1)
  );

  xTaskCreatePinnedToCore(
    avion1,    // Referencia a la funcion que vamos a ejecutar
    "avion1",  // Nombre para la funcion, sirve solamente para propositos de debugging
    4096,      // Tamaño del stack la tarea
    NULL,      // Parametro que recibe la funcion que le vamos a pasar
    1,         // Prioridad de la tarea
    NULL,      // no es importante
    0          // El core donde queremos que corra la task (0/1)
  );
  xTaskCreatePinnedToCore(
    avion2,    // Referencia a la funcion que vamos a ejecutar
    "avion2",  // Nombre para la funcion, sirve solamente para propositos de debugging
    4096,      // Tamaño del stack la tarea
    NULL,      // Parametro que recibe la funcion que le vamos a pasar
    1,         // Prioridad de la tarea
    NULL,      // no es importante
    0          // El core donde queremos que corra la task (0/1)
  );
  xTaskCreatePinnedToCore(
    avion3,    // Referencia a la funcion que vamos a ejecutar
    "avion3",  // Nombre para la funcion, sirve solamente para propositos de debugging
    4096,      // Tamaño del stack la tarea
    NULL,      // Parametro que recibe la funcion que le vamos a pasar
    1,         // Prioridad de la tarea
    NULL,      // no es importante
    0          // El core donde queremos que corra la task (0/1)
  );
  xTaskCreatePinnedToCore(
    avion4,    // Referencia a la funcion que vamos a ejecutar
    "avion4",  // Nombre para la funcion, sirve solamente para propositos de debugging
    4096,      // Tamaño del stack la tarea
    NULL,      // Parametro que recibe la funcion que le vamos a pasar
    1,         // Prioridad de la tarea
    NULL,      // no es importante
    0          // El core donde queremos que corra la task (0/1)
  );
  xTaskCreatePinnedToCore(
    avion5,    // Referencia a la funcion que vamos a ejecutar
    "avion5",  // Nombre para la funcion, sirve solamente para propositos de debugging
    4096,      // Tamaño del stack la tarea
    NULL,      // Parametro que recibe la funcion que le vamos a pasar
    1,         // Prioridad de la tarea
    NULL,      // no es importante
    0          // El core donde queremos que corra la task (0/1)
  );
}




void loop() {
}
