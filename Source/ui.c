#include "../header/ui.h"
#include "../header/uilib.h"
#include "../header/pv.h"
#include <conio.h>
#include <string.h>

BOX screen, rsaBox, cmdBox, keyBox, wrkBox, logBox, cptBox, pltBox,
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
    initBox(&wrkBox, "WORKSPACE"  , &rsaBox, FLEXX, KEYSIZE + 1, 5  , FULL   , FULL);
    initBox(&cptBox, "CIPHERTEXT" , &wrkBox, ABS  , NAN        , NAN, OVER   , FULL);
    initBox(&pltBox, "PLAINTEXT"  , &wrkBox, ABS  , NAN        , NAN, FULL   , FULL);
    initBox(&logBox, "LOG"        , &wrkBox, ABS  , NAN        , NAN, FULL   , FULL);

    rsaBox.show = showRsaBox;
    cmdBox.show = showCmdBox;
    keyBox.show = showKeyBox;
    wrkBox.show = showOptBox;
    logBox.show = showLogBox;
    cptBox.show = showCptBox;
    pltBox.show = showPltBox;

    enableBox(&rsaBox, TRUE);
    enableBox(&cmdBox, TRUE);
    enableBox(&keyBox, TRUE);
    enableBox(&wrkBox, TRUE);
    enableBox(&logBox, TRUE);
    enableBox(&cptBox, FALSE);
    enableBox(&pltBox, FALSE);

    resetDefaultPlaintext ();
    resetDefaultCiphertext();

    initEditText(&pltEditText, &pltBox);
    initEditText(&cptEditText, &cptBox);

    keyBox.xBox = 1;
    keyBox.yBox = 6;
    keyBox.gridType = INNER;
    initBox(&eBox, "E", &keyBox, ABS, 1, 1, 1, 1); 
    initBox(&dBox, "D", &keyBox, ABS, 1, 4, 1, 1); 
    initBox(&pBox, "P", &keyBox, ABS, 1, 5, 1, 1); 
    initBox(&qBox, "Q", &keyBox, ABS, 1, 6, 1, 1); 
    initBox(&nBox, "N", &keyBox, ABS, 1, 2, 1, 1); 
    
    eBox.show = showEBox;
    dBox.show = showDBox;
    pBox.show = showPBox;
    qBox.show = showQBox;
    nBox.show = showNBox;

    enableText(&eBox, TRUE, e.dec);
    enableText(&dBox, TRUE, d.dec);
    enableText(&pBox, TRUE, p.dec);
    enableText(&qBox, TRUE, q.dec);
    enableText(&nBox, TRUE, n.dec);

    enableBox(&eBox, TRUE);
    enableBox(&dBox, TRUE);
    enableBox(&pBox, TRUE);
    enableBox(&nBox, TRUE);
    enableBox(&qBox, TRUE);
    
    enableText(&logBox, FALSE, NULL);
    initOutText(&logText, &logBox);
    reassignText(&logText, logText.pos);
}

void showRsaBox()
{
    drawBox(&rsaBox, BCYAN, LIGHT , CURVE , BCYAN, TRUE);
    CSI(RESET);
}
void showCmdBox()
{
    drawBox(&cmdBox, WHITE, DOUBLE, DOUBLE, WHITE, TRUE); 
    CSI(RESET);
}
void showKeyBox()
{
    drawBox(&keyBox, WHITE, HEAVY , HEAVY , WHITE, TRUE); 
    CSI(RESET);
}
void showOptBox()
{
    drawBox(&wrkBox, WHITE, HEAVY , HEAVY , WHITE, TRUE); 
    CSI(RESET);
}
void showLogBox()
{
    drawBox(&logBox, WHITE, LIGHT , LIGHT , WHITE, TRUE); 
    showOutText(&logText);
    CSI(RESET);   
}
void showCptBox()
{
    drawBox(&cptBox, WHITE, LIGHT , LIGHT , WHITE, TRUE);
    resetText(&cptBox);
    showText(&cptBox); 
    CSI(RESET);
}
void showPltBox()
{
    drawBox(&pltBox, WHITE, LIGHT , LIGHT , WHITE, TRUE); 
    resetText(&pltBox);
    showText(&pltBox);
    CSI(RESET);
}
void showEBox()
{
    drawBox(&eBox, GREEN, LIGHT , LIGHT , WHITE, TRUE); 
    resetText(&eBox);
    showText(&eBox);
    CSI(RESET);
}
void showDBox()
{
    drawBox(&dBox, RED, LIGHT , LIGHT , WHITE, TRUE); 
    resetText(&dBox);
    showText(&dBox);
    CSI(RESET);
}
void showPBox()
{
    drawBox(&pBox, RED, LIGHT , LIGHT , WHITE, TRUE); 
    resetText(&pBox);
    showText(&pBox);
    CSI(RESET);
}
void showQBox()
{
    drawBox(&qBox, RED, LIGHT , LIGHT , WHITE, TRUE); 
    resetText(&qBox);
    showText(&qBox);
    CSI(RESET);
}
void showNBox()
{
    drawBox(&nBox, GREEN, LIGHT , LIGHT , WHITE, TRUE); 
    resetText(&nBox);
    showText(&nBox);
    CSI(RESET);
}

void clsInput()
{
    clearBlock(3, 3, MAXX - 2, 4);
    MOVEXY(3, 3); 			
    printf("\x1b[38;5;10m $ \x1b[39m"); 
}

