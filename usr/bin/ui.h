#ifndef __UIRSA__
#define __UIRSA__

#include "uilib.h"

#define MAXX      150
#define MAXY      50
#define CLRSCR    clearBlock(1, 1, MAXX, MAXY)

enum CommandArea
{
    // Command screen
    CMDTOP    = 2,
    CMDBOTTOM = 5,
    CMDLEFT   = 2,
    CMDRIGHT  = 5,
};
enum KeyArea
{
    // Key screen
    KEYTOP     = CMDBOTTOM + 1,
    KEYBOTTOM  = MAXY - 1,
    KEYLEFT    = 2,
    KEYRIGHT   = 49,
};
enum TextArea
{
    // Ouput screen
    OPTTOP    = CMDBOTTOM + 1,
    OTPBOTTOM = MAXY - 1,
    OTPLEFT   = KEYRIGHT + 1,
    OTPRIGHT  = MAXX - 1
};

extern BOX screen, rsaBox, cmdBox, keyBox, optBox;
void initUI();

#endif