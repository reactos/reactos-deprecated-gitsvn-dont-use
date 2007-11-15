#include "argparse.h"

void expandLinkerArg(std::string input, std::vector<std::string> &result)
{
    size_t comma;
    while((comma = input.find(',')) != std::string::npos)
    {
        result.push_back(input.substr(0,comma));
        input = input.substr(comma+1);
    }
    if(input.size() > 0)
        result.push_back(input);
}

bool getarg
(const std::string &shortName,
 const std::string &longName,
 bool remove,
 std::vector<std::string> &arguments,
 std::string &result,
 bool novar)
{
    for(size_t i = 0; i < arguments.size(); i++)
    {
        const std::string arg = arguments[i];
        
        // Find -Wl and parse
        if (arg.size() > 3 && arg.substr(0,3) == "-Wl")
        {
            std::vector<std::string> expanded;
            expandLinkerArg(arg.substr(4), expanded);
            if (getarg(shortName, longName, false, expanded, result))
            {
                if( remove ) arguments.erase(arguments.begin()+i);
                return true;
            }
        }
        // Catch -e etc
        else if (arg == shortName)
        {
            if  (arguments.size() > i+1)
            {
                result = arguments[i+1];
                if( remove )
                {
                    arguments.erase(arguments.begin()+i);
		    if (!novar)
                    	arguments.erase(arguments.begin()+i);
                }
            }
            return true;
        }
        // Catch --entry foo@12
        else if (longName.length())
        {
            if(arg == longName)
            {
                if  (arguments.size() > i+1)
                {
                    result = arguments[i+1];
                    if(remove)
                    {
                        arguments.erase(arguments.begin()+i);
                        if (!novar)
				arguments.erase(arguments.begin()+i);
                    }
                }
                return true;
            }
            // Catch --entry=foo@12
            else if (arg.size() > longName.size() && 
                     arg.substr(0,longName.size()) == longName &&
                     arg[longName.size()] == '=')
            {
                result = arg.substr(longName.size()+1);
                if( remove ) arguments.erase(arguments.begin()+i);
                return true;
            }
        }
    }

    return false;
}
