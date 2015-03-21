//
//  Constant Current Dummy Load
//
//  Copyright Runcible Software Pty Ltd, 2015
//

#include "Controller.h"

#include "LoadControl.h"
#include "Display.h"

namespace
{
  const char *MODE_NAMES[] = { "Constant","Pulsed","Sawtooth" };
  const char *MODE_QUAL_NAMES[] = { "","  50Hz", " 100Hz","  1kHz" };
}

Controller::Controller(
    Display      *display,
    LoadControl  *loadControl ) 
        :   m_display(display), 
            m_loadControl(loadControl),
            m_modeButton(A0), 
            m_enableButton(A1),
            m_selEncoder(9,8),
            m_new(true),
            m_lastEncoderValue(0),
            m_newEditField(true),
            m_startValue(0),
            m_lastQual(LoadControl::QUAL_50H),
            m_lastEdited(Display::MODE_FIELD)
{
  m_ampsSplitter.setCurrent(m_loadControl->getCurrent());
}

const char *Controller::modeToString( const LoadControl::OutputMode mode ) const
{
  return MODE_NAMES[mode];
}

const char *Controller::modeQualToString( 
  const LoadControl::ModeQualifier  qual ) const
{
  return MODE_QUAL_NAMES[qual];
}

void Controller::update()
{
  if ( m_new )
  {
    m_display->displayBanner();
    delay(1000);
    
    m_loadControl->setOutputMode(m_persistentSettings.getOutputMode());
    m_loadControl->setOutputModeQualifier(m_persistentSettings.getOutputModeQualifier());
    m_loadControl->setCurrent(m_persistentSettings.getCurrent());
    
    m_display->setMode(modeToString(m_loadControl->getOutputMode()));
    m_display->setModeQual(modeQualToString(m_loadControl->getOutputModeQualifier()));
    m_display->setCurrent(m_loadControl->getCurrent());
    m_display->setPowerState(m_loadControl->isEnabled());
    m_display->setFieldToEdit(m_persistentSettings.getEditField());
    m_new = false;
  }

  if ( m_enableButton.gotClick() )
  {
    bool newPowerState = !m_loadControl->isEnabled();
    m_loadControl->enable(newPowerState);
    m_display->setPowerState(newPowerState);
    if ( ! newPowerState )
    {
      m_display->setFieldToEdit((Display::EditField)m_lastEdited);
      m_lastEncoderValue = m_selEncoder.read();
      m_newEditField=true;
    }
    else
    {
      m_lastEdited = m_display->getFieldToEdit();
      m_display->setFieldToEdit(Display::NONE_FIELD);

      //
      //  Render once here as after this we won't render again
      //  until they disable the output
      //
      m_display->render();
    }
  } 
  else if ( ! m_loadControl->isEnabled() )
  {
    updateEditing();
  }
  
  m_display->render();
}

void Controller::updateEditing()
{
  if (  ! m_loadControl->isEnabled() 
        &&
        m_modeButton.gotClick() )
  {
    Display::EditField editField = m_display->getFieldToEdit();
    editField = (Display::EditField)toRollingRange(
            Display::MODE_FIELD,
            Display::MILLI_AMPS_FIELD,
            (editField+1));
    
    //
    //  If we are in const current mode, skip the qualifier field
    //
    if (  m_loadControl->getOutputMode() == LoadControl::MODE_CONST_CURRENT
          &&
          editField == Display::MODE_QUAL_FIELD )
    {
      editField = Display::AMPS_FIELD;
    }
    
    m_display->setFieldToEdit(editField);
    m_persistentSettings.setEditField(editField);
    m_lastEncoderValue = m_selEncoder.read();
    m_newEditField=true;
  }
  else if (! m_loadControl->isEnabled() )
  {
    updateScrollChanges();
  }
}

void Controller::updateScrollChanges()
{
  switch(m_display->getFieldToEdit())
  {
    case Display::MODE_FIELD:
    {
      updateModeField();
      break;
    }
    case Display::MODE_QUAL_FIELD:
    {
      updateModeQualField();
      break;
    }
    case Display::AMPS_FIELD:
    {
      updateAmpsField();
      break;
    }
    case Display::HUNDRED_MILLI_AMPS_FIELD:
    {
      updateHundreMiliAmpsField();
      break;
    }    
    case Display::MILLI_AMPS_FIELD:
    {
      updateMiliAmpsField();
      break;
    }
    case Display::NONE_FIELD:
    default:
      //
      //  Do nothing
      //
      break;
  }
}

int Controller::encoderToLimitedRange(int low,int high,int startValue)
{
  unsigned int newEncoderValue = m_selEncoder.read();
  unsigned int encoderDelta = newEncoderValue - m_lastEncoderValue;

  int newValue = startValue + encoderDelta;
  if ( newValue < low )
  {
    newValue = low;
    m_lastEncoderValue = newEncoderValue;
  }
  else if ( newValue > high )
  {
    newValue = high;
    m_lastEncoderValue = (high - startValue);
  }
  
  return newValue;
}

void Controller::updateModeField()
{
    LoadControl::OutputMode mode = m_loadControl->getOutputMode();
    mode = (LoadControl::OutputMode)encoderToRollingRange(
              LoadControl::MODE_CONST_CURRENT,
              LoadControl::MODE_SAWTOOTH,
              getStartValue(mode));
              
    setMode(mode);
    
    if ( mode == LoadControl::MODE_CONST_CURRENT )
    {
      setModeQualifier(LoadControl::QUAL_NONE);
    }
    else
    {
      if ( mode == LoadControl::MODE_SAWTOOTH )
      {
        //
        //  In Sawtooth mode we can't have 1KHz
        //
        if ( m_lastQual == LoadControl::QUAL_1K )
        {
          m_lastQual = LoadControl::QUAL_100H;
        }
      }

      setModeQualifier(m_lastQual);
    }
}

void Controller::updateModeQualField()
{
  LoadControl::ModeQualifier qual = m_loadControl->getOutputModeQualifier();
  
  qual = (LoadControl::ModeQualifier)encoderToRollingRange(
          LoadControl::QUAL_50H,
          m_loadControl->getOutputMode() == LoadControl::MODE_SAWTOOTH ? LoadControl::QUAL_100H : LoadControl::QUAL_1K,
          getStartValue(qual));
          
  m_lastQual = qual;
  setModeQualifier(qual);
}

void Controller::updateAmpsField()
{
  m_ampsSplitter.setAmps(
    encoderToRollingRange(0,3,getStartValue(m_ampsSplitter.getAmps())));

  setCurrent(m_ampsSplitter.getCurrent());
}

void Controller::updateHundreMiliAmpsField()
{
  m_ampsSplitter.setHundresOfMiliAmps(
    encoderToRollingRange(0,9,getStartValue(m_ampsSplitter.getHundresOfMiliAmps())));

  setCurrent(m_ampsSplitter.getCurrent());
}

void Controller::updateMiliAmpsField()
{
  m_ampsSplitter.setMiliAmps(
    encoderToRollingRange(0,99,getStartValue(m_ampsSplitter.getMiliAmps())));

  setCurrent(m_ampsSplitter.getCurrent());
}

int Controller::encoderToRollingRange(int low,int high,int startValue)
{
  return toRollingRange(low,high,startValue + (m_selEncoder.read() - m_lastEncoderValue)/2);  
}

int Controller::toRollingRange(int low, int high, int currentValue)
{
  int n = (high - low + 1);
  
  return (((currentValue - low) % n + n) % n) + low;
}

int Controller::getStartValue(int currentValue)
{
  if ( m_newEditField )
  {
    m_startValue = currentValue;
    m_newEditField = false;
  }
  
  return m_startValue;
}

void Controller::setMode( const LoadControl::OutputMode mode )
{
  m_persistentSettings.setOutputMode(mode);
  m_display->setMode(modeToString(mode));
  m_loadControl->setOutputMode(mode);
}

void Controller::setModeQualifier( const LoadControl::ModeQualifier mode )
{
  m_persistentSettings.setOutputModeQualifier(mode);
  m_display->setModeQual(modeQualToString(mode));
  m_loadControl->setOutputModeQualifier(mode);
}

void Controller::setCurrent( const float current)
{
  m_persistentSettings.setCurrent(current);
  m_display->setCurrent(current);
  m_loadControl->setCurrent(current);
}


