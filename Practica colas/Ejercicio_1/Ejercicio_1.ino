#include "data.h"


QueueHandle_t cola;

void productor(void *parameter) {
  Person personaTemporal;
  for (int i = 0; i < 200; i++) {                 // Se ejecuta una vez por persona
    vTaskDelay(pdMS_TO_TICKS(random(500, 2001))); // Espera aleatoriamente entre 500 y 2000 ms

    personaTemporal = getPersonById(i + 1);    // se guarda temporalmente la persona correspondiente a la pasada del for

    xQueueSend(cola, &personaTemporal, portMAX_DELAY); //se envia a la cola
  }
  while (1) {
    vTaskDelay(pdMS_TO_TICKS(10000));   //Una vez que pasa por todas las personas se queda en un bucle sin hacer nada
  }
}

void consumidor(void *parameter) {
  Person personaTemporal;
  while (1) {
    vTaskDelay(pdMS_TO_TICKS(1000));
    xQueueReceive(cola, &personaTemporal, portMAX_DELAY);  // Recibimos una persona de la cola 
    //Inicia la impresion de datos
    Serial.print("ID: ");
    Serial.println(personaTemporal.id);

    Serial.print("Nombre: ");
    Serial.print(personaTemporal.name+ " ");
    Serial.println(personaTemporal.last_name);

    Serial.print("Email: ");
    Serial.println(personaTemporal.last_name);
    //Finaliza la impresion de datos
  }
}


void setup() {
  Serial.begin(115200);

  cola = xQueueCreate(30, sizeof(Person)); // creamos cola de tipo Person con una capacidad de 30

  xTaskCreatePinnedToCore(
    productor,     // Referencia a la funcion que vamos a ejecutar
    "Task_Core0",  // Nombre para la funcion, sirve solamente para propositos de debugging
    4096,          // Tamaño del stack la tarea
    NULL,          // Parametro que recibe la funcion que le vamos a pasar
    1,             // Prioridad de la tarea
    NULL,          // no es importante
    0              // El core donde queremos que corra la task (0/1)
  );

  xTaskCreatePinnedToCore(
    consumidor,    // Referencia a la funcion que vamos a ejecutar
    "Task_Core1",  // Nombre para la funcion, sirve solamente para propositos de debugging
    4096,          // Tamaño del stack la tarea
    NULL,          // Parametro que recibe la funcion que le vamos a pasar
    1,             // Prioridad de la tarea
    NULL,          // no es importante
    1              // El core donde queremos que corra la task (0/1)
  );
}

void loop() {
}
