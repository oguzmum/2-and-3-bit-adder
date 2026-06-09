// Positive results are displayed normally; for negative results the decimal point lights up as a minus-sign indicator.

// Arduino input pins connected to the adder's output (carry + sum bits)
const int SUM_BIT3 = 2;  // carry bit (MSB / sign bit in two's complement)
const int SUM_BIT2 = 3;  // 2^2 = 4
const int SUM_BIT1 = 4;  // 2^1 = 2
const int SUM_BIT0 = 5;  // 2^0 = 1  (LSB)

// Arduino output pins for the 7-segment display (common cathode)
const int SEG_A = 7;
const int SEG_B = 8;
const int SEG_C = 9;
const int SEG_D = 10;
const int SEG_E = 11;
const int SEG_F = 12;
const int SEG_G = 13;
const int SEG_DP = 6;  // decimal point used as minus-sign indicator

const int SEG_PINS[] = { SEG_A, SEG_B, SEG_C, SEG_D, SEG_E, SEG_F, SEG_G };

// Segment patterns for 0–7 (bit6=a, bit5=b, …, bit0=g; 1 = segment ON)
const byte DIGIT_PATTERNS[] = {
  0b1111110, // 0: a b c d e f
  0b0110000, // 1:   b c
  0b1101101, // 2: a b   d e   g
  0b1111001, // 3: a b c d     g
  0b0110011, // 4:   b c     f g
  0b1011011, // 5: a   c d   f g
  0b1011111, // 6: a   c d e f g
  0b1110000, // 7: a b c
};

void setup() {
  pinMode(SUM_BIT3, INPUT);
  pinMode(SUM_BIT2, INPUT);
  pinMode(SUM_BIT1, INPUT);
  pinMode(SUM_BIT0, INPUT);

  for (int i = 0; i < 7; i++) {
    pinMode(SEG_PINS[i], OUTPUT);
  }
  pinMode(SEG_DP, OUTPUT);

  Serial.begin(9600);
}

// Interprets 4 input bits as a two's-complement signed integer (–8 … +7)
int readSum() {
  int b3 = digitalRead(SUM_BIT3);
  int b2 = digitalRead(SUM_BIT2);
  int b1 = digitalRead(SUM_BIT1);
  int b0 = digitalRead(SUM_BIT0);
  return (-8 * b3) + (4 * b2) + (2 * b1) + b0;
}

void showDigit(int n) {
  int absVal = (n < 0) ? -n : n;
  if (absVal > 7) return;
  byte pattern = DIGIT_PATTERNS[absVal];
  for (int i = 0; i < 7; i++) {
    digitalWrite(SEG_PINS[i], (pattern >> (6 - i)) & 1);
  }
  // decimal point acts as minus sign for negative results
  digitalWrite(SEG_DP, n < 0 ? HIGH : LOW);
}

void loop() {
  int val = readSum();
  showDigit(val);
  Serial.println(val);
}
