# espnowLib

This libaray is intent for easy-to use of [ESP-NOW function](https://espressif.com/en/products/software/esp-now/overview) of ESP8266 in [Arduino-ESP8266 environment](https://github.com/esp8266/Arduino).  
*ESP-NOW* is a protocol which enables low-power communication between ESP8266 nodes without WiFi connection.

This library supports the followings:
* Req-Ack protocol
* Buffer which stores received ESP-NOW packets
* Three packet classes -- req/ack/data 
* User defined packet types

## Quick Start

### Example sketch for controller node (device):
It sends message string to the slave node.
```Arduino
#include <ESP8266WiFi.h>
#include <espnowLib.h>

void setup() {
  WiFi.mode(WIFI_STA);
  setupEspNow(NULL, NULL, NULL); // setup esp-now
}

void loop() {
  uint8_t slaveMac[] = {0x1A,0xFE,0x34,0xD5,0xC7,0xAF}; // mac address of slave
  sendEspNowData(slaveMac, "message to be sent", 0); // send message to the slave, wait for ack packet
  delay(10000);
}
```

### Example sketch for slave node (server):
It receives message string from the controller node.
```Arduino
#include <ESP8266WiFi.h>
#include <espnowLib.h>

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAP("foobar", "12345678", 1, 0); // setup WiFi Access point
  setupEspNow(NULL, NULL, NULL); // setup esp-now
}

void loop() {
  for (int i = 0; i < espNowBuffer.recvDataBufferMax(); i++ ) { // for each data packet in buffer
    String data = espNowBuffer.getDataFromDataBuffer(i);
    Serial.println( data ); // action for received data packet
  }
  espNowBuffer.clearDataBuffer(); // clear data buffer

  delay(500);
}
```
### Example Description
`sendEspNow()` sends a ESP-NOW packet which includes specified data(string) to the slave node, and wait for ack packet reply. When the slave node receives the packet, it return ack packet and store the recevied packet into buffer `espNowBuffer` in background. `getDataFromDataBuffer(i)` retrieves the received data from the buffer.

## API Reference (Digest)

### setupEspNow
```Arduino
void setupEspNow(NULL, NULL, NULL)
```
This function should be called in `setup()` for both slave and controller node.  
This function initializes espnow and registers call back functions for espnow packet receive/send events.
The receive call back function automatically reply ack packet, and store received packet in the buffer `espNowBuffer`.
The call back functions are executed in background (ex, when `delay()` or `yield()` called).

### sendEspNow
```Arduino
bool sendEspNowReq(uint8_t *macaddr, uint8_t type)
bool sendEspNowData(uint8_t *macaddr, String message, uint8_t type)
```
Send ESP-NOW packet to the specified mac address. This function can be called either from control or slave node.
`sendEspNowReq()` sends req packet and `sendEspNowData()` sends data packet. User can specify arbitary number to `type`. `message` is content of the data packet.  
These function waits for ack packet replied. When ack packet received, return `true`. If timeout or fails to send packet, return `false`.

*Note:* Set WiFi mode before calling this function. For controller node, set WiFi mode to STA but never begin (never make connection to AP). For slave node, set WiFi mode to SoftAP mode.

### espNowBuffer
This contains three buffers -- req, ack, and data buffer. The size of each buffer is `EspNowBufferSize`.
It is a ring buffer, so when it overflow, the oldest data is overwritten.

#### espNowBuffer - clear
```Arduino
void espNowBuffer.clearDataBuffer()
void espNowBuffer.clearReqBuffer()
void espNowBuffer.clearAckBuffer()
```
These functions clear (make empty) the buffer for data/req/ack packets.

#### espNowBuffer - data retrieval
To retrieve data packet information from data buffer:
```Arduino
for(i=0; i < espNowBuffer.recvDataBufferMax(); i++ ) {
  uint8_t *mac = espNowBuffer.getMacFromDataBuffer(i);
  uint8_t type = espNowBuffer.getTypeFromDataBuffer(i);
  String message = espNowBuffer.getDataFromDataBuffer(i);
}
```
To retrieve req packet information from req buffer:
```Arduino
for(i=0; i < espNowBuffer.recvReqBufferMax(); i++ ) {
  uint8_t *mac = espNowBuffer.getMacFromReqBuffer(i);
  uint8_t type = espNowBuffer.getTypeFromReqBuffer(i);
}
```
