//Programa: NodeMCU e MQTT - Controle e Monitoramento IoT
//Autor: Pedro Bertoleti
//Adaptado: Cleyton da Silva Vitor

#include <ESP8266WiFi.h> // Importa a Biblioteca ESP8266WiFi
#include <PubSubClient.h> // Importa a Biblioteca PubSubClient
#include <DHT.h>


//defines:
                                            //defines de id mqtt e tópicos para publicação e subscribe
#define TOPICO_SUBSCRIBE "MQTTimdEnvia"     //tópico MQTT de escuta
#define TOPICO_PUBLISH   "MQTTimdSensor"    //tópico MQTT de envio de informações para Broker
                                            //IMPORTANTE: recomendamos fortemente alterar os nomes
                                            //            desses tópicos. Caso contrário, há grandes
                                            //            chances de você controlar e monitorar o NodeMCU
                                            //            de outra pessoa.
#define ID_MQTT  "IMD509"                   //id mqtt (para identificação de sessão)
                                            //IMPORTANTE: este deve ser único no broker (ou seja,
                                            //            se um client MQTT tentar entrar com o mesmo
                                            //            id de outro já conectado ao broker, o broker
                                            //            irá fechar a conexão de um deles).
                                            

//defines - mapeamento de pinos do NodeMCU
#define D0    16
#define D1    5
#define D2    4
#define D3    0
#define D4    2
#define D5    14
#define D6    12
#define D7    13
#define D8    15
#define D9    3
#define D10   1
#define DHTPIN D2 //Pino digital D2 (GPIO5) conectado ao DHT11
#define DHTTYPE DHT11 //Tipo do sensor DHT11





// WIFI
const char* SSID = "brisa-1712326"; // SSID / nome da rede WI-FI que deseja se conectar
const char* PASSWORD = "jtrmgnxz"; // Senha da rede WI-FI que deseja se conectar

// MQTT
const char* BROKER_MQTT = "broker.hivemq.com"; //URL do broker MQTT que se deseja utilizar
int BROKER_PORT = 1883; // Porta do Broker MQTT



//Variáveis e objetos globais
WiFiClient espClient; // Cria o objeto espClient
PubSubClient MQTT(espClient); // Instancia o Cliente MQTT passando o objeto espClient
DHT dht(DHTPIN, DHTTYPE); //Inicializando o objeto dht do tipo DHT passando como parâmetro o pino (DHTPIN) e o tipo do sensor (DHTTYP
float temperatura; //variável para armazenar a temperatura
char data[5];
char *valor;


//Prototypes
void initSerial();
void initWiFi();
void initMQTT();
void reconectWiFi();
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void VerificaConexoesWiFIEMQTT(void);
void InitOutput(void);

/*
    Implementações das funções
*/
void setup()
{
  //inicializações:
  InitOutput();
  initSerial();
  initWiFi();
  initMQTT();
  dht.begin(); //Inicializa o sensor DHT11
 
}

//Função: inicializa comunicação serial com baudrate 115200 (para fins de monitorar no terminal serial
//o que está acontecendo.

void initSerial()
{
  Serial.begin(115200);
}

//Função: inicializa e conecta-se na rede WI-FI desejada

void initWiFi()
{
  delay(10);
  Serial.println("------Conexao WI-FI------");
  Serial.print("Conectando-se na rede: ");
  Serial.println(SSID);
  Serial.println("Aguarde");

  reconectWiFi();
}

//Função: inicializa parâmetros de conexão MQTT(endereço do
//        broker, porta e seta função de callback)

void initMQTT()
{
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);   //informa qual broker e porta deve ser conectado
  MQTT.setCallback(mqtt_callback);            //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}

//Função: função de callback
//        esta função é chamada toda vez que uma informação de
//        um dos tópicos subescritos chega)

void mqtt_callback(char* topic, byte* payload, unsigned int length)
{
  String msg;

  //obtem a string do payload recebido
  for (int i = 0; i < length; i++)
  {
    char c = (char)payload[i];
    msg += c;
  }

  //toma ação dependendo da string recebida:
  //verifica se deve colocar nivel alto de tensão na saída D0:

  if (msg.equals("L"))
  {
    digitalWrite(D1, HIGH);
    //EstadoSaida = '1';
  }

 
  if (msg.equals("D"))
  {
    digitalWrite(D1, LOW);
    //EstadoSaida = '0';
  }

}

//Função: reconecta-se ao broker MQTT (caso ainda não esteja conectado ou em caso de a conexão cair)
//em caso de sucesso na conexão ou reconexão, o subscribe dos tópicos é refeito.

void reconnectMQTT()
{
  while (!MQTT.connected())
  {
    Serial.print("* Tentando se conectar ao Broker MQTT: ");
    Serial.println(BROKER_MQTT);
    if (MQTT.connect(ID_MQTT))
    {
      Serial.println("Conectado com sucesso ao broker MQTT!");
      MQTT.subscribe(TOPICO_SUBSCRIBE);
    }
    else
    {
      Serial.println("Falha ao reconectar no broker.");
      Serial.println("Havera nova tentatica de conexao em 2s");
      delay(2000);
    }
  }
}

//Função: reconecta-se ao WiFi

void reconectWiFi()
{
  //se já está conectado a rede WI-FI, nada é feito.
  //Caso contrário, são efetuadas tentativas de conexão
  if (WiFi.status() == WL_CONNECTED)
    return;

  WiFi.begin(SSID, PASSWORD); // Conecta na rede WI-FI

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Conectado com sucesso na rede ");
  Serial.print(SSID);
  Serial.println("IP obtido: ");
  Serial.println(WiFi.localIP());
}

//Função: verifica o estado das conexões WiFI e ao broker MQTT.
//        Em caso de desconexão (qualquer uma das duas), a conexão
//        é refeita.

void VerificaConexoesWiFIEMQTT(void)
{
  if (!MQTT.connected())
    reconnectMQTT(); //se não há conexão com o Broker, a conexão é refeita

  reconectWiFi(); //se não há conexão com o WiFI, a conexão é refeita
}

//Função: envia ao Broker o estado atual do DHT

void EnviaTemperatura(void)
{
  temperatura = dht.readTemperature();  
  //Serial.print("Temperatura: ");
  //Serial.print(temperatura); 
  //Serial.println(" ºC");
  //converte de float para char 
  valor = dtostrf(temperatura, 4, 2, data);
  MQTT.publish(TOPICO_SUBSCRIBE, valor);
  delay(5000);
}

//Função: inicializa o output em nível lógico baixo

void InitOutput(void)
{
  pinMode(D1, OUTPUT);
  digitalWrite(D1, LOW);
}


void loop()
{
  //garante funcionamento das conexões WiFi e ao broker MQTT
  VerificaConexoesWiFIEMQTT();

  //envia o a tenperatura lida pelo DHT11
  EnviaTemperatura();

  //keep-alive da comunicação com broker MQTT
  MQTT.loop();
}
