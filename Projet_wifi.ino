#include <ESP8266WiFi.h>
#include <Servo.h>
 
const char* ssid = "Grogne";
const char* password = "jomsy123";
Servo monServo;

WiFiServer server(80);
 
void setup() 
{
    
  // initialisation de la communication série
  Serial.begin(9600);
  monServo.attach(0);
  delay(100);

  // Connexion wifi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);

  // connection  en cours ...
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  // Wifi connecter
  Serial.println("WiFi connecté");
 
  // Démmarrage du serveur.
  server.begin();
  Serial.println("Serveur demarré !");
 
  // Affichage de l'adresse IP
  Serial.print("Utiliser cette adresse URL pour la connexion :");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() 
{
WiFiClient client;

  
  // Vérification si le client est connecter.
  client = server.available();
  if (!client)
  {
    return;
  }
 
  // Attendre si le client envoie des données ...
  Serial.println("nouveau client");
  while(!client.available()){
    delay(1);
  }
 
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  int value = LOW;
  if (request.indexOf("/OPEN") != -1)  {
    monServo.write(180);
    value = HIGH;
  }
  if (request.indexOf("/CLOSE") != -1)  {
    monServo.write(90);
    value = LOW;
  }
 
  // Réponse
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); 
  client.println("<!DOCTYPE HTML>");
  client.println("<html>"
  "<head>"
  "<style>"
  "h1{color:white;}" 
  "h3{color:white;}" 
  "body {background-color: black;}"
  "</style>"
  "</head>"
  "<body>");
 
 
  if(value == HIGH) {
    client.print("Ouvert");
  } else {
    client.print("Ferme");
  }
  client.println("<h1>Porte connectee</h1>");
  client.println("<h3>Etat de la porte : </h3>");
  client.println("<a href=\"/OPEN\"\"><button>Ouvrir </button></a>");
  client.println("<a href=\"/CLOSE\"\"><button>Fermer </button></a><br /></a>");
  client.println("<style> background-color:black;</style>");
  
  client.println("</body></html>");
 
  delay(1);
  Serial.println("Client deconnecté");
  Serial.println("");
 
}
