#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <list>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include "objectfile.h"
#include "header.h"
#include "argparse.h"

#define TYPE_PEHEADER (SHT_LOOS + 1)

int execute_command( bool verbose, const std::vector<std::string> &args )
{
	std::vector<const char *> argvect;
	pid_t pid;
	int status = 1;

	for( size_t i = 0; i < args.size(); i++ )
	{
		argvect.push_back(args[i].c_str());
	}
	argvect.push_back(NULL);
	if (!(pid = vfork()))
	{
		execv((const char *)argvect[0], (char *const*)&argvect[0]);
	}
	else
	{
		waitpid(pid, &status, 0);
	}
	return status;
}

void possibly_define
(std::vector<std::string> &args, 
 std::string label,
 std::string body = "1")
{
    std::string the_definition = std::string("-D") + label;
    for (size_t i = 0; i < args.size(); i++)
    {
        if (the_definition.size() <= args[i].size())
        {
            if (the_definition == args[i].substr(0,the_definition.size()))
            {
                // Already defined
                return;
            }
        }
    }
    args.insert(args.begin(), the_definition + "=" + body);
}

int main( int argc, char **argv ) {
    bool verbose = false, nostdlib = false, 
	nostartfiles = false, compile_only = false, is_dll = false, entry,
	mkheader = true;
    int status = 0;
    std::string gcc_name, gcc_hash_output, gcc_line, 
        mingw_lib_dir, output_file, entry_point = "", 
        image_base = "0x400000",
        section_align = "0x1000", file_align = "0x1000", subsystem = "cui",
        result;
    std::vector<std::string> gcc_args_str;
    const char *installdir = getenv("INSTALLDIR");
    installdir = installdir ? installdir : ".";

    for( int i = 1; i < argc; i++ )
        gcc_args_str.push_back(argv[i]);

    gcc_name = std::string(installdir) + "/bin/powerpc-unknown-elf-gcc";
    mingw_lib_dir = std::string(installdir) + "/lib";

    getarg("-gcc-name", "", true, gcc_args_str, gcc_name);
    getarg("-o", "", false, gcc_args_str, output_file);
    mkheader = !getarg("-N", "", false, gcc_args_str, result);
    compile_only = getarg("-c", "", false, gcc_args_str, result) ||
        getarg("-E", "", false, gcc_args_str, result);
    getarg("-mingw-lib-dir", "", true, gcc_args_str, mingw_lib_dir);
    verbose = getarg("-v", "", false, gcc_args_str, result);
    nostdlib = getarg("-nostdlib", "", false, gcc_args_str, result);
    nostartfiles = getarg("-nostartfiles", "", false, gcc_args_str, result);
    getarg("", "--subsystem", true, gcc_args_str, result);
    entry = getarg("-e", "--entry", true, gcc_args_str, entry_point);
    getarg("", "--image-base", true, gcc_args_str, image_base);
    getarg("", "--section-alignment", true, gcc_args_str, section_align);
    getarg("", "--file-alignment", true, gcc_args_str, file_align);
    is_dll = getarg("-shared", "", true, gcc_args_str, result, true);
    if (output_file.find(".gch") != std::string::npos)
    	compile_only = true;

    if( entry )
    {
        if(entry_point.size() && entry_point[0] == '_') 
        { 
            entry_point = entry_point.substr(1);
        }
        if( !compile_only )
        {
            gcc_args_str.push_back(std::string("-Wl,--entry=") + entry_point);
            gcc_args_str.push_back(std::string("-Wl,--undefined=") + entry_point);
        }
    }

    // Nostdlib
    if( !nostdlib && !compile_only ) {
	gcc_args_str.push_back(std::string("-L") + mingw_lib_dir);
	gcc_args_str.push_back("-lcrtdll");
	gcc_args_str.push_back("-lmingw32");
	gcc_args_str.push_back("-lkernel32");
    }

    // We never use the system start files or standard libs
    gcc_args_str.insert(gcc_args_str.begin(),"-nostdlib");
    gcc_args_str.insert(gcc_args_str.begin(),"-nostartfiles");
    gcc_args_str.insert(gcc_args_str.begin(),"include/crt");
    gcc_args_str.insert(gcc_args_str.begin(),"-isystem");
    // Add necessary definitions but allow overrides
    possibly_define(gcc_args_str,"_M_PPC");
    possibly_define(gcc_args_str,"_PPC_");
    possibly_define(gcc_args_str,"__PowerPC__");
    possibly_define(gcc_args_str,"stdcall","");
    possibly_define(gcc_args_str,"__stdcall__","");
    possibly_define(gcc_args_str,"cdecl","");
    possibly_define(gcc_args_str,"__cdecl__","");
    possibly_define(gcc_args_str,"fastcall","");
    possibly_define(gcc_args_str,"__fastcall__","");
    possibly_define(gcc_args_str,"WORDS_BIGENDIAN");
    possibly_define(gcc_args_str,"__MINGW_IMPORT","extern");
    possibly_define(gcc_args_str,"_CRTIMP","");
    possibly_define(gcc_args_str,"__NO_CTYPE_INLINES");
    possibly_define(gcc_args_str,"__declspec(x)","");
    /*if (!compile_only) gcc_args_str.push_back(mingw_lib_dir + "/libgcc.a"); */

    // Stack on driver name and dump commands flag
    if( verbose ) gcc_args_str.insert(gcc_args_str.begin(),"-v");
    if (!compile_only) 
    {
    	if( is_dll ) gcc_args_str.insert(gcc_args_str.begin(), "-Wl,-r");
        gcc_args_str.insert(gcc_args_str.begin(), "-Wl,-q");
        gcc_args_str.insert(gcc_args_str.begin(), "-Wl,-d");
        gcc_args_str.insert(gcc_args_str.begin(), "-Wl,--start-group");
        gcc_args_str.insert(gcc_args_str.begin(), std::string("-Wl,-T,") + mingw_lib_dir + "/ldscript");
        //gcc_args_str.insert(gcc_args_str.begin(), "-Wl,-shared,-Bsymbolic,-z,defs");
    }
    gcc_args_str.insert(gcc_args_str.begin(),gcc_name);

    if( verbose ) {
	fprintf( stderr, "#" );
	for( size_t i = 0; i < gcc_args_str.size(); i++ ) {
	    fprintf( stderr, " \"%s\"", gcc_args_str[i].c_str() );
	}
	fprintf( stderr, "\n" );
    }
    if (!compile_only) gcc_args_str.insert(gcc_args_str.end(), "-Wl,--end-group");

    if ( !(status = execute_command( verbose, gcc_args_str )) && !compile_only && mkheader )
    {
        /* Ok fixup the elf object file */
        ElfObjectFile eof(output_file);
        const ElfObjectFile::Symbol *entry_sym;
        
        if(!eof) exit(1); 
        
        entry_sym = eof.getNamedSymbol(entry_point);

        ElfPeHeader header
	(strtoul(image_base.c_str(), 0, 0),
	 strtoul(section_align.c_str(), 0, 0),
	 strtoul(file_align.c_str(), 0, 0),
	 entry_sym,
	 0x10000,
	 0x100000,
	 0x10000,
	 0x100000,
	 atoi(subsystem.c_str()),
	 is_dll,
	 &eof);
    
        eof.addSection(".peheader", header.getData(), TYPE_PEHEADER);
    }

    return status;
}
