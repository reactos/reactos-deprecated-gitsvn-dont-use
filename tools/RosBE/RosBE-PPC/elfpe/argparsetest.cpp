#include <stdio.h>
#include "argparse.h"

int main( int argc, char **argv )
{
    std::vector<std::string> args;
    std::string result;

    if (argc <= 3) return 1;

    for( int i = 3; i < argc; i++ )
    {
        args.push_back(argv[i]);
    }
    if (getarg(argv[1], argv[2], true, args, result))
    {
        printf("%s\n", result.c_str());
        for( int i = 0; i < args.size(); i++ )
        {
            printf("%s\n", args[i].c_str());
        }
        return 0;
    }
    else return 1;
}
