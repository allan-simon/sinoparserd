#include "MandarinItem.h"

MandarinItem::MandarinItem() {
    id = 0;
    type = SINOPARSER_MIXED;
    str_other_script = "";
    romanization = "";
}

MandarinItem::MandarinItem(int newid, std::string pinyin) {
    id = newid;
    type = SINOPARSER_MIXED;
    str_other_script = "";
    romanization = pinyin;
}

MandarinItem::MandarinItem(int newid, int newtype, std::string new_str_other_script, std::string pinyin) {
    id = newid;
    type = newtype;
    str_other_script = new_str_other_script;
    romanization = pinyin;

}

MandarinItem::~MandarinItem() {

}

void MandarinItem::debug() {

    std::cout << "[DEBUG] id: " << id << std::endl;
    std::cout << "[DEBUG] pinyin: " << romanization << std::endl;
    std::cout << "[DEBUG] otherscript: " << str_other_script << std::endl;
}
