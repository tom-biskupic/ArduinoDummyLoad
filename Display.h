//
//  Constant Current Dummy Load
//
//  Copyright Runcible Software Pty Ltd, 2015
//

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <LiquidCrystal.h>
#include "LoadControl.h"

//
//  Manages the LCD and displays the fields selected. Implements blinking
//  of the field being edited.
//
class Display
{
public:

  //
  //  Used to identify the field to blink
  //
  enum EditField
  {
    NONE_FIELD,
    MODE_FIELD,
    MODE_QUAL_FIELD,
    AMPS_FIELD,
    HUNDRED_MILLI_AMPS_FIELD,
    MILLI_AMPS_FIELD  
  };
  
  static const int NUM_EDIT_FIELDS;
  
  Display();

  //
  //  Used to display a welcome message when the device
  //  initializes
  //
  void displayBanner();
    
  //
  //  Set the mode field
  //
  void setMode( const char *mode );

  //
  //  Set the mode qualifier field
  //
  void setModeQual( const char *modeQual );
  
  //
  //  Set the power state field (enabled/disabled)
  //
  void setPowerState( bool on);
  
  //
  //  Set the output current field in the display
  //
  void setCurrent( float current );
  
  //
  //  Print the field into the display. Blinks the currently
  //  edited field
  //
  void render();

  //
  //  Sets the field being edited. The edited field is blinked
  //
  void setFieldToEdit( const EditField field );
  
  //
  //  Returns the field being edited
  //
  const EditField getFieldToEdit() const;
  
private:

  bool blinkOff() const;

  void copyAndPad(const char *source, char *dest, int length) const;
  void copyAndPadWithBlink(const char *source, char *dest, int length,bool blinking) const;
  
  LiquidCrystal m_lcd;
  float         m_current;
  bool          m_enabled;
  const char    *m_mode;
  const char    *m_modeQual;
  EditField     m_editField;
};

#endif // _DISPLAY_H_



