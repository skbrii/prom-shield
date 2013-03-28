#include <SD.h>
#include "RTClib.h"

class Prom_proto{
    private:
        const int relay[2] = {7, 8};
        const int relay_num = 2;

        const int digital_out[4] = {5, 6, A6, A7};
        const int digital_out_num = 4;

        const int digital_in[4] = {2, 3, 4, 9};
        const int digital_in_num = 4;

        const int analog_in[4] = {A0, A1, A2, A3};
        const int analog_in_num = 4;

        const int sd_cs  = 10; 
        const int sd_di  = 11;
        const int sd_do  = 12; 
        const int sd_clk = 13; 

        const int i2c_sda = A4;
        const int i2c_scl = A5;

        struct {
            int sd_present : 1;
            int sd_logging_enable : 1;
            int serial_logging_enable : 1;
        } status = { 
            .sd_present = 0, 
            .sd_logging_enable = 0,
            .serial_logging_enable = 0            
        };

    public:
        const HardwareSerial &rs232 = Serial;
        const HardwareSerial &log_serial = Serial;
        RTC_DS1307 rtc;

        void Prom_proto();
        void logging(String message);

        void enableSDlogging() {status.sd_logging_enable = 1};
        void disableSDlogging() {status.sd_logging_enable = 0};
}

void Prom_proto::Prom_proto(){
    for (int i = 0; i < relay_num; i++)
        pinMode(relay[i], OUTPUT);

    for (int i = 0; i < digital_out_num; i++)
        pinMode(digital_out[i], OUTPUT);

    for (int i = 0; i < digital_in_num; i++)
        pinMode(digital_in[i], INPUT);

    rs232.begin(9600);
    log_serial.begin(9600);
    status.serial_logging_enable = 1;

    Wire.begin;
    rtc.begin();

    if (! RTC.isrunning()){
        logging("RTC is NOT running!");
        // following line sets the RTC to the date & time this sketch was compiled
        RTC.adjust(DateTime(__DATE__, __TIME__));
    }

    sdLoggingStart();
}

void Prom_proto::logging(String message)
{
    DateTime now = rtc.now();

    // sending by serial
    if (status.serial_logging_enable){
        log_serial.print('[');
        log_serial.print(now.year, DEC);
        log_serial.print('-');
        log_serial.print(now.month, DEC);
        log_serial.print('-');
        log_serial.print(now.day, DEC);
        log_serial.print(' ');
        log_serial.print(now.hour, DEC);
        log_serial.print(':');
        log_serial.print(now.minute, DEC);
        log_serial.print(':');
        log_serial.print(now.second, DEC);
        log_serial.print("]  ");
        log_serial.print(message);
    }

    // writing to file
    if (status.sd_logging_enable && status.sd_present){
        ;
    }
}
