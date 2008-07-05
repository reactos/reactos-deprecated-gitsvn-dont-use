/*
   Copyright (C) Andrew Tridgell 2002
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "ccache.h"


/*
  execute a compiler backend, capturing all output to the given paths
  the full path to the compiler to run is in argv[0]
*/
int execute(char **argv, 
	    const char *path_stdout,
	    const char *path_stderr)
{
#ifndef _WIN32
	pid_t pid;
	int status;

	pid = fork();
	if (pid == -1) fatal("Failed to fork");
	
	if (pid == 0) {
		int fd;

		unlink(path_stdout);
		fd = open(path_stdout, O_WRONLY|O_CREAT|O_TRUNC|O_EXCL|O_BINARY, 0666);
		if (fd == -1) {
			exit(STATUS_NOCACHE);
		}
		dup2(fd, 1);
		close(fd);

		unlink(path_stderr);
		fd = open(path_stderr, O_WRONLY|O_CREAT|O_TRUNC|O_EXCL|O_BINARY, 0666);
		if (fd == -1) {
			exit(STATUS_NOCACHE);
		}
		dup2(fd, 2);
		close(fd);

		exit(execv(argv[0], argv));
	}

	if (waitpid(pid, &status, 0) != pid) {
		fatal("waitpid failed");
	}

	if (WEXITSTATUS(status) == 0 && WIFSIGNALED(status)) {
		return -1;
	}

	return WEXITSTATUS(status);
#else /* Should be portable */
    int   status = -2;
    int   fd, std_od = -1, std_ed = -1;

    unlink(path_stdout);
    std_od = _dup(1);
    fd = _open(path_stdout, O_WRONLY|O_CREAT|O_TRUNC|O_EXCL|O_BINARY, 0666);
    if (fd == -1) {
        status = STATUS_NOCACHE;
        cc_log("stdout error: failed to open %s\n", path_stdout);
        goto out;
    }
    /*std_od = */ _dup2(fd, 1);
    _close(fd);

    unlink(path_stderr);
    fd = _open(path_stderr, O_WRONLY|O_CREAT|O_TRUNC|O_EXCL|O_BINARY, 0666);
    std_ed = _dup(2); 
    if (fd == -1) {
        status = STATUS_NOCACHE;
        cc_log("stderr error: failed to open %s\n", path_stderr);
        goto out;
    }
    /*std_ed =*/ _dup2(fd, 2);
    _close(fd);

    /* Spawn process (_exec* familly doesn't return) */
    status = _spawnv(_P_WAIT, argv[0], (const char* const*)argv);

 out:
    cc_log("%s:\n  stdout -> %s\n  stderr -> %s\n  process status=%i\n",
           argv[0], path_stdout, path_stderr, status);
    if (status == -1) cc_log("Error %i: %s\n", errno, strerror(errno));

    /* Restore descriptors */
    if (std_od != -1) _dup2(std_od, 1);
    if (std_ed != -1) _dup2(std_ed, 2); 
    _flushall();

    return (status>0);
#endif
}



/*
 Check that the executable exists
*/
char is_exec_file(const char *fname, const char *exclude_name)
{
    struct stat st2;

#ifdef _WIN32
    UNREFERENCED_PARAMETER(exclude_name);
#else
    struct stat st1;
#endif

    if (access(fname, 0) == 0 &&
#ifndef _WIN32 /* Symlinks not used under windows */
        lstat(fname, &st1) == 0 &&
#endif
        stat(fname, &st2) == 0 &&
        S_ISREG(st2.st_mode)) {
#ifndef _WIN32 /* Symlinks not used under windows */
        /* if its a symlink then ensure it doesn't
           point at something called exclude_name */
        if (S_ISLNK(st1.st_mode)) {
            char *buf = x_realpath(fname);
            if (buf) {
                char *p = str_basename(buf);
                if (strcmp(p, exclude_name) == 0) {
                    /* its a link to "ccache" ! */
                    free(p);
                    free(buf);
                    return -1;
                }
                free(buf);
                free(p);
            }
        }
#endif    
        /* found it! */
        return 1;
    }
    return -1;
}

/*
  find an executable by name in $PATH. Exclude any that are links to exclude_name 
*/
char *find_executable(const char *name, const char *exclude_name)
{
	char *path;
	char *tok;
	const char *sep = ":";

	if (*name == PATH_SEP_CHAR) {
		return x_strdup(name);
	}

	path = getenv("CCACHE_PATH");
	if (!path) {
		path = getenv("PATH");
	}
	if (!path) {
		cc_log("no PATH variable!?\n");
		return NULL;
	}

	path = x_strdup(path);
	
	/* Determine path separator */
    if (strchr(path, ';')) sep = ";";

	/* search the path looking for the first compiler of the right name
	   that isn't us */
	for (tok=strtok(path, sep); tok; tok = strtok(NULL, sep)) {
		char *fname;
		x_asprintf(&fname, "%s"PATH_SEP"%s", tok, name);
		/* look for a normal executable file */
		

		if (is_exec_file(fname, exclude_name) > 0)
        {
			free(path);
			return fname;
		}
		free(fname);

			/* found it! */
#ifdef _WIN32 /* Add .exe under win32 */
		x_asprintf(&fname, "%s"PATH_SEP"%s.exe", tok, name);

		/* look for a normal executable file */
        if (is_exec_file(fname, exclude_name) > 0)
        {
			free(path);
			return fname;
		}
		free(fname);
#endif
	}
free(path);
	return NULL;
}
