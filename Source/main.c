#include <string.h>
#include <conio.h>
#include <stdio.h>
#include "../header/ui.h"
#include "../header/pv.h"
#include "../header/cmd.h"

int main()
{
    CLRSCR;

    initInt();
    initPV();  
    initUI();
	
    do 
    {
        resetText(&logBox);
        reassignText(&logText, startLine(&logText));
        showOutText(&logText);
        clsInput();
       
        printf("\x1b[?25h");
        inputCmd();
        printf("\x1b[?25l");
    
    } while(runCmd());

    writeCmdHis(&cmdHis, "data/cmdlog.txt");
    return 0;
}
