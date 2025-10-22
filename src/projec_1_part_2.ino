const int LED = 13;
String serialCmd = "";
String serial1Cmd = "";

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);      // Laptop USB
  Serial1.begin(9600);     // TTL cable (pins 19/18)

  Serial.println("Laptop ready!");
  Serial1.println("TTL ready!");
}

void loop() {
  // --- Check for messages from Serial (laptop) ---
  if (Serial.available()) {
    serialCmd = Serial.readStringUntil('\n');
    serialCmd.trim();

    if (serialCmd.equalsIgnoreCase("on")) {
      digitalWrite(LED, HIGH);
      Serial.println("LED turned ON");
      Serial1.println("LED ON");
    } 
    else if (serialCmd.equalsIgnoreCase("off")) {
      digitalWrite(LED, LOW);
      Serial.println("LED turned OFF");
      Serial1.println("LED OFF");
    } 
    else {
      Serial.println("Unknown command. Type ON or OFF.");
    }
  }

  // --- Check for messages from Serial1 (TTL) ---
  if (Serial1.available()) {
    serial1Cmd = Serial1.readStringUntil('\n');
    serial1Cmd.trim();

    if (serial1Cmd.equalsIgnoreCase("led on")) {
      Serial.println("I see the light!");
    } 
    else if (serial1Cmd.equalsIgnoreCase("led off")) {
      Serial.println("The light went out.");
    } 
  }
}
