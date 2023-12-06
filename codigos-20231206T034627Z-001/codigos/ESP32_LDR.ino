long prevMillisThingSpeak = 0;
int intervalThingSpeak = 1000; // Intervalo minímo para escrever no ThingSpeak write é de 15 segundos
#include <WiFi.h>
const char ssid[] = "(GPONNET_2.4)_JUSTINO";
const char password[] = "Tennopg13424134";
WiFiClient client;

#include <ThingSpeak.h>
const long CHANNEL = 2213071;
const char *WRITE_API = "02TPESD1K6OU0R8R";

#define LIGHT_SENSOR_PIN 36 // ESP32 pin GIOP36 (ADC0)

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("Envia os dados do sensor para o ThingSpeak usando o ESP32");
  Serial.println();

  WiFi.mode(WIFI_STA); //Modo Station
  ThingSpeak.begin(client);  // Inicializa o ThingSpeak
}

void loop() {
  float voltage = 0;

  voltage = analogRead(LIGHT_SENSOR_PIN) * (3.3 / 4095.0);
   
  // Conecta ou reconecta o WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Atenção para conectar o SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, password);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConectado");
  }
  
  Serial.println("LDR - Valor da voltagem:");
  Serial.println(voltage);
  delay(1000);

  if (millis() - prevMillisThingSpeak > intervalThingSpeak) {
    // Configura os campos com os valores
    ThingSpeak.setField(1, voltage);

    // Escreve no canal do ThingSpeak 
    int x = ThingSpeak.writeFields(CHANNEL, WRITE_API);
    if (x == 200) {
      Serial.println("Update realizado com sucesso");
    }
    else {
      Serial.println("Problema no canal - erro HTTP " + String(x));
    }

    prevMillisThingSpeak = millis();
  }
}
