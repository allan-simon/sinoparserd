#include "Database.h"
#include "DataParser.h"
#include <ctime>

/**
 *
 */
Database::Database() {
}

/**
 *
 *
 */
Database::~Database() {

}

/**
 *
 */
void Database::load_mandarin_data(std::string dataPath) {
    DataParser parser(mandarin_words);
    parser.read(dataPath);
}
/**
 *
 */
void Database::load_cantonese_data(std::string dataPath) {
    DataParser parser(cantonese_words);
    parser.read(dataPath);
}


/**
 *
 */
void Database::debug() {
    mandarin_words.debug();
    cantonese_words.debug();
}

