#include <stdlib.h>

int main()
{
    system(
        "mintty"
        " --position center"
        // " --Border void"
        " --config ./config"
        " --exec ./launch.sh"
    );
    return 0;
}
