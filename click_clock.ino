#include <SoftwareSerial.h> // needed for MidiTempo
#include <Aurora_MidiTempo.h>
#include <Aurora_TriggerTempo.h>

// ----- INPUTS
#define PIN_MIDI_TX 0
#define PIN_MIDI_RX 3

#define PIN_TOUCH_TEMPO A4
#define PIN_TRIGGER_TEMPO A3

#define PIN_TOUCH_EIGHT A7
#define PIN_TOUCH_QUARTER A6
#define PIN_TOUCH_HALF A5
#define PIN_MIDI_SIXTEENTH 4
#define PIN_MIDI_DOTTED_EIGHT 5
#define PIN_MIDI_EIGHT 6
#define PIN_MIDI_QUARTER 7
#define PIN_MIDI_QUARTER_TRIPLET 8
#define PIN_MIDI_HALF 9
#define PIN_MIDI_WHOLE 10
#define PIN_TRIGGER_EIGHT 11
#define PIN_TRIGGER_QUARTER 12

// ----- OUTPUTS
#define PIN_GATE_OUTPUT 13

// ----- TEMPO MODES
#define MODE_TOUCH_EIGHT 0
#define MODE_TOUCH_QUARTER 1
#define MODE_TOUCH_HALF 2
#define MODE_MIDI_SIXTEENTH 3
#define MODE_MIDI_DOTTED_EIGHT 4
#define MODE_MIDI_EIGHT 5
#define MODE_MIDI_QUARTER 6
#define MODE_MIDI_QUARTER_TRIPLET 7
#define MODE_MIDI_HALF 8
#define MODE_MIDI_WHOLE 9
#define MODE_TRIGGER_EIGHT 10
#define MODE_TRIGGER_QUARTER 11

// ----- SETTINGS
#define OUTPUT_GATE_DURATION 50


int tempoMode = MODE_TOUCH_QUARTER;
unsigned long lastGateMillis = 0;

bool midiGate = LOW;
bool touchGate = LOW;
bool triggerGate = LOW;
bool * outputGate = &touchGate;

Aurora_MidiTempo midiTempo(PIN_MIDI_RX, PIN_MIDI_TX);
Aurora_TriggerTempo touchTempo(PIN_TOUCH_TEMPO);
Aurora_TriggerTempo triggerTempo(PIN_TRIGGER_TEMPO);


void setTempoMode(bool debug = false);
void writeOutputGate(bool debug = false);

void setup() {
  delay(1000); // Boot recovery

  midiTempo.debug(true);
  pinMode(PIN_MIDI_SIXTEENTH, INPUT);
  pinMode(PIN_MIDI_DOTTED_EIGHT, INPUT);
  pinMode(PIN_MIDI_EIGHT, INPUT);
  pinMode(PIN_MIDI_QUARTER, INPUT);
  pinMode(PIN_MIDI_QUARTER_TRIPLET, INPUT);
  pinMode(PIN_MIDI_HALF, INPUT);
  pinMode(PIN_MIDI_WHOLE, INPUT);

  pinMode(PIN_TOUCH_EIGHT, INPUT);
  pinMode(PIN_TOUCH_QUARTER, INPUT);
  pinMode(PIN_TOUCH_HALF, INPUT);

  pinMode(PIN_TRIGGER_EIGHT, INPUT);
  pinMode(PIN_TRIGGER_QUARTER, INPUT);

  pinMode(PIN_GATE_OUTPUT, OUTPUT);
}

void loop() {
  setTempoMode();
  midiGate = midiTempo.getGate();
  touchGate = touchTempo.getGate();
  triggerGate = triggerTempo.getGate();
  writeOutputGate();
}

void writeOutputGate(bool debug) {
  bool gate = *outputGate;
  if (gate) {
    digitalWrite(PIN_GATE_OUTPUT, HIGH);
    lastGateMillis = millis();
  } else {
    if ((millis() - OUTPUT_GATE_DURATION) > lastGateMillis) {
      digitalWrite(PIN_GATE_OUTPUT, LOW);
    }
  }
}

int readTempoMode(bool debug = false) {
  if (digitalRead(PIN_MIDI_SIXTEENTH)) {
    return MODE_MIDI_SIXTEENTH;
  }
  if (digitalRead(PIN_MIDI_DOTTED_EIGHT)) {
    return MODE_MIDI_DOTTED_EIGHT;
  }
  if (digitalRead(PIN_MIDI_EIGHT)) {
    return MODE_MIDI_EIGHT;
  }
  if (digitalRead(PIN_MIDI_QUARTER)) {
    return MODE_MIDI_QUARTER;
  }
  if (digitalRead(PIN_MIDI_QUARTER_TRIPLET)) {
    return MODE_MIDI_QUARTER_TRIPLET;
  }
  if (digitalRead(PIN_MIDI_HALF)) {
    return MODE_MIDI_HALF;
  }
  if (digitalRead(PIN_MIDI_WHOLE)) {
    return MODE_MIDI_WHOLE;
  }
  if (digitalRead(PIN_TRIGGER_EIGHT)) {

    return MODE_TRIGGER_EIGHT;
  }
  if (digitalRead(PIN_TRIGGER_QUARTER)) {
    return MODE_TRIGGER_QUARTER;
  }
  if (analogRead(PIN_TOUCH_EIGHT) > 900) {
    return MODE_TOUCH_EIGHT;
  }
  if (analogRead(PIN_TOUCH_QUARTER) > 900) {
    return MODE_TOUCH_QUARTER;
  }
  if (analogRead(PIN_TOUCH_HALF) > 900) {
    return MODE_TOUCH_HALF;
  }
}

void setTempoMode(bool debug) {
  int newTempoMode = readTempoMode();
  if (newTempoMode != tempoMode) {
    switch (newTempoMode) {
      case MODE_MIDI_SIXTEENTH:
        midiTempo.setBeatDivision(Aurora_MidiTempo::DIVISION_SIXTEENTH);
        outputGate = &midiGate;
        break;
      case MODE_MIDI_DOTTED_EIGHT:
        midiTempo.setBeatDivision(Aurora_MidiTempo::DIVISION_DOTTED_EIGHT);
        outputGate = &midiGate;
        break;
      case MODE_MIDI_EIGHT:
        midiTempo.setBeatDivision(Aurora_MidiTempo::DIVISION_EIGHT);
        outputGate = &midiGate;
        break;
      case MODE_MIDI_QUARTER:
        midiTempo.setBeatDivision(Aurora_MidiTempo::DIVISION_QUARTER);
        outputGate = &midiGate;
        break;
      case MODE_MIDI_QUARTER_TRIPLET:
        midiTempo.setBeatDivision(Aurora_MidiTempo::DIVISION_QUARTER_TRIPLET);
        outputGate = &midiGate;
        break;
      case MODE_MIDI_HALF:
        midiTempo.setBeatDivision(Aurora_MidiTempo::DIVISION_HALF);
        outputGate = &midiGate;
        break;
      case MODE_MIDI_WHOLE:
        midiTempo.setBeatDivision(Aurora_MidiTempo::DIVISION_WHOLE);
        outputGate = &midiGate;
        break;
      case MODE_TRIGGER_EIGHT:
        triggerTempo.setBeatDivision(Aurora_TriggerTempo::DIVISION_EIGHT);
        outputGate = &triggerGate;
        break;
      case MODE_TRIGGER_QUARTER:
        triggerTempo.setBeatDivision(Aurora_TriggerTempo::DIVISION_QUARTER);
        outputGate = &triggerGate;
        break;
      case MODE_TOUCH_EIGHT:
        touchTempo.setBeatDivision(Aurora_TriggerTempo::DIVISION_EIGHT);
        outputGate = &touchGate;
        break;
      case MODE_TOUCH_QUARTER:
        touchTempo.setBeatDivision(Aurora_TriggerTempo::DIVISION_QUARTER);
        outputGate = &touchGate;
        break;
      case MODE_TOUCH_HALF:
        touchTempo.setBeatDivision(Aurora_TriggerTempo::DIVISION_HALF);
        outputGate = &touchGate;
        break;
    }
    tempoMode = newTempoMode;
  }
}
