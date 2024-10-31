#include "microbit.h"

#ifndef NEOPIXEL_H
#define NEOPIXEL_H

namespace neopixel {
    void setLeds(int pin, int* leds, int numLeds) {
        microbit::pins::setMode(pin, 0x00000001);
        if (pin < 32) {
            int msk = 1 << pin;
            for (int i = 0; i < numLeds; i++) {
                int val = leds[i];
                for (int j = 0; j < 24; j++) {
                    if (val & 0x800000) {
                        ::write(P0_BASE + P_OUTSET, msk);
                        for (volatile int k = 0; k < 55; k++);
                        ::write(P0_BASE + P_OUTCLR, msk);
                    } else {
                        ::write(P0_BASE + P_OUTSET, msk);
                        ::write(P0_BASE + P_OUTCLR, msk);
                        for (volatile int k = 0; k < 55; k++);
                    }
                    val <<= 1;
                }
            }
            ::write(P0_BASE + P_OUTCLR, msk);
            for (volatile int k = 0; k < 8000; k++);
        } else {
            int msk = 1 << (pin - 32);
            for (int i = 0; i < numLeds; i++) {
                int val = leds[i];
                for (int j = 0; j < 24; j++) {
                    if (val & 0x800000) {
                        ::write(P1_BASE + P_OUTSET, msk);
                        for (volatile int k = 0; k < 55; k++);
                        ::write(P1_BASE + P_OUTCLR, msk);
                    } else {
                        ::write(P1_BASE + P_OUTSET, msk);
                        ::write(P1_BASE + P_OUTCLR, msk);
                        for (volatile int k = 0; k < 55; k++);
                    }
                    val <<= 1;
                }
            }
            ::write(P1_BASE + P_OUTCLR, msk);
            for (volatile int k = 0; k < 8000; k++);
        }
    }
    void fill(int pin, int numLeds, int val = 0x000000) {
        int buf[numLeds] = {};
        for (int i = 0; i < numLeds; i++) {
            buf[i] = val;
        }
        setLeds(pin, buf, numLeds);
    }
}

#endif