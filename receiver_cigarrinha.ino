#include <esp_now.h>
#include <WiFi.h>

#define TXD1 19
#define RXD1 21

int n = 2; // numero de cigarrinhas
int parada; // variavel que diz qual foi a parada q o carrinho passou
bool passagem = false; // variavel usada pra marcar q houve uma passagem 
bool stt_servico = false; // variavel q diz se o carrinho ta em rota ou nn


HardwareSerial mySerial(1);
// Must match the senders' structure
typedef struct contador {z
    int id; // must be unique for each sender board
    bool stt_carrinho;
} contador;

// Create a contador called myData
contador myData;
// Create a structure to hold the readings from each board
contador ponto1;
contador ponto2;


// Create an array with all the structures
contador paradas[n] = {ponto1, ponto2};

void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("Parada  %u: %u bytes\n", myData.id, len);

  paradas[myData.id].stt_carrinho = myData.stt_carrinho;// Update the structures with the new incoming data
  
  if (myData.id = 0 && paradas[0].stt_carrinho == true && stt_serviço == true){ // esse if marca se o carrinho esta indo pra doca
    stt_servico = false; // se o carrinho estiver saindo marca como false
  } else{
    stt_servico = true; //se nn marca como true
  }
  Serial.println(paradas);
  passagem = true;// marca q precisa mandar dados pro esp servidor
}

void setup() {

  Serial.begin(115200);
  mySerial.begin(115200, SERIAL_8N1, RXD1, TXD1);  // UART setup
  
  //define o esp como station pro esp-now
  WiFi.mode(WIFI_STA);

  //inicializa o esp-now
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
  if (passagem == true){
    mySerial.print(String(myData.id)); // manda qual parada o carrinho acabou de passar
    mySerial.print(String(stt_servico)); // manda se o carrinho ainda esta em rota
    passagem = false;
  } 
  delay(2);
}
