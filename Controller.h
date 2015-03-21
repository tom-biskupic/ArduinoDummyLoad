//
//  Constant Current Dummy Load
//
//  Copyright Runcible Software Pty Ltd, 2015
//

#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "ButtonMonitor.h"
#include "LoadControl.h"
#include "AmpsSplitter.h"
#include "PersistentSettings.h"

#define ENCODER_USE_INTERRUPTS 1

#include <Encoder.h>

class Display;

//
//  Main controller for the constant current dummy load. 
//  Checks the input devices to see if the user has clicked a button or
//  turned the shaft encoder, updates the display based on the user inputs,
//  updates the LoadControl with based on the users input.
//
//  Will load the last settings from NVRAM on initial update
//
class Controller
{
public:

  //
  //  Initialise the controller with the given display and LoadControl
  //
  Controller(  Display      *display,
               LoadControl  *loadControl );
  
  //
  //  Check the user inputs and update the system state as required
  //  Update the display to enable blinking etc.
  //
  void update();

private:

  //
  //  Converst the mode value to a string for display
  //
  const char *modeToString( const LoadControl::OutputMode mode ) const;

  //
  //  Converst the mode qualifier value to a string for display
  //
  const char *modeQualToString( const LoadControl::ModeQualifier qual ) const;

  //
  //  Updates the editing mode
  //
  void updateEditing();
  
  //
  //  Updates the currently edited field based on the shaft
  //  encoder movement
  //
  void updateScrollChanges();
  
  //
  //  Reads the encoder delta, adds it to the current value and clamps the result
  //
  int encoderToLimitedRange(int low,int high,int currentValue);
  
  //
  //  Same as above but if the value goes passed the high limit it
  //  rolls back around to the low limit
  //
  int encoderToRollingRange(int low,int high,int currentValue);
  
  //
  //  Adjusts the current value so if it is above the hight value
  //  or below the low value it is 'rolled' around the end
  //
  int toRollingRange(int low, int high, int currentValue);
  
  int getStartValue(int currentValue);
  
  void updateModeField();
  void updateModeQualField();
  void updateAmpsField();
  void updateHundreMiliAmpsField();
  void updateMiliAmpsField();

  void setMode( const LoadControl::OutputMode mode );
  void setModeQualifier( const LoadControl::ModeQualifier mode );
  void setCurrent( const float current);
  
  bool                        m_new;
  Display                     *m_display;
  LoadControl                 *m_loadControl;
  ButtonMonitor               m_modeButton;
  ButtonMonitor               m_enableButton;
  Encoder                     m_selEncoder;  
  int                         m_lastEncoderValue;
  int                         m_startValue;
  bool                        m_newEditField;
  LoadControl::ModeQualifier  m_lastQual;
  int                         m_lastEdited;
  AmpsSplitter                m_ampsSplitter;
  PersistentSettings          m_persistentSettings;
};

#endif // _CONTROLLER_H

