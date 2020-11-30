#include <WiFi.h>
#include <PubSubClient.h>

char ssid[] = "NOME-WIFI";
const char* password =  "SENHA-WIFI";
const char* mqttServer = "mqtt.eclipse.org";
const int mqttPort = 1883;
const char* mqttUser = "abcdefg";
const char* mqttPassword = "123456";

WiFiClient espClient;
PubSubClient client(espClient);

int contador = 1;
char mensagem[30];

int echo = 7;
int trigger = 6;
int cont = 0;
float tempoAtual = 0;
float tempoFinal = 0;
float tempo = 0;
float distancia;

void TX();
void RX();

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Iniciando conexao com a rede WiFi..");
  }
  Serial.println("Conectado na rede WiFi!");
  
  pinMode(echo, INPUT);
  pinMode(trigger, OUTPUT);
}
void loop()
{
  //Faz a conexao com o broker MQTT
  reconectabroker();
  TX();
  RX();
  
  Serial.print("Mensagem enviada: ");
  Serial.println(String(distancia));
  //Envia a mensagem ao broker
  String msg;
  msg=String(distancia);
  client.publish("InstruUni", msg.c_str());
  Serial.println("Mensagem enviada com sucesso...");
 
  
  //Aguarda 30 segundos para enviar uma nova mensagem
  delay(30000);
}
void reconectabroker()
{
  //Conexao ao broker MQTT
  client.setServer(mqttServer, mqttPort);
  while (!client.connected())
  {
    Serial.println("Conectando ao broker MQTT...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword ))
    {
      Serial.println("Conectado ao broker!");
    }
    else
    {
      Serial.print("Falha na conexao ao broker - Estado: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void TX(){ 
  while(cont < 9){
    
      digitalWrite(trigger, LOW);
      delayMicroseconds(2); 
      digitalWrite(trigger, HIGH); // ----------- Emite 8 pulsos de 10 microsegundos ----------
      delayMicroseconds(10);
      digitalWrite(trigger, LOW);
      tempoAtual = millis(); // ----------- Guarda o tempo q o ultimo pulso foi enviado ----------
      cont++;    
  }
}

void RX(){
  if (digitalRead(echo) == HIGH){ // ----------- Guardo o tempo que o transdutor recbe a resposta ----------
    tempo = (tempoFinal - tempoAtual);
    distancia = (((tempo * 0.001) * 343)/2); // ----------- calcula a distancia do objeto  ----------
  }  
}
