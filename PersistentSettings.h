//
//  Constant Current Dummy Load
//
//  Copyright Runcible Software Pty Ltd, 2015
//

#include "LoadControl.h"
#include "Display.h"

#ifndef __PERSISTENT_SETTINGS_H__
#define  __PERSISTENT_SETTINGS_H__

//
//  PersistentSettings is a singleton class used to
//  store and retrieve the last settings used on the 
//  dummy load
//
class PersistentSettings
{
public:
  /**
  Store the updated output mode.
  */
  virtual const LoadControl::OutputMode getOutputMode() const;
  
  /**
  Returns the output mode last used.
  */
  virtual void setOutputMode( const LoadControl::OutputMode mode );

  /**
  Store the updated output mode qualifier
  */
  virtual const LoadControl::ModeQualifier getOutputModeQualifier() const;
  
  /**
  Returns the output mode qualifier last used.
  */
  virtual void setOutputModeQualifier( const LoadControl::ModeQualifier mode );
  
  /**
  Store the updated output current
  */
  virtual const float getCurrent() const;
  
  /**
  Returns the output current last used.
  */
  virtual void setCurrent( const float current );
  
  /**
  Set edit field
  */
  virtual void setEditField( const Display::EditField value );

  /**
  Returns the field
  */
  virtual const Display::EditField getEditField() const;
  
protected:

  static PersistentSettings *getInstance();
};

#endif //  __PERSISTENT_SETTINGS_H__
