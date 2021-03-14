#include <SoftwareSerial.h>

SoftwareSerial wifi(2, 3); // RX Arduino, TX Arduino

String mySSID = "Mysterious";         // ssid
String myPass = "tyto180517";           // pass
String ServerIP = "192.168.43.106";      // Ip server
String host = "Host: tytomulyono.com";      //host

String url = "GET /tytomulyonoesp01/index.php";
String data2server;

void setup() {
  Serial.begin(9600);
  wifi.begin(9600);
  wifi.println("AT");
  ShowResponse(3000);
  
  wifi.println("AT+CWMODE=1");
  ShowResponse(3000);
  
  Serial.println("Reset ESP");
  wifi.println("AT+RST");
  ShowResponse(3000);
  delay(100);
  
  Serial.print("Connect AP : ");
  Serial.println(mySSID);
  connectAP(mySSID, myPass);
}

void connectAP(String SSID, String Pass){                     // fungsi koneksi ke AP
  //bila ESP baru harus set Mode ke station (AT+CWMODE=1) terlebih dahulu
  Serial.println("Connecting...");
  wifi.print("AT+CWJAP=\"");
  wifi.print(SSID);
  wifi.print("\",\"");
  wifi.print(Pass);
  wifi.println("\"");
  ShowResponse(5000);
  wifi.println("AT+CIFSR");
  ShowResponse(5000);
}

void loop() {
  Serial.println("Check Mode Device From Server = ");
  
  data2server = url;
  data2server += " HTTP/1.1";
  
  send_data();
  Serial.println(ShowResponseServer(5000));
  wifi.println("AT+CIPCLOSE=4");

  delay(5000);
}

void ShowResponse(int wait){
  Serial.print("response ESP : ");
  long timeNOW = millis();
  String hasilread;
  while(millis()-timeNOW < wait){
    while(wifi.available()){
      hasilread = wifi.readString();
      Serial.println(hasilread);
      delay(10);
      return;
    }
  }
}

void send_data(){
  wifi.println("AT+CIPMUX=1");
  ShowResponse(2000);
  
  wifi.print("AT+CIPSTART=4,\"TCP\",\"");
  wifi.print(ServerIP);
  wifi.println("\",80");
  ShowResponse(2000);

  wifi.print("AT+CIPSEND=4,");
  wifi.println(data2server.length()+host.length()+6);  //6 for \r\n
  
  Serial.println(data2server);
  Serial.println(host);
  Serial.println();
  delay(100);
  
  wifi.println(data2server);
  wifi.println(host);
  wifi.println();
}

String ShowResponseServer(int wait){      // baca data dari server
  Serial.print("response FROM SERVER : ");
  long timeNOW = millis();
  String hasilread;
  while(millis()-timeNOW < wait){
    while(wifi.available()){
      hasilread = wifi.readString();
    }
    return hasilread;
  }
  return "Time out - no data";
}
