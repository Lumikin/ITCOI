#include <WiFi.h>
#include <PubSubClient.h>

// =========================
// Wi-Fi
// =========================
const char *ssid = "CONECTA+";
const char *password = "senai512";

// =========================
// MQTT
// =========================
const char *mqtt_server = "192.168.4.XXX";
const int mqtt_port = 3026;

const char *mqtt_topic = "/GRUPO01/lucas/led";

// =========================
// LED
// =========================
const int LED_PIN = 2;

// =========================
// Objetos
// =========================
WiFiClient espClient;
PubSubClient client(espClient);

// =========================
// Conectar ao Wi-Fi
// =========================
void setup_wifi()
{

    Serial.println();
    Serial.print("Conectando ao Wi-Fi: ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("Wi-Fi conectado!");

    Serial.print("IP do ESP32: ");
    Serial.println(WiFi.localIP());
}

// =========================
// Receber mensagem MQTT
// =========================
void callback(char *topic, byte *payload, unsigned int length)
{

    Serial.print("Mensagem recebida no tópico: ");
    Serial.println(topic);

    // Converte o payload para String
    String mensagem = "";

    for (unsigned int i = 0; i < length; i++)
    {
        mensagem += (char)payload[i];
    }

    Serial.print("Mensagem: ");
    Serial.println(mensagem);

    // Remove espaços extras
    mensagem.trim();

    // Verifica o valor recebido
    if (mensagem == "true")
    {

        digitalWrite(LED_PIN, HIGH);

        Serial.println("LED LIGADO");
    }
    else if (mensagem == "false")
    {

        digitalWrite(LED_PIN, LOW);

        Serial.println("LED DESLIGADO");
    }
    else
    {

        Serial.println("Valor desconhecido. Use true ou false.");
    }
}

// =========================
// Conectar ao MQTT
// =========================
void reconnect()
{

    while (!client.connected())
    {

        Serial.print("Conectando ao MQTT...");

        // ID único para o ESP32
        String clientId = "ESP32-LED-";
        clientId += String(random(0xffff), HEX);

        if (client.connect(clientId.c_str()))
        {

            Serial.println(" conectado!");

            // Assina o tópico
            if (client.subscribe(mqtt_topic))
            {

                Serial.print("Inscrito no tópico: ");
                Serial.println(mqtt_topic);
            }
            else
            {

                Serial.println("Falha ao assinar o tópico!");
            }
        }
        else
        {

            Serial.print(" falhou. Código: ");
            Serial.println(client.state());

            Serial.println("Tentando novamente em 5 segundos...");
            delay(5000);
        }
    }
}

// =========================
// SETUP
// =========================
void setup()
{

    Serial.begin(115200);

    // Configura o LED
    pinMode(LED_PIN, OUTPUT);

    // Começa com o LED desligado
    digitalWrite(LED_PIN, LOW);

    // Conecta ao Wi-Fi
    setup_wifi();

    // Configura MQTT
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
}

// =========================
// LOOP
// =========================
void loop()
{

    // Verifica conexão MQTT
    if (!client.connected())
    {
        reconnect();
    }

    // Mantém a comunicação MQTT
    client.loop();
}