#include "Thread.h"
#include "mbed.h"
#include "rtos.h"
#include "SDFileSystem.h"
#include "wave_player.h"
#include "uLCD_4DGL.h"
#include "ultrasonic.h"
#include "Motor.h"

DigitalOut led1(LED1);
DigitalOut led2(LED4);
// I/O device connections for wave player
SDFileSystem sd(p5, p6, p7, p8, "sd"); //SD card
AnalogOut DACout(p18);
wave_player waver(&DACout);

// I/O device connections for bluetooth
Serial blue(p28,p27);

// I/O device connections for sonar and Timer instantiation
DigitalOut trigger(p12);
DigitalIn echo(p13);
Timer sonar;

// I/O device connections for LCD and Mutex instantiation
uLCD_4DGL uLCD(p9,p10,p11);
Mutex lcd_mutex;

// // I/O device connections for motors
Motor left_wheel(p26,p25,p24);
Motor right_wheel(p21,p22,p23);

volatile bool autonomous_mode = 0;
volatile int correction = 0;
volatile int distance = 0;
volatile int blue_num = 0;

void dist(int measured_distance) {
    distance = measured_distance;
}
ultrasonic mu(p12, p13, .1, 1, &dist);

void update_sonar_reading(const void * arg) {
    while(1) {
        if (autonomous_mode) {
            mu.checkDistance();
        }
        Thread::wait(30);
    }
}

void display_mode(const void * arg) {
    while(1) {
        lcd_mutex.lock();
        uLCD.locate(1, 6);
            uLCD.printf("                   ");
        if (autonomous_mode) {
            uLCD.color(RED);
            uLCD.printf("Autonomous Mode");
        } else {
            uLCD.color(GREEN + BLUE);
            uLCD.printf("Bluetooth Mode");
        }
        lcd_mutex.unlock();
        Thread::wait(1000);
    }
}

void read_bnum(const void * arg) {
    char bnum = 0;
    char bhit = 0;
    while (1) {
        while (!blue.readable()) Thread::yield();
        lcd_mutex.lock();
        if (blue.getc()=='!') {
            if (blue.getc()=='B') { //button data packet
                bnum = blue.getc(); //button number
                bhit = blue.getc(); //1=hit, 0=release
                if (bhit == '1') {
                    switch (bnum) {
                        case '1': //number button 1
                            blue_num = 1;
                            break;
                        case '2':
                            blue_num = 2;
                            break;
                        case '5': //button 5 up arrow
                            blue_num = 5;
                            break;
                        case '6': //button 6 down arrow
                            blue_num = 6;
                            break;
                        case '7': //button 7 left arrow
                            blue_num = 7;
                            break;
                        case '8': //button 8 right arrow
                            blue_num = 8;
                            break;
                        default:
                            blue_num = 0;
                            break;
                    }
                }
            }
        }
        lcd_mutex.unlock();
        Thread::wait(50);
    }
}

void move_robot(const void * arg) {
    while (1) {
        if (autonomous_mode) {
            led1 = 1;
            led2 = 0;
            if (blue_num == 1) {
                autonomous_mode = !autonomous_mode;
                blue_num = 0;
            } else if (distance <= 250) {
                right_wheel.speed(0.0);
                left_wheel.speed(0.5);
                Thread::wait(100);
                right_wheel.speed(0.0);
                left_wheel.speed(0.5);
                Thread::wait(100);
            } else {
                left_wheel.speed(0.5);
                right_wheel.speed(0.5);
            }
        } else {
            led1 = 0;
            led2 = 1;
            if (blue_num == 1) {
                autonomous_mode = !autonomous_mode;
                blue_num = 0;
            } else if (blue_num == 2) {
                left_wheel.speed(0);
                right_wheel.speed(0);
            } else if (blue_num == 5) {
                left_wheel.speed(0.5);
                right_wheel.speed(0.5);
            } else if (blue_num == 6) {
                left_wheel.speed(-0.5);
                right_wheel.speed(-0.5);
            } else if (blue_num == 7) {
                right_wheel.speed(0.3);
                left_wheel.speed(1.0);
            } else if (blue_num == 8) {
                right_wheel.speed(1.0);
                left_wheel.speed(0.3);
            }
        }
        Thread::wait(50);
    }
}

int main()
{
    // Code to setup LCD
    uLCD.cls();
    uLCD.printf("Robot Display Mode");
    Thread t1(update_sonar_reading);
    Thread t2(display_mode);
    Thread t3(move_robot);
    Thread t4(read_bnum);

    mu.startUpdates();

    while(1){
        FILE *wave_file;
        Thread::wait(1000);
        if (autonomous_mode) {
            wave_file=fopen("/sd/dir/Autonomous.wav","r");
        } else { 
            wave_file=fopen("/sd/dir/Bluetooth.wav","r");
        }
        waver.play(wave_file);
        fclose(wave_file);
    }
}

// References
// https://os.mbed.com/users/4180_1/notebook/using-the-hc-sr04-sonar-sensor/
// https://os.mbed.com/cookbook/Obstacle-Detection-Robot
// https://hamblen.ece.gatech.edu/489X/classmat/Labs/4180lab3RTOS_F19.pdf
// https://os.mbed.com/users/4180_1/code/WavePlayer_HelloWorld_RTOS/
// https://os.mbed.com/users/4180_1/notebook/ulcd-144-g2-128-by-128-color-lcd/
