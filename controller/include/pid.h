#ifndef PID_H
#define PID_H



namespace controller{
    //对变量的声明
    struct PID_STRUCT{
        double SetPoint;            //设定值
        double Kp;                  //比例系数
        double Ki;                  //积分系数
        double Kd;                  //微分系数
    };
    class PID
    {
        private:
            PID_STRUCT sPID;
            double LastError = 0;           //最后一次误差Er[-1]
            double PrevError = 0;           //最后第二次误差Er[-2]
            double SumError = 0;            //误差积分  
        public:
            PID(double Kp, double Ki, double Kd, double SetPoint);
            double cal(double input);
            void resetSumError();
    };
}

#endif