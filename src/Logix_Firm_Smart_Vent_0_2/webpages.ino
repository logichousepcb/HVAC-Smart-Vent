

void handleConfig() {
  conf.handleFormRequest(&server);
  if (server.hasArg("SAVE")) {
    uint8_t cnt = conf.getCount();
    Serial.println("*********** Configuration ************");
    for (uint8_t i = 0; i<cnt; i++) {
      Serial.print(conf.getName(i));
      Serial.print(" = ");
      Serial.println(conf.values[i]);
    }
  }
}

//-----------------------------ROOT--------------------

void handleRoot () {
static const char PROGMEM ROOT_HTML[] = R"rawliteral( 
<!DOCTYPE html><html lang="en"class=""><head><meta charset='utf-8'>
  <meta name="viewport"content="width=device-width,initial-scale=1,user-scalable=no"/>
  <title>LoGiX DIY</title>
  <script>var x=null,lt,to,tp,pc='';function eb(s){return document.getElementById(s)}function qs(s){return document.querySelector(s)}function sp(i){eb(i).type=(eb(i).type==='text'?'password':'text')}function wl(f){window.addEventListener('load',f)}var ft;function la(p){a=p||'';clearTimeout(ft);clearTimeout(lt);if(x!=null){x.abort()}x=new XMLHttpRequest();x.onreadystatechange=function(){if(x.readyState==4&&x.status==200){var s=x.responseText.replace(/{t}/g,"<table style='width:100%'>").replace(/{s}/g,"<tr><th>").replace(/{m}/g,"</th><td style='width:20px;white-space:nowrap'>").replace(/{e}/g,"</td></tr>");eb('l1').innerHTML=s;clearTimeout(ft);clearTimeout(lt);lt=setTimeout(la,2345)}};x.open('GET','.?m=1'+a,true);x.send();ft=setTimeout(la,20000)}function lc(v,i,p){if(eb('s')){if(v=='h'||v=='d'){var sl=eb('sl4').value;eb('s').style.background='linear-gradient(to right,rgb('+sl+'%,'+sl+'%,'+sl+'%),hsl('+eb('sl2').value+',100%,50%))'}}la('&'+v+i+'='+p)}wl(la);function jd(){var t=0,i=document.querySelectorAll('input,button,textarea,select');while(i.length>=t){if(i[t]){i[t]['name']=(i[t].hasAttribute('id')&&(!i[t].hasAttribute('name')))?i[t]['id']:i[t]['name']}t++}}wl(jd);</script><style>div,fieldset,input,select{padding:5px;font-size:1em}fieldset{background:#4f4f4f}p{margin:0.5em 0}input{width:100%;box-sizing:border-box;-webkit-box-sizing:border-box;-moz-box-sizing:border-box;background:#dddddd;color:#000000}input[type=checkbox],input[type=radio]{width:1em;margin-right:6px;vertical-align:-1px}input[type=range]{width:99%}select{width:100%;background:#dddddd;color:#000000}textarea{resize:vertical;width:98%;height:318px;padding:5px;overflow:auto;background:#1f1f1f;color:#65c115}body{text-align:center;font-family:verdana,sans-serif;background:#252525}td{padding:0px}button{border:0;border-radius:0.3rem;background:#1fa3ec;color:#faffff;line-height:2.4rem;font-size:1.2rem;width:100%;-webkit-transition-duration:0.4s;transition-duration:0.4s;cursor:pointer}button:hover{background:#0e70a4}.bred{background:#d43535}.bred:hover{background:#931f1f}.bgrn{background:#47c266}.bgrn:hover{background:#5aaf6f}a{color:#1fa3ec;text-decoration:none}.p{float:left;text-align:left}.q{float:right;text-align:right}.r{border-radius:0.3em;padding:2px;margin:6px 2px}</style></head><body><div style='text-align:left;display:inline-block;color:#eaeaea;min-width:340px;'><div style='text-align:center;color:#eaeaea;'>

<h3>Logix DIY</h3>
<body>
  
  <p>
    <form id=but2 style="display: block;"action='opencontrol'method='get'>
      <button>OPEN Vent</button>
    </form>
  </p>
  <p>
    <form id=but2 style="display: block;"action='closecontrol'method='get'>
      <button>CLOSE Vent</button>
    </form>
  </p>
  <p>
    <form id=but2 style="display: block;"action='config'method='get'>
      <button>Configuration</button>
    </form>
  </p>
  <p>
    <form id=but6 style="display: block;"action='update'method='get'>
      <button>Firmware Update</button>
    </form>
  </p>
  <p>
    <form id=but6 style="display: block;"action='http://192.168.1.130:5050/Office'method='get'>
      <button>DASHBAORD</button>
    </form>
  </p>
  <p>
    <form id=but4 style="display: block;"action='restart'method='get'onsubmit='return confirm("Confirm Restart");'>
      <button name='rst'class='button bred'>Restart</button>
    </form>
  </p>

<div style='text-align:right;font-size:11px;'><hr/>Logix DIY - Open Source Software by Mike Degenaars logixpcb@gmail.com <br>
https://github.com/logichousepcb/Logix_Multifunction <br>
)rawliteral";
String html_new ="<p style=\"text-align:center\"><span style=\"font-family:Arial,Helvetica,sans-serif\"><span style=\"font-size:20px\">";
String html_outline = ROOT_HTML;

sensors.requestTemperatures(); 
// html_outline += html_new; 
html_outline += "Firmware version: ";
html_outline += CURR_VERS;
html_outline += CURR_VERS_NAME;
html_outline += html_new; 
html_outline += "VENT TEMPERATURE - ";
html_outline += String(sensors.getTempFByIndex(0));
html_outline += html_new; 
html_outline += "VENT STATE - "; 
html_outline += switchState;
html_outline += "</div></div></body></html>";
server.send(200, "text/html", html_outline);
}


//-----------------------------RESTART--------------------
void handleRestart() {   // NEED TO ADD ARE YOU SURE?
     ESP.restart();
}

//-----------------------------OPEN Control--------------------
void handleOpenControl () {
  switchState = "OPEN";
  switchStateint = 0;
//  sensors.requestTemperatures();
  Serial.print("Vent changed to ");
  Serial.println(switchState);
  open_vent();
  
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
}
  
//-----------------------------CLOSE Control--------------------
void handleCloseControl () {
  switchState = "CLOSED";
  switchStateint = 1;
//  sensors.requestTemperatures();
  Serial.print("Vent changed to ");
  Serial.println(switchState);
  close_vent();
  
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303);                         // Send it back to the browser with an HTTP status 303 (See Other) to redirect
}
