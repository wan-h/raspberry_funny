#include "buzzer.h"
#include <wiringPi.h>
#include <softTone.h>
#include <iostream>
#include <assert.h>

#define BuzPin    0


#define  CL1  131
#define  CL2  147
#define  CL3  165
#define  CL4  175
#define  CL5  196
#define  CL6  221
#define  CL7  248

#define  CM1  262
#define  CM2  294
#define  CM3  330
#define  CM4  350
#define  CM5  393
#define  CM6  441
#define  CM7  495

#define  CH1  525
#define  CH2  589
#define  CH3  661
#define  CH4  700
#define  CH5  786
#define  CH6  882
#define  CH7  990

namespace sensor{
Buzzer::Buzzer(){
    // 初始化GPIO
    assert(wiringPiSetup() != -1);
    assert(softToneCreate(BuzPin) != -1);
}
int Buzzer::doubleClock(){
    softToneWrite(BuzPin, CH7);	
	delay(500);
    softToneWrite(BuzPin, CH7);
}
}