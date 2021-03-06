//
// Thermostat for Pichiko heater
//
extern "C" {
#include "user_interface.h" // for sleep mode
}

#include <ESP8266WiFi.h>

#include <NTP.h>
#include <MyOTA.h>
#include <MyLib.h>
#include <MyCockpit.h>
#include <MyStatistic.h>

int id = 7; // device No.

// interval
CheckInterval CI; // read sensor
CheckInterval CIthermo; // thermo stat
CheckInterval CIupload; // send ifttt
CheckInterval CIddns(1000 * 60 * 60 * 24); // send ddns


// I/O pin
int sensorPin = A0;   //アナログピン
int relayPin = 5;     //digital 5番ピン
//ADC_MODE(ADC_VCC); // for use of getVcc. ADC pin must be open

// status variables
byte relayOn;

//
void setup() {
  Serial.begin(115200);
  Serial.println("");
  Serial.println("start ESP......................");
  //DebugOut.setToFile();
  //Serial.setDebugOutput(true);
  //WiFi.printDiag(DebugOut);

  // load json config
  jsonConfig.load();
  jsonConfig.setFlush(jsonConfigFlush);
  jsonConfig.flush();
  JsonObject &conf = jsonConfig.obj();  

  // relay pin setup
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // turn off
  relayOn = 0;

  wifi_set_sleep_type(LIGHT_SLEEP_T); // default=modem
  WiFiConnect(id);
  printSystemInfo();

  ntp_begin(2390);  // 2390 はローカルのUDPポート。空いている番号なら何番でもいいです。
  setupMyOTA();
  SET_THIS_SKETCH();
  addMyCockpit("/status", 0, []() {
    String o = "";
    o += "relay on: " + String(relayOn) + "\r\n";
    o += "interval read:   " + String(CI.get()) + "\r\n";
    o += "interval thermo: " + String(CIthermo.get()) + "\r\n";
    o += "interval IFTTT:  " + String(CIupload.get()) + "\r\n";
    MyStatistic *temp = sensorRead();
    o += "temp: " + temp->summary() + "\r\n";
    server.send(200, "text/plain", o);
  });
  setupMyCockpit();
}

void loop() {
  JsonObject &conf = jsonConfig.obj();
  loopMyOTA();
  loopMyCockpit();

  if ( CI.check() ) {
    // read sensor
    MyStatistic *temp = sensorRead();
    DebugOut.println(getDateTimeNow() + ": " + temp->summary());

    //----- thermostat
    if ( CIthermo.check() ) {
      if (temp->average() > (double)conf["thermoHigh"]) {
        digitalWrite(relayPin, LOW);
        relayOn = 0;
        DebugOut.println(getDateTimeNow()+": turn off relay at "+String(temp->average()));
        //triggerIFTTT("basic", getDateTimeNow(), "turn on relay", String(temp->average()));
      } else if (temp->average() < (double)conf["thermoLow"] ) {
        digitalWrite(relayPin, HIGH);
        relayOn = 1;
        DebugOut.println(getDateTimeNow()+": turn on relay at "+String(temp->average()));
        //triggerIFTTT("basic", getDateTimeNow(), "turn off relay", String(temp->average()));
      }
    }

    // send to Cloud, alive
    if ( CIupload.check() ) {
      int code = triggerBigQuery("basic_rcv",getDateTimeNow(), "basic", getDateTimeNow(), temp->summary());
      if( code >= 300 ) {
        triggerIFTTT("basic", getDateTimeNow(), temp->summary(), String(temp->average()));
      }
      //String js = String() + "{\"temperature\":{\"value\":" + temp->average() + ", \"timestamp\":" + now() + "000}}";
      //triggerUbidots("thermo", js);      
      //String url = String("http://orangepione.sada.org/cgi-bin/storelog.py?v=")+URLEncode("thermo,"+getDateTimeNow()+","+temp->average());
      //String resp = HttpGet(url.c_str());
      //DebugOut.println("trigger orangepi: response="+resp);
      String js = String() + "{\"timestamp\":\""+getDateTimeISOUTC(now())+"\", \"value\":"+temp->average()+"}";
      triggerM2X("basic", "temperature", js);
      HttpGet("http://svrsta.sada.org/alive?id=7"); // send alive signal to ServerSTA
    }
  }

  if ( CIddns.check() ) updateDDNS();
  delay(100);
}


void jsonConfigFlush(){
  JsonObject &conf = jsonConfig.obj();
  // set default to json
  if ( !conf.containsKey("thermoLow") ) {
    conf["thermoLow"] = 21.0;
    jsonConfig.save();
  }
  if ( !conf.containsKey("thermoHigh") ) {
    conf["thermoHigh"] = 21.3;
    jsonConfig.save();
  }
  if ( !conf.containsKey("intervalRead") ) {
    conf["intervalRead"] = 1000 * 10;
    jsonConfig.save();
  }
  if ( !conf.containsKey("intervalThermo") ) {
    conf["intervalThermo"] = 1000 * 30;
    jsonConfig.save();
  }
  if ( !conf.containsKey("intervalUpload") ) {
    conf["intervalUpload"] = 1000 * 300;
    jsonConfig.save();
  }
  // reflect conf to global variables
  CI.set((unsigned long)conf["intervalRead"]);
  CIthermo.set((unsigned long)conf["intervalThermo"]);
  CIupload.set((unsigned long)conf["intervalUpload"]);
  if ( conf["DebugOut"] ) {
    int t = conf["DebugOut"];
    DebugOut.setType(t);
  }
}

MyStatistic *sensorRead() {
  static MyStatistic tempStat;
  static double prev = 0.0;
  for (int i = 0; i < 10; i++) { //空読み
    int sensorValue = analogRead(sensorPin);    //アナログ0番ピンからの入力値を取得
    delay(2);
  }
  for (int retry = 0; retry < 10; retry++) {
    tempStat.clear();
    for (int i = 0; i < 20; i++) {
      int sensorValue = analogRead(sensorPin);    //アナログ0番ピンからの入力値を取得
      float temp  = modTemp(sensorValue);     //温度センサーからの入力値を変換
      tempStat.add(temp);
      delay(2);
    }
    if( tempStat.maximum() - tempStat.minimum() < 0.4 && tempStat.minimum() < prev + 0.4 ) break;
    delay(20);
  }
  Serial.println(tempStat.summary());
  prev = tempStat.minimum();
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

