//
//  Constant Current Dummy Load
//
//  Copyright Runcible Software Pty Ltd, 2015
//

#include "Display.h"

namespace
{
  const int LCD_LEN=16;
  const int LCD_LINES=2;
  
  const char *ENABLED_STRING="Enabled";
  const char *DISABLED_STRING="Disabled";
}

const int Display::NUM_EDIT_FIELDS(4);

Display::Display() 
  :   m_lcd(7, 6, 5, 4, 3, 2), 
      m_current(0.0f), 
      m_enabled(false), 
      m_mode(NULL),
      m_modeQual(NULL),
      m_editField(NONE_FIELD)
{
  m_lcd.begin(LCD_LEN,LCD_LINES);
}

void Display::displayBanner()
{
  m_lcd.setCursor(0,0);
  m_lcd.print("Tom's Constant ");
  m_lcd.setCursor(0,1);
  m_lcd.print("current load ");
  m_lcd.display();
}
    
void Display::setMode( const char *mode )
{
  m_mode = mode;
}
  
void Display::setModeQual( const char *modeQual )
{
  m_modeQual = modeQual;
}

void Display::setPowerState( bool on)
{
  m_enabled = on;
}
  
void Display::setCurrent( float current )
{
  m_current = current;
}

void Display::copyAndPad(const char *source, char *dest, int length) const
{
  int sourceLen = strlen(source);
  int i;
  for(i=0;i<length;i++)
  {
    if ( i < sourceLen )
    {
      dest[i] = source[i];
    }
    else
    {
      dest[i] = ' ';
    }
  }
}

void  Display::copyAndPadWithBlink(const char *source, char *dest, int length,bool blinking) const
{
  if ( blinking && blinkOff() )
  {
    copyAndPad("",dest,length);
  }
  else
  {
    copyAndPad(source,dest,length);
  }
}

bool Display::blinkOff() const
{
  return ((millis()/200) % 2)==0;
}

void Display::render()
{
  char buffer[LCD_LEN+1];
  char currentStr[6];
  
  copyAndPadWithBlink(m_mode,buffer,10,m_editField == MODE_FIELD);
  copyAndPadWithBlink(m_modeQual,buffer+10,6,m_editField == MODE_QUAL_FIELD);
  
  buffer[LCD_LEN] = '\0';
  m_lcd.setCursor(0,0);
  m_lcd.print(buffer);

  
  copyAndPadWithBlink( (m_enabled ? ENABLED_STRING : DISABLED_STRING), buffer, 11,m_enabled);
  
  dtostrf(m_current,5,3,currentStr);
  copyAndPadWithBlink(currentStr,buffer+11,1,m_editField == AMPS_FIELD);
  buffer[12]='.';
  
  copyAndPadWithBlink(currentStr+2,buffer+13,1,m_editField == HUNDRED_MILLI_AMPS_FIELD);
  copyAndPadWithBlink(currentStr+3,buffer+14,2,m_editField == MILLI_AMPS_FIELD);
  buffer[LCD_LEN] = '\0';
  
  m_lcd.setCursor(0,1);
  m_lcd.print(buffer);
}

void Display::setFieldToEdit( const EditField field )
{
  m_editField = field;
}

const Display::EditField Display::getFieldToEdit() const
{
  return m_editField;
}

// Const
// Pulse100Hz
// Ramp
// 
// 0123456789012345
// Const  100Hz
// Disabled   3.123
