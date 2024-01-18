/*****************************************************************************
  *                                                                           
  *  Copyright (c) 2024 www.der-frickler.net                   
  *                                                                           
  *                                                                           
  *****************************************************************************
  *                                                                           
  *  
  *                                                                           
  *                                                                           
  *****************************************************************************/
#include <Arduino.h>
#include <GrblParserC.h>

#define DEBUG 

#define PIN_G 4
#define PIN_Y 3
#define PIN_R 2

#define GRBL_FROM 6
#define GRBL_TO 7


String lstate = "";

extern "C" void fnc_putchar(uint8_t c) {
    Serial1.write(c);
}

extern "C" int fnc_getchar() {
    if (Serial1.available()) {
        return Serial1.read();
    }
    return -1;
}

extern "C" void show_state(const char* state) {
    String sstate = state;
    // state will be one of:
    //  Idle Run Hold:(0/1) Jog Home Alarm Check Door(1:0:2) Sleep 

    // available Effects:
    //  https://github.com/kitesurfer1404/WS2812FX/tree/master#effects 
    if (sstate != lstate) {
       /*
        if(sstate == "Idle") {
            digitalWrite(PIN_G, )
        } else if(sstate == "Run") {
           
        } else if(sstate.startsWith("Hold")) {
            
        } else if(sstate == "Jog") {
            
        } else if(sstate == "Home") {
           
        } else if(sstate == "Alarm") {
            
        } else if(sstate == "Check") {
            
        } else if(sstate.startsWith("Door")) {
            
        } else if(sstate == "Sleep") {
            
        }
        */ 
        lstate = state;

        digitalWrite(PIN_R, sstate == "Alarm" || sstate == "Sleep" || sstate.startsWith("Door"));
        digitalWrite(PIN_G, sstate == "Idle" || sstate == "Run");
        digitalWrite(PIN_Y, sstate == "Jog" || sstate == "Home" || sstate.startsWith("Check"));
    }

#ifdef DEBUG 
    Serial.print(state);
#endif
}

/*
extern "C" void show_dro(const pos_t* axes, const pos_t* wcos, bool isMpos, bool* limits, size_t n_axis) {
    char delim = ' ';
    for (size_t i = 0; i < n_axis; i++) {
        Serial.print(delim); delim = ','; String a(axes[i]); Serial.print(a.c_str());
    }
}
extern "C" void show_file(const char* filename, file_percent_t percent) {
    Serial.print(" File: "); Serial.print(filename);Serial.print(" Progress: ");Serial.print(percent);
}
extern "C" void show_spindle_coolant(int spindle, bool flood, bool mist) {
    Serial.print(" Spindle: "); Serial.print(spindle);Serial.print(" Coolant: F:");Serial.print(flood);Serial.print(" M: ");Serial.print(mist);
}
extern "C" void show_feed_spindle(uint32_t feedrate, uint32_t spindle_speed) {
    Serial.print(" Feed: "); Serial.print(feedrate);Serial.print(" Spindle RPM:");Serial.print(spindle_speed);
}
extern "C" void show_overrides(override_percent_t feed_ovr, override_percent_t rapid_ovr, override_percent_t spindle_ovr) {
}
*/

extern "C" void end_status_report() {
#ifdef DEBUG 
    Serial.println("");
#endif
}

extern "C" int milliseconds() {
    return millis();
}

void setup() {

    Serial.begin(115200);
    Serial1.begin(115200, SERIAL_8N1, GRBL_FROM, GRBL_TO);

    Serial.println("FluidNC stacklight status");

    pinMode(PIN_R, OUTPUT);
    pinMode(PIN_G, OUTPUT);
    pinMode(PIN_Y, OUTPUT);

    fnc_wait_ready();
    fnc_putchar('?');           // Initial status report
    fnc_send_line("$G", 1000);  // Initial modes report
}

void loop() {
    fnc_poll();

}
