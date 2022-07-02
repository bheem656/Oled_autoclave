// #include <Arduino.h>
#include <U8g2lib.h>
#include "HardwareTimer.h"

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);
HardwareTimer timer(1);

void Draw_Menu(void);
/********************  Global variables  ***************************/
#define encoder0PinA PA0 // right
#define encoder0PinB PA1 // left
#define selectButton PA2
volatile uint8_t encoder0Pos = 0;
volatile uint8_t count = 0;
byte dir = 0;

volatile bool page = 0;
volatile uint32_t counter = 0;

volatile uint32_t ticks = 0;
volatile uint32_t last_ticks = 0;
uint32_t last_ticks_n = 0;
///////////////////////////

// bool ledOn = false;
void OnTimer1Interrupt()
{
  ticks++;
  // ledOn = !ledOn;
  // digitalWrite(PC13, ledOn ? HIGH : LOW);
}

//////////////////////////////////////////////

void setup()
{

  pinMode(encoder0PinA, INPUT_PULLUP);
  pinMode(encoder0PinB, INPUT_PULLUP);
  pinMode(selectButton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoder0PinB), doEncoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(selectButton), doSelect, FALLING);

  // pinMode(PC13, OUTPUT);
  // Configure timer
  timer.setPrescaleFactor(2564); // Set prescaler to 2564 => timer frequency = 168MHz/2564 = 65522 Hz (from prediv'd by 1 clocksource of 168 MHz)
  timer.setOverflow(32761);      // Set overflow to 32761 => timer frequency = 65522 Hz / 32761 = 2 Hz
  timer.attachInterrupt(1, OnTimer1Interrupt);
  timer.refresh(); // Make register changes take effect
  timer.resume();  // Start

  u8g2.begin();
}

void loop()
{

  Draw_Menu();
}

void Draw_Menu()
{

  u8g2.firstPage();
  do
  {
    if (!page)
    {

      // u8g2_font_u8glib_4_tf   u8g2_font_pcsenior_8f
      // u8g2_font_u8glib_4_tr  u8g2_font_trixel_square_tr u8g2_font_profont10_tr u8g2_font_luBS08_tf
      // u8g2.setFont(u8g2_font_ncenB10_tr);

      u8g2.setFont(u8g2_font_profont10_tr);
      u8g2.drawStr(2, 60, "https://cognateindia.com"); // u8g2_font_u8glib_4_tr  u8g2_font_pcsenior_8f

      u8g2.setFont(u8g2_font_ncenB10_tr); // u8g2_font_ncenB10_tr  https://cognateindia.com/
      u8g2.drawStr(30, 10, "Autoclave");
      u8g2.drawHLine(20, 15, 90);
      u8g2.drawStr(10, 40, "<<<");
      u8g2.drawStr(90, 40, ">>>");

      switch (count)
      {
      case 0:
        u8g2.drawStr(45, 40, "Er 01");
        break;
      case 1:
        u8g2.drawStr(45, 40, "Er 02");
        break;

      case 2:
        u8g2.drawStr(45, 40, "Er 03");
        break;
      case 3:
        u8g2.drawStr(45, 40, "Er 04");
        break;

      case 4:
        u8g2.drawStr(45, 40, "Er 05");
        break;
      case 5:
        u8g2.drawStr(45, 40, "Er 06");
        break;

      case 6:
        u8g2.drawStr(45, 40, "Er 07");
        break;
      case 7:
        u8g2.drawStr(45, 40, "Er 08");
        break;

      case 8:
        u8g2.drawStr(45, 40, "Er 09");
        break;
      case 9:
        u8g2.drawStr(45, 40, "Er 10");
        break;

      case 10:
        u8g2.drawStr(45, 40, "Er 12");
        break;
      case 11:
        u8g2.drawStr(45, 40, "Er 14");
        break;

      case 12:
        u8g2.drawStr(45, 40, "Er 98");
        break;
      case 13:
        u8g2.drawStr(45, 40, "Er 99");
        break;

      default:
        break;
      }
    }

    else
    {

      // u8g2_font_u8glib_4_tf  u8g2_font_u8glib_4_tr u8g2_font_pcsenior_8f u8g2_font_trixel_square_tr u8g2_font_profont10_tr u8g2_font_luBS08_tf
      // u8g2.setFont(u8g2_font_ncenB10_tr);

      switch (count)
      {
      case 0:
        u8g2.drawStr(2, 25, "Steam ");
        u8g2.drawStr(2, 40, "Generator over");
        u8g2.drawStr(2, 55, "Temperature");
        break;
      case 1:
        u8g2.drawStr(2, 25, "Heating ");
        u8g2.drawStr(2, 40, "Ring over");
        u8g2.drawStr(2, 55, "Temperature");
        break;

      case 2:
        u8g2.drawStr(2, 25, "Chamber ");
        u8g2.drawStr(2, 40, "Over");
        u8g2.drawStr(2, 55, "Temperature");
        break;
      case 3:
        u8g2.drawStr(2, 25, "Fail to");
        u8g2.drawStr(2, 40, "Maintain Temp");
        u8g2.drawStr(2, 55, "And Pressure");
        break;

      case 4:
        u8g2.drawStr(2, 25, "Pressure");
        u8g2.drawStr(2, 40, "Not");
        u8g2.drawStr(2, 55, "Exhausted");
        break;
      case 5:
        u8g2.drawStr(2, 25, "Door open");
        u8g2.drawStr(2, 40, "During ");
        u8g2.drawStr(2, 55, "Cycle");
        break;

      case 6:
        u8g2.drawStr(2, 40, "Working");
        u8g2.drawStr(2, 55, "Overtime");
        break;
      case 7:
        u8g2.drawStr(2, 40, "Over");
        u8g2.drawStr(2, 55, "Pressure");
        break;

      case 8:
        u8g2.drawStr(2, 25, "In-chamber");
        u8g2.drawStr(2, 40, " Sensors temp.");
        u8g2.drawStr(2, 55, "Too high / Low");
        break;
      case 9:
        u8g2.drawStr(2, 25, "Temp. and ");
        u8g2.drawStr(2, 40, "Pressure");
        u8g2.drawStr(2, 55, "Doesn't match");
        break;

      case 10:
        u8g2.drawStr(2, 25, "Vacuum ");
        u8g2.drawStr(2, 40, "Fail over");
        u8g2.drawStr(2, 55, "Temperature");
        break;
      case 11:
        u8g2.drawStr(2, 25, "In-chamber");
        u8g2.drawStr(2, 40, " Sensors temp.");
        u8g2.drawStr(2, 55, "Differs too much");
        break;

      case 12:
        u8g2.drawStr(2, 25, "Out OF");
        u8g2.drawStr(2, 40, "Power");
        u8g2.drawStr(2, 55, "During cycle");
        break;
      case 13:
        u8g2.drawStr(2, 40, "Forced ");
        u8g2.drawStr(2, 55, "Exit");
        break;

      default:
        break;
      }

      // u8g2.drawStr(90, 40, ">>>");
    }

  } while (u8g2.nextPage());
}

/********************  Switch Interupt Function  ***************************/
void doSelect()
{

  // delay(700);
  delay(50);
  // detachInterrupt(digitalPinToInterrupt(selectButton));
  if ((ticks - last_ticks) >= 2)
  {
    last_ticks = ticks;
    page = !page;

    if (page)
    {
      detachInterrupt(digitalPinToInterrupt(encoder0PinB));
    }
    else
    {
      attachInterrupt(digitalPinToInterrupt(encoder0PinB), doEncoderA, CHANGE);
    }
  }

  // delay(500);
  // attachInterrupt(digitalPinToInterrupt(selectButton), doSelect, FALLING);
}

/**
 * @brief ISR function for rotation of switch
 * out :  Total count
 *
 */
void doEncoderA()
{
  delay(50);

  // last_ticks_n = ticks;
  if (digitalRead(encoder0PinA) == HIGH)
  {
    if (digitalRead(encoder0PinB) == LOW && encoder0Pos > 0)
    {
      encoder0Pos = encoder0Pos - 1;
      dir = 0;
    }
    else
    {
      encoder0Pos = encoder0Pos + 1;
      dir = 1;
    }
  }
  else
  {
    if (digitalRead(encoder0PinB) == LOW)
    {
      encoder0Pos = encoder0Pos + 1;
      dir = 1;
    }
    else
    {
      if (encoder0Pos > 0)
      {
        encoder0Pos = encoder0Pos - 1;
        dir = 0;
      }
    }
  }

  if (encoder0Pos <= 26)
  {
    count = encoder0Pos >> 1;
  }

  else
  {
    encoder0Pos = 0;
    count = encoder0Pos >> 1;
  }
}
