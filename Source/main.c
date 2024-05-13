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

    initInt();
    initPrgVar();  
    e.val = int256_c("BB39", HEXMODE); fulfillKey(&e);
    n.val = int256_c("EC4B", HEXMODE); fulfillKey(&n);
    p.val = int256_c("F1"  , HEXMODE); fulfillKey(&p);
    q.val = int256_c("FB"  , HEXMODE); fulfillKey(&q);
    d.val = int256_c("a9"  , HEXMODE); fulfillKey(&d);
    initUI();
	
    do 
    {
        showOutText(&outText);
        clsInput();
       
        printf("\x1b[?25h");
        inputCmd();
        printf("\x1b[?25l");
    
    } while(runCmd());

    writeCmdHis(&cmdHis, "data/cmdlog.txt");
    return 0;
}
