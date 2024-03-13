// Enable serial gateway 
#define MY_GATEWAY_SERIAL
// Enable debug prints to serial monitor
#include <MySensors.h>
#include <Bounce2.h>

#define OFFICE_WINDOW_SMALL  2
#define OFFICE_WINDOW_BIG  3
#define BEDROOM_WINDOW_SMALL  4
#define BEDROOM_WINDOW_BIG  5
#define LIVING_ROOM_WINDOW_LEFT  6
#define LIVING_ROOM_WINDOW_RIGHT  7
#define BALCONY  8
#define DOORS  9

class ReedSwitch {
  private: 
    short pin;
    Bounce debouncer= Bounce();
    MyMessage msg;
    byte value;

  public: ReedSwitch(short pin){
    this->pin = pin;
    pinMode(pin,INPUT_PULLUP);
    debouncer.attach(pin);
    debouncer.interval(1000);
    msg = MyMessage(pin, V_TRIPPED);
  }

  void presentSensor() {
    present(this->pin, S_DOOR);
  }

  void update() {
    debouncer.update();
    byte valueNew = debouncer.read();
    if (valueNew != value) {
     send(msg.set(valueNew==HIGH ? 1 : 0));
     value = valueNew;
    }
  }
};

ReedSwitch reedSwitches [] = {
  (OFFICE_WINDOW_SMALL),
  (OFFICE_WINDOW_BIG),
  (BEDROOM_WINDOW_SMALL),
  (BEDROOM_WINDOW_BIG),
  (LIVING_ROOM_WINDOW_LEFT),
  (LIVING_ROOM_WINDOW_RIGHT),
  (BALCONY),
  (DOORS),
};

void setup()  
{  
}

void presentation() {
  for (ReedSwitch reedSwitch : reedSwitches) {
    reedSwitch.presentSensor();
  }
  sendSketchInfo("Win/Doors reed switches", "1.0");
}

void loop() {
   for(int i=0; i<sizeof(reedSwitches)/sizeof(reedSwitches[0]); i++){
    reedSwitches[i].update();
  }
}
