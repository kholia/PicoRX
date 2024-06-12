// https://awsh.org/si5351-quadrature/
// Test program to evaluate Si5351 quadrature output using Etherkit 5351 Library
// Example based on ZL2CTM  Jan. 2019 blog post
// Si5351 Quadrature Clock Output down to 3MHz
// Credit to Brian Harper M1CEM and Miguel Bartié PY2OHH
//
// QSD note: The opamp output voltage was 12 mV  with an S9 signal input
// regardless of supply voltage (5 or 12VDC).

#include <Wire.h>

#include <stdint.h>

#include <si5351.h>
Si5351 si5351;

// Define Constants and Vaviables
static const uint64_t pll_min = 60000000000ULL;
static const uint64_t pll_max = 90000000000ULL;

// Globals
volatile uint64_t freq = 28074000ULL;
volatile int Even_Divisor = 0;
volatile int oldEven_Divisor = 0;
uint64_t pfreq;

void EvenDivisor()
{
  if (freq < 6850000)
  {
    Even_Divisor = 126;
  }
  if ((freq >= 6850000) && (freq < 9500000))
  {
    Even_Divisor = 88;
  }
  if ((freq >= 9500000) && (freq < 13600000))
  {
    Even_Divisor = 64;
  }
  if ((freq >= 13600000) && (freq < 17500000))
  {
    Even_Divisor = 44;
  }
  if ((freq >= 17500000) && (freq < 25000000))
  {
    Even_Divisor = 34;
  }
  if ((freq >= 25000000) && (freq < 36000000))
  {
    Even_Divisor = 24;
  }
  if ((freq >= 36000000) && (freq < 45000000)) {
    Even_Divisor = 18;
  }
  if ((freq >= 45000000) && (freq < 60000000)) {
    Even_Divisor = 14;
  }
  if ((freq >= 60000000) && (freq < 80000000)) {
    Even_Divisor = 10;
  }
  if ((freq >= 80000000) && (freq < 100000000)) {
    Even_Divisor = 8;
  }
  if ((freq >= 100000000) && (freq < 146600000)) {
    Even_Divisor = 6;
  }
  if ((freq >= 150000000) && (freq < 220000000)) {
    Even_Divisor = 4;
  }
}

void SendFrequency()
{
  EvenDivisor();
  si5351.set_freq_manual(freq * SI5351_FREQ_MULT, Even_Divisor * freq * SI5351_FREQ_MULT, SI5351_CLK0);
  si5351.set_freq_manual(freq * SI5351_FREQ_MULT, Even_Divisor * freq * SI5351_FREQ_MULT, SI5351_CLK1);
  si5351.set_phase(SI5351_CLK0, 0);
  si5351.set_phase(SI5351_CLK1, Even_Divisor);

  if (Even_Divisor != oldEven_Divisor) {
    si5351.pll_reset(SI5351_PLLA);
    oldEven_Divisor = Even_Divisor;
  }
  Serial.print("Even Divisor:  ");
  Serial.println(Even_Divisor);
  Serial.print("New Freq:  ");
  Serial.println(freq);
  Serial.print("Sending: ");
  pfreq = (freq * SI5351_FREQ_MULT);
  Serial.println(pfreq);
}

// Debug helper
void led_flash()
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(250);
  digitalWrite(LED_BUILTIN, LOW);
  delay(250);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(250);
  digitalWrite(LED_BUILTIN, LOW);
  delay(250);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(250);
  digitalWrite(LED_BUILTIN, LOW);
  delay(250);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(250);
  digitalWrite(LED_BUILTIN, LOW);
}

void setup()
{
  int ret;

  Serial.begin(115200);

  // I2C pins
  Wire.setSDA(0);
  Wire.setSCL(1);
  Wire.begin();

  // Initialize the Si5351
  // ret = si5351.init(SI5351_CRYSTAL_LOAD_8PF, 26000000, 0); // I am using a 26 MHz TCXO
  ret = si5351.init(SI5351_CRYSTAL_LOAD_8PF, 25000000, 0);
  if (ret != true) {
    while (1)
      led_flash();
  }

  si5351.drive_strength(SI5351_CLK0, SI5351_DRIVE_2MA);
  si5351.drive_strength(SI5351_CLK1, SI5351_DRIVE_2MA);
  si5351.drive_strength(SI5351_CLK2, SI5351_DRIVE_2MA);

  SendFrequency();
}

void loop()
{
  /* si5351_set_freq(7074000ULL * 100ULL, SI5351_CLK1);
    si5351_set_freq(10136000ULL * 100ULL, SI5351_CLK1);
    si5351_set_freq(14074000ULL * 100ULL, SI5351_CLK1);
    si5351_set_freq(18100000ULL * 100ULL, SI5351_CLK1);
    si5351_set_freq(21074000ULL * 100ULL, SI5351_CLK1);
    si5351_set_freq(24915000ULL * 100ULL, SI5351_CLK1);
    si5351_set_freq(28074000ULL * 100ULL, SI5351_CLK1); */

  delay(100);

}
