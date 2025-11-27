
#include <esp_now.h>
#include <WiFi.h>
#include "driver/rtc_io.h"

#define BUTTON_PIN_BITMASK(GPIO) (1ULL << GPIO)  // 2 ^ GPIO_NUMBER in hex
#define USE_EXT0_WAKEUP          1                     // 1 = EXT0 wakeup, 0 = EXT1 wakeup
#define WAKEUP_GPIO              GPIO_NUM_35

// REPLACE WITH THE RECEIVER'S MAC Address
uint8_t broadcastAddress[] = {
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};
// Must match the receiver structure
typedef struct contador {
    int id; // must be unique for each sender board
    bool stt_carrinho;
} contador;

// Create a struct_message called myData
contador myData;

// Create peer interface
esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

const byte pinFttrans = 35; //pino do fototransistor
               
void setup(){
  Serial.begin(115200); //inicializa o serial
  pinMode(pinFttrans, INPUT); //pino de entrada
  
  WiFi.mode(WIFI_STA);// põe o esp como station

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  esp_sleep_enable_ext0_wakeup(WAKEUP_GPIO, 0);  //põe o esp em deep sleep e acorda qnd o pino 35 ler low
  rtc_gpio_pullup_dis(WAKEUP_GPIO);
  rtc_gpio_pulldown_en(WAKEUP_GPIO);
  myData.id = 1;
  myData.stt_carrinho = true;
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
}
 
void loop(){
  Serial.print(analogRead(pinFttrans)); // escreve a leitura do pino 35
  Serial.println(" Recebendo sinal infravermelho"); //escreve a frase
  
  Serial.println("Going to sleep now");//escreve a frase
  esp_deep_sleep_start();
}
