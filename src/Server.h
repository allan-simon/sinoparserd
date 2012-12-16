#ifndef SINOPARSER_SERVER_H
#define SINOPARSER_SERVER_H

#include <iostream>
#include "Database.h"
#include "Parser.h"
#include "CantoneseItem.h"
#include "MandarinItem.h"



class SinoparserServer {


    public:
        Database db;
        Parser<CantoneseItem> parser_cantonese;
        Parser<MandarinItem> parser;

        SinoparserServer(std::string address, int port, Database& ndb);
        ~SinoparserServer();
};


#endif
