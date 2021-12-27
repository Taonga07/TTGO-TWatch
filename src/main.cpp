#define LILYGO_WATCH_2020_V3
#include <LilyGoWatch.h>
#include <Arduino.h>


TTGOClass *watch;
AXP20X_Class *power;
bool irq = false;
bool bttnstatus = true;


void watch_sleep(bool abool){
	power->setPowerOutPut(AXP202_LDO3, abool);
	power->setPowerOutPut(AXP202_LDO4, abool);
	power->setPowerOutPut(AXP202_LDO2, abool);
	power->setPowerOutPut(AXP202_EXTEN, abool);
	power->setPowerOutPut(AXP202_DCDC2, abool);
}
void setup() {
    watch = TTGOClass::getWatch(); // Get TTGOClass instance
    watch->begin(); // Initialize the hardware, the BMA423 sensor has been initialized internally
    watch->openBL(); // Turn on the backlight
    //Receive objects for easy writing
    power = watch->power;
    pinMode(AXP202_INT, INPUT_PULLUP);
    attachInterrupt(AXP202_INT, [] {
        irq = true;
    }, FALLING);

    // Must be enabled first, and then clear the interrupt status,
    // otherwise abnormal
    power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ, true);
    power->clearIRQ(); //  Clear interrupt status
}
void loop() {
    if (irq){
        power->clearIRQ(); // reset interupt status
		watch_sleep(bttnstatus);
        bttnstatus = !bttnstatus;
    }
    else{
        if (bttnstatus){
            watch->tft->fillScreen(TFT_BLUE);
        }
    }
}