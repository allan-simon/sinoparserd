#ifndef SINOPARSER_NG_DATA_PARSER_H
#define SINOPARSER_NG_DATA_PARSER_H

#include <expat.h>
#include "Database.h"
#include "Index.h"

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
class Database;

class DataParser {

    protected:
	    XML_Parser handle;


        void error_print(XML_Parser handle);
    
    public:
        DataParser( Index< MandarinItem>&index);
        DataParser( Index< CantoneseItem>&index);
        virtual bool read(std::string data_path);
        virtual ~DataParser();

};

/**
 *
 */
static void element_end(void* data, XML_Char const *name) {

}

/**
 *
 */
static XML_Char * get_attr(XML_Char const **attr, XML_Char const *name) {
	int i;
	for (i = 0; attr[i]; i += 2) {
		if (strcmp(attr[i], name) == 0)
			return (XML_Char *) attr[i + 1];
	}
	return NULL;
}


#endif
