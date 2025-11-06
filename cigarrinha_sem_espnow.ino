
const byte pinoFototransistor = 35; //PINO ANALÓGICO UTILIZADO PELO FOTOTRANSISTOR
               
void setup(){
  Serial.begin(115200); //INICIALIZAÇÃO DA SERIAL
  pinMode(pinoFototransistor, INPUT); //DEFINE O PINO COMO ENTRADA
}
 
void loop(){
  if(analogRead(pinoFototransistor) < 2500){ //SE A LEITURA DO FOTOTRANSISTOR FOR MENOS QUE 1020 BITS
    Serial.println(analogRead(pinoFototransistor)); 
  }else{ 
    Serial.print(analogRead(pinoFototransistor)); // IMPRIME O VALOR LIDO
    Serial.println(" Sinal infravermelho interrompido"); //IMPRIME O TEXTO NA SERIAL
  }
  delay(100);
}