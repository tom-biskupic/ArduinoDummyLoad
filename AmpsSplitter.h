//
//  Constant Current Dummy Load
//
//  Copyright Runcible Software Pty Ltd, 2015
//

#ifndef _AMPS_SPLITTER_H_
#define _AMPS_SPLITTER_H_

//
//  This class is used to split the electrical current into
//  seperate fields so they can be edited by the main controller
//
class AmpsSplitter
{
public:
  AmpsSplitter();
 
  //
  //  Sets the electrical current value
  //
  void setCurrent(const float value);
  
  //
  //  Returns the electrical current. This changes
  //  as a result of changes to the amps, hundres of mili-amps
  //  and miliamps fields
  //
  const float getCurrent() const;
  
  //
  //  Returns the amps part of the electrical
  //  current setting
  //
  int getAmps() const;
  
  //
  //  Sets the amps part of the electrical current setting
  //  
  void setAmps(const int value);
  
  //
  //  Returns the hundreds of mili-amps part of the electrical
  //  current setting
  //
  int getHundresOfMiliAmps() const;
  
  //
  //  Sets the hundreds of mili-amps part of the electrical
  //  current setting
  //  
  void setHundresOfMiliAmps(const int value);
  
  //
  //  Returns the mili-amps part of the electrical
  //  current setting
  //
  int getMiliAmps() const;
  
  //
  //  Sets the mili-amps part of the electrical
  //  current setting
  //  
  void setMiliAmps(const int value);
  
private:

  int  m_amps;
  int  m_hundredsOfMilis;
  int  m_milis;
  
};

#endif // _AMPS_SPLITTER_H_
