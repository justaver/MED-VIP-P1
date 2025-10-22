// --- Simple 3-state FSM with 3 LEDs, controlled by Serial and Serial1 ---
// Mega 2560: Serial (USB), Serial1 on pins 19(RX1)/18(TX1).

enum State : uint8_t { S0 = 0, S1 = 1, S2 = 2 };
State current = S0;

const uint8_t LED_PINS[3] = {8, 9, 10};

String bufUSB, bufTTL;

void announceState() {
  const char* names[3] = {"S0", "S1", "S2"};
  Serial.print("STATE=");
  Serial.println(names[current]);
  Serial1.print("STATE=");
  Serial1.println(names[current]);
}

void applyOutputs() {
  // Turn on only the LED for the current state
  for (uint8_t i = 0; i < 3; ++i) {
    digitalWrite(LED_PINS[i], (i == current) ? HIGH : LOW);
  }
}

bool handleCommand(const String& cmd) {
  // Returns true if state changed
  if (cmd.equalsIgnoreCase("S0")) { current = S0; applyOutputs(); return true; }
  if (cmd.equalsIgnoreCase("S1")) { current = S1; applyOutputs(); return true; }
  if (cmd.equalsIgnoreCase("S2")) { current = S2; applyOutputs(); return true; }
  if (cmd.equalsIgnoreCase("NEXT")) {
    current = static_cast<State>((current + 1) % 3);
    applyOutputs(); return true;
  }
  if (cmd.equalsIgnoreCase("PREV")) {
    current = static_cast<State>((current + 2) % 3); // -1 mod 3
    applyOutputs(); return true;
  }
  if (cmd.equalsIgnoreCase("STATUS")) {
    announceState(); return false;
  }
  // Unknown command
  Serial.println("Unknown cmd. Use: S0 S1 S2 NEXT PREV STATUS");
  return false;
}

void tryReadLine(Stream& port, String& buffer) {
  // Non-blocking line reader (ends on \n). Trims CRLF.
  while (port.available()) {
    char c = (char)port.read();
    if (c == '\r') continue;
    if (c == '\n') {
      buffer.trim();
      if (buffer.length() > 0) {
        bool changed = handleCommand(buffer);
        if (changed) announceState();
      }
      buffer = "";
    } else {
      // limit growth to avoid runaway if no newline ever comes
      if (buffer.length() < 64) buffer += c;
    }
  }
}

void setup() {
  for (uint8_t i = 0; i < 3; ++i) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }

  Serial.begin(9600);   // Laptop USB
  Serial1.begin(9600);  // Adafruit TTL

  delay(300);
  applyOutputs();
  Serial.println("FSM ready. Type: S0 S1 S2 NEXT PREV STATUS");
  Serial1.println("FSM ready. Type: S0 S1 S2 NEXT PREV STATUS");
  announceState();
}

void loop() {
  // Read from both ports
  tryReadLine(Serial,  bufUSB);
  tryReadLine(Serial1, bufTTL);
}
