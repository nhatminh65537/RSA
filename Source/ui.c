#include "../header/ui.h"
#include "../header/uilib.h"
#include "../header/prgvar.h"
#include <conio.h>
#include <string.h>

BOX screen, rsaBox, cmdBox, keyBox, optBox, logBox, cptBox, pltBox,
    eBox  , dBox  , qBox  , pBox  , nBox;
void showRsaBox(), showCmdBox(), showKeyBox(), showOptBox(), showLogBox(),
     showCptBox(), showPltBox(), showEBox()  , showDBox()  , showPBox()  ,
     showQBox()  , showNBox()  ;

void initUI()
{
    initBox(&screen, "SCREEN"     , NULL   , FIX  , 0          , 0  , MAXX   , MAXY);
    initBox(&rsaBox, "RSA-PROJECT", &screen, ABS  , 1          , 1  , FULL   , FULL);
    initBox(&cmdBox, "COMMAND"    , &rsaBox, ABS  , 1          , 1  , FULL   , 4   );
    initBox(&keyBox, "KEY"        , &rsaBox, GRID , 1          , 5  , KEYSIZE, FULL);
    initBox(&optBox, "OUTPUT"     , &rsaBox, FLEXX, KEYSIZE + 1, 5  , FULL   , FULL);
    initBox(&cptBox, "CIPHERTEXT" , &optBox, ABS  , NAN        , NAN, OVER   , FULL);
    initBox(&pltBox, "PLAINTEXT"  , &optBox, ABS  , NAN        , NAN, FULL   , FULL);
    initBox(&logBox, "LOG"        , &optBox, ABS  , NAN        , NAN, FULL   , FULL);

    rsaBox.show = showRsaBox;
    cmdBox.show = showCmdBox;
    keyBox.show = showKeyBox;
    optBox.show = showOptBox;
    logBox.show = showLogBox;
    cptBox.show = showCptBox;
    pltBox.show = showPltBox;

    enableBox(&rsaBox, TRUE);
    enableBox(&cmdBox, TRUE);
    enableBox(&keyBox, TRUE);
    enableBox(&optBox, TRUE);
    enableBox(&logBox, TRUE);
    enableBox(&cptBox, TRUE);
    enableBox(&pltBox, TRUE);

    keyBox.xBox = 1;
    keyBox.yBox = 6;
    keyBox.gridType = INNER;
    initBox(&eBox, "E", &keyBox, ABS, 1, 1, 1, 1); enableBox(&eBox, TRUE);
    initBox(&dBox, "D", &keyBox, ABS, 1, 4, 1, 1); enableBox(&dBox, TRUE);
    initBox(&pBox, "P", &keyBox, ABS, 1, 5, 1, 1); enableBox(&pBox, TRUE);
    initBox(&qBox, "Q", &keyBox, ABS, 1, 6, 1, 1); enableBox(&qBox, TRUE);
    initBox(&nBox, "N", &keyBox, ABS, 1, 2, 1, 1); enableBox(&nBox, TRUE);
    eBox.show = showEBox;
    dBox.show = showDBox;
    pBox.show = showPBox;
    qBox.show = showQBox;
    nBox.show = showNBox;

    enableText(&logBox, TRUE, logText.text);

    show(&rsaBox);
}

void showRsaBox()
{
    // CSI(DIM); 
    // clearBox(&rsaBox);
    drawBox(&rsaBox, BCYAN, LIGHT , CURVE , BCYAN, TRUE);
    CSI(RESET);
}
void showCmdBox()
{
    // CSI(DIM);   
    // clearBox(&cmdBox); 
    drawBox(&cmdBox, WHITE, DOUBLE, DOUBLE, WHITE, TRUE); 
    CSI(RESET);
}
void showKeyBox()
{
    // CSI(DIM); 
    // clearBox(&keyBox);
    drawBox(&keyBox, WHITE, HEAVY , HEAVY , WHITE, TRUE); 
    CSI(RESET);
}
void showOptBox()
{
    // CSI(DIM); 
    // clearBox(&optBox);
    drawBox(&optBox, WHITE, HEAVY , HEAVY , WHITE, TRUE); 
    CSI(RESET);
}
void showLogBox()
{
    // CSI(DIM); 
    // clearBox(&logBox);
    drawBox(&logBox, WHITE, LIGHT , LIGHT , WHITE, TRUE); 
    showText(&logBox);
    CSI(RESET);   
}
void showCptBox()
{
    // CSI(DIM); 
    // clearBox(&cptBox);
    drawBox(&cptBox, WHITE, LIGHT , LIGHT , WHITE, TRUE);
    showText(&cptBox); 
    CSI(RESET);
}
void showPltBox()
{
    // CSI(DIM); 
    // clearBox(&pltBox);
    drawBox(&pltBox, WHITE, LIGHT , LIGHT , WHITE, TRUE); 
    CSI(RESET);
    showText(&pltBox);
}
void showEBox()
{
    // CSI(DIM); 
    // clearBox(&eBox);
    drawBox(&eBox, GREEN, LIGHT , LIGHT , WHITE, TRUE); 
    CSI(RESET);
}
void showDBox()
{
    // CSI(DIM); 
    // clearBox(&dBox);
    drawBox(&dBox, RED, LIGHT , LIGHT , WHITE, TRUE); 
    CSI(RESET);
}
void showPBox()
{
    // CSI(DIM); 
    // clearBox(&pBox);
    drawBox(&pBox, RED, LIGHT , LIGHT , WHITE, TRUE); 
    CSI(RESET);
}
void showQBox()
{
    // CSI(DIM); 
    // clearBox(&qBox);
    drawBox(&qBox, RED, LIGHT , LIGHT , WHITE, TRUE); 
    CSI(RESET);
}
void showNBox()
{
    // CSI(DIM); 
    // clearBox(&nBox);
    drawBox(&nBox, GREEN, LIGHT , LIGHT , WHITE, TRUE); 
    CSI(RESET);
}

void clsInput()
{
    clearBlock(3, 3, MAXX - 2, 4);
    MOVEXY(3, 3); 			
    printf("\x1b[38;5;10m $ \x1b[39m"); 
}

void inputCmd()
{
    char c;
    int inHis = 0;
    
    resetCmd(&cmd);
    
    while (TRUE)
    {        
        c = getch();

        switch (c){
            case RETURN:
                if (cmd.count != 0 ){
                    recordCmd(&cmdHis, cmd.string, 1);
                    strcat(logText.text, "\x1b[38;5;42m$ \x1b[38;5;229m");
                    strcat(logText.text, cmd.string);
                    strcat(logText.text, "\x1b[39m\n");
                    return;
                }
                break;
            case BACKSPACE:
                if (cmd.count > 0){
                    --cmd.pos;
                    deleteChar(&cmd);

                    putch(c); putch(' '); putch(c);
                    for (int i = cmd.pos; i < cmd.count; ++i)
                        putch(cmd.string[i]);
                    if (cmd.count - cmd.pos > 0)
                        CUB(cmd.count - cmd.pos);
                }
                break;
            case TAB:
                break;
            case EXT:
                c = getch();
                switch (c)
                {
                    case 71:
                        break;
                    case UP:
                        clsInput();
                        if (!inHis) {
                            recordCmd(&cmdHis, cmd.string, 0);
                            cmdHis.pos = cmdHis.cur;
                        }
                        retrievCmd(&cmd, preCmd(&cmdHis));
                        printf(cmd.string);
                        inHis = 1;
                        continue;
                    case 73:
                        break;
                    case LEFT:
                        if (cmd.pos < cmd.count){
                            ++cmd.pos;
                            CUF(1);
                        }
                        break;
                    case RIGHT:
                    if (0 < cmd.pos){
                            --cmd.pos;
                            CUB(1);
                        }
                        break;
                    case DOWN:
                        clsInput();
                        if (!inHis) {
                            recordCmd(&cmdHis, cmd.string, 0);
                            cmdHis.pos = cmdHis.cur;
                        }
                        retrievCmd(&cmd, sucCmd(&cmdHis));
                        printf(cmd.string);
                        inHis = 1;
                        continue;
                    case 81:
                        break;
                    case 83:
                        break;
                }
                break;
            default:
                insertChar(&cmd, c);
                putch(c);
                for (int i = cmd.pos; i < cmd.count; ++i)
                    putch(cmd.string[i]);
                if (cmd.count - cmd.pos > 0)
                    CUB(cmd.count - cmd.pos);
                break;
        }
        inHis = 0;
        // if (32 <= c & c <= 126){
            
        // }
        // else 
        // }
        // else if (c == '\b'){
              
        // }
        // else if ((unsigned) c > 126 || c < 32){
        //     char buff[8];
        //     while ((unsigned) c > 126){
        //         sprintf(buff , "%d ", (int) c);
        //         c = getch();
        //         strcat(logText.text, buff);
        //     }
        //         sprintf(buff , "%d ", (int) c);
        //         strcat(logText.text, buff);
        //         strcat(logText.text, "\n");
        //         show(&logBox);
        // }
    } 
}

