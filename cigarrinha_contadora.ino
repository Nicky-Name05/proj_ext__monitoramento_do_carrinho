
#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH THE RECEIVER'S MAC Address
uint8_t broadcastAddress[] = {
0x94, 0xb5, 0x55, 0x26, 0x76, 0xcc
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

const byte pinoFototransistor = 35; //PINO ANALÓGICO UTILIZADO PELO FOTOTRANSISTOR
               
void setup(){
  Serial.begin(115200); //INICIALIZAÇÃO DA SERIAL
  pinMode(pinoFototransistor, INPUT); //DEFINE O PINO COMO ENTRADA
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

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
}
 
void loop(){
  if(analogRead(pinoFototransistor) < 2500){ //SE A LEITURA DO FOTOTRANSISTOR FOR MENOS QUE 1020 BITS
    Serial.print(analogRead(pinoFototransistor)); // IMPRIME O VALOR LIDO
    Serial.println(" Recebendo sinal infravermelho"); //IMPRIME O TEXTO NA SERIAL
    myData.id= 1;
    myData.stt_carrinho = true;
    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    
    if (result == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
  }else{ 
    Serial.print(analogRead(pinoFototransistor)); // IMPRIME O VALOR LIDO
    Serial.println(" Sinal infravermelho interrompido"); //IMPRIME O TEXTO NA SERIAL
  }
  delay(100);
}