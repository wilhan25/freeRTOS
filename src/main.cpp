#include <Arduino.h>

//mutex
SemaphoreHandle_t mutexSerial;

void TaskPlacaA(void *pv);
void TaskPlacaB(void *pv);


void setup() {
  Serial.begin(115200);

  //config do mutex
  mutexSerial = xSemaphoreCreateMutex();
  if(mutexSerial == NULL){
    Serial.println("Erro ao criar mutex");
    return;
  }

  //config das tasks
  xTaskCreate(
    TaskPlacaA,
    "Placa A",
    2048,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    TaskPlacaB,
    "Placa B",
    2048,
    NULL,
    1,
    NULL
  );
}

void loop(){}

void TaskPlacaA(void *pv){
  while (1)
  {
    /* code */
    if(xSemaphoreTake(mutexSerial, portMAX_DELAY)==pdTRUE){
      Serial.print("[Tarefa A] Iniciando envio... ");
      vTaskDelay(100/portTICK_PERIOD_MS);
      Serial.println("Finalizado com sucesso ! esp32 Liberado !");

      xSemaphoreGive(mutexSerial);
    }
    vTaskDelay(500/portTICK_PERIOD_MS);
  }  
}

void TaskPlacaB(void *pv){
  while (1)
  {
    /* code */
    if(xSemaphoreTake(mutexSerial, portMAX_DELAY)==pdTRUE){
      Serial.print("{Tarefa B} Coletando Dados Fictício... ");
      Serial.println("Coelta concluída sem erros.");

      xSemaphoreGive(mutexSerial);
    }

    vTaskDelay(500/portTICK_PERIOD_MS);
  }  
}