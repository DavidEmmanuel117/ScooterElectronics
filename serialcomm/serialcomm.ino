#define _ACTION_LIBRE   0
#define _ACTION_FRENA   1
#define _ACTION_5KMH    2
#define _ACTION_10KMH   3
#define _ACTION_15KMH   4


int x;

// const int baudrate = 115200;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1);

  // Wait Serial port to open
  while(!Serial) {}
}

void loop() {
  
  x = SerialEvent();
  // x = Serial.parseInt();
  Serial.println(x);
  Serial.println(getAction(x));
}

int SerialEvent() {
  while (!Serial.available());

  // readString() reads until "\r\n" and the parses to int
  return Serial.readString().toInt();
}

String getAction(int x) {
  if (x == _ACTION_LIBRE) return("libre");
  else if (x == _ACTION_FRENA) return("frena");
  else if (x == _ACTION_5KMH) return("5KPH");
  else if (x == _ACTION_10KMH) return("10KPH");
  else if (x == _ACTION_15KMH) return("15KPH");
  else return("OTHER");
}
