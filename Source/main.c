#include <string.h>
#include <conio.h>
#include <stdio.h>
#include "../header/ui.h"
#include "../header/prgvar.h"
#include "../header./cmd.h"

char plaintext[2048], cmdOpt[2048];

int main()
{
    CLRSCR;
    
    initUI();
    initPrgVar();

    // enableText(&optBox, TRUE, logText.text);
    			
    do 
    {
        show(&logBox);
        clsInput();
        inputCmd();
    } while(runCmd());

    writeCmdHis(&cmdHis, "data/cmdlog.txt");
    return 0;
}
