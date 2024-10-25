#include <Mouse.h>
#include <Keyboard.h>

unsigned long timer1 = 0;
unsigned long timer2 = 0;
unsigned long timer3 = 0;
unsigned long timer4 = 0;
constexpr double tick64ms = 15.6;

bool isTimerElapsed(unsigned long& timer, unsigned long interval)
{
    if (millis() - timer >= interval)
    {
        timer = millis();
        return true;
    }
    
    return false;
}

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

        if (message == "MOUSE_MIDDLE_HOLDED" && isTimerElapsed(timer1, 100))
        {
            Serial.println("ARDUINO_INITIATED\n");
            Mouse.click(MOUSE_LEFT);
        } 
        else if (message == "SPACE_BUTTON_HOLDED")
        {
            if (isTimerElapsed(timer2, tick64ms * 39))
            {
                Mouse.move(0, 0, -1);
            }
            if (isTimerElapsed(timer3, tick64ms * 8))
            {
                Keyboard.press(KEY_LEFT_CTRL);
                Keyboard.release(KEY_LEFT_CTRL);
            }
            if (isTimerElapsed(timer4, tick64ms * 2))
            {
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
        else if (message == "MOUSE_LEFT_CLICK")
        {
            Mouse.click(MOUSE_LEFT);
        }
    }
}