#include <iostream>
#include <glibmm/ustring.h>
#include "Server.h"
#include "Args.h"
#include "Database.h"

int main(int argc, char* argv[]) {
    Args args(argc, argv);

    // otherwise std::cout << Glib::ustring doesn't work
    //
    setlocale(LC_ALL,"");

    Database db;
    if (!args.mandarinDataPath.empty()) {
        db.load_mandarin_data(args.mandarinDataPath);
    }
    if (!args.cantoneseDataPath.empty()) {
        db.load_cantonese_data(args.cantoneseDataPath);
    }
    SinoparserServer server(args.listenHost ,args.listenPort , db);

    return 0;
}



