/****************************所谓PID即为P（比例）+I（积分）+D（微分）*****************************************************
**(1)P控制（比例控制）：    e(t) = SP - y(t);        SP--设定值   e(t)--误差值（设定的值和当前的误差） y(t)--反馈值（当前值）
**适用于滞后性不大的系统   u(t) = e(t) * P;        u(t)--输出值（需要输出的） P--比例系数
**
**(2)PI控制（比例+积分）：  u(t) = Kp*e(t) + Ki∑e(t) + u0
**可使系统进入稳态无稳态误差u(t)--输出 Kp--比例放大系数 ki--积分放大系数 e(t)--误差 u0--控制量基准值（基础偏差）
**
**(3)PD控制(微分控制)：     
**微分项解决响应速度问题   积分项解决稳定性
**(4)PID控制（比例积分微分控制）：
**微分项解决响应速度问题   u(t) = Kp*e(t) + Ki∑e(t)+ Kd[e(t) – e(t-1)] + u0            
**采样周期（即反馈值的采样周期）
**控制周期（就是每隔多长时间进行一次PID运算，并将结果输出）
****************************小结：P（比例）--即时性 I(积分)--响应速度 D（微分）--稳定性************************************
*/
#include<iostream>
#include<string>
#include"pid.h"

namespace controller{
PID::PID(double Kp, double Ki, double Kd, double SetPoint){
    sPID.Kp = Kp;
    sPID.Ki = Ki;
    sPID.Kd = Kd;
    sPID.SetPoint = SetPoint;
}
double PID::cal(double input){
    double dError;                              //当前微分
    double Error;                               //当前偏差
    Error = sPID.SetPoint - input;           //偏差值=设定值-输入值（当前值）
    SumError += Error;                      //积分=积分+偏差  --偏差的累加
    dError = LastError - PrevError;     //当前微分 = 最后误差 - 之前误差
    PrevError = LastError;              //更新“之前误差”
    LastError = Error;                      //更新“最后误差”
    //比例项 = 比例常数 * 偏差
    //积分项 = 积分常数 * 误差积分
    //微分项 = 微分常数 * 当前微分
    std::cout << "Kp: " << Error << "; Ki: " << SumError << " Kd: " << dError << std::endl;
    return (sPID.Kp * Error + sPID.Ki * SumError + sPID.Kd * dError);
}
void PID::resetSumError(){
    SumError = 0.0;
}
}