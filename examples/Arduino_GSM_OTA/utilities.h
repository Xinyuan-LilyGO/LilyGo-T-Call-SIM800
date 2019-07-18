#ifndef UTILITIES_H_INCLUDED
#define UTILITIES_H_INCLUDED
void printDeviceInfo()
{
  Serial.println();
  Serial.println("--------------------------");
  Serial.println(String("Firmware: ") + "v1.0.0 build " __DATE__ " " __TIME__);
#if defined(ESP8266)
  Serial.println(String("Flash:    ") + ESP.getFlashChipRealSize() / 1024 + "K");
  Serial.println(String("ESP core: ") + ESP.getCoreVersion());
  Serial.println(String("FW info:  ") + ESP.getSketchSize() + "/" + ESP.getFreeSketchSpace() + ", " + ESP.getSketchMD5());
#elif defined(ESP32)
  Serial.println(String("Flash:    ") + ESP.getFlashChipSize() / 1024 + "K");
  Serial.println(String("ESP sdk:  ") + ESP.getSdkVersion());
  Serial.println(String("Chip rev: ") + ESP.getChipRevision());
#endif
  Serial.println(String("Free mem: ") + ESP.getFreeHeap());
  Serial.println("--------------------------");
}

bool parseURL(String url, String& protocol, String& host, int& port, String& uri)
{
  int index = url.indexOf(':');
  if(index < 0) {
    return false;
  }

  protocol = url.substring(0, index);
  url.remove(0, (index + 3)); // remove protocol part

  index = url.indexOf('/');
  String server = url.substring(0, index);
  url.remove(0, index);       // remove server part

  index = server.indexOf(':');
  if(index >= 0) {
    host = server.substring(0, index);          // hostname
    port = server.substring(index + 1).toInt(); // port
  } else {
    host = server;
    if (protocol == "http") {
      port = 80;
    } else if (protocol == "https") {
      port = 443;
    }
  }

  if (url.length()) {
    uri = url;
  } else {
    uri = "/";
  }
  return true;
}
#endif
