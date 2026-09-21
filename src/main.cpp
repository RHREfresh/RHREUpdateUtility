#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>

#include <Macro.hpp>
#include <Error.hpp>
#include <File.hpp>

void printHelp(char **argv)
{
    printf("RHRE Update Utility v1.0\n");
    printf("RHRE Update Utility was built " __DATE__ " " __TIME__ "\n\n");

    printf("usage: %s <path to downloaded executable> <path to old executable>\n", argv[0]);
    exit(1);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printHelp(argv);
    }

    return 0;
}
