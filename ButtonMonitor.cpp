//
//  Constant Current Dummy Load
//
//  Copyright Runcible Software Pty Ltd, 2015
//

#include "ButtonMonitor.h"
#include "Arduino.h"

namespace
{
  int DEBOUNCE_TIME = 50;
}

ButtonMonitor::ButtonMonitor(int line) : m_lastState(0), m_buttonLine(line)
{
  pinMode(m_buttonLine,INPUT);
}
  
bool ButtonMonitor::gotClick()
{
  int newState = digitalRead(m_buttonLine);
  bool gotClick = false;
  
  if ( ! m_lastState && newState )
  {
    gotClick = true;
  }
  
  m_lastState = newState;
    
  return gotClick;
}

