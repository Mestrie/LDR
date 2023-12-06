#include <WiFi.h>
#include <HTTPClient.h>
#include <SPIFFS.h>

#define LIGHT_SENSOR_PIN 36 // ESP32 pin GIOP36 (ADC0)

const char* ssid = "(GPONNET_2.4)_JUSTINO";          // Nome da rede Wi-Fi
const char* password = "Tennopg13424134";     // Senha da rede Wi-Fi

bool stopFlag = false; // Sinalizador para controlar a parada do loop

void setup() {
  Serial.begin(9600);
  if (!SPIFFS.begin(true)) {
    Serial.println("Falha ao inicializar o SPIFFS");
    while (1);
  }
  Serial.println("SPIFFS inicializado com sucesso");

  // Conectar à rede Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando à rede Wi-Fi...");
  }
  Serial.println("Conectado à rede Wi-Fi");
}

void loop() {
  int analogValue = analogRead(LIGHT_SENSOR_PIN);

  Serial.print("Analog Value = ");
  Serial.print(analogValue);

  if (analogValue < 40) {
    Serial.println(" => Dark");
  } else if (analogValue < 800) {
    Serial.println(" => Dim");
  } else if (analogValue < 2000) {
    Serial.println(" => Light");
  } else if (analogValue < 3200) {
    Serial.println(" => Bright");
  } else {
    Serial.println(" => Very bright");
  }

  if (Serial.available()) {
    char input = Serial.read();
    if (input == 's' || input == 'S') {
      stopFlag = true;
    }
  }

  if (stopFlag) {
    Serial.println(" => Leitura finalizada");
    Serial.println("Resultados finais:");

    File dataFile = SPIFFS.open("/dados.csv", "r");
    if (dataFile) {
      while (dataFile.available()) {
        Serial.write(dataFile.read());
      }
      dataFile.close();
    } else {
      Serial.println("Erro ao abrir o arquivo");
    }

    enviarDadosGoogleSheets();

    while (true) {
      // Loop infinito
    }
  }

  File dataFile = SPIFFS.open("/dados.csv", "a");
  if (dataFile) {
    dataFile.print(analogValue);
    dataFile.print(",");

    if (analogValue < 40) {
      dataFile.println("Dark");
    } else if (analogValue < 800) {
      dataFile.println("Dim");
    } else if (analogValue < 2000) {
      dataFile.println("Light");
    } else if (analogValue < 3200) {
      dataFile.println("Bright");
    } else {
      dataFile.println("Very bright");
    }

    dataFile.close();
  } else {
    Serial.println("Erro ao abrir o arquivo");
  }

  delay(500);
}

void enviarDadosGoogleSheets() {
  File dataFile = SPIFFS.open("/dados.csv", "r");
  if (dataFile) {
    // Lê todo o conteúdo do arquivo CSV
    String csvContent = "";
    while (dataFile.available()) {
      csvContent += (char)dataFile.read();
    }
    dataFile.close();

    // Define a URL do endpoint do Google Sheets
    String url = "https://docs.google.com/spreadsheets/d/1oyCQGnIR_20atVWS5kqTSjveEDYa35aXQKUBCMIPfeI/edit?usp=sharing";  // Substitua YOUR_FORM_ID pela ID do formulário

    // Cria um objeto HTTPClient
    HTTPClient http;

    // Envia os dados para o Google Sheets usando uma requisição HTTP POST
    http.begin(url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpResponseCode = http.POST("entry.123=" + csvContent);  // Substitua "123" pelo campo correto no formulário

    // Verifica se a requisição foi enviada com sucesso
    if (httpResponseCode == 200) {
      Serial.println("Dados enviados com sucesso para o Google Sheets");
    } else {
      Serial.println("Falha ao enviar dados para o Google Sheets");
    }

    // Libera os recursos da requisição HTTP
    http.end();
  } else {
    Serial.println("Erro ao abrir o arquivo");
  }
}

