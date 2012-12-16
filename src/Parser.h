#ifndef SINOPARSER_PARSER_H
#define SINOPARSER_PARSER_H

#include <glibmm/ustring.h>
#include <vector>
#include <map>
#include "Index.h"
#include "Database.h"

#define TRADITIONAL_SCRIPT true
#define SIMPLIFIED_SCRIPT false


template <typename T>
class Parser{
    private:
        Glib::ustring text_to_parse;
        std::vector<Glib::ustring> segments;
        std::vector<T*> items;
        std::map<Glib::ustring, Glib::ustring> convertChinese2Latin;
        std::map<Glib::ustring, Glib::ustring> convertLatin2Chinese;

        inline void init_maps();

        Glib::ustring convert_trash_segment(Glib::ustring segment, bool toLatin);
        Glib::ustring convert_trash_char(Glib::ustring trashChar, bool toLatin);


        inline std::string romanize_segment(int segmentNbr);
        inline std::string trad_segment(int segmentNbr);
        inline std::string simp_segment(int segmentNbr);

    public:
        Parser();
        Parser(Glib::ustring text);
        Parser(char* text);

        void change_text(char* text); 

        std::string segmented_text();
        std::string romanize();
        std::string trad();
        std::string simp();
        std::string change_script();
        std::vector<Glib::ustring> get_segments();

        bool guess_script();
        void parse_against_index(Index<T>& index);
        //Parser(std::string text);

        void debug();

        virtual ~Parser();
};



// we will not  try to find words larger than this
#define WINDOW 20

/**
 *
 */
template <typename T>
Parser<T>::Parser() {
    text_to_parse = "";
    init_maps();
}


/**
 * 
 */
template <typename T>
Parser<T>::Parser(Glib::ustring text) {
    text_to_parse = text;
    init_maps();
}

/**
 * 
 */
template <typename T>
Parser<T>::Parser(char* text) {
    text_to_parse = Glib::ustring(text);
    init_maps();

}
/**
 *
 *
 */
template <typename T>
inline void Parser<T>::init_maps() {
    convertChinese2Latin.insert(std::pair<Glib::ustring, Glib::ustring>("。","."));
    convertChinese2Latin.insert(std::pair<Glib::ustring, Glib::ustring>("、",","));
    convertChinese2Latin.insert(std::pair<Glib::ustring, Glib::ustring>("？","?"));
    convertChinese2Latin.insert(std::pair<Glib::ustring, Glib::ustring>("，",","));
    convertChinese2Latin.insert(std::pair<Glib::ustring, Glib::ustring>("！","!"));
    convertChinese2Latin.insert(std::pair<Glib::ustring, Glib::ustring>("；",";"));
    convertChinese2Latin.insert(std::pair<Glib::ustring, Glib::ustring>("：",":"));
    convertChinese2Latin.insert(std::pair<Glib::ustring, Glib::ustring>("‘","'"));
    convertChinese2Latin.insert(std::pair<Glib::ustring, Glib::ustring>("“","\""));
    convertChinese2Latin.insert(std::pair<Glib::ustring, Glib::ustring>("”","\""));

    convertLatin2Chinese.insert(std::pair<Glib::ustring, Glib::ustring>(".","。"));
    convertLatin2Chinese.insert(std::pair<Glib::ustring, Glib::ustring>("?","？"));
    convertLatin2Chinese.insert(std::pair<Glib::ustring, Glib::ustring>(",","，"));
    convertLatin2Chinese.insert(std::pair<Glib::ustring, Glib::ustring>("!","！"));
    convertLatin2Chinese.insert(std::pair<Glib::ustring, Glib::ustring>(";","；"));
    convertLatin2Chinese.insert(std::pair<Glib::ustring, Glib::ustring>(":","："));
    convertLatin2Chinese.insert(std::pair<Glib::ustring, Glib::ustring>("\"","“"));


}

/**
 * 
 */

template <typename T>
void Parser<T>::change_text(char* text) {
    text_to_parse = Glib::ustring(text);
    segments.clear();
    items.clear();
}
/**
 *
 */
template <typename T>
Parser<T>::~Parser() {

}

/**
 * Print the internal state of the current Parser instance
 * for debugging purpose only
 */
template <typename T>
void Parser<T>::debug() {
    std::cout << "[DEBUG] " << text_to_parse << std::endl; 
}


/**
 *
 */

template <typename T>
void Parser<T>::parse_against_index(Index<T> & index) {


    int startPosition = 0;

    // algorithm is the following 
    // 1 - from start try to find the longest word present in the database
    // then go at the end of this word and start again etc.
    // if during the moment the first character is unknown in the database
    // then we consider we're facing non-chinese words stuff (latin name, 
    // punctuation, random trash ...) in this case we will try to make the 
    // longest block of such characters and we will continue normally as
    // soon as we encounter a character we known (AKA a chinese character)


    // TODO we can make it faster by splitting the text on ponctuation 
    // TODO we can maybe make it more efficient (but slower) 
    // by using a bit more complex algorithm
    //    list all the possible block (regarless to what's before and what's 
    //    next) then make a graph where each node will be the single characters
    //    the edge will be the words (if we have the sentence ABCDEFG and a
    //    word BCDE then we will have the edge B->E
    //
    //    by playing with the weigth of each edge and with a shortest/longest
    //    path algo (DANTZIG ?) we can determine the most accurate segmentation   

    // NOTE : the current algorithm will not be able to find mixed script words
    // for example it will not consider 谢謝 as a single word (but it's a bit 
    // borderline :p)

    while (startPosition < text_to_parse.size()) {
        Glib::ustring longestMatchBlock = "";
        T* longestMatchItem = NULL;

        std::string tempBlock = text_to_parse.substr(startPosition,1);

        T* tempItem  = index.get_item(tempBlock);

        if (tempItem != NULL) {
            // normal mode try to find the longest word starting 
            // at the end of the previous one
            for (int i = 1; i  <= MIN(text_to_parse.size()-startPosition, WINDOW) ; i++) { 
                
                tempBlock = text_to_parse.substr(startPosition,i);

                tempItem  = index.get_item(tempBlock);

                // if we add one more character the word has no more meaning
                // so we consider it as the current longest one
                if (tempItem != NULL) {
                    longestMatchBlock = tempBlock;
                    longestMatchItem  = tempItem;
                }
            }

        } else {
            // trash mode try to find the longest block of contiguous 
            // unknown characters
            for (int i = startPosition ; i < text_to_parse.size(); i++) { 
                
                tempBlock = text_to_parse.substr(i,1);

                tempItem  = index.get_item(tempBlock);

                // if we known the last characters then we can start
                // normal mode again
                if (tempItem != NULL) {
                    break;
                }

                longestMatchBlock += tempBlock;
                longestMatchItem  = tempItem;
            }
        }

        segments.push_back(longestMatchBlock); 
        items.push_back(longestMatchItem);

        startPosition += longestMatchBlock.size();
    }
   
}

/**
 *
 *
 */

template <typename T>
std::string Parser<T>::segmented_text() {
    
    std::string temp = "";
    int size = segments.size();
   
    if (size < 1) {
        return "";    
    }
    temp += segments.at(0);

    for (int i = 1; i < size ; i++) {
        temp += " " + segments[i];
    }
    return temp;
}


/**
 *
 */
template <typename T>
std::string Parser<T>::romanize() {

    std::string temp = "";
    int size = segments.size();
    if (size < 1) {
        return temp;    
    }

    temp += romanize_segment(0);
    for (int i = 1; i < size ; i++) {
        temp += " " ;
        temp += romanize_segment(i);
    }
    return temp;
}

/**
 *
 */
template <typename T>
inline std::string Parser<T>::romanize_segment(int segmentNbr) {
    std::string temp = "";
    T* tempItem = items[segmentNbr];
    
    if (tempItem != NULL) {
        temp += tempItem->romanization;
    } else {
        temp += convert_trash_segment(segments[segmentNbr], true);
    }
    return temp;
}



/**
 *
 */
template <typename T>
std::string Parser<T>::trad() {

    int size = segments.size();
    if (size < 1) {
        return "";    
    }

    std::string temp = "";

    for (int i = 0; i < size ; i++) {
        temp += trad_segment(i);
    }
    return temp;
}

/**
 *
 */
template <typename T>
inline std::string Parser<T>::trad_segment(int segmentNbr) {
    MandarinItem *tempItem = items[segmentNbr];
    std::string temp = "";
    if (tempItem != NULL) {
        if (tempItem->type == SINOPARSER_TRADITIONAL) {
            temp += segments[segmentNbr];
        } else {
            temp += tempItem->str_other_script;
        }
    } else {
        temp += convert_trash_segment(segments[segmentNbr], false);
    }
    return temp;
}


/**
 *
 */

template <typename T>
std::string Parser<T>::simp() {

    int size = segments.size();
    if (size < 1) {
        return "";    
    }
    std::string temp = "";
    for (int i = 0; i < size ; i++) {
        temp += simp_segment(i);
    }
    return temp;
}

/**
 *
 */
template <typename T>
inline std::string Parser<T>::simp_segment(int segmentNbr) {
    MandarinItem *tempItem = items[segmentNbr];
   
    std::string temp = ""; 
    if (tempItem != NULL) {
        if (tempItem->type == SINOPARSER_SIMPLIFIED) {
            temp += segments[segmentNbr];
        } else {
            temp += tempItem->str_other_script;
        }
    } else {
        temp+= convert_trash_segment(segments[segmentNbr], false);
    }
    return temp;
}


/**
 *
 *
 * TODO: can be done during parsing
 */

template <typename T>
bool Parser<T>::guess_script() {
    int size = segments.size();
    int nbrSimp = 0;
    int nbrTrad = 0;
    if (size < 1) {
        return SIMPLIFIED_SCRIPT;    
    }

    for (int i = 0; i < size ; i++) {
        MandarinItem *tempItem = items[i];
    
        if (tempItem != NULL) {
            if (tempItem->type == SINOPARSER_SIMPLIFIED) {
                nbrSimp += segments[i].size();
            } else if (tempItem->type == SINOPARSER_TRADITIONAL) {
                nbrTrad += segments[i].size();
            }
        }
    }
    // if we have more traditional chinese written words then
    // we consider the sentence as mainly traditional
    // as simplified otherwise

    if (nbrTrad > nbrSimp) {
        return TRADITIONAL_SCRIPT;
    } else {
        return SIMPLIFIED_SCRIPT;
    }

}

/**
 *
 */

template <typename T>
std::string Parser<T>::change_script() {

    if (guess_script() == TRADITIONAL_SCRIPT) {
        return simp();
    } else {
        return trad();
    }
}

/**
 *
 *
 *
 */

template <typename T>
Glib::ustring Parser<T>::convert_trash_segment(Glib::ustring segment, bool toLatin) {
    Glib::ustring temp("");
    for (int i = 0; i < segment.size() ; i++) {
        temp += convert_trash_char(segment.substr(i,1), toLatin); 
    }
    return temp;
}


/**
 *
 */

template <typename T>
Glib::ustring Parser<T>::convert_trash_char(Glib::ustring trashChar, bool toLatin) {

    std::map<Glib::ustring, Glib::ustring>::iterator iter;
    std::map<Glib::ustring, Glib::ustring> convertMap ;

    if (toLatin) {
        convertMap = convertChinese2Latin;
    } else {
        convertMap = convertLatin2Chinese;
    }
    
    iter = convertMap.find(trashChar);

    if (iter != convertMap.end()) {
        return iter->second;
    } else {
        return trashChar;
    }
}

/**
 *
 *
 */

template <typename T>
std::vector<Glib::ustring> Parser<T>::get_segments() {
    return segments;
}


#endif
