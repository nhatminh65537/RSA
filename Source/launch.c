#include <stdlib.h>

int main()
{
    system(
        "cd usr/bin &&"
        "mintty"
        " --position center"
        " --config ./config"
        " --exec ../../launch.sh"
    );
    return 0;
}
