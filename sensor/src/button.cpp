#include "button.h"
#include <assert.h>
#include <wiringPi.h>

#define BtnPin		24
#define Gpin		21
#define Rpin	    22

namespace sensor{
Button::Button(){
    // 初始化GPIO
    assert(wiringPiSetup() != -1);
    pinMode(BtnPin, INPUT);
}
int Button::push(){
    if(1 == digitalRead(BtnPin)){
			delay(10);
        if(1 == digitalRead(BtnPin)){
            return 1;
        }
    }
    return 0;
}
}