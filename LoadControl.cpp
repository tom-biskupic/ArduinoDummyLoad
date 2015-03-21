//
//  Constant Current Dummy Load
//
//  Copyright Runcible Software Pty Ltd, 2015
//

#include "LoadControl.h"

namespace
{
  //
  //  Must match the entries in the ModeQualifier enum
  //
  const unsigned long QUAL_TO_FREQEUENCY[] = 
  {
        0,        //  QUAL_NONE
        50,       //  QUAL_50H,
        100,      //  QUAL_100H,
        1000      //  QUAL_1K
  };
  
  LoadControl *instance = NULL;
  
  const unsigned long TIMER1_RESOLUTION = 65536UL;
  const unsigned long SAWTOOTH_UPDATE_FREQ = 5000UL;
  
//  const float RESISTENCE = 1.0f;
}

ISR(TIMER1_COMPA_vect)
{
  LoadControl::ISREntry();
}

LoadControl::LoadControl() 
  : m_dac(DAC_MCP49xx::MCP4922,10),
    m_current(1.0f),
    m_enabled(false),
    m_mode(MODE_CONST_CURRENT),
    m_qualifier(QUAL_NONE),
    m_periodInMicros(0),
    m_halfPeriod(0),
    m_pulseOn(false)
{
  instance = this;
}

void LoadControl::ISREntry()
{
  instance->update();
}

int calcDacValue( int baseCount, float baseValue, float gradient,float voltage)
{
  return round(1000.0f * ((voltage - baseValue) / gradient))+baseCount;
}

void LoadControl::setCurrentOnDAC(float current)
{
  //m_dac.output( (int)(current * 1000.0 + 0.012632192 ));
  //m_dac.output( (int)(current * 1.001869644 * 1000.0 + 1.0));
  m_dac.output(round(current * 1000.0));
  
  int   dacValue = 0;
  
  if ( current < 1.0f )
  {
    dacValue = calcDacValue(0,0.0f,0.997839,current);
  }
  else if ( current < 2.0f )
  {
    dacValue = calcDacValue(1000,0.99785,0.994220f,current);
  }
  else
  {
    dacValue = calcDacValue(2000,1.99207,0.995360f,current);
  }
  
  m_dac.output(dacValue);
  
  //m_dac.output(round(current/0.9985 * 1000.0f));
}

void LoadControl::initTimer()
{
  //
  //  Stop interrupts
  //
  cli();

  TCCR1A = 0;
  TCCR1B = 0;
  
  //
  //  Initialize counter value to 0
  //
  TCNT1  = 0;
    
  // 
  //  Calculate compare match value and choose pre-scaler
  //  value for double frequency.
  //
  unsigned long frequency = m_mode == MODE_SAWTOOTH ? (SAWTOOTH_UPDATE_FREQ/2) : QUAL_TO_FREQEUENCY[m_qualifier];
  unsigned long cycles = (F_CPU)/frequency/2;
  unsigned char clockBits = 0;
  unsigned long counter = 0;
  
  if ( cycles < TIMER1_RESOLUTION )
  {
    clockBits = (1 << CS10);
    counter = cycles;
  }
  else if ( (cycles/8) < TIMER1_RESOLUTION)
  {
    clockBits = (1 << CS11);
    counter = cycles/8UL;
  }
  else if ( (cycles/64) < TIMER1_RESOLUTION )
  {
    clockBits = (1 << CS11) | (1 << CS10);
    counter = cycles/64UL;
  }
  else if ( (cycles/256) < TIMER1_RESOLUTION )
  {
    clockBits = (1 << CS12);
    counter = cycles/256UL;
  }
  else
  {
    clockBits = (1 << CS12) | (1 << CS10);
    counter = cycles/1024UL;
  }
  
  OCR1A = (unsigned short)(counter-1);
  
  // 
  //  Turn on CTC mode so it clears the count
  //  when it matches (WGM12)
  //  
  TCCR1B |= (1 << WGM12) | clockBits;
  
  // 
  //  Enable timer compare interrupt
  //
  TIMSK1 |= (1 << OCIE1A);
  
  //
  //  Re-enable interrupts
  //
  sei();
}

void LoadControl::stopTimer()
{
  //
  //  Stop interrupts
  //
  cli();

  //
  //  set timer0 interrupt
  //
  TCCR1A = 0;
  TCCR1B = 0;
  
  //
  //  Initialize counter value to 0
  //
  TCNT1  = 0;
  TIMSK1 &= ~(1 << OCIE1A);
  
  //
  //  Re-enable interrupts
  //
  sei();
}

void LoadControl::enable(bool enabled)
{
  m_enabled = enabled;
  
  if ( m_enabled )
  {
    if ( m_mode != MODE_CONST_CURRENT )
    {
      if ( m_mode == MODE_SAWTOOTH )
      {
        unsigned long frequency = QUAL_TO_FREQEUENCY[m_qualifier];
        m_sawtoothCounter = 0;
        m_sawtoothMaxCounter = SAWTOOTH_UPDATE_FREQ/frequency;
        m_sawtoothIncrement = m_current/(float)(m_sawtoothMaxCounter);
      }
      initTimer();
    }
    else
    {
      setCurrentOnDAC(m_current);
    }
  }
  else
  {
    if ( m_mode != MODE_CONST_CURRENT )
    {
      stopTimer();
      setCurrentOnDAC(0.0f);
    }
    else
    {
      setCurrentOnDAC(0.0f);
    }
  }
}

bool LoadControl::isEnabled() const
{
  return m_enabled;
}

void LoadControl::setCurrent( float current )
{
  m_current = current;

  if ( m_enabled )
  {
    setCurrentOnDAC(m_current);
  }
}

float LoadControl::getCurrent() const
{
  return m_current;
}

void LoadControl::update()
{
  if ( m_enabled )
  {
    switch(m_mode)
    {
      case MODE_PULSED:
      {
        m_pulseOn = ! m_pulseOn;
        setCurrentOnDAC(m_pulseOn ? m_current : 0.0f);
        break;
      }
      case MODE_SAWTOOTH:
      {
        m_sawtoothCounter++;
        if ( m_sawtoothCounter > m_sawtoothMaxCounter )
        {
          m_sawtoothCounter = 0;
        }
        
        float current = (float)(m_sawtoothCounter) * m_sawtoothIncrement;
        
        setCurrentOnDAC(current);
        break;
      }    
      case MODE_CONST_CURRENT:
      default:
        break;
    }
  }
  else
  {
    setCurrentOnDAC(0.0f);
  }
}

void LoadControl::setOutputMode( const OutputMode mode )
{
  m_mode = mode;
}

const LoadControl::OutputMode LoadControl::getOutputMode() const
{
  return m_mode;
}

void LoadControl::setOutputModeQualifier(const ModeQualifier qualifier)
{
  m_qualifier = qualifier;
  
  switch(m_qualifier)
  {
    case QUAL_50H:
      m_periodInMicros = 1000000/50;
      break;        
    case QUAL_100H:
      m_periodInMicros = 1000000/100;
      break;
    case QUAL_1K:
      m_periodInMicros = 1000000/1000;
      break;
    case QUAL_NONE:
    default:
      break;
  }
 
   m_halfPeriod = m_periodInMicros/2;
}

const LoadControl::ModeQualifier LoadControl::getOutputModeQualifier() const
{
  return m_qualifier;
}

