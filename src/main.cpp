#include <Arduino.h>

void Task0(void *pv);
void Task1(void *pv);

void setup() {
  Serial.begin(115200);

  xTaskCreatePinnedToCore(
    Task0,
    "task0",
    2048,
    NULL,
    1,
    NULL,
    0
  );

  xTaskCreatePinnedToCore(
    Task1,
    "task1",
    2048,
    NULL,
    1,
    NULL,
    1
  );
}

void loop(){}

void Task0 (void *pv){
  while (1)
  {
    /* code */
    Serial.print("[TASK 0] - Olá! estourodando no Core: ");
    Serial.println(xPortGetCoreID());
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }  
}

void Task1(void *pv){
  while (1)
  {
    /* code */
    Serial.print("[TASK 1] - Olá! Estou rodando no Core: ");
    Serial.println(xPortGetCoreID());

    vTaskDelay(1000/portTICK_PERIOD_MS);
  }  
}
