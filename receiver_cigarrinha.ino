#include <esp_now.h>
#include <WiFi.h>

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
contador boardsStruct[2] = {ponto1, ponto2};

void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("Parada  %u: %u bytes\n", myData.id, len);
  // Update the structures with the new incoming data
  boardsStruct[myData.id-1].stt_carrinho = myData.stt_carrinho;
  paradas[myData.id-1] = myData.stt_carrinho;
  Serial.print(paradas);
  Serial.println();
  paradas[myData.id-1]++;
}

int n = 2; // numero de paradas monitoradas mais a entrada/saida
int paradas [n] = {0, 0};

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  
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
}
