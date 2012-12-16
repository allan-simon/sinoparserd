#ifndef SINOPARSER_NG_ITEM_H
#define SINOPARSER_NG_ITEM_H

#include <iostream>
#include "SinoParserItem.h"

#define SINOPARSER_SIMPLIFIED -1
#define SINOPARSER_MIXED       0
#define SINOPARSER_TRADITIONAL 1   


// NOTE: 
// for the moment data will be store this way
//
// key => {A,B,...}
// with {} the MandarinItem
//
// so for example
// 说 => {42,shuo1,說} 
// 說 => {43,shuo1,说}
//
// TODO there is maybe a more space efficient way to store this
//

class MandarinItem : public SinoParserItem {
    public:
        int id; 
        int type;
        std::string str_other_script;

        void debug();

        MandarinItem();
        MandarinItem(int id, std::string pinyin);
        MandarinItem(int id, int type, std::string other_script, std::string pinyin);
        ~MandarinItem();

}; 


#endif
