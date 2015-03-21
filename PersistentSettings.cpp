//
//  Constant Current Dummy Load
//
//  Copyright Runcible Software Pty Ltd, 2015
//

#include "PersistentSettings.h"
#include <EEPROM.h>

namespace
{
  const unsigned int OUTPUT_MODE_OFFSET = 0;
  const unsigned int OUTPUT_QUAL_OFFSET = OUTPUT_MODE_OFFSET + sizeof(LoadControl::OutputMode);
  const unsigned int CURRENT_OFFSET = OUTPUT_QUAL_OFFSET + sizeof(LoadControl::ModeQualifier);
  const unsigned int EDIT_FIELD_OFFSET = CURRENT_OFFSET + sizeof(float);
}

template<typename T> 
void clamp( T& value, const T max, const T defaultValue)
{
  if ( value > max )
  {
    value = defaultValue;
  }
}

class PersistentSettingsImpl : public PersistentSettings
{
public:
  virtual const LoadControl::OutputMode getOutputMode() const
  {
    LoadControl::OutputMode returnValue;
    readValue((unsigned char *)&returnValue,sizeof(returnValue),OUTPUT_MODE_OFFSET);
    clamp(returnValue,LoadControl::MODE_SAWTOOTH,LoadControl::MODE_CONST_CURRENT);

    return returnValue;
  }
  
  virtual void setOutputMode( const LoadControl::OutputMode mode )
  {
    writeValue((const unsigned char *)&mode,sizeof(LoadControl::OutputMode),OUTPUT_MODE_OFFSET);
  }

  virtual const LoadControl::ModeQualifier getOutputModeQualifier() const
  {
    LoadControl::ModeQualifier returnValue;
    readValue((unsigned char *)&returnValue,sizeof(returnValue),OUTPUT_QUAL_OFFSET);
    clamp(returnValue,LoadControl::QUAL_1K,LoadControl::QUAL_NONE);

    return returnValue;
  }
  
  virtual void setOutputModeQualifier( const LoadControl::ModeQualifier mode )
  {
    writeValue((const unsigned char *)&mode,sizeof(LoadControl::ModeQualifier),OUTPUT_QUAL_OFFSET);
  }

  virtual const float getCurrent() const
  {
    float returnValue;
    readValue((unsigned char *)&returnValue,sizeof(returnValue),CURRENT_OFFSET);
    if ( isnan(returnValue) )
    {
      returnValue = 0.0f;
    }
    
    clamp(returnValue,3.99f,1.0f);

    return returnValue;
  }
  
  virtual void setCurrent( const float current )
  {
    writeValue((const unsigned char *)&current,sizeof(current),CURRENT_OFFSET);
  }
  
  virtual void setEditField( const Display::EditField value )
  {
    writeValue((const unsigned char*)&value,sizeof(value),EDIT_FIELD_OFFSET);
  }

  virtual const Display::EditField getEditField() const
  {
    Display::EditField returnValue;
    readValue((unsigned char *)&returnValue,sizeof(returnValue),EDIT_FIELD_OFFSET);
    clamp(returnValue,Display::MILLI_AMPS_FIELD,Display::MODE_FIELD);

    return returnValue;
  }

protected:

  void readValue( unsigned char *target, unsigned int size, unsigned int offset ) const
  {
    for(int i=0;i<size;i++)
    {
      target[i] = EEPROM.read(offset+i);
    }
  }
  
  void writeValue( const unsigned char *value, unsigned int size, unsigned int offset ) const
  {
    for(int i=0;i<size;i++)
    {
      EEPROM.write(offset+i,value[i]);
    }    
  }
};

const LoadControl::OutputMode PersistentSettings::getOutputMode() const
{
  return getInstance()->getOutputMode();
}

void PersistentSettings::setOutputMode( const LoadControl::OutputMode mode )
{
  getInstance()->setOutputMode(mode);
}

const LoadControl::ModeQualifier PersistentSettings::getOutputModeQualifier() const
{
  return getInstance()->getOutputModeQualifier();
}

void PersistentSettings::setOutputModeQualifier( const LoadControl::ModeQualifier mode )
{
  getInstance()->setOutputModeQualifier(mode);
}

const float PersistentSettings::getCurrent() const
{
  return getInstance()->getCurrent();
}

void PersistentSettings::setCurrent( const float current )
{
  getInstance()->setCurrent(current);
}

void PersistentSettings::setEditField( const Display::EditField value )
{
  getInstance()->setEditField(value);
}

const Display::EditField PersistentSettings::getEditField() const
{
  return getInstance()->getEditField();
}

PersistentSettings *PersistentSettings::getInstance()
{
  static PersistentSettingsImpl *instance = NULL;
  
  if ( instance == NULL )
  {
    instance = new PersistentSettingsImpl();
  }

  return instance;
}

