#ifndef SINOPARSER_INDEX_H
#define SINOPARSER_INDEX_H

#include <iostream>
#include <map>
extern "C" {
#include "tato/TatoTreeStr.h"
}

/**
 * TODO to easily switch from TatoTreeStr or STL map
 * we should add an option in the make process
 *
 */

template <typename T>
class Index {
    private: 
        std::map<std::string, T*> store;
        //TatoTreeStr *store;

    public:
        void add_item(std::string, T* newItem);
        T* get_item(std::string key);
        void debug();

        Index();
        ~Index();

};

/**
 *
 */
template <typename T>
void Index<T>::add_item(std::string key, T* newItem) {
    store.insert(std::pair<std::string, T*>(key,newItem));
/*
    if( !tato_tree_str_insert_unique(store, key.c_str(), newItem)) {
        std::cout << "    [NOTICE] duplicate key: " << key << std::endl;
        return;
    };
    tato_tree_str_debug(store);
*/
}

/**
 *
 */
template <typename T>
T* Index<T>::get_item(std::string key) {
    
    typename std::map<std::string, T*>::iterator it =  store.find(key);
    if (it != store.end()) {
        return it->second;
    } else {
        return NULL;
    }
    
    //return (T*) tato_tree_str_find(store, key.c_str());
}

/**
 *
 */
template <typename T>
void Index<T>::debug() {
    //std::cout << "[DEBUG] size: " << tato_tree_str_size(store) << std::endl;
    //tato_tree_str_debug(store);
}


/**
 *
 */
template <typename T>
Index<T>::Index() {
   // store = tato_tree_str_new();
}
/**
 *
 */
template <typename T>
Index<T>::~Index() {
   // tato_tree_str_free(store);
}
#endif
