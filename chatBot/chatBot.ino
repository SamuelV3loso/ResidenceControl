#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
#include <UniversalTelegramBot.h>

#define BOTtoken "************************" //CodeIOTBot

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);


const char* ssid = "*****"; //Nome do wi-fi
const char* password = "*******"; //Senha do wi-fi

String id, text;//Váriaveis para armazenamento do ID e TEXTO gerado pelo Usuario
unsigned long tempo;
bool status = false;

void connect() {
  WiFi.begin(ssid, password);
  Serial.println(".");
  delay(2000);
}

void readTel() {
  int newmsg = bot.getUpdates(bot.last_message_received + 1);
     
  for (int i = 0; i < newmsg; i++)//Caso haja X mensagens novas, fara este loop X Vezes.
  {
    text = bot.messages[i].text;//Armazenara o TEXTO do Usuario à Váriavel
    text.toUpperCase();//Converte a STRING_TEXT inteiramente em Maiuscúla
    
    if (text.indexOf("ON") > -1)//Caso o texto recebido contenha "ON"
    {
      bot.sendSimpleMessage(bot.messages[i].chat_id, "\xF0\x9F\x92\xA1 A lampada esta acesa.", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      digitalWrite(D0, 0);
      
    }

    else if (text.indexOf("OFF") > -1)//Caso o texto recebido contenha "OFF"
    {
      bot.sendSimpleMessage(bot.messages[i].chat_id, "\xF0\x9F\x92\xA1 A lampada esta apagada.", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      digitalWrite(D0, 1);
     
    }

    else if (text.indexOf("/START") > -1)//Caso o texto recebido contenha "START"
    {
      String from_name = bot.messages[i].from_name;
      if (from_name == "") from_name = "querido";

          String welcome = "Ola "+ from_name + ", bem vindo ao bot gerenciador de dispositivos embarcados, desenvolvido por Samuel Veloso.\n";
          welcome += "Esta eh a sua lista de comandos ate o momento.\n\n";
          welcome += "on : acender a lampada\n";
          welcome += "off : apagar a lampada\n";
          welcome += "/start : ver esta mensagem novamente\n";
          bot.sendMessage(bot.messages[i].chat_id, welcome, "Markdown");
      
      bot.sendSimpleMessage(bot.messages[i].chat_id, "Seu id no telegram eh: " + bot.messages[i].chat_id, "");//Envia uma mensagem com seu ID.
    } else//Caso o texto recebido nao for nenhum dos acima, Envia uma mensagem de erro.
    {
      bot.sendSimpleMessage(bot.messages[i].chat_id, "Comando Invalido", "");
    }
  }
}

void setup() {

  Serial.begin(115200);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  pinMode(LED_BUILTIN, OUTPUT);
  //bot.begin();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) connect();
  readTel();
}
