#include <Mouse.h>
#include <Keyboard.h>

unsigned long timer1 = 0, timer2 = 0, timer3 = 0, timer4 = 0;
constexpr double TICK_64MS = 15.6;

bool is_timer_elapsed(unsigned long& timer, unsigned long interval)
{
    unsigned long current_time = millis();

    if (current_time - timer >= interval)
    {
        timer = current_time;
        return true;
    }

    return false;
}

void handle_mouse_middle_holded()
{
    if (is_timer_elapsed(timer1, 300))
    {
        Mouse.click(MOUSE_LEFT);
    }
}

void handle_space_button_holded()
{
    if (is_timer_elapsed(timer2, TICK_64MS * 39))
    {
        Mouse.move(0, 0, -1);
    }
    if (is_timer_elapsed(timer3, TICK_64MS * 8))
    {
        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.release(KEY_LEFT_CTRL);
    }
    if (is_timer_elapsed(timer4, TICK_64MS * 2))
    {
        Mouse.move(0, 0, -1);
    }
}

void handle_mouse_left_holded(const String& message)
{
    int index1 = message.indexOf(',');
    int index2 = message.indexOf(',', index1 + 1);

    if (index1 != -1 && index2 != -1)
    {
        double x = message.substring(0, index1).toDouble();
        double y = message.substring(index1 + 1, index2).toDouble();

        Mouse.move(x, y);
    }
}

void handle_keyboard_press_q()
{
    delay(10);
    Keyboard.write('q');
    delay(25);
    Keyboard.write('q');
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

        if (message == "MOUSE_MIDDLE_HOLDED")
        {
            handle_mouse_middle_holded();
        }
        else if (message == "SPACE_BUTTON_HOLDED")
        {
            handle_space_button_holded();
        }
        else if (message.startsWith("MOUSE_LEFT_HOLDED:"))
        {
            handle_mouse_left_holded(message.substring(18));
        }
        else if (message == "MOUSE_LEFT_CLICK")
        {
            Mouse.click(MOUSE_LEFT);
        }
        else if (message == "KEYBOARD_PRESS_Q")
        {
            handle_keyboard_press_q();
        }
    }
}