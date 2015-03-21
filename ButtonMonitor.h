//
//  Constant Current Dummy Load
//
//  Copyright Runcible Software Pty Ltd, 2015
//

#ifndef _BUTTON_MONITOR_H_
#define _BUTTON_MONITOR_H_

//
//  This class is used to monitor the state of an input line connected
//  to a button and determines if the button was clicked since
//  the last update
//
class ButtonMonitor
{
public:
  ButtonMonitor(int line);
  
  bool gotClick();
  
private:

  int   m_buttonLine;
  int   m_lastState;
};

#endif
