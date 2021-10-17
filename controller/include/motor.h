#ifndef MOTOR_H
#define MOTOR_H

namespace controller{
    class Motor
    {
    public:
        Motor();
        void t_up(unsigned int left_speed, unsigned int right_speed);
        void t_stop();
        void t_down(unsigned int left_speed, unsigned int right_speed);
        void t_left(unsigned int left_speed, unsigned int right_speed);
        void t_right(unsigned int left_speed, unsigned int right_speed);
        void go(int left_speed, int right_speed);
    };
}

#endif