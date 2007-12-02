#ifndef ELFPE_ARGPARSE_H
#define ELFPE_ARGPARSE_H

#include <vector>
#include <string>

bool getarg
(const std::string &shortName,
 const std::string &longName,
 bool remove,
 std::vector<std::string> &arguments,
 std::string &result,
 bool novar = false);

#endif//ELFPE_ARGPARSE_H
