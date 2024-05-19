#include <string.h>
#include <conio.h>
#include <time.h>
#include "../header/cmd.h"
#include "../header/int512.h"
#include "../header/ui.h"
#include "../header/pv.h"
#include "../header/uilib.h"
#include "../header/rsa.h"

char cmdList[][FULLCMDLEN] = {
    "show-plt",
    "show-cpt",
    "show-log", 
    "unshow-plt", 
    "unshow-cpt", 

    "unshow-log",
    "info-program",
    "info-pbl",
    "help",
    "unload-plt",

    "unload-cpt",
    "unload-key",
    "focus-plt",
    "focus-cpt",
    "focus-log",

    "edit-plt",
    "edit-cpt",
    "exit",
    "load-plt",
    "load-cpt",

    "load-key",
    "save-plt",
    "save-cpt",
    "save-key",
    "where-key",

    "where-cpt",
    "where-plt",
    "gene-key",
    "clear-log",
    "encrypt",

    "decrypt"
};
int cmdListLen = 31;

int searchCmd(char* cmd, int s, int fullflag) 
{
    for (int i = 0; i < cmdListLen; ++i){
        int index = (s+i)%cmdListLen;
        if (strcmp(cmd, cmdList[index])               == 0 )             return index;
        if (strncmp(cmd, cmdList[index], strlen(cmd)) == 0 && !fullflag) return index;
    }
    return -1;
}

char* phraseCmd(char* c, char args[][CMDARRCLEN])
{
    char *cur = *args;
    int count = 0;
    
    while (*c == ' ' || *c == ';') ++c;

    while (*c != 0 && *c != ';'){
        if (*c == ' '){
            *cur = 0;
            ++count;
            cur = *(args + count);
            while (*c == ' ') ++c;
            continue;
        }
        *cur = *c;
        ++cur; ++c;
    }
    **(args + count + 1) = 0;
    *cur = 0;
    cur = *args;
    return c;
}

char* getParaVal(char* para, char args[][CMDARRCLEN])
{
    int onPara = 0;
    char (*arg)[CMDARRCLEN] = (args + 1);
    while (**arg != 0){
        if (strcmp(para, *arg) == 0){
            if (**++arg == '-') return "";
            else                return *arg;
        } 
        if (**arg != '-' && !onPara && strcmp(para, "") == 0) return *arg;
        if (**arg == '-') onPara = 1;
        else              onPara = 0;
        ++arg;
    }
    return NULL; 
}

int runCmd()
{
    char *c = cmd.string;
    char args[CMDARRLEN][CMDARRCLEN];
    char message[MESSLEN];
    
    while (*c)
    {    
        c = phraseCmd(c, args);
        switch (searchCmd(args[0], 0, 1)){
            case 0 : cptBox.sx = OVER;
                     enableBox(&pltBox, TRUE); break;
            case 1 : enableBox(&cptBox, TRUE); break;
            case 2 : enableBox(&logBox, TRUE); break;
            case 3 : cptBox.sx = FULL;
                     enableBox(&pltBox, FALSE); break;
            case 4 : enableBox(&cptBox, FALSE); break;
            case 5 : enableBox(&logBox, FALSE); break; 
            case 6 : addFile(&logText, "data/info-program.txt"); break;
            case 7 : infoPblCmd();       break;
            case 8 : helpCmd(args + 1) ; break;
            case 9 : unloadPltCmd(args); break;
            case 10: unloadCptCmd(args); break;
            case 11: unloadKeyCmd(args); break;
            case 12: focusCmd(& plainText); break;
            case 13: focusCmd(&cipherText); break;
            case 14: focusLogCmd(&logText); break;
            case 15: editPltCmd()         ; break;
            case 16: editPltCmd()         ; break;
            case 17: return 0             ; break;
            case 18: loadPltCmd(args)              ; break;  
            case 19: loadCptCmd(args)              ; break;
            case 20: loadKeyCmd(args)              ; break;
            case 21: saveTextCmd(& plainText, args); break;
            case 22: saveTextCmd(&cipherText, args); break;
            case 23: saveKeyCmd(args)              ; break;
            case 24: whereKeyCmd()                 ; break;
            case 25: whereCptCmd()                 ; break;
            case 26: wherePltCmd()                 ; break;
            case 27: genkeyCmd(args) ; break;
            case 28: clearLogCmd()   ; break;
            case 29: encryptCmd(args); break;
            case 30: decryptCmd(args); break;
            default:
                strcpy(message, "rsa: ");
                strcat(strcat(message, args[0]), ": command not found!");
                addError(&logText, message);
                break;
        }            
    }
    return 1;
} 

void infoPblCmd()
{
    cptBox.sx = FULL;
    enableBox(&pltBox, FALSE);
    enableBox(&cptBox, FALSE);
    addFile(&logText, "data/info-pbl.txt"); 
}

void genkeyCmd(char args[][CMDARRCLEN])
{
    if (!spu && getParaVal("-f", args) == NULL) {
        addWarning(&logText, "This public key haven't saved yet. Phease save it or add flag -f to force action.");
        return;
    }
    if (!spr && getParaVal("-f", args) == NULL) {
        addWarning(&logText, "This private key haven't saved yet. Phease save it or add flag -f to force action.");
        return;
    }

    char *file = getParaVal("-pr", args);
    int mode = 0;
    if (getParaVal("-e", args) != NULL) {
        e.val = int256_c(getParaVal("-e", args), DECMODE);
        mode = 2;
    } else if (getParaVal("-d", args) != NULL) {
        d.val = int256_c(getParaVal("-d", args), DECMODE);
        mode = 1;
    } else if (file != NULL) {
        FILE* f = fopen(file , "rb");
        if (f == NULL){
            addError(&logText, "The private key file doesn't exist.");
            fclose(f);
            return;
        }
        fclose(f);
        mode = 3;
    }

    spr = 0; hpr = 1; 
    spu = 0; hpu = 1;

    int t = time(NULL);
    genkey(mode, &p.val, &q.val, &n.val, &e.val, &d.val, file);
    int new_t = time(NULL);
    char mess[MESSLEN];
    sprintf(mess, "Time-consuming: %ds\n", new_t - t);
    addText(&logText, mess);

    fulfillKey(&e);
    fulfillKey(&n);
    fulfillKey(&p);
    fulfillKey(&q);
    fulfillKey(&d);
    show(&keyBox);
}

void editPltCmd()
{
    if (!cplt){
        addError(&logText, "Can't edit this plaintext . Please reset file.");
        return;
    }
    splt = 0;
    enableText(&pltBox, FALSE, pltBox.text);
    printf("\x1b[?25h");
    edit(&pltEditText);
    printf("\x1b[?25l");
    FILE *f = fopen(defaultPlaintextFile, "w");
    fprintf(f, pltEditText.text);
    fclose(f);
} 

void editCptCmd()
{
    if (!scpt){
        addError(&logText, "Can't edit this ciphertext. Please reset file.");
        return;
    }
    scpt = 0;
    enableText(&cptBox, FALSE, cptBox.text);
    printf("\x1b[?25h");
    edit(&cptEditText);
    printf("\x1b[?25l");
    FILE *f = fopen(defaultCiphertextFile, "w");
    fprintf(f, cptEditText.text);
    fclose(f);   
}

void whereKeyCmd()
{
    if (!hpu){
        addText(&logText, "Public  key path: (none)\n");
    } else if (!spu) {
        addText(&logText, "Public  key path: (unsaved)\n");
    } else {
        char mess[64];
        strcpy(mess, "Public  key path: ");
        strcat(strcat(mess, publicKeyFile), "\n");
        addText(&logText, mess);
    }

    if (!hpr){
        addText(&logText, "Private key path: (none)\n");
    } else if (!spu) {
        addText(&logText, "Private key path: (unsaved)\n");
    } else {
        char mess[64];
        strcpy(mess, "Private key path: ");
        strcat(strcat(mess, privateKeyFile), "\n");
        addText(&logText, mess);
    }
}

void wherePltCmd()
{
    char message[128];
    strcpy(message, "Plaintext  path: ");
    strcat(message,  plainText.file);
    if (!hplt) strcat(message,  " (default)");
    strcat(message, "\n");
    addText(&logText, message);
}

void whereCptCmd()
{
    char message[128];
    strcpy(message, "Ciphertext path: ");
    strcat(message,  cipherText.file);
    if (!hcpt) strcat(message,  " (default)");
    strcat(message, "\n");
    addText(&logText, message);
}

void unloadPltCmd(char args[][CMDARRCLEN])
{
    if (!splt && getParaVal("-f", args) == NULL){
        addWarning(&logText, "The plaintext  haven't saved yet. Phease save it or add flag -f to force action.");
        return;
    }
    resetDefaultPlaintext();
    resetText(&pltBox);
    showText(&pltBox);
}

void unloadCptCmd(char args[][CMDARRCLEN])
{
    if (!scpt && getParaVal("-f", args) == NULL){
        addWarning(&logText, "The ciphertext haven't saved yet. Phease save it or add flag -f to force action.");
        return;
    }
    resetDefaultCiphertext();
    resetText(&cptBox);
    showText(&pltBox);
}

void unloadKeyCmd(char args[][CMDARRCLEN])
{
    if (getParaVal("-pu", args) != NULL && getParaVal("-pr", args) == NULL){
        if (!spu && getParaVal("-f", args) == NULL) {
            addWarning(&logText, "This public key haven't saved yet. Phease save it or add flag -f to force action.");
            return;
        }
        resetDefaultPuKey();
    } else if (getParaVal("-pr", args) != NULL && getParaVal("-pu", args) == NULL){
        if (!spr && getParaVal("-f", args) == NULL) {
            addWarning(&logText, "This private key haven't saved yet. Phease save it or add flag -f to force action.");
            return;
        }
        resetDefaultPrKey();
    } else{
        if (!spu && getParaVal("-f", args) == NULL) {
            addWarning(&logText, "This public key haven't saved yet. Phease save it or add flag -f to force action.");
            return;
        }
        resetDefaultPuKey();

        if (!spr && getParaVal("-f", args) == NULL) {
            addWarning(&logText, "This private key haven't saved yet. Phease save it or add flag -f to force action.");
            return;
        }
       resetDefaultPrKey();
    }
    show(&keyBox);
}

void loadPltCmd(char args[][CMDARRCLEN])
{
    if (!splt && getParaVal("-f", args) == NULL){
        addWarning(&logText, "The plaintext  haven't saved yet. Phease save it or add flag -f to force action.");
        return;
    }
    FILE *f = fopen(getParaVal("", args), "r");
    if (f == NULL) {
        addError(&logText, "The plaintext  doesn't exist");
        fclose(f);
        return;
    }
    fclose(f);
    hplt = 1;
    splt = 1;
    cplt = 0;
    loadToText(&plainText , getParaVal("", args), &pltBox);
}

void loadCptCmd(char args[][CMDARRCLEN])
{
    if (!scpt && getParaVal("-f", args) == NULL){
        addWarning(&logText, "The ciphertext haven't saved yet. Phease save it or add flag -f to force action.");
        return;
    }
    FILE *f = fopen(getParaVal("", args), "r");
    if (f == NULL) {
        addError(&logText, "The ciphertext doesn't exist");
        fclose(f);
        return;
    }
    fclose(f);
    hcpt = 1;
    scpt = 1;
    ccpt = 0;
    loadToText(&cipherText, getParaVal("", args), &cptBox);
}

void loadKeyCmd(char args[][CMDARRCLEN])
{
    if (getParaVal("", args) == NULL) {
        addError(&logText, "Don't have input file.");
        return;
    }
    char *file = getParaVal("", args);
    FILE *f;
    if (getParaVal("-pu", args) != NULL && getParaVal("-pr", args) == NULL){
        if (!spu && getParaVal("-f", args) == NULL) {
            addWarning(&logText, "This public key haven't saved yet. Phease save it or add flag -f to force action.");
            return;
        }
        strcpy(publicKeyFile, file);
        f = fopen(file, "rb");
        if (f == NULL){
            addError(&logText, "The public  key file doesn't exist.");
            fclose(f);
            return;
        }
        fread(n.val.value, 1, MAXBYTE, f);
        fread(e.val.value, 1, MAXBYTE, f);
        fclose(f);
        hpu = 1;
        spu = 1;
    } else if (getParaVal("-pr", args) != NULL && getParaVal("-pu", args) == NULL){
        if (!spr && getParaVal("-f", args) == NULL) {
            addWarning(&logText, "This private key haven't saved yet. Phease save it or add flag -f to force action.");
            return;
        }
        strcpy(privateKeyFile, file);
        f = fopen(file, "rb");
        if (f == NULL){
            addError(&logText, "The private key file doesn't exist.");
            fclose(f);
            return;
        }
        fread(p.val.value, 1, MAXBYTE, f);
        fread(q.val.value, 1, MAXBYTE, f);
        fread(d.val.value, 1, MAXBYTE, f);
        fclose(f);
        hpr = 1;
        spr = 1;
    } else{
        char name[64];
        
        if (!spu && getParaVal("-f", args) == NULL) {
            addWarning(&logText, "This public key haven't saved yet. Phease save it or add flag -f to force action.");
            return;
        }
        strcpy(publicKeyFile, file);
        f = fopen(strcat(publicKeyFile, ".puk"), "rb");
        if (f == NULL){
            addError(&logText, "The public  key file doesn't exist");
            fclose(f);
            return;
        }
        fread(n.val.value, 1, MAXBYTE, f);
        fread(e.val.value, 1, MAXBYTE, f);
        fclose(f);
        hpu = 1;
        spu = 1;

        if (!spr && getParaVal("-f", args) == NULL) {
            addWarning(&logText, "This private key haven't saved yet. Phease save it or add flag -f to force action.");
            return;
        }
        strcpy(privateKeyFile, file);
        f = fopen(strcat(privateKeyFile, ".prk"), "rb");
        if (f == NULL){
            addError(&logText, "The private key file doesn't exist");
            fclose(f);
            return;
        }
        fread(p.val.value, 1, MAXBYTE, f);
        fread(q.val.value, 1, MAXBYTE, f);
        fread(d.val.value, 1, MAXBYTE, f);
        fclose(f);
        hpr = 1;
        spr = 1;
    }
    fulfillKey(&e);
    fulfillKey(&n);
    fulfillKey(&p);
    fulfillKey(&q);
    fulfillKey(&d);
    show(&keyBox);
}

void saveKeyCmd(char args[][CMDARRCLEN])
{
    if (getParaVal("", args) == NULL) {
        addError(&logText, "Don't have output file");
        return;
    }
    char *file = getParaVal("", args);
    FILE *f;
    if (getParaVal("-pu", args) != NULL && getParaVal("-pr", args) == NULL){
        if (!hpu){
            addError(&logText, "Don't have public key.");
            return;
        }
        strcpy(publicKeyFile , file);
        f = fopen(file, "wb");
        fwrite(n.val.value, 1, MAXBYTE, f);
        fwrite(e.val.value, 1, MAXBYTE, f);
        fclose(f);
        spu = 1;
    } else if (getParaVal("-pr", args) != NULL && getParaVal("-pu", args) == NULL){
        if (!hpr){
            addError(&logText, "Don't have private key");
            return;
        }
        strcpy(privateKeyFile, file);
        f = fopen(file, "wb");
        fwrite(p.val.value, 1, MAXBYTE, f);
        fwrite(q.val.value, 1, MAXBYTE, f);
        fwrite(d.val.value, 1, MAXBYTE, f);
        fclose(f);
        spr = 1;
    } else{

        if (!hpu){
            addError(&logText, "Don't have public key.");
            return;
        }
        strcpy(publicKeyFile , file);
        f = fopen(strcat(publicKeyFile , ".puk"), "wb");
        fwrite(n.val.value, 1, MAXBYTE, f);
        fwrite(e.val.value, 1, MAXBYTE, f);
        fclose(f);
        spu = 1;

        if (!hpr){
            addError(&logText, "Don't have private key");
            return;
        }
        strcpy(privateKeyFile, file);
        f = fopen(strcat(privateKeyFile, ".prk"), "wb");
        fwrite(p.val.value, 1, MAXBYTE, f);
        fwrite(q.val.value, 1, MAXBYTE, f);
        fwrite(d.val.value, 1, MAXBYTE, f);
        fclose(f);
        spr = 1;
    }
}

void saveTextCmd(TEXT* text, char args[][CMDARRCLEN])
{
    if (getParaVal("", args) == NULL) {
        addError(&logText, "Don't have output file");
        return;
    }
    char *file = getParaVal("", args);
    saveText(text, file);
    loadToText(text, file, text->box);
    if (strcmp(args[0], "save-plt") == 0) hplt = 1, hplt = 1, cplt = 0;
    if (strcmp(args[0], "save-cpt") == 0) hcpt = 1, hcpt = 1, ccpt = 0;
}

void helpCmd(char args[][CMDARRCLEN])
{
    if (strcmp(args[0], "") == 0)
        addFile(&logText, "data/help/help.txt");
    else {
        switch (searchCmd(args[0], 0, 1)){
            case 9 : addFile(&logText, "data/help/unload-plt.txt"); break;
            case 10: addFile(&logText, "data/help/unload-cpt.txt"); break;
            case 11: addFile(&logText, "data/help/unload-key.txt"); break;
            case 18: addFile(&logText, "data/help/load-plt.txt"); break;  
            case 19: addFile(&logText, "data/help/load-cpt.txt"); break;
            case 20: addFile(&logText, "data/help/load-key.txt"); break;
            case 21: addFile(&logText, "data/help/save-plt.txt"); break;
            case 22: addFile(&logText, "data/help/save-cpt.txt"); break;
            case 23: addFile(&logText, "data/help/save-key.txt"); break;
            case 27: addFile(&logText, "data/help/gene-key.txt"); break;
            case 29: addFile(&logText, "data/help/decrypt.txt" ); break;
            case 30: addFile(&logText, "data/help/encrypt.txt" ); break;
            default:
                char mess[MESSLEN];
                sprintf(mess, "Sorry! Can't fnd help file for \"%s\".\n", args[0]);
                addError(&logText, mess);
                break;
        }
    }
}

void clearLogCmd()
{
    *logText.text = '\0';
    logText.pos = logText.text;
    logText.end = logText.text;
}

void focusCmd(TEXT* text)
{
    while (1){
        BOX* box = text->box;
        int linesize = box->right - box->left - 1;
        int pagesize = box->top - box->bottom - 1; 
        char c = getch();
        if (c == EXT){
            c = getch();
            switch (c)
            {
                case UP:
                    readText(text, -linesize);
                    break;
                case DOWN:
                    readText(text, linesize);
                    break;
                case PGDN:
                    readText(text, -linesize*pagesize/2);
                    break;
                case PGUP:
                    readText(text, linesize*pagesize/2);
                    break;
            }
        }
        if (c == EXIT){
            break;
        }
        resetText(text->box);
        // clearBox(&pltBox);
        showText(text->box);
    }
}

void focusLogCmd(OUTTEXT* out)
{
    while (1){
        char c = getch();
        if (c == EXT){
            c = getch();
            switch (c)
            {
            case UP:
                reassignText(out, prevLine(out, out->pos));
                break;
            case DOWN:
                reassignText(out, nextLine(out, out->pos));
                break;
            case PGDN:
                break;
            case PGUP:
                break;
            }
        }
        if (c == EXIT){
            break;
        }
        resetText(out->box);
        showOutText(out);
    }
}

void encryptCmd(char args[][CMDARRCLEN])
{
    char *input, *output;
    INT512 ek, nk;

    if (getParaVal("", args) != NULL) input = getParaVal("", args);
    else if (hplt || !splt)           input = plainText.file;
    else {
        addError(&logText, "Can't find plaintext");
        return;
    }
    FILE *f = fopen(input, "r");
    if (f == NULL) {
        addError(&logText, "The plaintext  doesn't exist");
        fclose(f);
        return;
    }
    fclose(f);

    if (getParaVal("-k", args) != NULL) {
        FILE* f = fopen(getParaVal("-k", args), "rb");
        if (f == NULL){
            addError(&logText, "The public  key file doesn't exist.");
            return;
        }
        fread(nk.value, 1, MAXBYTE, f);
        fread(ek.value, 1, MAXBYTE, f);
        fclose(f);
    } else if (hpu){
        nk = n.val;
        ek = e.val;
    } else{
        addError(&logText, "Don't have public key for encryption");
        return;
    }

    if (getParaVal("-o", args) != NULL) 
        output = getParaVal("-o", args);
    else if (hcpt || !scpt){
        if (getParaVal("-f", args) == NULL){
            addWarning(&logText, "This action can be change your current content of ciphertext file. Please add output file or put flag -f to force action.");
            return;
        } else {
            output = cipherText.file;
            ccpt = 0;
        }
    } else {
        output = cipherText.file;
        ccpt = 0;
    }
    if (!hcpt) scpt = 0;

    int t = time(NULL);
    encrypt(e.val, n.val, input, output);
    int new_t = time(NULL);
    char mess[MESSLEN];
    sprintf(mess, "Time-consuming: %ds\n", new_t - t);
    addText(&logText, mess);
    addSuccess(&logText, "Encryption finished successly.");
    readText(&cipherText, 0);
    resetText(&cptBox);
    showText(&cptBox);
}

void decryptCmd(char args[][CMDARRCLEN])
{
    char *input, *output;
    INT512 dk, pk, qk;

    if (getParaVal("", args) != NULL) input = getParaVal("", args);
    else if (hcpt || !scpt)           input = cipherText.file;
    else {
        addError(&logText, "Can't find ciphertext");
        return;
    }
    FILE *f = fopen(input, "r");
    if (f == NULL) {
        addError(&logText, "The ciphertext doesn't exist");
        fclose(f);
        return;
    }
    fclose(f);

    if (getParaVal("-k", args) != NULL) {
        FILE* f = fopen(getParaVal("-k", args), "rb");
        if (f == NULL){
            addError(&logText, "The prinvate key file doesn't exist.");
            return;
        }
        fread(pk.value, 1, MAXBYTE, f);
        fread(qk.value, 1, MAXBYTE, f);
        fread(dk.value, 1, MAXBYTE, f);
        fclose(f);
    } else if (hpr){
        dk = d.val;
        pk = p.val;
        qk = q.val;
    } else{
        addError(&logText, "Don't have private key for encryption");
        return;
    }

    if (getParaVal("-o", args) != NULL) output = getParaVal("-o", args);
    else if (hplt || !splt){
        if (getParaVal("-f", args) == NULL){
            addWarning(&logText, "This action can be change your current content of plaintext file. Please add output file or put flag -f to force action.");
            return;
        } else {
            output = plainText.file;
            cplt = 0;
        }
    } else {
        output = plainText.file;
        cplt = 0;
    }
    if (!hplt) splt = 0;

    int t = time(NULL);
    decrypt(d.val, p.val, q.val, input, output);
    int new_t = time(NULL);
    char mess[MESSLEN];
    sprintf(mess, "Time-consuming: %ds\n", new_t - t);
    addText(&logText, mess);
    addSuccess(&logText, "Decryption finished successly.");
    readText(&plainText, 0);
    resetText(&pltBox);
    showText(&pltBox);
}
