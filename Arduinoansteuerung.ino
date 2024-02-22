// ================== Zeit ==================
unsigned int ms, ms_mem;
byte sek, sek_mem;

// Digital Output
bool DO_Band;       // relais 11
bool DO_step_enabq; // step_enabq 8    // Schrittmotor 1/3
bool DO_step_dir;   // step_dir 9      // Schrittmotor 2/3
bool DO_step_step;  // step_step 10    // Schrittmotor 3/3

// Digital Input
bool DI_Limit_Switch; // limit_switch 4 Lichtschranke
bool DI_light_sens;   // light_sens 5    // Lichtsensor
bool DI_Taster;       // start_key 12    // Startknopf

int iSchrittkette;
unsigned int tonPause;

char inChar, c;

void setup() {
  Serial.begin(9600);

  pinMode(11, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(12, INPUT);
}

void loop() {
  unsigned int msSPS = millis();
  ms = msSPS - ms_mem;
  ms_mem = msSPS;
  if (ms > 10000) {ms = 0;}

  byte sekSPS = msSPS / 1000; // 26.11.2021
  bool bTickSek = sek_mem != sekSPS;
  sek = sekSPS - sek_mem;
  sek_mem = sekSPS;
  if (sek > 10) {sek = 0;}

  // Rücksetzen
  if (iSchrittkette  >= 5 && DI_Taster)
  {
    Serial.print('e');
    iSchrittkette = 0;
  }
  
  switch (iSchrittkette)
  {
    case 0: // warten auf Taster => Band Start
      inChar = c = ' ';
      tonPause = 0;
      DO_Band = 0;
      if (!DI_Taster)
      {
        iSchrittkette = 1;
      }
      break;

    case 1: // warten auf Taster => Band Start

      if(Serial.available())
      {
        c = Serial.read();
        if (isAlpha(c))
        {
          inChar = c;
        }
         break;
      }      
      if (inChar == 'x')
      {
        inChar = c = ' ';
        DO_Band = 1;
        iSchrittkette = 5;
      }

      
      if (DI_Taster)
      {
        iSchrittkette = 2;
      }
      break;

    case 2: // warten auf Taster => Band Start
      if (!DI_Taster)
      {
        DO_Band = 1;
        iSchrittkette = 5;
      }
      break;

    case 5: // warten auf Lichtschranke => Zeit Start
      if (DI_light_sens)
      {
        iSchrittkette = 6;
      }
      break;

    case 6: // warten auf Lichtschranke => Zeit Start
      tonPause = 0;
      if (!DI_light_sens)
      {
        iSchrittkette = 10;
      }
      break;

    case 10: // Band Stop
      tonPause += ms;
      if (tonPause >= 1500) // ms Band von Lichtschranke bis Kamera
      {
        DO_Band = 0;
        tonPause = 0;
        iSchrittkette = 20;
      }
      break;

    case 20: // Befehl Snapshot senden
      tonPause += ms;
      if (tonPause >= 1000) // ms bis Befehl Snapshot gesendet wird
      {
        Serial.print("s");
        tonPause = 0;
        inChar = c = ' ';
        iSchrittkette = 30;
      }
      break;

    case 30:  // warten auf Befehl von Kamera c oder i
      tonPause = 0;
      if(Serial.available())
      {
        c = Serial.read();
        if (isAlphaNumeric(c))
        {
          inChar = c;
        }
        break;
      }      
      if (inChar == '1')
      {
        DO_step_dir = 1;
//        Serial.println("c wurde empfangen");
        iSchrittkette = 40;
      }
      if (inChar == '0')
      {
        DO_step_dir = 0;
//        Serial.println("i wurde empfangen");
        iSchrittkette = 40;
      }
      break;

    case 40: // Schrittmotor Takten
      tonPause = 0;
      DO_step_enabq = 1;
      DO_step_step = 1;
      iSchrittkette = 50;
      break;

    case 50: // Takten bis Endschalter betädigt wurde
      if (!DI_Limit_Switch)
      {
        tonPause += ms;
        if (tonPause >= 3)
        {
          DO_step_step = 0;
          iSchrittkette = 40;
        }
      }
      else
      {
        tonPause += ms;
        if (tonPause >= 100) // ms Halten von Klappe
        {
          DO_step_dir = 0;
          DO_step_step = 0;
          DO_step_enabq = 0;
          tonPause = 0;
          DO_Band = 1;
          iSchrittkette = 5;
        }
      }
      break;

//    case 60: // Rücksetzen
//      tonPause += ms;
//      if (tonPause >= 4000) // ms Band bis Auswurf Material
//      {
//        DO_Band = 0;
//        tonPause = 0;
//        iSchrittkette = 5;
//      }
//      break;
  }


  digitalWrite(11, DO_Band);
  digitalWrite(8, !DO_step_enabq);
  digitalWrite(9, !DO_step_dir);
  digitalWrite(10, !DO_step_step);
  
  DI_Limit_Switch = !digitalRead(4);
  DI_light_sens   = !digitalRead(5);
  DI_Taster       = !digitalRead(12);

/*
  if (bTickSek)
  {
    Serial.print("DI_Limit_Switch ");
    Serial.print(DI_Limit_Switch);

    Serial.print(" DI_light_sens ");
    Serial.print(DI_light_sens);
    
    Serial.print(" DI_Taster ");
    Serial.print(DI_Taster);
    
    Serial.print(" iSchrittkette ");
    Serial.print(iSchrittkette);
    
    Serial.print(" inChar:");
    Serial.print(inChar);
    
    Serial.print(" DO_step_step ");
    Serial.print(DO_step_step);
    
    Serial.println("");
  }
*/
}
