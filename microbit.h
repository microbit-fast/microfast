#include "pinmap.h"

#ifndef MICROBIT_H
#define MICROBIT_H

#define LOG_LEVEL             3

#define P0_BASE               0x50000000
#define P1_BASE               0x50000300
#define P_OUT                 0x504
#define P_OUTSET              0x508
#define P_OUTCLR              0x50C
#define P_IN                  0x510
#define PULL_NONE             0x0
#define PULL_DOWN             0x1
#define PULL_UP               0x3

#define RNG_BASE              0x4000D000
#define RNG_START             0x000
#define RNG_STOP              0x004
#define RNG_EVENTS_VALRDY     0x100
#define RNG_VAL               0x508

#define TIMER_BASE            0x40008000
#define TIMER_START           0x000
#define TIMER_STOP            0x004
#define TIMER_CLEAR           0x00C
#define TIMER_EVENTS_COMPARE0 0x140
#define TIMER_MODE            0x504
#define TIMER_BITMODE         0x508
#define TIMER_PRESCALER       0x510
#define TIMER_CC0             0x540

#define SERIAL_BASE           0x40002000
#define SERIAL_STARTRX        0x000
#define SERIAL_STOPRX         0x004
#define SERIAL_STARTTX        0x008
#define SERIAL_STOPTX         0x00C
#define SERIAL_RDXRDY         0x108
#define SERIAL_ENABLE         0x500
#define SERIAL_PSEL_TX        0x50C
#define SERIAL_PSEL_RX        0x514
#define SERIAL_RX             0x518
#define SERIAL_TX             0x51C
#define SERIAL_BAUDRATE       0x524

#define I2C0_BASE             0x40003000
#define I2C1_BASE             0x40004000
// #define I2C_STARTRX           0x000
#define I2C_STARTTX           0x008
#define I2C_STOP              0x014
#define I2C_SUSPEND           0x01C
#define I2C_RESUME            0x020
#define I2C_EVENTS_STOPPED    0x104
// #define I2C_EVENTS_RXREADY    0x108
#define I2C_EVENTS_TXSENT     0x11C
#define I2C_ENABLE            0x500
#define I2C_PSEL_SCL          0x508
#define I2C_PSEL_SDA          0x50C
// #define I2C_RXD               0x518
#define I2C_TXD               0x51C
#define I2C_FREQUENCY         0x524
#define I2C_ADDRESS           0x588

#define ADC_BASE              0x40007000
#define ADC_START             0x000
#define ADC_SAMPLE            0x004
#define ADC_STOP              0x008
#define ADC_EVENTS_STARTED    0x100
#define ADC_EVENTS_END        0x104
#define ADC_EVENTS_STOPPED    0x114
#define ADC_ENABLE            0x500
#define ADC_CH_0_PSELP        0x510
#define ADC_CH_0_PSELN        0x514
#define ADC_CH_0_CONFIG       0x518
#define ADC_RESOLUTION        0x5F0
#define ADC_RESULT_PTR        0x62C
#define ADC_RESULT_MAXCNT     0x630

#define TEMP_BASE             0x4000C000
#define TEMP_START            0x000
#define TEMP_STOP             0x004
#define TEMP_EVENTS_DATARDY   0x100
#define TEMP_TEMP             0x508


#define abs(x) ((x)>0?(x):-(x))

extern "C" void write(unsigned int reg, unsigned int val);
extern "C" unsigned int read(unsigned int reg);
extern "C" void nop(void);

void delay(unsigned int us, unsigned int sixteenths = 0) {
    write(TIMER_BASE + TIMER_MODE, 0x0);
    write(TIMER_BASE + TIMER_EVENTS_COMPARE0, 0x0);
    write(TIMER_BASE + TIMER_START, 0x1);
    write(TIMER_BASE + TIMER_CLEAR, 0x1);
    write(TIMER_BASE + TIMER_CC0, (us * 16) + sixteenths);
    while (!read(TIMER_BASE + TIMER_EVENTS_COMPARE0));
    write(TIMER_BASE + TIMER_STOP, 0x1);
}

void error(const char* str) {
    if (LOG_LEVEL >= 0) {
        write(SERIAL_BASE + SERIAL_ENABLE, 0x4);
        write(SERIAL_BASE + SERIAL_STARTTX, 0x1);
        write(SERIAL_BASE + SERIAL_BAUDRATE, 0x01D7E000);
        write(SERIAL_BASE + SERIAL_PSEL_TX, 0x00000006);
        const char* start = "\033[1;31merror: ";
        const char* end = "\033[0m\n\r";
        for (int i = 0; start[i] != '\0'; i++) {
            write(SERIAL_BASE + SERIAL_TX, start[i]);
            delay(500);
        }
        for (int i = 0; str[i] != '\0'; i++) {
            write(SERIAL_BASE + SERIAL_TX, str[i]);
            delay(500);
        }
        for (int i = 0; end[i] != '\0'; i++) {
            write(SERIAL_BASE + SERIAL_TX, end[i]);
            delay(500);
        }
        write(SERIAL_BASE + SERIAL_STOPTX, 0x1);
        write(SERIAL_BASE + SERIAL_ENABLE, 0x0);
    }
}

void warning(const char* str) {
    if (LOG_LEVEL >= 1) {
        write(SERIAL_BASE + SERIAL_ENABLE, 0x4);
        write(SERIAL_BASE + SERIAL_STARTTX, 0x1);
        write(SERIAL_BASE + SERIAL_BAUDRATE, 0x01D7E000);
        write(SERIAL_BASE + SERIAL_PSEL_TX, 0x00000006);
        const char* start = "\033[1;33mwarning: ";
        const char* end = "\033[0m\n\r";
        for (int i = 0; start[i] != '\0'; i++) {
            write(SERIAL_BASE + SERIAL_TX, start[i]);
            delay(500);
        }
        for (int i = 0; str[i] != '\0'; i++) {
            write(SERIAL_BASE + SERIAL_TX, str[i]);
            delay(500);
        }
        for (int i = 0; end[i] != '\0'; i++) {
            write(SERIAL_BASE + SERIAL_TX, end[i]);
            delay(500);
        }
        write(SERIAL_BASE + SERIAL_STOPTX, 0x1);
        write(SERIAL_BASE + SERIAL_ENABLE, 0x0);
    }
}

void debug(const char* str) {
    if (LOG_LEVEL >= 2) {
        write(SERIAL_BASE + SERIAL_ENABLE, 0x4);
        write(SERIAL_BASE + SERIAL_STARTTX, 0x1);
        write(SERIAL_BASE + SERIAL_BAUDRATE, 0x01D7E000);
        write(SERIAL_BASE + SERIAL_PSEL_TX, 0x00000006);
        const char* start = "\033[1;36mdebug: ";
        const char* end = "\033[0m\n\r";
        for (int i = 0; start[i] != '\0'; i++) {
            write(SERIAL_BASE + SERIAL_TX, start[i]);
            delay(500);
        }
        for (int i = 0; str[i] != '\0'; i++) {
            write(SERIAL_BASE + SERIAL_TX, str[i]);
            delay(500);
        }
        for (int i = 0; end[i] != '\0'; i++) {
            write(SERIAL_BASE + SERIAL_TX, end[i]);
            delay(500);
        }
        write(SERIAL_BASE + SERIAL_STOPTX, 0x1);
        write(SERIAL_BASE + SERIAL_ENABLE, 0x0);
    }
}

void trace(const char* str) {
    if (LOG_LEVEL >= 3) {
        write(SERIAL_BASE + SERIAL_ENABLE, 0x4);
        write(SERIAL_BASE + SERIAL_STARTTX, 0x1);
        write(SERIAL_BASE + SERIAL_BAUDRATE, 0x01D7E000);
        write(SERIAL_BASE + SERIAL_PSEL_TX, 0x00000006);
        const char* start = "\033[1;32mtrace: ";
        const char* end = "\033[0m\n\r";
        for (int i = 0; start[i] != '\0'; i++) {
            write(SERIAL_BASE + SERIAL_TX, start[i]);
            delay(500);
        }
        for (int i = 0; str[i] != '\0'; i++) {
            write(SERIAL_BASE + SERIAL_TX, str[i]);
            delay(500);
        }
        for (int i = 0; end[i] != '\0'; i++) {
            write(SERIAL_BASE + SERIAL_TX, end[i]);
            delay(500);
        }
        write(SERIAL_BASE + SERIAL_STOPTX, 0x1);
        write(SERIAL_BASE + SERIAL_ENABLE, 0x0);
    }
}

namespace std {
    unsigned int strlen(const char* str) {
        unsigned int len = 0;
        while (str[len] != '\0') {
            len++;
        }
        return len;
    }
    char* strcpy(char* dest, const char* src) {
        char* original_dest = dest;
        while (*src != '\0') {
            *dest = *src;
            dest++;
            src++;
        }
        *dest = '\0';
        return original_dest;
    }
    bool isdigit(const char* str) {
        if (str == nullptr) {
            return false;
        }
        while (*str != '\0') {
            if (*str < '0' || *str > '9') {
                return false;
            }
            str++;
        }
        return true;
    }
    bool strcmp(const char* str1, const char* str2) {
        while (*str1 && (*str1 == *str2)) {
            ++str1;
            ++str2;
        }
        return (*str1 == *str2);
    }
    char* itoa(long value, char *string, int radix = 10) {
        char tmp[33];
        char *tp = tmp;
        long i;
        unsigned long v;
        int sign;
        char *sp;
        if (radix > 36 || radix <= 1)
        {
            return 0 ;
        }
        sign = (radix == 10 && value < 0);
        if (sign)
        {
            v = -value;
        }
        else
        {
            v = (unsigned long)value;
        }
        while (v || tp == tmp)
        {
            i = v % radix;
            v = v / radix;
            if (i < 10)
            *tp++ = i+'0';
            else
            *tp++ = i + 'a' - 10;
        }
        sp = string;
        if (sign)
            *sp++ = '-';
        while (tp > tmp)
            *sp++ = *--tp;
        *sp = 0;
        return string;
    }
}

class string {
private:
    char* data;
    int len;
public:
    string() {
        data = "";
        len = 0;
    }
    string(char* str) {
        len = std::strlen(str);
        data = str;
        // std::strcpy(data, str);  //NOTE: ???
    }
    string(const char* str) {
        len = std::strlen(str);
        data = (char*)str;
    }/*
    string(char chr) {
        static char tmp[2];
        tmp[0] = chr;
        tmp[1] = '\0';
        data = tmp;
    }*/
    string(int number) {
        char buffer[255];
        char* tmp = buffer;
        std::itoa(number, tmp, 10);
        len = std::strlen(tmp);
        data = tmp;
    }
    string(long int number) {
        char buffer[255];
        char* tmp = buffer;
        std::itoa(number, tmp);
        len = std::strlen(tmp);
        data = tmp;
    }
    string(long long int number) {
        char buffer[255];
        char* tmp = buffer;
        std::itoa(number, tmp);
        len = std::strlen(tmp);
        data = tmp;
    }
    string(unsigned int number) {
        char buffer[255];
        char* tmp = buffer;
        std::itoa(number, tmp);
        len = std::strlen(tmp);
        data = tmp;
    }
    string(unsigned long int number) {
        char buffer[255];
        char* tmp = buffer;
        std::itoa(number, tmp);
        len = std::strlen(tmp);
        data = tmp;
    }
    string(unsigned long long int number) {
        char buffer[255];
        char* tmp = buffer;
        std::itoa(number, tmp);
        len = std::strlen(tmp);
        data = tmp;
    }
    string(const string& str) {
        len = str.len;
        data = new char[len + 1];
        std::strcpy(data, str.data);
    }
    ~string() {
    }/*
    operator int() const {
        char* str;
        std::strcpy(str, data);
        int result = 0;
        bool isNegative = false;
        while (*str == ' ') {
            str++;
        }
        if (*str == '-') {
            isNegative = true;
            str++;
        } else if (*str == '+') {
            str++;
        }
        while (*str >= '0' && *str <= '9') {
            result = result * 10 + (*str - '0');
            str++;
        }
        return isNegative ? -result : result;
    }
    operator double() const {
        char* str;
        std::strcpy(str, data);
        double result = 0.0;
        double divisor = 10.0;
        bool isNegative = false;
        bool inDecimal = false;
        while (*str == ' ') {
            str++;
        }
        if (*str == '-') {
            isNegative = true;
            str++;
        } else if (*str == '+') {
            str++;
        }
        while (*str) {
            if (*str == '.') {
                inDecimal = true;
            } else if (std::isdigit(str)) {
                if (inDecimal) {
                    result += (*str - '0') / divisor;
                    divisor *= 10.0;
                } else {
                    result = result * 10.0 + (*str - '0');
                }
            } else {
                break;
            }
            str++;
        }
        return isNegative ? -result : result;
    }*/
    const char* c_str() {
        return data;
    }
    operator const char*() const {
        return data;
    }
    operator char*() const {
        return data;
    }
    string operator+(const string& other) const {
        char tmp[255];
        char* new_data = tmp;
        std::strcpy(new_data, data);
        std::strcpy(new_data + len, other.data);
        string result = new_data;
        return result;
    }
    string operator+(const char& n) const {
        string other = n;
        char tmp[255];
        char* new_data = tmp;
        std::strcpy(new_data, data);
        std::strcpy(new_data + len, other.data);
        string result = new_data;
        return result;
    }
    string operator+(const char* n) const {
        string other = n;
        char tmp[255];
        char* new_data = tmp;
        std::strcpy(new_data, data);
        std::strcpy(new_data + len, other.data);
        string result = new_data;
        return result;
    }
    string operator+(const int& n) const {
        string other = n;
        char tmp[255];
        char* new_data = tmp;
        std::strcpy(new_data, data);
        std::strcpy(new_data + len, other.data);
        string result = new_data;
        return result;
    }
    string operator+(const long int& n) const {
        string other = n;
        char tmp[255];
        char* new_data = tmp;
        std::strcpy(new_data, data);
        std::strcpy(new_data + len, other.data);
        string result = new_data;
        return result;
    }
    string operator+(const long long int& n) const {
        string other = n;
        char tmp[255];
        char* new_data = tmp;
        std::strcpy(new_data, data);
        std::strcpy(new_data + len, other.data);
        string result = new_data;
        return result;
    }
    string operator+(const unsigned int& n) const {
        string other = n;
        char tmp[255];
        char* new_data = tmp;
        std::strcpy(new_data, data);
        std::strcpy(new_data + len, other.data);
        string result = new_data;
        return result;
    }
    string operator+(const unsigned long int& n) const {
        string other = n;
        char tmp[255];
        char* new_data = tmp;
        std::strcpy(new_data, data);
        std::strcpy(new_data + len, other.data);
        string result = new_data;
        return result;
    }
    string operator+(const unsigned long long int& n) const {
        string other = n;
        char tmp[255];
        char* new_data = tmp;
        std::strcpy(new_data, data);
        std::strcpy(new_data + len, other.data);
        string result = new_data;
        return result;
    }
    bool operator==(const char* str) const {
        return std::strcmp(data, str);
    }
    bool operator==(char str) const {
        return std::strcmp(data, (char*)str);
    }
    bool operator==(const string& other) const {
        return std::strcmp(data, other.data);
    }
    char& operator[](int index) const {
        if (index < 0) {
            return data[(std::strlen(data) + index)];
        } else {
            return data[index];
        }
    }
};

namespace microbit {
    namespace serial {
        void openTX(int tx = UART_TX) {
            ::write(SERIAL_BASE + SERIAL_ENABLE, 0x4);
            ::write(SERIAL_BASE + SERIAL_STARTTX, 0x1);
            ::write(SERIAL_BASE + SERIAL_BAUDRATE, 0x01D7E000);
            ::write(SERIAL_BASE + SERIAL_PSEL_TX, tx);
        }
        void closeTX() {
            ::write(SERIAL_BASE + SERIAL_STOPTX, 0x1);
            ::write(SERIAL_BASE + SERIAL_ENABLE, 0x0);
        }
        void openRX(int rx = UART_RX) {
            ::write(SERIAL_BASE + SERIAL_ENABLE, 0x4);
            ::write(SERIAL_BASE + SERIAL_STARTRX, 0x1);
            ::write(SERIAL_BASE + SERIAL_BAUDRATE, 0x01D7E000);
            ::write(SERIAL_BASE + SERIAL_PSEL_RX, rx);
        }
        void closeRX() {
            ::write(SERIAL_BASE + SERIAL_STOPRX, 0x1);
            ::write(SERIAL_BASE + SERIAL_ENABLE, 0x0);
        }
        void write(const char* str, const char* end = "\n\r") {
            openTX();
            for (int i = 0; str[i] != '\0'; i++) {
                ::write(SERIAL_BASE + SERIAL_TX, str[i]);
                ::delay(500);
            }
            for (int i = 0; end[i] != '\0'; i++) {
                ::write(SERIAL_BASE + SERIAL_TX, end[i]);
                ::delay(500);
            }
            closeTX();
        }
    }
    namespace pins {
        void setMode(int pin, unsigned int mode) {
            if (pin < 32) {
                ::write(P0_BASE + 0x700 + (pin * 4), mode);
            } else {
                ::write(P1_BASE + 0x700 + ((pin - 32) * 4), mode);
            }
        }
        void write(int pin, int value) {
            setMode(pin, 0x00000001);
            if (pin < 32) {
                if (value) {
                    ::write(P0_BASE + P_OUTSET, (1 << pin));
                } else {
                    ::write(P0_BASE + P_OUTCLR, (1 << pin));
                }
            } else {
                if (value) {
                    ::write(P1_BASE + P_OUTSET, (1 << (pin - 32)));
                } else {
                    ::write(P1_BASE + P_OUTCLR, (1 << (pin - 32)));
                }
            }
        }
        int read(int pin, int pull = PULL_NONE) {
            int result = 0;
            setMode(pin, 0x00000000 | (pull << 2));
            if (pin < 32) {
                result = (::read(P0_BASE + P_IN) >> pin) & 0x1;
            } else {
                result = (::read(P1_BASE + P_IN) >> (pin - 32)) & 0x1;
            }
            return result;
        }
        void setDigitalValue(int pin, int value) {
            write(pin, value);
        }
        int getDigitalValue(int pin, int def = -1) {
            int up = read(pin, PULL_UP);
            int down = read(pin, PULL_DOWN);
            if (up && down) {
                return 1;
            }
            if (!up && !down) {
                return 0;
            }
            return def;
        }
        int getAnalogValue(int pin) {
            int p = 0;
            short value = 0;
            switch (pin) {
                case 2:
                    p = 1;
                    break;
                case 3:
                    p = 2;
                    break;
                case 4:
                    p = 3;
                    break;
                case 5:
                    p = 4;
                    break;
                case 28:
                    p = 5;
                    break;
                case 29:
                    p = 6;
                    break;
                case 30:
                    p = 7;
                    break;
                case 31:
                    p = 8;
                    break;
                default:
                    return 0;
            }
            ::write(ADC_BASE + ADC_RESOLUTION, 0x1);
            ::write(ADC_BASE + ADC_ENABLE, 0x1);
            ::write(ADC_BASE + ADC_CH_0_CONFIG, 0x00001000);//, 0x00001200);
            ::write(ADC_BASE + ADC_CH_0_PSELP, p);
            ::write(ADC_BASE + ADC_CH_0_PSELN, 0x0);
            ::write(ADC_BASE + ADC_RESULT_PTR, (unsigned int)&value);
            ::write(ADC_BASE + ADC_RESULT_MAXCNT, 0x1);
            ::write(ADC_BASE + ADC_START, 0x1);
            while (!::read(ADC_BASE + ADC_EVENTS_STARTED));
            ::write(ADC_BASE + ADC_EVENTS_STARTED, 0x0);
            ::write(ADC_BASE + ADC_SAMPLE, 0x1);
            while (!::read(ADC_BASE + ADC_EVENTS_END));
            ::write(ADC_BASE + ADC_EVENTS_END, 0x0);
            ::write(ADC_BASE + ADC_STOP, 0x1);
            while (!::read(ADC_BASE + ADC_EVENTS_STOPPED));
            ::write(ADC_BASE + ADC_EVENTS_STOPPED, 0x0);
            ::write(ADC_BASE + ADC_ENABLE, 0x0);
            if (value < 0)
                value = 0;
            return value;
        }
    }
    namespace i2c0 {
        void init(int scl = I2C_SCL, int sda = I2C_SDA) {
            ::write(I2C1_BASE + I2C_ENABLE, 0x0);
            microbit::pins::setMode(scl, 0x0000060E);
            microbit::pins::setMode(sda, 0x0000060E);
            ::write(I2C0_BASE + I2C_EVENTS_TXSENT, 0x0);
            ::write(I2C0_BASE + I2C_PSEL_SCL, I2C_SCL);
            ::write(I2C0_BASE + I2C_PSEL_SDA, I2C_SDA);
            ::write(I2C0_BASE + I2C_ENABLE, 0x5);
        }
        void write(unsigned char addr, unsigned char val, int frequency = 100) {
            ::write(I2C0_BASE + I2C_FREQUENCY, frequency == 250 ? 0x04000000 : frequency == 400 ? 0x06680000 : 0x01980000);
            if (::read(I2C0_BASE + I2C_ENABLE) != 0x5) init();
            ::write(I2C0_BASE + I2C_ADDRESS, addr);
            ::write(I2C0_BASE + I2C_RESUME, 0x1);
            ::write(I2C0_BASE + I2C_TXD, val);
            ::write(I2C0_BASE + I2C_STARTTX, 0x1);
            int timeout = 10000;
            while (!::read(I2C0_BASE + I2C_EVENTS_TXSENT) && timeout-- != 0);
            ::write(I2C0_BASE + I2C_EVENTS_TXSENT, 0x0);
            ::write(I2C0_BASE + I2C_EVENTS_STOPPED, 0x0);
            ::write(I2C0_BASE + I2C_STOP, 0x1);
            while (!::read(I2C0_BASE + I2C_EVENTS_STOPPED));
        }
        void writeBuffer(unsigned char addr, unsigned char* data, int size, int frequency = 100) {
            ::write(I2C0_BASE + I2C_FREQUENCY, frequency == 250 ? 0x04000000 : frequency == 400 ? 0x06680000 : 0x01980000);
            if (::read(I2C0_BASE + I2C_ENABLE) != 0x5) init();
            ::write(I2C0_BASE + I2C_ADDRESS, addr);
            ::write(I2C0_BASE + I2C_RESUME, 0x1);
            ::write(I2C0_BASE + I2C_TXD, *data++);
            ::write(I2C0_BASE + I2C_STARTTX, 0x1);
            int timeout = 10000;
            while (size-- != 0) {
                timeout = 10000;
                while (!::read(I2C0_BASE + I2C_EVENTS_TXSENT) && timeout-- != 0);
                ::write(I2C0_BASE + I2C_EVENTS_TXSENT, 0x0);
                ::write(I2C0_BASE + I2C_TXD, *data++);
            }
            ::write(I2C0_BASE + I2C_EVENTS_STOPPED, 0x0);
            ::write(I2C0_BASE + I2C_STOP, 0x1);
            while (!::read(I2C0_BASE + I2C_EVENTS_STOPPED));
        }
    }
    namespace i2c1 {
        void init(int scl = I2C_INT_SCL, int sda = I2C_INT_SDA) {
            ::write(I2C1_BASE + I2C_ENABLE, 0x0);
            microbit::pins::setMode(scl, 0x0000060E);
            microbit::pins::setMode(sda, 0x0000060E);
            ::write(I2C1_BASE + I2C_EVENTS_TXSENT, 0x0);
            ::write(I2C1_BASE + I2C_PSEL_SCL, I2C_SCL);
            ::write(I2C1_BASE + I2C_PSEL_SDA, I2C_SDA);
            ::write(I2C1_BASE + I2C_FREQUENCY, 0x01980000);
            ::write(I2C1_BASE + I2C_ENABLE, 0x5);
        }
        void write(unsigned char addr, unsigned char val, int frequency = 100) {
            ::write(I2C1_BASE + I2C_FREQUENCY, frequency == 250 ? 0x04000000 : frequency == 400 ? 0x06680000 : 0x01980000);
            if (::read(I2C1_BASE + I2C_ENABLE) != 0x5) init();
            ::write(I2C1_BASE + I2C_ADDRESS, addr);
            ::write(I2C1_BASE + I2C_RESUME, 0x1);
            ::write(I2C1_BASE + I2C_TXD, val);
            ::write(I2C1_BASE + I2C_STARTTX, 0x1);
            int timeout = 10000;
            while (!::read(I2C1_BASE + I2C_EVENTS_TXSENT) && timeout-- != 0);
            ::write(I2C1_BASE + I2C_EVENTS_TXSENT, 0x0);
            ::write(I2C1_BASE + I2C_EVENTS_STOPPED, 0x0);
            ::write(I2C1_BASE + I2C_STOP, 0x1);
            while (!::read(I2C1_BASE + I2C_EVENTS_STOPPED));
        }
        void writeBuffer(unsigned char addr, unsigned char* data, int size, int frequency = 100) {
            ::write(I2C1_BASE + I2C_FREQUENCY, frequency == 250 ? 0x04000000 : frequency == 400 ? 0x06680000 : 0x01980000);
            if (::read(I2C1_BASE + I2C_ENABLE) != 0x5) init();
            ::write(I2C1_BASE + I2C_ADDRESS, addr);
            ::write(I2C1_BASE + I2C_RESUME, 0x1);
            ::write(I2C1_BASE + I2C_TXD, *data++);
            ::write(I2C1_BASE + I2C_STARTTX, 0x1);
            int timeout = 10000;
            while (size-- != 0) {
                timeout = 10000;
                while (!::read(I2C1_BASE + I2C_EVENTS_TXSENT) && timeout-- != 0);
                ::write(I2C1_BASE + I2C_EVENTS_TXSENT, 0x0);
                ::write(I2C1_BASE + I2C_TXD, *data++);
            }
            ::write(I2C1_BASE + I2C_EVENTS_STOPPED, 0x0);
            ::write(I2C1_BASE + I2C_STOP, 0x1);
            while (!::read(I2C1_BASE + I2C_EVENTS_STOPPED));
        }
    }
    namespace i2c = i2c0;
    namespace display {
        void setPixel(int x, int y, int val) {
            if (0 <= y && y < 5 && 0 <= x && x < 5) {
                const int rows[] = {ROW_1, ROW_2, ROW_3, ROW_4, ROW_5};
                const int columns[] = {COL_1, COL_2, COL_3, COL_4, COL_5};
                microbit::pins::setDigitalValue((int)rows[y], val ? 1 : 0);
                microbit::pins::setDigitalValue((int)columns[x], val ? 0 : 1);
            }
        }
        void clear() {
            for (int x = 0; x < 5; x++) {
                for (int y = 0; y < 5; y++) {
                    setPixel(x, y, 0);
                }
            }
        }
        void drawImage(string img) {/*
            static const char* clock[24] = {
                "00100:00100:00100:00000:00000",
                "00010:00100:00100:00000:00000",
                "00010:00010:00100:00000:00000",
                "00001:00010:00100:00000:00000",
                "00000:00011:00100:00000:00000",
                "00000:00001:00110:00000:00000",
                "00000:00000:00111:00000:00000",
                "00000:00000:00110:00001:00000",
                "00000:00000:00100:00011:00000",
                "00000:00000:00100:00010:00001",
                "00000:00000:00100:00010:00010",
                "00000:00000:00100:00100:00010",
                "00000:00000:00100:00100:00100",
                "00000:00000:00100:00100:01000",
                "00000:00000:00100:01000:01000",
                "00000:00000:00100:01000:10000",
                "00000:00000:00100:11000:00000",
                "00000:00000:01100:10000:00000",
                "00000:00000:11100:00000:00000",
                "00000:10000:01100:00000:00000",
                "00000:11000:00100:00000:00000",
                "10000:01000:00100:00000:00000",
                "01000:01000:00100:00000:00000",
                "01000:00100:00100:00000:00000",
            };*/
            if (img == "happy")
                img = "00000:01010:00000:10001:01110";
            if (img == "sad")
                img = "00000:01010:00000:01110:10001";
            if (img == "heart")
                img = "01010:11111:11111:01110:00100";
            if (img == "small heart")
                img = "00000:01010:01110:00100:00000";
            if (img == "yes")
                img = "00000:00001:00010:10100:01000";
            if (img == "no")
                img = "10001:01010:00100:01010:10001";
            if (img == "confused")
                img = "00000:01010:00000:01010:10101";
            if (img == "asleep")
                img = "00000:11011:00000:01110:00000";
            if (img == "square")
                img = "11111:10001:10001:10001:11111";
            if (img == "small square")
                img = "00000:01110:01010:01110:00000";
            for (int y = 0; y < 5; y++) {
                for (int x = 0; x < 5; x++) {
                    setPixel(x, y, img[(y * 6) + x] == '1');
                    delay(100);
                    setPixel(x, y, 0);
                }
            }
        }
    }
    namespace sound {
        void tone(int frequency, int duration, int volume = 100, int pin = SPEAKER) {
            if (frequency <= 0 or volume <= 0) {
                long int period = 1000000L / 1;
                long int highTime = period / (10000 / 100);
                long int lowTime = period - highTime;
                long int len = duration / highTime;
                for (int i = 0; i < len; i++) {
                    ::delay(highTime);
                    ::delay(lowTime);
                }
                return;
            }
            long int period = 1000000L / frequency;
            long int highTime = period / (10000 / volume);
            long int lowTime = period - highTime;
            long int len = duration / (period / 100);
            for (int i = 0; i < len; i++) {
                microbit::pins::setDigitalValue(pin, 1);
                ::delay(highTime);
                microbit::pins::setDigitalValue(pin, 0);
                ::delay(lowTime);
            }
        }
    }
    void init() {
        // LOG ----- PROGRAM STARTED -----
        ::write(SERIAL_BASE + SERIAL_ENABLE, 0x4);
        ::write(SERIAL_BASE + SERIAL_STARTTX, 0x1);
        ::write(SERIAL_BASE + SERIAL_BAUDRATE, 0x01D7E000);
        ::write(SERIAL_BASE + SERIAL_PSEL_TX, 0x00000006);
        const char* str = "\033[1;33m----- PROGRAM STARTED -----\033[0m\n\r";
        for (int i = 0; str[i] != '\0'; i++) {
            ::write(SERIAL_BASE + SERIAL_TX, str[i]);
            ::delay(500);
        }
        ::write(SERIAL_BASE + SERIAL_STOPTX, 0x1);
        ::write(SERIAL_BASE + SERIAL_ENABLE, 0x0);
        // TIMER
        ::write(TIMER_BASE + TIMER_BITMODE, 0x3);
        ::write(TIMER_BASE + TIMER_PRESCALER, 0x0);
    }
    int random() {
        ::write(RNG_BASE + RNG_START, 0x1);
        while (!::read(RNG_BASE + RNG_EVENTS_VALRDY));
        ::write(RNG_BASE + RNG_EVENTS_VALRDY, 0x0);
        int r = ::read(RNG_BASE + RNG_VAL);
        ::write(RNG_BASE + RNG_STOP, 0x1);
        return r;
    }
}
#endif