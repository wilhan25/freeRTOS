#include <Arduino.h>

#define led_pin 2
#define ldr_pin 34

//controlador da fila
QueueHandle_t filaLuminosidade;

//funções
void TaskLeituraLDR(void *pv);
void ControleLED(void *pv);


void setup() {
  Serial.begin(115200);

  // criação da fila
  filaLuminosidade = xQueueCreate(5,sizeof(int));
  if(filaLuminosidade == NULL){
    Serial.println("Erro ao criar fila");
    return;
  }

  // criação das tasks
  xTaskCreate(
    TaskLeituraLDR,
    "LerLDR",
    2048,
    NULL,
    1,
    NULL
  );

  xTaskCreate(
    ControleLED,
    "controleLED",
    2048,
    NULL,
    1,
    NULL
  );  
}

void loop(){}

void TaskLeituraLDR(void *pv){
  pinMode(ldr_pin,INPUT);

  while (1)
  {
    int leituraLDR = analogRead(ldr_pin);

    //fila para mandar os dados 
    xQueueSend(filaLuminosidade, &leituraLDR, 10/portTICK_PERIOD_MS);

    vTaskDelay(200/portTICK_PERIOD_MS);
  }  
}

void ControleLED(void *pv){
  pinMode(led_pin, OUTPUT);
  int valorRecebido;

  while (1)
  {
    if(xQueueReceive(filaLuminosidade, &valorRecebido, portMAX_DELAY) == pdPASS){
      Serial.print("Valor do LDR: ");
      Serial.print(valorRecebido);
      
      if(valorRecebido > 1500){
        digitalWrite(led_pin,HIGH);
        Serial.println("Ligando led.");
      }
      else{
        digitalWrite(led_pin, LOW);
        Serial.println("Ta claro já, desliga o LED !");
      }
      Serial.println("-----------------------");
    }
  }  
}
