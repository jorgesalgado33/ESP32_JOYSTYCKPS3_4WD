/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include <Arduino.h>
#include <Ps3Controller.h>
#include "ControlMotors/MotorManager.h"

#define PIN_PWM2_1  13
#define PIN_DIR2_1  12
#define PIN_PWM1_1  14
#define PIN_DIR1_1  27

#define PIN_PWM2_2  25
#define PIN_DIR2_2  33
#define PIN_PWM1_2  32
#define PIN_DIR1_2  16

MotorManager *myMotors;

int8_t i8DutyCycle = 30;
int8_t i8_STOP = 0;
int8_t i8_facteur =3;

int player = 0;
int battery = 0;

void MotorsSTOP(void);
void Forward_LEFT(void);
void Forward_RIGHT(void);
void Backward_DOWN(void);
void Forward_UP(void);
void Turn_RIGHT(void);
void Turn_LEFT(void);
void MotorsDirection1(void);
void MotorsDirection2(void);
void MotorANALOG(int8_t i8DutyCycle1, int8_t i8DutyCyvle2);

void notify()
{
    //--- Digital cross/square/triangle/circle button events ---
    if( Ps3.event.button_down.cross ){
        Serial.println("Started pressing the cross button");
        
        MotorsSTOP();
    }
    if( Ps3.event.button_up.cross ){
        Serial.println("Released the cross button");

        MotorsSTOP(); 

    }
    if( Ps3.event.button_down.square ){
        Serial.println("Started pressing the square button");

        
     
    }
    if( Ps3.event.button_up.square ){
        Serial.println("Released the square button");

        if(i8DutyCycle<100)
            i8DutyCycle+=5;


    }
    if( Ps3.event.button_down.triangle ){
        Serial.println("Started pressing the triangle button");
        
        

    }
    if( Ps3.event.button_up.triangle ){
        Serial.println("Released the triangle button");

        if(i8DutyCycle>-100)
            i8DutyCycle-=5;


    }

    if( Ps3.event.button_down.circle ){
        Serial.println("Started pressing the circle button");
        
        if(i8_facteur>0)
            i8_facteur-=1;
        
        }

    if( Ps3.event.button_up.circle ){
        Serial.println("Released the circle button");
        
        MotorsSTOP(); 

    }

    //--------------- Digital D-pad button events --------------
    if( Ps3.event.button_down.up ){
        Serial.println("Started pressing the up button");
        Forward_UP();
        }
    if( Ps3.event.button_up.up ){
        Serial.println("Released the up button");
        
        MotorsSTOP();
        }

    if( Ps3.event.button_down.right ){
        Serial.println("Started pressing the right button");
        Forward_RIGHT();
    }
    if( Ps3.event.button_up.right ){
        Serial.println("Released the right button");
        MotorsSTOP();
    }

    if( Ps3.event.button_down.down ){
        Serial.println("Started pressing the down button");
        Backward_DOWN();
        }
    if( Ps3.event.button_up.down ){
        Serial.println("Released the down button");
        MotorsSTOP();
    }

    if( Ps3.event.button_down.left ){

        Serial.println("Started pressing the left button");
        Forward_LEFT();
    }

    if( Ps3.event.button_up.left ){
        Serial.println("Released the left button");
        MotorsSTOP();
    }
    //------------- Digital shoulder button events -------------
    if( Ps3.event.button_down.l1 ){
        Serial.println("Started pressing the left shoulder button");
        Turn_LEFT();
    }

    if( Ps3.event.button_up.l1 ){
        Serial.println("Released the left shoulder button");
        MotorsSTOP();
    }

    if( Ps3.event.button_down.r1 ){
        Serial.println("Started pressing the right shoulder button");
        Turn_RIGHT();
    }
    if( Ps3.event.button_up.r1 ){
        Serial.println("Released the right shoulder button");
        MotorsSTOP();
    }

    //-------------- Digital trigger button events -------------
    if( Ps3.event.button_down.l2 ){
        Serial.println("Started pressing the left trigger button");
        if(i8_facteur<10)
            i8_facteur+=1;
    }
    if( Ps3.event.button_up.l2 ){   
        Serial.println("Released the left trigger button");
       
        MotorsSTOP();
    }

    if( Ps3.event.button_down.r2 ){
        Serial.println("Started pressing the right trigger button");
    }
    if( Ps3.event.button_up.r2 ){
        Serial.println("Released the right trigger button");
    }
    //--------------- Digital stick button events --------------
    if( Ps3.event.button_down.l3 )
        Serial.println("Started pressing the left stick button");
    if( Ps3.event.button_up.l3 )
        Serial.println("Released the left stick button");

    if( Ps3.event.button_down.r3 )
        Serial.println("Started pressing the right stick button");
    if( Ps3.event.button_up.r3 )
        Serial.println("Released the right stick button");

    //---------- Digital select/start/ps button events ---------
    if( Ps3.event.button_down.select )
        Serial.println("Started pressing the select button");
    if( Ps3.event.button_up.select )
        Serial.println("Released the select button");

    if( Ps3.event.button_down.start )
        Serial.println("Started pressing the start button");
    if( Ps3.event.button_up.start )
        Serial.println("Released the start button");

    if( Ps3.event.button_down.ps )
        Serial.println("Started pressing the Playstation button");
    if( Ps3.event.button_up.ps )
        Serial.println("Released the Playstation button");


    //---------------- Analog stick value events ---------------
   if( abs(Ps3.event.analog_changed.stick.lx) + abs(Ps3.event.analog_changed.stick.ly) > 2 ){

       /*[JSA]*/
       /*Conversion*/
        int8_t i8DataCycle1 = (-Ps3.data.analog.stick.lx)*0.78;
       
        int8_t i8DataCycle2 = (Ps3.data.analog.stick.ly)*0.78;


        MotorANALOG((i8DataCycle1*i8_facteur/10), (i8DataCycle2*i8_facteur/10));

       
       Serial.print("Moved the left stick:");
       Serial.print(" x="); Serial.print(Ps3.data.analog.stick.lx, DEC);
       Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ly, DEC);
       Serial.println();
   
    }

   if( abs(Ps3.event.analog_changed.stick.rx) + abs(Ps3.event.analog_changed.stick.ry) > 2 ){

       Serial.print("Moved the right stick:");
       Serial.print(" x="); Serial.print(Ps3.data.analog.stick.rx, DEC);
       Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ry, DEC);
       Serial.println();
       
   }

   //--------------- Analog D-pad button events ----------------
   if( abs(Ps3.event.analog_changed.button.up) ){
       Serial.print("Pressing the up button: ");
       Serial.println(Ps3.data.analog.button.up, DEC);
   }

   if( abs(Ps3.event.analog_changed.button.right) ){
       Serial.print("Pressing the right button: ");
       Serial.println(Ps3.data.analog.button.right, DEC);
   }

   if( abs(Ps3.event.analog_changed.button.down) ){
       Serial.print("Pressing the down button: ");
       Serial.println(Ps3.data.analog.button.down, DEC);
   }

   if( abs(Ps3.event.analog_changed.button.left) ){
       Serial.print("Pressing the left button: ");
       Serial.println(Ps3.data.analog.button.left, DEC);
   }

   //---------- Analog shoulder/trigger button events ----------
   if( abs(Ps3.event.analog_changed.button.l1)){
       Serial.print("Pressing the left shoulder button: ");
       Serial.println(Ps3.data.analog.button.l1, DEC);
   }

   if( abs(Ps3.event.analog_changed.button.r1) ){
       Serial.print("Pressing the right shoulder button: ");
       Serial.println(Ps3.data.analog.button.r1, DEC);
   }

   if( abs(Ps3.event.analog_changed.button.l2) ){
       Serial.print("Pressing the left trigger button: ");
       Serial.println(Ps3.data.analog.button.l2, DEC);
   }

   if( abs(Ps3.event.analog_changed.button.r2) ){
       Serial.print("Pressing the right trigger button: ");
       Serial.println(Ps3.data.analog.button.r2, DEC);
   }

   //---- Analog cross/square/triangle/circle button events ----
   if( abs(Ps3.event.analog_changed.button.triangle)){
       Serial.print("Pressing the triangle button: ");
       Serial.println(Ps3.data.analog.button.triangle, DEC);
   }

   if( abs(Ps3.event.analog_changed.button.circle) ){
       Serial.print("Pressing the circle button: ");
       Serial.println(Ps3.data.analog.button.circle, DEC);
   }

   if( abs(Ps3.event.analog_changed.button.cross) ){
       Serial.print("Pressing the cross button: ");
       Serial.println(Ps3.data.analog.button.cross, DEC);
   }

   if( abs(Ps3.event.analog_changed.button.square) ){
       Serial.print("Pressing the square button: ");
       Serial.println(Ps3.data.analog.button.square, DEC);
   }

   //---------------------- Battery events ---------------------
    if( battery != Ps3.data.status.battery ){
        battery = Ps3.data.status.battery;
        Serial.print("The controller battery is ");
        if( battery == ps3_status_battery_charging )      Serial.println("charging");
        else if( battery == ps3_status_battery_full )     Serial.println("FULL");
        else if( battery == ps3_status_battery_high )     Serial.println("HIGH");
        else if( battery == ps3_status_battery_low)       Serial.println("LOW");
        else if( battery == ps3_status_battery_dying )    Serial.println("DYING");
        else if( battery == ps3_status_battery_shutdown ) Serial.println("SHUTDOWN");
        else Serial.println("UNDEFINED");
    }

}

void onConnect(){
    Serial.println("Connected.");
}

void setup()
{
    Serial.begin(115200);
    Serial2.begin(9600);

    Ps3.attach(notify);
    Ps3.attachOnConnect(onConnect);
    Ps3.begin("78:18:81:0d:17:dd");

    Serial.println("Ready.");

    myMotors = new MotorManager();

    myMotors->Begin(PIN_DIR1_1,PIN_DIR2_1,PIN_PWM1_1,PIN_PWM2_1,PIN_DIR1_2, PIN_DIR2_2, PIN_PWM1_2, PIN_PWM2_2);

}

void loop()
{
    if(!Ps3.isConnected())
        return;
    


    //-------------------- Player LEDs -------------------
    Serial.print("Setting LEDs to player "); Serial.println(player, DEC);
    Ps3.setPlayer(player);

    player += 1;
    if(player > 10) player = 0;


    //------ Digital cross/square/triangle/circle buttons ------
    if( Ps3.data.button.cross && Ps3.data.button.down )
        Serial.println("Pressing both the down and cross buttons");
    if( Ps3.data.button.square && Ps3.data.button.left )
        Serial.println("Pressing both the square and left buttons");
    if( Ps3.data.button.triangle && Ps3.data.button.up )
        Serial.println("Pressing both the triangle and up buttons");
    if( Ps3.data.button.circle && Ps3.data.button.right )
        Serial.println("Pressing both the circle and right buttons");

    if( Ps3.data.button.l1 && Ps3.data.button.r1 )
        Serial.println("Pressing both the left and right bumper buttons");
    if( Ps3.data.button.l2 && Ps3.data.button.r2 )
        Serial.println("Pressing both the left and right trigger buttons");
    if( Ps3.data.button.l3 && Ps3.data.button.r3 )
        Serial.println("Pressing both the left and right stick buttons");
    if( Ps3.data.button.select && Ps3.data.button.start )
        Serial.println("Pressing both the select and start buttons");

    delay(2000);
}


void Forward_LEFT(void){
   
    //myMotors->PWM1(i8_STOP);
    myMotors->PWM2(i8DutyCycle);     /*forward LEFT*/
    myMotors->PWM3(i8DutyCycle);    /*forward LEFT*/
    //myMotors->PWM4(i8_STOP);
}

void Forward_RIGHT(void){

    //myMotors->PWM1(i8_STOP);
    myMotors->PWM2(-i8DutyCycle);    /* forward RIGHT */
    myMotors->PWM3(-i8DutyCycle);     /* forward RIGHT */
    //myMotors->PWM4(i8_STOP);
}

void Backward_DOWN(void){
   
    myMotors->PWM1(i8DutyCycle);      /* backward DOWN*/
    //myMotors->PWM2(i8_STOP);
    //myMotors->PWM3(i8_STOP);
    myMotors->PWM4(i8DutyCycle);     /* backward DOWN*/
}

void Forward_UP(void){
   
    myMotors->PWM1(-i8DutyCycle);    /* forward UP */
    //myMotors->PWM2(i8_STOP);
    //myMotors->PWM3(i8_STOP);
    myMotors->PWM4(-i8DutyCycle);      /* forward UP */
}

void Turn_RIGHT(void){
   
    /* TURN RIGHT */
    myMotors->PWM1(-i8DutyCycle);       /* forward UP */
    myMotors->PWM2(i8DutyCycle);        /*forward LEFT*/
    myMotors->PWM3(-i8DutyCycle);       /* forward RIGHT */
    myMotors->PWM4(i8DutyCycle);        /* backward DOWN*/
}

void Turn_LEFT(void){
   
    /* TURN LEFT */
    myMotors->PWM1(i8DutyCycle);        /* backward DOWN*/
    myMotors->PWM2(-i8DutyCycle);       /* forward RIGHT */
    myMotors->PWM3(i8DutyCycle);        /*forward LEFT*/
    myMotors->PWM4(-i8DutyCycle);       /* forward UP */
}

void MotorsDirection1(void){
   
    /* TURN LEFT */
    myMotors->PWM1(-i8DutyCycle);        /* forward UP */   
    myMotors->PWM2(-i8DutyCycle);       /* forward RIGHT */
    myMotors->PWM3(-i8DutyCycle);        /* forward RIGHT */
    myMotors->PWM4(-i8DutyCycle);       /* forward UP */
}

void MotorsDirection2(void){
   
    /* TURN LEFT */
    myMotors->PWM1(i8DutyCycle);        /* backward DOWN*/
    myMotors->PWM2(i8DutyCycle);       /*forward LEFT*/
    myMotors->PWM3(i8DutyCycle);        /*forward LEFT*/
    myMotors->PWM4(i8DutyCycle);       /* backward DOWN*/
}
void MotorsSTOP(void){
   
    myMotors->PWM1(i8_STOP);
    myMotors->PWM2(i8_STOP);
    myMotors->PWM3(i8_STOP);
    myMotors->PWM4(i8_STOP);

}

void MotorANALOG(int8_t i8DutyCycle1, int8_t i8DutyCyvle2){
        myMotors->PWM1(i8DutyCyvle2);
        myMotors->PWM2(i8DutyCycle1);    /* forward RIGHT */
        myMotors->PWM3(i8DutyCycle1);     /* forward RIGHT */
        myMotors->PWM4(i8DutyCyvle2);
}