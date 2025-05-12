void tarea(void *parameter) {
  int core = xPortGetCoreID();  // Obtenemos el ID del core actual

  Serial.print("Ejecutando en el core:  ");
  Serial.println(core);

  while (1){
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void setup() {
  Serial.begin(115200);  // Inicializamos el puerto serie
  delay(100);            // Esperamos un poco para que el Serial esté listo

  // Creamos la tarea y la fijamos al core 1
  xTaskCreatePinnedToCore(
    tarea,         // Función que ejecuta la tarea
    "Task_Core1",  // Nombre de la tarea
    4096,          // Tamaño del stack en palabras
    NULL,          // Parámetro que se le pasa a la tarea
    1,             // Prioridad de la tarea
    NULL,          // Handle (opcional, no lo usamos aquí)
    1              // Core donde queremos que corra la tarea (0 o 1)
  );
  
  xTaskCreatePinnedToCore(
    tarea,         // Función que ejecuta la tarea
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
