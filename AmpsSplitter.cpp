//
//  Constant Current Dummy Load
//
//  Copyright Runcible Software Pty Ltd, 2015
//

#include "AmpsSplitter.h"

AmpsSplitter::AmpsSplitter() 
  :   m_amps(0),
      m_hundredsOfMilis(0),
      m_milis(0)
{
}
 
void AmpsSplitter::setCurrent(const float value)
{
  float rest = value;
  m_amps = (int)rest;
  rest -= ((float)m_amps);
  rest *= 10.0f;
  
  m_hundredsOfMilis = (int)rest;
  rest -= (float)m_hundredsOfMilis;
  rest *= 100.0f;
  
  m_milis = (int)rest;
}

const float AmpsSplitter::getCurrent() const
{
  return (float)m_amps + (float)m_hundredsOfMilis * 0.1f + (float)m_milis * 0.001;
}

int AmpsSplitter::getAmps() const
{
  return m_amps;
}

void AmpsSplitter::setAmps(const int value)
{
  m_amps = value;
}

int AmpsSplitter::getHundresOfMiliAmps() const
{
  return m_hundredsOfMilis;
}

void AmpsSplitter::setHundresOfMiliAmps(const int value)
{
  m_hundredsOfMilis = value;
}

int AmpsSplitter::getMiliAmps() const
{
  return m_milis;
}

void AmpsSplitter::setMiliAmps(const int value)
{
  m_milis = value;
}

