#include "data.h"

#define verde 14      //Defino pines del esp32
#define amarillo 12
#define rojo 13

QueueHandle_t cola;

void setRGY(bool R, bool G, bool Y) {   //Funcion para prender y apagar los LEDs
  if (R) {
    digitalWrite(rojo, HIGH);
  } else {
    digitalWrite(rojo, LOW);
  }

  if (G) {
    digitalWrite(verde, HIGH);
  } else {
    digitalWrite(verde, LOW);
  }

  if (Y) {
    digitalWrite(amarillo, HIGH);
  } else {
    digitalWrite(amarillo, LOW);
  }
}

void productor(void *parameter) {
  LedCommand comando;
  while (1) {
    comando = randomCommand(); 
    if (comando == LED_BLINK) {
      xQueueSendToFront(cola, &comando, portMAX_DELAY);  // Envía al principio de la cola si es LED_BLINK
    } else {
      xQueueSend(cola, &comando, portMAX_DELAY);      // Envía al final de la cola en el resto de casos
    }
  }
}

void consumidor(void *parameter) {
  LedCommand comando;
  while (1) {
    xQueueReceive(cola, &comando, portMAX_DELAY);
    if (comando == LED_BLINK) {                    //Genero un parpadeo de los 3 LEDs
      setRGY(1, 1, 1);   
      vTaskDelay(pdMS_TO_TICKS(400));
      setRGY(0, 0, 0);
      vTaskDelay(pdMS_TO_TICKS(200));
      setRGY(1, 1, 1);
      vTaskDelay(pdMS_TO_TICKS(400));
    }
    if (comando == LED_RED) {
      setRGY(1, 0, 0);
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
    if (comando == LED_GREEN) {
      setRGY(0, 1, 0);
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
    if (comando == LED_YELLOW) {
      setRGY(0, 0, 1);
      vTaskDelay(pdMS_TO_TICKS(1000));
    }

    setRGY(0, 0, 0);                 //Apago todos los LEDs y dejo 1 seg hasta el siguiente comando
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}



void setup() {
  Serial.begin(115200);

  cola = xQueueCreate(30, sizeof(LedCommand));

  pinMode(verde, OUTPUT);
  pinMode(amarillo, OUTPUT);
  pinMode(rojo, OUTPUT);


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
