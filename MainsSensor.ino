// A simple trigger to sense the mains power outage
#define LED_PIN 13
#define MAINS_VOLTAGE_SENSOR_PIN A3

// variable to store current mains power state
bool mainsPowered;
dword lastReportTime;

ZUNO_SETUP_DEBUG_MODE(DEBUG_ON);
ZUNO_SETUP_SLEEPING_MODE(ZUNO_SLEEPING_MODE_ALWAYS_AWAKE);

// you can read more on http://z-uno.z-wave.me/Reference/ZUNO_SENSOR_BINARY/
ZUNO_SETUP_CHANNELS(
  ZUNO_SENSOR_BINARY(ZUNO_SENSOR_BINARY_TYPE_DOOR_WINDOW, get_mains),
  ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_VOLTAGE, SENSOR_MULTILEVEL_SCALE_VOLT, 
    SENSOR_MULTILEVEL_SIZE_TWO_BYTES, SENSOR_MULTILEVEL_PRECISION_TWO_DECIMALS, get_battery)

//   ZUNO_SENSOR_MULTILEVEL(ZUNO_SENSOR_MULTILEVEL_TYPE_VOLTAGE, get_battery)
);

void setup() {
  mainsPowered = true;
  lastReportTime = 0;
  pinMode(LED_PIN, OUTPUT); // set LED pin as output
  pinMode(MAINS_VOLTAGE_SENSOR_PIN, INPUT);
//  Serial.begin(19200);
}

void loop() {
  word currentState = analogRead(MAINS_VOLTAGE_SENSOR_PIN);
  bool newMainsPowered = currentState > 500;
  if (newMainsPowered != mainsPowered) {
    mainsPowered = newMainsPowered;
    zunoSendReport(1);
    //byte val = get_mains();
    //zunoSendUncolicitedReport(1, val);
  }

  if (!mainsPowered) {
    digitalWrite(LED_PIN, HIGH);  // shine the LED
  } else {
    digitalWrite(LED_PIN, LOW);  // shine the LED
  }

  if (millis() - lastReportTime > 30000 || millis() < lastReportTime || 
    lastReportTime == 0) {
    zunoSendReport(1);
    //byte val = get_mains();
    //zunoSendUncolicitedReport(1, val);
    lastReportTime = millis();
  }
}

byte get_mains() {
  if (!mainsPowered) {
    return 0xff; // return "Triggered" state to the controller
  } else {
    return 0; // return "Idle" state to the controller
  }
}

word get_battery(){
  return 0;
}
