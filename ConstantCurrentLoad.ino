//
//  Constant Current Dummy Load
//
//  Copyright Runcible Software Pty Ltd, 2015
//

//
//  For some reason if I don't include these here but only include
//  them in the h files, they don't get included. Weirdness in Arduino
//  environment
//
#include <SPI.h>
#include <DAC_MCP49xx.h>
#include <Encoder.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>

#include "Controller.h"
#include "Display.h"
#include "LoadControl.h"

LoadControl loadControl;
Display      displayObj;
Controller    controller(&displayObj,&loadControl);

void setup() 
{
}

void loop() 
{
  controller.update();  
}

