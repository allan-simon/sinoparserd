#ifndef SINOPARSER_CANTONESE_ITEM_H
#define SINOPARSER_CANTONESE_ITEM_H

#include <iostream>
#include "SinoParserItem.h"

class CantoneseItem : public SinoParserItem {
    public:

        CantoneseItem(int id, std::string jyutping);
        ~CantoneseItem();
        
        void debug();
};
#endif
