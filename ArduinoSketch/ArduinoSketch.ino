#include <Mouse.h>
#include <Keyboard.h>

unsigned long timer1 = 0;
unsigned long timer2 = 0;
unsigned long timer3 = 0;
unsigned long timer4 = 0;
unsigned long tick64ms = 15.6;

void setup()
{
    Mouse.begin();
    Keyboard.begin();
    Serial.begin(9600);
}

void loop()
{
    if (Serial.available() > 0)
    {
        String message = Serial.readStringUntil('\0');

        if (message == "MOUSE_MIDDLE_HOLDED")
        {
            if (millis() - timer1 >= 100)
            {
                timer1 = millis();
                Serial.println("ARDUINO_INITIATED\n");
                Mouse.click(MOUSE_LEFT);
            }
        }
        else if (message == "SPACE_BUTTON_HOLDED")
        {
            if (millis() - timer2 >= tick64ms * 39)
            {
                timer2 = millis();
                Mouse.move(0, 0, -1);
            }

            if (millis() - timer3 >= tick64ms * 8)
            {
                timer3 = millis();
                Keyboard.press(KEY_LEFT_CTRL);
                Keyboard.release(KEY_LEFT_CTRL);
            }

            if (millis() - timer4 >= tick64ms * 2)
            {
                timer4 = millis();
                Mouse.move(0, 0, -1);
            }
        }
        else if (message.startsWith("MOUSE_LEFT_HOLDED:"))
        {
          message = message.substring(18);
          int index1 = message.indexOf(',');
          int index2 = message.indexOf(',', index1 + 1);
          
          double x = message.substring(0, index1).toDouble();
          double y = message.substring(index1 + 1, index2).toDouble();
          
          Mouse.move(x, y);
        }
    }
}
