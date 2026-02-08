#include "input/keyboard/keyboard.h"
#include "interprer/interprer.h"
#include "screen/screen.h"
#include "apps/time/time.h"

int str = 24;
bool key_condition[128];
bool caps = false;

void load()
{
    init_rtc();
    clear();
    print("OS Started. Write help for commands", 0, 0x0a);
}

void enter()
{
    unsigned char scan = read_keyboard();
    
    bool shift = key_condition[0x2a] || key_condition[0x36];
    bool reg = false;
    if (scan & 0x80)
        key_condition[scan & 0x7f] = false;
    else
    {
        key_condition[scan] = true;

        if (scan == 0x3a) caps = !caps;
        reg = shift || caps;
    }
    
    char c = scan2char(scan, reg);

    if (scan == 0x1c)
    {
        char *cmd = get_str(str);
        clear_str(str);
        exec(cmd);
    }
    else if (scan == 0x0e)
    {
        hide_cursor();
        backspace(str);
        show_cursor();
    }
    else if (c != 0x00) 
    {
        hide_cursor();
        print_char(c, str, 0x0a);
        show_cursor();
    }
}

void kmain()
{
    load();
    while (1) 
    {
        replace("Primitive OS>", str - 1, 0x0a);
        
        enter();
    }
}