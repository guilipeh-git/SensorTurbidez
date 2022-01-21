#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "ITF-WiFi-3D";
const char* password = "ITF_2000";

IPAddress local_IP(10, 240, 160, 112);
IPAddress gateway(10, 240, 12, 123);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8); 
IPAddress secondaryDNS(8, 8, 4, 4);



WebServer server(80);

String html(){
  return PROGMEM R"rawliter(
  <body style= 'padding:20px;margin:0;display:flex;height: 250px;justify-content:center;align-items: center;overflow:auto'>
  <div style='text-align:center;border:1px solid #ccc;background:#ccc;border-radius:5px;padding:0 20px ;width:600px;margin-top:400px;margin-bottom: 100px;'>
    <div style= 'border:1px solid #ccc;margin:30px 130px;text-align: start;'>
      <h1>NTU: <i id= ntu></i> </h1>
      <h1>Tensao: <i id= Tensao></i> </h1>
      <h1>sensorValue: <i id= sensorValue></i> </h1>
    </div>
  </div>
<script>
  setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        let json = this.responseText;
        json = json.replaceAll("'",'"');
        let resposta = JSON.parse(json);
        document.getElementById("ntu").innerHTML = resposta.ntu;
        document.getElementById("Tensao").innerHTML = resposta.Tensao;
        document.getElementById("sensorValue").innerHTML = resposta.sensorValue;
      }
    };
    xhttp.open("GET", "/tbdz", true);
    xhttp.send()},500);
</script>
  )rawliter";
}

void handleRoot() { 
  server.send(200, "text/plain", String(SensorTurbidez()));
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {
  pinMode(33, OUTPUT);
  Serial.begin(115200);

  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
    }
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", [](){
    server.send(200,"text/html", html());
    });
  server.on("/tbdz", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}
