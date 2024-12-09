#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <DHT.h>
#include <UrlEncode.h>

const char* ssid = "ET@eheio";      // Nom de votre réseau Wi-Fi
const char* password = "eheio2023"; // Mot de passe Wi-Fi

// DHT Sensor settings
#define DHTPIN D5      // Pin où le capteur est connecté (GPIO14 sur NodeMCU)
#define DHTTYPE DHT11  // Si vous utilisez un DHT11, changez ceci en DHT11
DHT dht(DHTPIN, DHTTYPE);

// WhatsApp settings
String phoneNumber = "212682406187"; // Numéro de téléphone au format international
String apiKey = "9988380"; // API Key pour CallMeBot

void sendMessage(String message) {
  String url = "http://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);
  
  WiFiClient client;    
  HTTPClient http;
  
  // Initialisation de la requête HTTP
  http.begin(client, url);

  // Spécification de l'en-tête content-type
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  // Envoi de la requête HTTP POST
  int httpResponseCode = http.POST(url);
  
  // Vérification du code de réponse
  if (httpResponseCode == 200) {
    Serial.println("Message envoyé avec succès !");
  } else {
    Serial.println("Erreur lors de l'envoi du message");
    Serial.print("Code de réponse HTTP: ");
    Serial.println(httpResponseCode);
  }

  // Libération des ressources
  http.end();
}

void setup() {
  Serial.begin(115200);
  
  // Initialisation du capteur DHT
  dht.begin();
  
  // Connexion au Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connexion au Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connecté au réseau Wi-Fi avec l'adresse IP: ");
  Serial.println(WiFi.localIP());

  // Attendre quelques secondes pour que le capteur DHT se stabilise
  delay(2000);

  // Envoyer les données de température et d'humidité
  sendTempHumidity();
}

void sendTempHumidity() {
  // Lire les données de température et d'humidité
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  // Vérifier si les lectures du capteur DHT ont échoué
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Échec de la lecture du capteur DHT !");
    return;
  }
  
  // Afficher la température et l'humidité dans le moniteur série
  Serial.print("Température: ");
  Serial.print(temperature);
  Serial.println(" °C");
  
  Serial.print("Humidité: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Préparer le message à envoyer sur WhatsApp
  String message = "Température: " + String(temperature) + "°C\nHumidité: " + String(humidity) + "%";
  
  // Envoyer le message via WhatsApp
  sendMessage(message);
}

void loop() {
  // Envoyer les données toutes les 10 minutes (600000 ms)
  sendTempHumidity();
  delay(600000);  // Attente de 10 minutes
}
