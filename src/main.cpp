#include <Arduino.h>

#define btn_pin 18
#define led_pin 2

TaskHandle_t xTaskSireneBombeiros = NULL;

void IRAM_ATTR isr_atendente(){
  BaseType_t xTarefaMaisImportanteAcordou = pdFALSE;

  vTaskNotifyGiveFromISR(xTaskSireneBombeiros, &xTarefaMaisImportanteAcordou);
  if(xTarefaMaisImportanteAcordou == pdTRUE){
    portYIELD_FROM_ISR();
  }
}

void TaskBombeiro(void *pv){
  pinMode(btn_pin, INPUT_PULLUP);
  pinMode(led_pin, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(btn_pin), isr_atendente, FALLING);

  int cliques = 0;
  while (1)
  {
    /* code */
    ulTaskNotifyTake(pdTRUE,portMAX_DELAY);
    cliques++;
    digitalWrite(led_pin, !digitalRead(led_pin));
    Serial.print("[BOMBEIRO] Atendi o chamado! clique número: ");
    Serial.println(cliques);
    vTaskDelay(250/portTICK_PERIOD_MS);
    ulTaskNotifyTake(pdTRUE,0);
    Serial.println("[BOMBEIRO] Voltei pro quartel esperando o clique .... \n");
  }  
}

void setup() {
  Serial.begin(115200);

  xTaskCreate(TaskBombeiro, "bombeiro", 2048,NULL,3,&xTaskSireneBombeiros);
}

void loop() {
  // Vazio
}