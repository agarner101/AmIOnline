#include<SPI.h>
#include<Ethernet.h>

byte mac[] = { 0x10, 0x15, 0xBA, 0xBE, 0xAB, 0xCD };

IPAddress ip(192, 168, 0, 110);
IPAddress myDns(1, 1, 1, 1);
EthernetClient client;

char server[] = "www.google.com";
const long delayMillis = 10L * 1000L; //Ten Second delays between successful calls.
long lastConnectionTime = 0;
long stateChangeTime = 0;
boolean isConnected = false;

int redLedPin = 7;
int greenLedPin = 6;

void setup() {
  setupLeds();
  Serial.begin(9600);
  while (!Serial) {
    ; //wait for connection
  }

  delay(1000);
  Ethernet.begin(mac, ip, myDns);

  Serial.print("IP address: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  if (client.available()) {
    char c = client.read();
    //Serial.write(c);
  }

  if (shouldMakeRequest()) {
    boolean hasConnection = httpRequest();
    showOutput(hasConnection);
  }
}

boolean shouldMakeRequest() {
  if (lastConnectionTime == 0) {
    return true;
  }
  return (millis() - lastConnectionTime) > delayMillis;
}

boolean httpRequest() {
  client.stop();

  if (client.connect(server, 80)) {
    lastConnectionTime = millis();

    client.println("GET / HTTP/1.1");
    client.println("Host: www.google.com");
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();

    client.println("Connection: close");
    client.println();

    return true;
  } else {
    return false;
  }
}

void setupLeds() {
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);

  digitalWrite(redLedPin, HIGH);
  digitalWrite(greenLedPin, HIGH);
}

void showOutput(boolean hasConnection) {
  long currentTime = millis();
  if (isConnected != hasConnection) {
    Serial.println("State change!");
    stateChangeTime = currentTime;
    isConnected = hasConnection;
  }
  Serial.print(hasConnection ? "Connected:    " : "Disconnected: ");
  Serial.print(currentTime);
  Serial.print(hasConnection ? "  Up Time: " : "  Down Time: ");
  Serial.println(currentTime - stateChangeTime);
  
  digitalWrite(redLedPin, !hasConnection);
  digitalWrite(greenLedPin, hasConnection);
}




