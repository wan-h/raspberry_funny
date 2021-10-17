#include "motor.h"
#include <assert.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softPwm.h>


int PWMA = 1;
int AIN2 = 2;
int AIN1 = 3;

int PWMB = 4;
int BIN2 = 5;
int BIN1 = 6;


namespace controller{
Motor::Motor(){
    // 初始化GPIO
    assert(wiringPiSetup() != -1);
    pinMode (1, OUTPUT);	//PWMA
    pinMode (2, OUTPUT);	//AIN2
    pinMode (3, OUTPUT);	//AIN1
	
    pinMode (4, OUTPUT);	//PWMB
    pinMode (5, OUTPUT);	//BIN2
	pinMode (6, OUTPUT);    //BIN1
	
	/*PWM output*/
    softPwmCreate(PWMA, 0, 100);
	softPwmCreate(PWMB, 0, 100); 
}
void Motor::t_up(unsigned int left_speed, unsigned int right_speed)
{
	 digitalWrite(AIN2,0);
	 digitalWrite(AIN1,1);
	 softPwmWrite(PWMA,left_speed);
	 
	 digitalWrite(BIN2,0);
	 digitalWrite(BIN1,1);
	 softPwmWrite(PWMB,right_speed);
}

void Motor::t_stop()
{
	 digitalWrite(AIN2,0);
	 digitalWrite(AIN1,0);
	 softPwmWrite(PWMA,0);
	 
	 digitalWrite(BIN2,0);
	 digitalWrite(BIN1,0);
	 softPwmWrite(PWMB,0);
}

void Motor::t_down(unsigned int left_speed, unsigned int right_speed)
{
	 digitalWrite(AIN2,1);
	 digitalWrite(AIN1,0);
	 softPwmWrite(PWMA,left_speed);
	 
	 digitalWrite(BIN2,1);
	 digitalWrite(BIN1,0);
	 softPwmWrite(PWMB,right_speed);
}

void Motor::t_left(unsigned int left_speed, unsigned int right_speed)
{
	 digitalWrite(AIN2,1);
	 digitalWrite(AIN1,0);
	 softPwmWrite(PWMA,left_speed);
	 
	 digitalWrite(BIN2,0);
	 digitalWrite(BIN1,1);
	 softPwmWrite(PWMB,right_speed);
}

void Motor::t_right(unsigned int left_speed, unsigned int right_speed)
{
	 digitalWrite(AIN2,0);
	 digitalWrite(AIN1,1);
	 softPwmWrite(PWMA,left_speed);
	 
	 digitalWrite(BIN2,1);
	 digitalWrite(BIN1,0);
	 softPwmWrite(PWMB,right_speed);
}
void Motor::go(int left_speed, int right_speed)
{
	if(left_speed >= 0){
		if(right_speed >= 0){
			t_up(left_speed, right_speed);
		} else {
			t_right(left_speed, abs(right_speed));
		}
	} else {
		if(right_speed >= 0){
			t_left(abs(left_speed), right_speed);
		} else {
			t_down(abs(left_speed), abs(right_speed));
		}
	}
}
}