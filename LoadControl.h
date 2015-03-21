//
//  Constant Current Dummy Load
//
//  Copyright Runcible Software Pty Ltd, 2015
//

#ifndef _LOAD_CONTROL_H_
#define _LOAD_CONTROL_H_

#include <DAC_MCP49xx.h>

//
//  Controls the current output and implement the different
//  output modes. Also supports enabling/disabling the output
//
class LoadControl
{
public:

  //
  //  The output modes supported
  //
  enum OutputMode
  {
    MODE_CONST_CURRENT=0,
    MODE_PULSED=1,
    MODE_SAWTOOTH=2
  };
  
  //
  //  Defines the mode qualifiers for the output modes
  //  Note only QUAL_NONE is supported for constant current mode
  //  Note also that only QUAL_50H, QUAL_100H are supported for MODE_SAWTOOTH
  //
  enum ModeQualifier
  {
    QUAL_NONE=0,
    QUAL_50H=1,
    QUAL_100H=2,
    QUAL_1K=3
  };
  
  LoadControl();
  
  //
  //  Enables/disables the output
  //
  void enable(bool enabled=false);
  
  //
  //  Returns true if the output is enabled
  //
  bool isEnabled() const;
  
  //
  //  Sets the output current or maximum output current in
  //  pulse or sawtooth mode
  //
  void setCurrent( float current );
  
  //
  //  Returns the current output current
  //
  float getCurrent() const;
  
  //
  //  Sets the output mode
  //
  void setOutputMode( const OutputMode mode );
  
  //
  //  Returns the output mode
  //
  const OutputMode getOutputMode() const;
  
  //
  //  Sets the output mode qualifier
  //
  void setOutputModeQualifier(const ModeQualifier qualifier);
  
  //
  //  Returns the output mode qualifier
  //
  const ModeQualifier getOutputModeQualifier() const;
  
  //
  //  Updates the output (deprecated)
  //  
  void update();
  
  //
  //  Called by the interrupt service routine
  //
  static void ISREntry();
  
private:

  void setCurrentOnDAC(float current);  
  void initTimer();
  void stopTimer();
  
  float           m_current;
  bool            m_enabled;
  OutputMode      m_mode;
  ModeQualifier   m_qualifier;
  int             m_periodInMicros;
  int             m_halfPeriod;  
  DAC_MCP49xx     m_dac;
  
  //
  //  State for pulsing
  //
  bool            m_pulseOn;
  unsigned long   m_sawtoothCounter;
  unsigned long   m_sawtoothMaxCounter;
  float           m_sawtoothIncrement;
};

#endif
