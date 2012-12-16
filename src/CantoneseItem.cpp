#include "CantoneseItem.h"

/**
 *
 */
CantoneseItem::CantoneseItem(int nid, std::string jyutping) {
    id = nid;
    romanization = jyutping;
}
/**
 *
 */
CantoneseItem::~CantoneseItem() {

}

/**
 *
 */
void CantoneseItem::debug() {
    std::cout << "[DEBUG] id: " << id << std::endl;
    std::cout << "[DEBUG] jyutping: " << romanization << std::endl;

}
