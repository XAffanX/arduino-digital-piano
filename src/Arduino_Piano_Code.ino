#include <LiquidCrystal.h>

// ============================================
// ARDUINO PIANO - ENES100
// 8 Manual Keys (C4 to C5)
// 2 Automatic Songs
// 16x2 LCD with Dancing Girl
// ============================================

// Manual piano button pins
const int cNote = 9;
const int dNote = 8;
const int eNote = 7;
const int fNote = 6;
const int gNote = 5;
const int aNote = 4;
const int bNote = 3;
const int c5Note = 12;

// Piezo buzzer
const int Piezo = 2;

// Automatic song buttons
const int abcButton = 10;
const int maryButton = 11;

// LCD pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

// ============================================
// MUSICAL NOTE FREQUENCIES
// ============================================

const int NOTE_C4 = 262;
const int NOTE_D4 = 294;
const int NOTE_E4 = 330;
const int NOTE_F4 = 349;
const int NOTE_G4 = 392;
const int NOTE_A4 = 440;
const int NOTE_B4 = 494;
const int NOTE_C5 = 523;

// ============================================
// DANCING GIRL CHARACTERS
// ============================================

byte girl1[8] = {
  B01110,
  B01110,
  B00100,
  B11111,
  B10101,
  B01110,
  B01010,
  B10001
};

byte girl2[8] = {
  B01110,
  B01110,
  B00100,
  B11100,
  B10110,
  B01110,
  B10010,
  B00101
};

byte girl3[8] = {
  B01110,
  B01110,
  B00100,
  B00111,
  B01101,
  B01110,
  B01001,
  B10100
};

// ============================================
// ABC ALPHABET SONG
// ============================================
// Traditional ABC song melody

const int abcMelody[] = {

 NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4,
 NOTE_A4, NOTE_A4, NOTE_G4,

 NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4,
 NOTE_D4, NOTE_D4, NOTE_C4,

 NOTE_G4, NOTE_G4, NOTE_F4, NOTE_F4,
 NOTE_E4, NOTE_E4, NOTE_D4,

 NOTE_G4, NOTE_G4, NOTE_F4, NOTE_F4,
 NOTE_E4, NOTE_E4, NOTE_D4,

 NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4,
 NOTE_A4, NOTE_A4, NOTE_G4,

  NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4,
  NOTE_D4, NOTE_D4, NOTE_C4
};

const int abcDuration[] = {

 300,300,300,300,300,300,600,
 300,300,300,300,300,300,600,

 300,300,300,300,300,300,600,
 300,300,300,300,300,300,600,

  300,300,300,300,300,300,600,
  300,300,300,300,300,300,600
};

// ============================================
// MARY HAD A LITTLE LAMB
// ============================================

const int maryMelody[] = {

 NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4,
 NOTE_E4, NOTE_E4, NOTE_E4,

 NOTE_D4, NOTE_D4, NOTE_D4,
 NOTE_E4, NOTE_G4, NOTE_G4,

 NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4,
 NOTE_E4, NOTE_E4, NOTE_E4,

  NOTE_E4, NOTE_D4, NOTE_D4,
  NOTE_E4, NOTE_D4, NOTE_C4
};

const int maryDuration[] = {

 300,300,300,300,300,300,600,
 300,300,600,300,300,600,

  300,300,300,300,300,300,600,
  300,300,300,300,300,600
};

// ============================================
// BUTTON STATES
// ============================================

bool lastABCState = HIGH;
bool lastMaryState = HIGH;

// -1 means no manual note is pressed
int lastManualNote = -1;

// ============================================
// SETUP
// ============================================

void setup() {

 // All 8 manual keys use external
 // 10k ohm pull-down resistors.
 // Pressed = HIGH
 // Released = LOW

 pinMode(cNote, INPUT);
 pinMode(dNote, INPUT);
 pinMode(eNote, INPUT);
 pinMode(fNote, INPUT);
 pinMode(gNote, INPUT);
 pinMode(aNote, INPUT);
 pinMode(bNote, INPUT);

 // FIXED: C5 now behaves like other keys
 pinMode(c5Note, INPUT);

    // Song buttons connect to GND
    // Pressed = LOW
    pinMode(abcButton, INPUT_PULLUP);
    pinMode(maryButton, INPUT_PULLUP);

    pinMode(Piezo, OUTPUT);

    Serial.begin(9600);

    // Initialize LCD
    lcd.begin(16, 2);

    // Create dancing girl characters
    lcd.createChar(0, girl1);
    lcd.createChar(1, girl2);
    lcd.createChar(2, girl3);

    showIdleScreen();
}

// ============================================
// DEFAULT LCD SCREEN
// ============================================

void showIdleScreen() {

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("ARDUINO PIANO");

    lcd.setCursor(0, 1);
    lcd.print("PLAY A NOTE!");
}

// ============================================
// DISPLAY PRESSED NOTE
// ============================================

void showManualNote(const char* noteName,
                  const char* solfege,
                  int frequency) {

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("PLAYING: ");
    lcd.print(noteName);

    lcd.setCursor(0, 1);
    lcd.print(solfege);
    lcd.print(" ");
    lcd.print(frequency);
    lcd.print(" Hz");

    Serial.print("Playing: ");
    Serial.println(noteName);
}

// ============================================
// DANCING GIRL ANIMATION
// ============================================

void danceGirl(int frame) {

    lcd.setCursor(7, 1);
    lcd.write((uint8_t)(frame % 3));
}

// ============================================
// PLAY ONE SONG NOTE
// ============================================

void playNote(int frequency,
             int duration,
             int frame) {

    danceGirl(frame);

    tone(Piezo, frequency);

    delay(duration);

    noTone(Piezo);

    delay(80);
}

// ============================================
// PLAY AUTOMATIC SONG
// ============================================

void playSong(const int melody[],
             const int durations[],
             int length,
             const char* songName) {

    noTone(Piezo);

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print(songName);

    lcd.setCursor(0, 1);
    lcd.print("DANCING");

    for (int i = 0; i < length; i++) {

        playNote(
          melody[i],
          durations[i],
          i
        );
    }

    noTone(Piezo);

    lastManualNote = -1;

    showIdleScreen();
}

// ============================================
// PLAY ABC SONG
// ============================================

void playABC() {

    Serial.println("Playing ABC Song");

    int length =
      sizeof(abcMelody) / sizeof(abcMelody[0]);

    playSong(
      abcMelody,
      abcDuration,
      length,
      "ABC SONG"
    );
}

// ============================================
// PLAY MARY HAD A LITTLE LAMB
// ============================================

void playMary() {

    Serial.println("Playing Mary Had a Little Lamb");

    int length =
      sizeof(maryMelody) / sizeof(maryMelody[0]);

    playSong(
      maryMelody,
      maryDuration,
      length,
      "MARY HAD A LAMB"
    );
}

// ============================================
// MANUAL PIANO: C4 THROUGH C5
// ============================================

void manualPiano() {

    int currentNote = -1;
    int frequency = 0;

    const char* noteName = "";
    const char* solfege = "";

    // C4 - Button 1
    if (digitalRead(cNote) == HIGH) {

        currentNote = 0;
        frequency = NOTE_C4;
        noteName = "C4";
        solfege = "DO";
    }

    // D4 - Button 2
    else if (digitalRead(dNote) == HIGH) {

        currentNote = 1;
        frequency = NOTE_D4;
        noteName = "D4";
        solfege = "RE";
    }

    // E4 - Button 3
    else if (digitalRead(eNote) == HIGH) {

        currentNote = 2;
        frequency = NOTE_E4;
        noteName = "E4";
        solfege = "MI";
    }

    // F4 - Button 4
    else if (digitalRead(fNote) == HIGH) {

        currentNote = 3;
        frequency = NOTE_F4;
        noteName = "F4";
        solfege = "FA";
    }

    // G4 - Button 5
 else if (digitalRead(gNote) == HIGH) {

     currentNote = 4;
     frequency = NOTE_G4;
     noteName = "G4";
     solfege = "SOL";
 }

 // A4 - Button 6
 else if (digitalRead(aNote) == HIGH) {

     currentNote = 5;
     frequency = NOTE_A4;
     noteName = "A4";
     solfege = "LA";
 }

 // B4 - Button 7
 else if (digitalRead(bNote) == HIGH) {

     currentNote = 6;
     frequency = NOTE_B4;
     noteName = "B4";
     solfege = "TI";
 }

 // C5 - Button 8
 // FIXED: Pressed = HIGH
 else if (digitalRead(c5Note) == HIGH) {

     currentNote = 7;
     frequency = NOTE_C5;
     noteName = "C5";
     solfege = "DO";
 }

 // ========================================
 // PLAY PRESSED NOTE
 // ========================================

 if (currentNote != -1) {

     // Produce sound
     tone(Piezo, frequency);

     // Update LCD only if note changed
     if (currentNote != lastManualNote) {

         showManualNote(
           noteName,
           solfege,
           frequency
         );

         lastManualNote = currentNote;
     }
 }

 // ========================================
 // NO BUTTON PRESSED
 // ========================================

 else {

     noTone(Piezo);

     if (lastManualNote != -1) {

         showIdleScreen();

            lastManualNote = -1;
        }
    }
}

// ============================================
// MAIN LOOP
// ============================================

void loop() {

    bool abcState = digitalRead(abcButton);
    bool maryState = digitalRead(maryButton);

    // ========================================
    // D10: ABC ALPHABET SONG
    // ========================================

    if (abcState == LOW &&
        lastABCState == HIGH) {

        delay(30);

        if (digitalRead(abcButton) == LOW) {

            playABC();
        }
    }

    // ========================================
    // D11: MARY HAD A LITTLE LAMB
    // ========================================

    else if (maryState == LOW &&
            lastMaryState == HIGH) {

        delay(30);

        if (digitalRead(maryButton) == LOW) {

            playMary();
        }
    }

    // Update song button states
    lastABCState = digitalRead(abcButton);
    lastMaryState = digitalRead(maryButton);

    // ========================================
    // MANUAL PIANO MODE
    // ========================================

    if (lastABCState == HIGH &&
        lastMaryState == HIGH) {

      manualPiano();
    }
    else {

        noTone(Piezo);
    }

    delay(10);
}
