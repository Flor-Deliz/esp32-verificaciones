#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoOTA.h>

// WiFi
const char* ssid = "Deliz";
const char* pass = "23456789";

// Tus datos
const char* NOMBRE  = "Flor Deliz Ccasa Ccahuana";
const char* ORDEN   = "6"; // tu número de orden/código
const char* UNI     = "Universidad Nacional de San Antonio Abad del Cusco (UNSAAC)";
const char* CARRERA = "Ingeniería Informática y de Sistemas";
const char* CORREO  = "210922@unsaac.edu.pe"; // cámbialo

WebServer server(80);

// HTML bonito (responsive + card)
const char PAGE[] PROGMEM = R"HTML(
<!doctype html><html lang="es"><head>
<meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>Ficha | ESP32</title>
<style>
  :root{--bg:#0e1117;--card:#161b22;--acc:#33a0ff;--txt:#e6edf3;--mut:#9aa4b2;}
  *{box-sizing:border-box} body{margin:0;background:linear-gradient(135deg,#0e1117,#141a24);
  font:16px/1.45 system-ui,Segoe UI,Roboto;color:var(--txt);display:grid;place-items:center;min-height:100vh;padding:24px}
  .wrap{max-width:820px;width:100%}
  .card{background:var(--card);border:1px solid #2b3240;border-radius:18px;box-shadow:0 10px 30px #0006;padding:28px}
  .header{display:flex;align-items:center;gap:18px;margin-bottom:18px}
  .avatar{width:64px;height:64px;border-radius:14px;background:linear-gradient(135deg,#1e90ff,#7aa2f7);display:grid;place-items:center;font-weight:800}
  h1{font-size:22px;margin:0} .sub{color:var(--mut);margin-top:4px}
  .grid{display:grid;grid-template-columns:1fr 1fr;gap:14px;margin-top:18px}
  .item{background:#0f1420;border:1px solid #263044;border-radius:12px;padding:14px}
  .lbl{font-size:12px;letter-spacing:.06em;color:var(--mut);text-transform:uppercase;margin-bottom:6px}
  .val{font-size:16px;word-break:break-word}
  .badge{display:inline-block;background:linear-gradient(90deg,#2383e2,#7aa2f7);color:#fff;padding:6px 10px;border-radius:999px;font-size:12px}
  .footer{display:flex;justify-content:space-between;align-items:center;margin-top:16px;color:var(--mut);font-size:13px}
  .btn{appearance:none;border:1px solid #2b72d6;background:#14233a;color:#dbe9ff;padding:8px 12px;border-radius:10px;cursor:pointer}
  @media (max-width:620px){.grid{grid-template-columns:1fr}}
</style></head>
<body><div class="wrap">
  <div class="card">
    <div class="header">
      <div class="avatar">ESP32</div>
      <div>
        <h1>Ficha de Presentación</h1>
        <div class="sub">Servidor web corriendo en tu ESP32</div>
      </div>
    </div>

    <span class="badge">Activo</span>

    <div class="grid">
      <div class="item"><div class="lbl">Nombre completo</div><div class="val">%NOMBRE%</div></div>
      <div class="item"><div class="lbl">N.º de orden</div><div class="val">%ORDEN%</div></div>
      <div class="item"><div class="lbl">Universidad</div><div class="val">%UNI%</div></div>
      <div class="item"><div class="lbl">Carrera</div><div class="val">%CARRERA%</div></div>
      <div class="item" style="grid-column:1/-1"><div class="lbl">Correo</div><div class="val">%CORREO%</div></div>
    </div>

    <div class="footer">
      <div id="ip">IP: cargando...</div>
      <button class="btn" onclick="location.reload()">Actualizar</button>
    </div>
  </div>
</div>
<script>
  document.getElementById('ip').textContent = 'IP: ' + (location.host||'local');
</script>
</body></html>
)HTML";

String renderPage(){
  String html = FPSTR(PAGE);
  html.replace("%NOMBRE%",  NOMBRE);
  html.replace("%ORDEN%",   ORDEN);
  html.replace("%UNI%",     UNI);
  html.replace("%CARRERA%", CARRERA);
  html.replace("%CORREO%",  CORREO);
  return html;
}

void handleRoot(){ server.send(200,"text/html", renderPage()); }
void handleJson(){
  String j = String("{\"nombre\":\"")+NOMBRE+"\",\"orden\":\""+ORDEN+
             "\",\"universidad\":\""+UNI+"\",\"carrera\":\""+CARRERA+
             "\",\"correo\":\""+CORREO+"\"}";
  server.send(200,"application/json", j);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.begin(ssid, pass);
  Serial.print("Conectando");
  while (WiFi.status()!=WL_CONNECTED){ Serial.print("."); delay(400); }
  Serial.println("\nIP: " + WiFi.localIP().toString());

  server.on("/", handleRoot);
  server.on("/api/info", handleJson);
  server.begin();

  ArduinoOTA.setHostname("MiESP32");
  ArduinoOTA.setPassword("mi_clave_ota");
  ArduinoOTA.begin();
}

void loop() {
  server.handleClient();
  ArduinoOTA.handle();
}
