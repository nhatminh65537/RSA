#include <stdlib.h>

int main()
{
    system(
        "mintty"
        " --title \"RSA Project\""
        " --size 150x50"
        " --position center"
        " --config ./config"
        " --exec ./launch.sh"
    );
    return 0;
}