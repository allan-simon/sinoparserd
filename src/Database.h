#ifndef SINOPARSER_NG_DATABASE_H
#define SINOPARSER_NG_DATABASE_H


#include "MandarinItem.h"
#include "CantoneseItem.h"
#include "Index.h"



class Database {

    public:

        Index<MandarinItem> mandarin_words;  
        Index<CantoneseItem> cantonese_words;  

        Database();



        void load_mandarin_data(std::string dataPath);
        void load_cantonese_data(std::string dataPath);
        void debug();

        ~Database();
};

#endif
