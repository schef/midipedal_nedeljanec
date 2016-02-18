const int selectPin[] = { 10, 11, 12, 9 };
const int dataPin[] = { 8, 7, 6, 5, 4, 3, 2 };

int keyNote[4][7];
int keyNoteOld[4][7];

long lastDebounceTime[4][7];
long debounceDelay = 20;

void noteOn(int cmd, int pitch, int velocity) {
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}

void initPins(){

    for (int i = 0; i < 4; i++){
      pinMode(selectPin[i], OUTPUT);
    }
    for (int i = 0; i < 7; i++){
      pinMode(dataPin[i], INPUT);
    }
    
    //for (int s = 0; s < 6; s++) {
    //  for (int d = 0; d < 12; d++) {
    //    lastDebounceTime[s][d] = 0;
    //  }
    //}
    
}

void readNote(){
    //select
    for (int s = 0; s < 4; s++) {
        digitalWrite(selectPin[s], HIGH);

        //data
        for (int d = 0; d < 7; d++) {
            keyNote[s][d] = digitalRead(dataPin[d]);
            //if (keyNote[s][d] != keyNoteOld[s][d]){
            //  lastDebounceTime[s][d] = millis();
            //}
        }
        digitalWrite(selectPin[s], LOW);
    }
}

void setup()   {                
  Serial.begin(31250);
  //Serial.begin(9600);
  initPins();
}

void loop() {
  readNote();
  
  for (int s = 0; s < 4; s++){
    for (int d = 0; d < 7; d++){
     // if ((millis() - lastDebounceTime[s][d]) > debounceDelay) {
      if (keyNote[s][d] == 1 && keyNote[s][d] != keyNoteOld[s][d]){
        
        if ((millis() - lastDebounceTime[s][d]) > debounceDelay) {
        //Serial.print(s);
        //Serial.print(":");
        //Serial.print(d);
        //Serial.println("=1");
        //usbMIDI.sendNoteOn(35+s*5+d, 100, s+1);
        if (s == 3 && d == 6){
          noteOn(0x97, d+(s*7)+48-1, 0);
          //Serial.print(d+(s*7)+48-1);
        }
        else {
          noteOn(0x97, d+(s*7)+48, 0);
          //Serial.print(d+(s*7)+48);
        }
        //delay(100);
        keyNoteOld[s][d] = keyNote[s][d];
        lastDebounceTime[s][d] = millis();
        }
      }
        else if (keyNote[s][d] == 0 && keyNote[s][d] != keyNoteOld[s][d]){
          if ((millis() - lastDebounceTime[s][d]) > debounceDelay) {
        //Serial.print(s);
        //Serial.print(":");
        //Serial.print(d);
        //Serial.println("=0");
        //usbMIDI.sendNoteOff(35+s*5+d, 100, s+1);
        if (s == 3 && d == 6){
        noteOn(0x97, d+(s*7)+48-1, 0x7F);
        //Serial.print(d+(s*7)+48-1);
        }
        else {
        noteOn(0x97, d+(s*7)+48, 0x7F);
        //Serial.print(d+(s*7)+48);
        }
        //delay(100);
        keyNoteOld[s][d] = keyNote[s][d];
        lastDebounceTime[s][d] = millis();
        }
      }
    }
  }
}
