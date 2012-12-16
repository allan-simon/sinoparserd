#include "DataParser.h"
#include "MandarinItem.h"
#include "CantoneseItem.h"

#define BUFFER_SIZE 4096

static int count = 0;
/**
 *
 */
static void element_start(void* data, XML_Char const *name, XML_Char const **attr) {
    Index<MandarinItem> * index = (Index<MandarinItem>*) data;
	//<item>
    //std::cout << "[DEBUG] read element " << std::endl;
	if (strcmp(name, "item") == 0) {

		char *id = get_attr(attr, "id");
		std::string simp(get_attr(attr, "simp"));
		std::string trad(get_attr(attr, "trad"));
		std::string pinyin(get_attr(attr, "pinyin"));

        MandarinItem* tempItem = NULL;
        //std::cout << "[NOTICE] toto" << std::endl; 
        // if both trad and simp version are the same then we add only one
        // item otherwise we add 2, one for each scripts
        if (simp.compare(trad) == 0) {
            count++;
            //std::cout << "<item id='"<< count <<"' trad='' simp='"<< simp << "' pinyin='"<< pinyin <<"' />'" << std::endl;
            tempItem = new MandarinItem(
                count,
                SINOPARSER_MIXED,
                simp,
                pinyin
            );
            index->add_item(simp, tempItem);
        } else {
            count++;
            // TODO for the moment we add the same ID
            // first simplified version ...
            tempItem = new MandarinItem(
                count,
                SINOPARSER_SIMPLIFIED,
                trad,
                pinyin
            );
            index->add_item(simp, tempItem);
            count++;
            // ... then traditional version
            tempItem = new MandarinItem(
                count,
                SINOPARSER_TRADITIONAL,
                simp,
                pinyin
            );
            index->add_item(trad, tempItem);
        }

	}

}

/**
 *
 */
static void element_start_cantonese(void* data, XML_Char const *name, XML_Char const **attr) {
    Index<CantoneseItem> *index = (Index<CantoneseItem> *) data;
	//<item>
	if (strcmp(name, "item") == 0) {

		char *id = get_attr(attr, "id");
		std::string str(get_attr(attr, "str"));
		std::string jyutping(get_attr(attr, "jyutping"));

        count++;
        CantoneseItem* tempItem = new CantoneseItem(
            count,
            jyutping
        );
        index->add_item(str, tempItem);

	}

};



/**
 *
 */
void DataParser::error_print(XML_Parser handle) {
	int code = XML_GetErrorCode(handle);
	int col = XML_GetCurrentColumnNumber(handle);
	int line = XML_GetCurrentLineNumber(handle);
    
	std::cerr <<  "[ERROR] Expat error (" << code  << ") on (" << line <<":"<< col <<"): "<<  XML_ErrorString((XML_Error)code) << std::endl;
}

/**
 *
 */
DataParser::DataParser(Index<MandarinItem>& index) {
	handle = XML_ParserCreate("utf-8");
	XML_SetUserData(handle, &index);
	XML_SetElementHandler(handle, element_start, element_end);
}

/**
 *
 */
DataParser::DataParser(Index<CantoneseItem> & index) {
	handle = XML_ParserCreate("utf-8");
	XML_SetUserData(handle, &index);
	XML_SetElementHandler(handle, element_start_cantonese, element_end);
}



/**
 *
 */
DataParser::~DataParser() {
	XML_ParserFree(handle);
}

/**
 *
 */
bool DataParser::read(std::string path) {
	FILE *f = fopen(path.c_str(), "rb");
	if (f == NULL) {
		std::cerr << "Could not read input file" << std::endl;
		return false;
	}

	//std::cout << "[NOTICE] Loading data from " << path << std::endl;
	void *buffer = malloc(BUFFER_SIZE);
	for (;;) {
		size_t readen = fread(buffer, 1, BUFFER_SIZE, f);
		int last = readen != BUFFER_SIZE ? 1 : 0;

		if (XML_Parse(handle, (char*) buffer, readen, last) == 0) {
			error_print(handle);

			free(buffer);
			fclose(f);
			return false;
		}
		if (last)
			break;
	}

	fclose(f);
	free(buffer);
	return true;
}

