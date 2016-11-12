//
// Thermostat for Pichiko heater
//
extern "C" {
#include <user_interface.h> // for sleep mode
}

#include <ESP8266WiFi.h>

#include <NTP.h>
#include <MyOTA.h>
#include <MyLib.h>
#include <MyCockpit.h>
#include <Statistic.h>

// interval
CheckInterval CI(1000 * 10); // read sensor
CheckInterval CIthermo(1000 * 30); // thermo stat
CheckInterval CIifttt(1000 * 60 * 5); // send ifttt
CheckInterval CIddns(1000 * 60 * 60 * 24); // send ddns


// I/O pin
int sensorPin = A0;   //アナログピン
int relayPin = 5;     //digital 5番ピン
//ADC_MODE(ADC_VCC); // for use of getVcc. ADC pin must be open

// status variables
byte relayOn;
float thermoHigh = 100.0; // in ℃
float thermoLow  = -100.0; // in ℃

//
void setup() {
  Serial.begin(115200);
  Serial.println("");
  Serial.println("start ESP......................");
  //DebugOut.setToFile();
  //Serial.setDebugOutput(true);
  //WiFi.printDiag(DebugOut);

  // relay pin setup
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // turn off
  relayOn = 0;

  wifi_set_sleep_type(LIGHT_SLEEP_T); // default=modem
  WiFiConnect();
  printSystemInfo();

  ntp_begin(2390);  // 2390 はローカルのUDPポート。空いている番号なら何番でもいいです。
  setupMyOTA();
  addHtmlMyCockpit(String("Sketch: ") + THIS_SKETCH + "<BR><BR>");
  addMyCockpit("/status", 0, []() {
    String o = "";
    o += "thermo low:  " + String(thermoLow) + "\r\n";
    o += "thermo high: " + String(thermoHigh) + "\r\n";
    o += "relay on: " + String(relayOn) + "\r\n";
    o += "interval read:   " + String(CI.get()) + "\r\n";
    o += "interval thermo: " + String(CIthermo.get()) + "\r\n";
    o += "interval IFTTT:  " + String(CIifttt.get()) + "\r\n";
    server.send(200, "text/plain", o);
  });
  addMyCockpit("/interval", 3, []() {
    String n1 = server.arg(0);
    String n2 = server.arg(1);
    String n3 = server.arg(2);
    CI.set(n1.toInt());
    CIthermo.set(n2.toInt());
    CIifttt.set(n3.toInt());
    server.send(200, "text/plain", "ok");
  });
  addMyCockpit("/thermoLowHigh", 2, []() {
    String n1 = server.arg(0);
    String n2 = server.arg(1);
    thermoLow = n1.toFloat();;
    thermoHigh = n2.toFloat();;
    server.send(200, "text/plain", "ok");
  });
  setupMyCockpit();
}

void loop() {
  loopMyOTA();
  loopMyCockpit();

  if ( CI.check() ) {
    // read sensor
    Statistic *temp = sensorRead();
    DebugOut.println(getDateTimeNow() + ": " + temp->summary());

    //----- thermostat
    if ( CIthermo.check() ) {
      if (temp->average() > thermoHigh) {
        digitalWrite(relayPin, LOW);
        relayOn = 0;
        Serial.println("turn off relay");
        triggerIFTTT("basic", getDateTimeNow(), "turn on relay", String(temp->average()));    
      } else if (temp->average() < thermoLow ) {
        digitalWrite(relayPin, HIGH);
        relayOn = 1;
        Serial.println("turn on relay");
        triggerIFTTT("basic", getDateTimeNow(), "turn off relay", String(temp->average()));    
      }
    }

    // send to IFTTT
    if ( CIifttt.check() ) {
      triggerIFTTT("basic", getDateTimeNow(), temp->summary(), String(temp->average()));
    }
  }

  if( CIddns.check() ) updateDDNS();
  delay(100);
}



Statistic *sensorRead() {
  static Statistic tempStat;
  tempStat.clear(20.0, 0.01);
  for (int i = 0; i < 10; i++) { //空読み
    int sensorValue = analogRead(sensorPin);    //アナログ0番ピンからの入力値を取得
    delay(2);
  }
  for (int i = 0; i < 20; i++) {
    int sensorValue = analogRead(sensorPin);    //アナログ0番ピンからの入力値を取得
    float temp  = modTemp(sensorValue);     //温度センサーからの入力値を変換
    tempStat.add(temp);
    delay(2);
  }
  Serial.println(tempStat.summary());
  return &tempStat;
}

//アナログ入力値を摂氏度℃に変換
float modTemp(int analog_val) {
  //float v  = 5;     // 基準電圧値( V ) // arduino default
  //float v  = 1.1;     // 基準電圧値( V ) // arduino internal
  float v  = 1.0;     // 基準電圧値( V ) // esp8266
  float tempC = ((v * analog_val) / 1024) * 100;  // 摂氏に換算
  return tempC;
}
