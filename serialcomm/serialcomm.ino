int x;

// const int baudrate = 115200;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1);
}

void loop() {
  while (!Serial.available());
  
  x = Serial.readString().toInt();
  
  Serial.println(x + 1);
}
