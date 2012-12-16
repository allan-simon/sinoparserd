#ifndef SINOPARSER_ITEM_H
#define SINOPARSER_ITEM_H

#include <iostream>

class SinoParserItem {
    public:
        int id;
        std::string romanization;

        SinoParserItem();
        virtual ~SinoParserItem() = 0;

        virtual void debug();
};
#endif
