#include <Arduino.h>

#define BIT_WIFI_OK   (1 << 0)
#define BIT_SENSOR_OK (1 << 1)

EventGroupHandle_t xGrupoDeEventos = NULL;

void TaskConectaWifi(void *pv){
  Serial.println("[WI-FI] Iniciando conexão...");
  vTaskDelay(3000/portTICK_PERIOD_MS);

  Serial.println("[WI-FI] Conectado com Sucesso! Ligando flag de WIFI_OK...");
  xEventGroupSetBits(xGrupoDeEventos, BIT_WIFI_OK);

  vTaskDelete(NULL);
}

void TaskCalibraSensor(void *pv){
  Serial.println("[SENSORES] Iniciando Calibração dos sensores...");
  vTaskDelay(3000/portTICK_PERIOD_MS);

  Serial.println("[SENSORES] Calibragem Concluída! Ativando Flags de Sensor_OK...");
  xEventGroupSetBits(xGrupoDeEventos, BIT_SENSOR_OK);

  vTaskDelete(NULL);
}

void taskMain(void *pv){
  while (1)
  {
    //checar se todos os sistemas estão OK (wifi e sensor)
    Serial.println("[MESTRA] Aguardando WIFI e SENSOR ficarem prontos ...");

    EventBits_t bitsResultantes = xEventGroupWaitBits(
      xGrupoDeEventos,
      BIT_WIFI_OK | BIT_SENSOR_OK,
      pdTRUE,
      pdTRUE,
      portMAX_DELAY
    );

    Serial.println("\n==================================================");
    Serial.println("[MESTRA] TODOS OS HARDWARES PRONTOS!");
    Serial.printf("[MESTRA] Logs dos bits que acordaram a task: 0x%X\n", bitsResultantes);
    Serial.println("[MESTRA] Executando loop de transmissão de dados na nuvem...");
    Serial.println("==================================================\n");

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }  
}

void setup() {
  Serial.begin(115200);

  vTaskDelay(5000/portTICK_PERIOD_MS);

  xGrupoDeEventos = xEventGroupCreate();

  if(xGrupoDeEventos != NULL){
    xTaskCreate(TaskConectaWifi, "wifi", 2048, NULL,1,NULL);
    xTaskCreate(TaskCalibraSensor,"sensor", 2048, NULL,1, NULL);

    xTaskCreate(taskMain,"mestra", 2048, NULL, 2, NULL);
  }
  else{
    Serial.println("Houve um erro ao criar o grupo de eventos!");
  }
}

void loop() {
  // Vazio
}