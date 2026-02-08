#include <stdbool.h>
#include "base/printf.h"
#include "input/power/power.h"
#include "file_system/file.h"
#include "screen/screen.h"
#include "apps/time/time.h"
#include "base/base.h"

#define null (void *)0

void list_files()
{
    if (fs.count == 0)
    {
        replace("Files not found", 2, 0x0a);
    }
    else
    {
        for (int i = 0; i < fs.count; i++)
        {
            replace(fs.files[i].name, i + 2, 0x0a);
        }
    }
}
void make(char name[])
{
    if (name[0] != '\0' && name[0] != ' ')
    {
        create_file(name);
        replace("File created", 2, 0x0a);
    }
    else
    {
        replace("Please enter file name", 2, 0x0a);
    }
}
void te(char name[])
{
    char *text = strchr(name, ' ');

    if (text == null)
    {
        for (int i = 0; i < fs.count; i++)
        {
            if (strcmp(name, fs.files[i].name) == 0)
            {
                print(fs.files[i].data, 1, 0x0a);
                return;
            }
        }
        replace("File not found", 1, 0x0a);
        return;
    }

    int is_found = -1;
    for (int i = 0; i < fs.count; i++)
    {
        if (strcmp(fs.files[i].name, name) == 0)
        {
            is_found = i;
            break;
        }
    }

    if (is_found == -1)
    {
        replace("File not found", 2, 0x0a);
        return;
    }
    if (text != 0)
        copy_str(text, fs.files[is_found].data);
    replace("File updated succesful", 2, 0x0a);
}
void time()
{
    char str[100];

    int h, m, s;
    get_time(&h, &m, &s);

    sprintf_(str, "%d:%d:%d", h, m, s);
    print(str, 0, 0x0a);
}
void tz(char *cmd)
{
    if (cmd[5] == ' ' && cmd[6] != '\0')
    {
        set_timezone(atoi(&cmd[6]));

        char str[255];
        sprintf_(str, "New timezone: %d", get_timezone());
        print(str, 0, 0x0a);
    }
    else
    {
        char str[255];
        sprintf_(str, "Current timezone: %d", get_timezone());
        print(str, 0, 0x0a);
    }
    
}
void rng(char *cmd)
{
    char str[255];
    int min = atoi(&cmd[6]);
    int max = atoi(strchr(cmd, ' ') + 1);

    if (max < min)
    {
        print("Maximum should be bigger than minimum", 0, 0x0a);
        return;
    }
    
    int trash;
    srand(trash);

    int result = min + rand() % (max - min + 1);
    sprintf_(str, "Random number from %d to %d: %d", min, max, result);
    print(str, 0, 0x0a);
}
void help()
{
    char commands[][100] = {
        "hello - hello message",
        "clear - clear console",
        "help - list commands",
        "write [text] - print text",
        "make [file] - create file",
        "dlt [file] - delete file",
        "list - show files",
        "rand [mix] [max] - random number generator",
        "time - time for current timezone",
        "tzone [zone] - set timezone",
        "tzone - get current timezone",
        "te - text editor",
        "- te [file] [name] - modify file",
        "- te [file] - show file data",
        "shutdown - shutdown system",
        "reload - reload system",
    };

    int count = sizeof(commands) / sizeof(commands[0]);
    for (int i = 0; i < count; i++)
    {
        replace(commands[i], i + 1, 0x0a);
    }
}
void exec(char cmd[])
{
    clear();

    if (strncmp(cmd, "write", 5) == 0)
    {
        replace(&cmd[5], 3, 0x0a);
    }
    else if (strcmp(cmd, "list") == 0)
    {
        list_files();
    }
    else if (strncmp(cmd, "make ", 5) == 0)
    {
        make(&cmd[5]);
    }
    else if (strncmp(cmd, "dlt ", 4) == 0)
    {
        char *name = &cmd[4];
        delete_file(name);
    }
    else if (strncmp(cmd, "te ", 3) == 0)
    {
        te(&cmd[3]);
    }
    else if (strcmp(cmd, "shutdown") == 0)
    {
        shutdown();
    }
    else if (strcmp(cmd, "reload") == 0)
    {
        reboot();
    }
    else if (strcmp(cmd, "time") == 0)
    {
        time();
    }
    else if (strncmp(cmd, "tzone", 5) == 0)
    {
        tz(cmd);
    }
    else if (strncmp(cmd, "rand ", 5) == 0)
    {
        rng(cmd);
    }
    else if (strcmp(cmd, "hello") == 0)
    {
        replace("Hello. Welcome to PrimitiveOS by Tigrics.", 1, 0x0a);
    }
    else if (strcmp(cmd, "help") == 0)
    {
        help();
    }
    else if (strcmp(cmd, "clear") == 0)
    {
        clear();
    }
    else
    {
        replace("Command not found", 1, 0x0a);
    }
}