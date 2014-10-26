#ifndef Costume_h
#define Costume_h

#define COORDINATOR_AT_DH "13A200"
#define COORDINATOR_AT_DL "40B451CF"

#define COSTUME1_AT_DH "13A200"
#define COSTUME1_AT_DL "40AFBED4"

#define COSTUME2_AT_DH "13A200"
#define COSTUME2_AT_DL "40B45171"

#define RUBANLED_LEFT_PIN_BLUE 7
#define RUBANLED_LEFT_PIN_RED 6
#define RUBANLED_LEFT_PIN_GREEN 5

#define RUBANLED_RIGHT_PIN_BLUE 10
#define RUBANLED_RIGHT_PIN_RED 9
#define RUBANLED_RIGHT_PIN_GREEN 8

#define COLOR_BLUE 1
#define COLOR_RED 2
#define COLOR_GREEN 3

class Costume
{
  public:
    Costume(int pin);
    void turnONShoulder(int color);
        void turnOFFShoulder(int color);

    
    void lightButton();
    void sendRFID(String rfid);
  private:
    int _pin;
};

#endif
