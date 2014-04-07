#include <iostream>
#include "Utf8String.h"


/**
 *
 */
int character_octet_size (const unsigned char firstByte) {

    // lead bit is zero, must be a single ascii
    if ((firstByte & 0x80 ) == 0 ) {
        return 1; 
    }

    // 110x xxxx
    if ((firstByte & 0xE0 ) == 0xC0 ) {
        return 2;
    }
    // 1110 xxxx
    if ((firstByte & 0xF0 ) == 0xE0 ) {
        return 3;
    }
    // 1111 0xxx
    if ((firstByte & 0xF8 ) == 0xF0 ) {
        return 4;
    }
    
    // Unrecognized lead byte (%02x)\n", firstByte );
    return -1;
}

/**
 *
 */
Segments create_new_utf8_string (std::string stringToSegment) {

    std::string utf8Character;
    //4 because that's the max size in byte of a utf-8 character
    utf8Character.reserve(4);
    Segments segmentedLine;

    int characterSize = 0;
    for (int i = 0; i < stringToSegment.size(); i++) {
        unsigned char byte = stringToSegment[i];
        //if we we have finished previous utf8 character
        //we take the size of the new one
        if (characterSize <= 0) {
            characterSize = character_octet_size(byte);
        }

        //we push current byte in current utf-8 character
        //we're building
        utf8Character.push_back(byte);
        characterSize--;

        //if we have consumed all bytes of current utf-8 character
        //it is now ready to be added to the list of utf-8 characters
        if (characterSize <= 0) {
            segmentedLine.push_back(utf8Character);
            segment.clear();
        }
    }

    return segmentedLine;
};


/**
 *
 */
Utf8String::Utf8String() {

};

/**
 *
 */
Utf8String::Utf8String(std::string stringToSegment) {
    segments = create_new_utf8_string(stringToSegment);
};

/**
 *
 */
Utf8String::Utf8String(char* charToSegment) {
    //it should be possible to have something a little more efficient
    //by not using an intermediate string, but rather iterating on the array
    //of char until we met a \0
    std::string stringToSegment(charToSegment);
    segments = create_new_utf8_string(stringToSegment);
};

/**
 *
 */
Utf8String::Utf8String(const char* charToSegment) {
    //it should be possible to have something a little more efficient
    //by not using an intermediate string, but rather iterating on the array
    //of char until we met a \0
    std::string stringToSegment(charToSegment);
    segments = create_new_utf8_string(stringToSegment);
};

/**
 *
 */
std::string Utf8String::to_string() const {
    std::string toOutput; 
    for (int i = 0; i < segments.size(); i++) {
        toOutput += (segments[i]);
    }

    return toOutput;
};

/**
 *
 */
size_t Utf8String::size() const {
    return segments.size();
}


/**
 *
 */
std::string Utf8String::substr(size_t start, size_t size) const {
    std::string toOutput; 
    for (int i = 0; i < size; i++) {
        toOutput += (segments[start + i]);
    }
    return toOutput;
}


/**
 *
 */
std::ostream& operator<< (std::ostream& stream, const Utf8String& utf8String) { 
    stream << utf8String.to_string();
}
