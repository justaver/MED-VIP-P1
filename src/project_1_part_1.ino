void setup() {
  Serial.begin(9600);     // USB (UART0)
  Serial1.begin(9600);    // UART1 (pins 19 RX1, 18 TX1)
  delay(500);
  Serial.println("USB only should be visible");
}
void loop() {
  Serial.println("HIII");
  delay(1000);
  Serial1.println("THIS SHOULD NOT APPEAR ON USB");
  delay(1000);
}

