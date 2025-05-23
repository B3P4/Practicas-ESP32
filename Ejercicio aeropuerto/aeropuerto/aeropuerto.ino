SemaphoreHandle_t pista[3];  // Hago un vector de semaforos, uno para cada pista

QueueHandle_t listaEspera;           // Cola que lleva el orden de los aviones que piden permiso de aterrizar
QueueHandle_t canalComunicacion[5];  // Vector de colas  que comunican la torre con los aviones. Específicamente para que la torre indique en que pista aterrizar

int recibirPistaVacia() {    //Creo una funcion que devuelva una pista vacia.
  int pistaVacia = 69;        //Creo una variable para la pista vacia. Le asigno un valor cualquiera que no corresponda a una pista para
  while (pistaVacia == 69) {  ///que este while funcione mientras la pista no haya sido asignada  
    for (int i = 0; i < 3; i++) {
      if (xSemaphoreTake(pista[i], 1) == pdTRUE) {  //Recorre cada pista y trata de tomarla, el tiempo de espera es de 1ms asi si el semaforo esta tomado pasa al siguiente
        xSemaphoreGive(pista[i]);                   //Apenas toma el semaforo lo libera
        pistaVacia = i;
        break; //Sale del for una vez que encuentra una pista vacia
      }
    }
    vTaskDelay(pdMS_TO_TICKS(100)); //Pequeño delay para dejar un timepo si no encontro pistas vacias 
  }
  return pistaVacia;
}

void avion(void *pvParameters) {  //Tarea para el avion
  int id = (int)pvParameters;     //Recibo un parametro que servira para identificar los aviones
  int aterrizaje;                 //Variable que va indicar la pista donde aterrizar
  while (1) {
    vTaskDelay(pdMS_TO_TICKS(random(3000, 6001)));  //Espera un timepo aleatorio entre 3 y 6 segundos
    Serial.printf("Avion %d: Solicitando permiso para aterrizar\n", id + 1);
    xQueueSend(listaEspera, &id, portMAX_DELAY);  //El avion se envia a la cola de espera

    xQueueReceive(canalComunicacion[id], &aterrizaje, portMAX_DELAY);  //Espera a que la torre le indique en que pista aterrizar
    xSemaphoreTake(pista[aterrizaje], portMAX_DELAY);
    Serial.printf("Avion %d: Aterrizando en la pista %d\n", id + 1, aterrizaje + 1);
    vTaskDelay(pdMS_TO_TICKS(random(5000, 10001)));  //Ocupa la pista un tiempo aleatorio entre 5 y 10 segundos

    Serial.printf("Avion %d: Pista %d liberada\n", id + 1, aterrizaje + 1);
    xSemaphoreGive(pista[aterrizaje]);
  }
}


void torre(void *parameter) {
  int id;
  int pista;
  while (1) {
    xQueueReceive(listaEspera, &id, portMAX_DELAY);  //Recibe el primer avion de la cola
    pista = recibirPistaVacia();                     //Recibe una pista libre

    Serial.printf("Torre de control: Permiso concedido para el avión %d para aterrizar en la pista %d\n", id + 1, pista + 1);
    xQueueSend(canalComunicacion[id], &pista, portMAX_DELAY);  //Envia el numero de la pista libre al avion
    vTaskDelay(pdMS_TO_TICKS(100));                            //Delay porque a veces la torre le concede el permiso para aterrizar a la misma pista a dos aviones seguidos
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
    2,        // Prioridad de la tarea
    NULL,     // no es importante
    1         // El core donde queremos que corra la task (0/1)
  );


  for (int i = 0; i < 5; i++) {
    xTaskCreatePinnedToCore(
      avion,      // Referencia a la funcion que vamos a ejecutar
      "avion",    // Nombre para la funcion, sirve solamente para propositos de debugging
      4096,       // Tamaño del stack la tarea
      (void *)i,  // Parametro que recibe la funcion que le vamos a pasar
      1,          // Prioridad de la tarea
      NULL,       // no es importante
      0           // El core donde queremos que corra la task (0/1)
    );
  }
}

void loop() {
}
