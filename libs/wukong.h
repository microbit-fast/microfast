#include "microbit.h"

#ifndef WUKONG_H
#define WUKONG_H

namespace wukong {
    const unsigned char ADDR = 0x10;
    void setLightIntensity(int light) {
        microbit::i2c::writeBuffer(ADDR, (unsigned char[4]){0x11, 0x0, 0x0, 0x0}, 4, 400);
        microbit::i2c::writeBuffer(ADDR, (unsigned char[4]){0x12, 0x96, 0x0, 0x0}, 4, 400);
        microbit::i2c::writeBuffer(ADDR, (unsigned char[4]){0x12, light, 0x0, 0x0}, 4, 400);
        microbit::i2c::writeBuffer(ADDR, (unsigned char[4]){0x11, 0xA0, 0x0, 0x0}, 4, 400);
    }
    void setMotorSpeed(int motor, int speed) {
        int dir = 0x1;
        if (speed < 0) {
            dir = 0x2;
            speed = -speed;
        }
        microbit::i2c::writeBuffer(ADDR, (unsigned char[4]){motor, dir, speed, 0x0}, 4, 400);
    }
    void setMotorsSpeed(int motor1, int motor2) {
        setMotorSpeed(1, motor1);
        setMotorSpeed(2, motor2);
    }
    void setServoAngle(int servo, int angle) {
        microbit::i2c::writeBuffer(ADDR, (unsigned char[4]){servo + 0x3, angle, 0x0, 0x0}, 4, 400);
    }
}

#endif