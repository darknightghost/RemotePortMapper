#include <cstdio>

#include <common/logger/logger.h>

int main(int argc, char *argv[])
{
    (void)(argc);
    (void)(argv);
    log_debug("aaaaaa"
              << "bbbb" << 0.5);
    return 0;
}
