#include "ui.h"
#include "uilib.h"

BOX screen, rsaBox, cmdBox, keyBox, optBox, logBox, cptBox, pltBox,
    aBox  , bBox  , qBox  , pBox  , nBox;
void showRsaBox(), showCmdBox(), showKeyBox(), showOptBox(), showLogBox(),
     showCptBox(), showPltBox(), showABox()  , showBBox()  , showPBox()  ,
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

    keyBox.xBox = 2;
    keyBox.yBox = 6;
    keyBox.gridType = INNER;
    initBox(&aBox, "A", &keyBox, ABS, 1, 1, 1, 1); enableBox(&aBox, TRUE);
    initBox(&bBox, "B", &keyBox, ABS, 2, 1, 1, 1); enableBox(&bBox, TRUE);
    initBox(&pBox, "P", &keyBox, ABS, 1, 3, 1, 1); enableBox(&pBox, TRUE);
    initBox(&qBox, "Q", &keyBox, ABS, 1, 4, 1, 1); enableBox(&qBox, TRUE);
    initBox(&nBox, "N", &keyBox, ABS, 1, 5, 1, 1); enableBox(&nBox, TRUE);
    aBox.show = showABox;
    bBox.show = showBBox;
    pBox.show = showPBox;
    qBox.show = showQBox;
    nBox.show = showNBox;

    show(&screen);
}

void showRsaBox()
{
    // CSI(DIM); 
    drawBox(&rsaBox, BCYAN, LIGHT , CURVE , BCYAN, TRUE);
    CSI(RESET);
}
void showCmdBox()
{
    // CSI(DIM);    
    drawBox(&cmdBox, WHITE, DOUBLE, DOUBLE, WHITE, TRUE); 
    CSI(RESET);
}
void showKeyBox()
{
    // CSI(DIM); 
    drawBox(&keyBox, WHITE, HEAVY , HEAVY , WHITE, TRUE); 
    CSI(RESET);
}
void showOptBox()
{
    // CSI(DIM); 
    drawBox(&optBox, WHITE, HEAVY , HEAVY , WHITE, TRUE); 
    CSI(RESET);
}
void showLogBox()
{
    // CSI(DIM); 
    drawBox(&logBox, WHITE, LIGHT , LIGHT , WHITE, TRUE); 
    showText(&logBox);
    CSI(RESET);   
}
void showCptBox()
{
    // CSI(DIM); 
    drawBox(&cptBox, WHITE, LIGHT , LIGHT , WHITE, TRUE);
    showText(&cptBox); 
    CSI(RESET);
}
void showPltBox()
{
    // CSI(DIM); 
    drawBox(&pltBox, WHITE, LIGHT , LIGHT , WHITE, TRUE); 
    CSI(RESET);
    showText(&pltBox);
}
void showABox()
{
    // CSI(DIM); 
    drawBox(&aBox, WHITE, LIGHT , LIGHT , WHITE, TRUE); 
    CSI(RESET);
}
void showBBox()
{
    // CSI(DIM); 
    drawBox(&bBox, WHITE, LIGHT , LIGHT , WHITE, TRUE); 
    CSI(RESET);
}
void showPBox()
{
    // CSI(DIM); 
    drawBox(&pBox, WHITE, LIGHT , LIGHT , WHITE, TRUE); 
    CSI(RESET);
}
void showQBox()
{
    // CSI(DIM); 
    drawBox(&qBox, WHITE, LIGHT , LIGHT , WHITE, TRUE); 
    CSI(RESET);
}
void showNBox()
{
    // CSI(DIM); 
    drawBox(&nBox, WHITE, LIGHT , LIGHT , WHITE, TRUE); 
    CSI(RESET);
}