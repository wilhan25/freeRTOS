#include <Arduino.h>

#define LED_PIN 2

// Ponteiros para os nossos temporizadores
TimerHandle_t xTimerPiscaLed = NULL;
TimerHandle_t xTimerDesligaSistema = NULL;

// 1. CALLBACK DO TIMER AUTO-RELOAD (Executa repetidamente)
void CallbackPiscaLed(TimerHandle_t xTimer) {
  // Lembre-se: esta função roda dentro da Timer Daemon Task.
  // Ela deve ser ultra-rápida. Sem delays aqui dentro!
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  Serial.println("[TIMER] LED Invertido (Auto-Reload)");
}

// 2. CALLBACK DO TIMER ONE-SHOT (Executa apenas uma vez)
void CallbackDesligaSistema(TimerHandle_t xTimer) {
  Serial.println("\n==================================================");
  Serial.println("[TIMER] O tempo de inatividade acabou! (One-Shot)");
  Serial.println("[TIMER] Desligando módulos de hardware...");
  Serial.println("==================================================\n");
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  Serial.println("Criando os Software Timers...");

  // Criando o Timer do LED (Auto-Reload)
  xTimerPiscaLed = xTimerCreate(
    "TimerLED",                     // Nome do timer
    pdMS_TO_TICKS(1000),           // Período: 1000ms (1 segundo)
    pdTRUE,                        // Auto-Reload? pdTRUE = Sim / pdFALSE = One-Shot
    (void *)0,                     // ID do timer (opcional, pode ser usado para identificar o timer)
    CallbackPiscaLed               // A função de callback que ele vai chamar
  );

  // Criando o Timer de Desligamento (One-Shot)
  xTimerDesligaSistema = xTimerCreate(
    "TimerDesliga",
    pdMS_TO_TICKS(7000),           // Período: 7000ms (7 segundos)
    pdFALSE,                       // Auto-Reload? pdFALSE = Não (roda só uma vez)
    (void *)1,
    CallbackDesligaSistema
  );

  // IMPORTANTE: Os timers são criados "dormentes". Precisamos iniciá-los!
  if (xTimerPiscaLed != NULL && xTimerDesligaSistema != NULL) {
    // O segundo parâmetro (0) é o tempo máximo que o sistema aceita esperar se a fila do timer estiver cheia
    xTimerStart(xTimerPiscaLed, 0);
    xTimerStart(xTimerDesligaSistema, 0);
    Serial.println("Timers iniciados com sucesso!");
  }
}

void loop() {
  // O loop principal fica completamente livre para outras coisas!
  vTaskDelay(5000 / portTICK_PERIOD_MS);
  Serial.println("[LOOP] Eu continuo rodando aqui no meu canto...");
}