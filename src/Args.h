#ifndef SINOPARSER_ARGS_H
#define SINOPARSER_ARGS_H

#include <iostream>
#include <getopt.h>
#include <cstdlib>

/**
 * Args will handle the parsing of the command line option
 *
 */

class Args {

    public:
        std::string mandarinDataPath;
        std::string cantoneseDataPath;
        int listenPort;
        std::string listenHost; 

        Args(int argc, char *argv[]);
        ~Args();
        void default_options();
        void help();
        void version();

};

#define GET_ARG(var, fct) \
	var = fct(optarg);

#define GET_ARG_STR(var) \
	GET_ARG(var, std::string)

#define GET_ARG_INT(var) \
	GET_ARG(var, atoi)

#define GET_ARG_BOOL(var) \
	this->var = true;


#endif 
