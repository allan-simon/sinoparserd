#include "Args.h"


/**
 * Constructor for the Args class
 *
 */

Args::Args(int argc, char *argv[]) {
    default_options();

	struct option long_options[] = {
		{ "mandarin-data", 1, NULL, 'm' },
		{ "cantonese-data", 1, NULL, 'c' },
		{ "listen-host", 1, NULL, 'h' },
		{ "listen-port", 1, NULL, 'p' },
		{ "help", 0, NULL, '?' },
		{ "version", 0, NULL, 'v' },
		{ NULL, 0, NULL, 0 }
	};
	int option_index, c;

	while (
        (c = getopt_long(
            argc,
            argv,
            "m:c:h:p:?v",
            long_options,
            &option_index
        )) != -1
    ) {
		switch (c) {
			case 'c': 
				GET_ARG_STR(cantoneseDataPath);
				break;
			case 'm': 
				GET_ARG_STR(mandarinDataPath);
				break;
			case 'h':
				GET_ARG_STR(listenHost);
				break;
			case 'p':
				GET_ARG_INT(listenPort);
				break;
			case '?':
				help();
				exit(0);
				break;
			case 'v':
				version();
				exit(0);
				break;
		}
    }

}

/**
 *
 */

Args::~Args() {

}
/**
 * Print the help
 *
 */

void Args::help() {
	std::cout << " Options:" << std::endl;
	std::cout << "  -m, --mandarin-data    Set path of mandarin data file (default empty)" << std::endl;
	std::cout << "  -c, --cantonese-data   Set path of cantonese data file (default empty)" << std::endl;
	std::cout << "  -h, --listen-host      Set listening host (default is 0.0.0.0)" << std::endl;
	std::cout << "  -p, --listen-port      Set listening port (default is 8080)" << std::endl;
	std::cout << "  -?, --help             Show help options and exit" << std::endl;
	std::cout << "  -v, --version          Show the version number and exit" << std::endl;
}

/**
 * Print the software name and version
 *
 */
void Args::version() {
    std::cout << "Sinoparserd 0.1" << std::endl;
}

/**
 * configure the default option 
 */
void Args::default_options() {
    listenPort = 8080;
    listenHost = "0.0.0.0";
    cantoneseDataPath = "";
    mandarinDataPath = "";
}
