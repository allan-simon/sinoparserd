#include <vector>
#include <string>
#include <ostream>

/**
 * Use the first byte of a utf-8 character to determine how long
 * in byte this character will be,  return that size
 * or -1 if the first byte is a non valid utf-8 character first byte
 */
int character_octet_size (const unsigned char firstByte);

typedef std::vector<std::string> Segments; 

/**
 * segment a standard string into a vector of utf-8 character
 */
Segments create_new_utf8_string (std::string);


/**
 * Minimal String-like class to represent a utf-8 string
 * in order to work on a per-character basis rather than per byte
 */
class Utf8String {
    private:
        Segments segments;

    public:
        Utf8String();
        Utf8String(std::string);
        Utf8String(char*);
        Utf8String(const char*);

        /**
         * Return a standard string representation of the instance
         */
        std::string to_string() const;

        /**
         * Equivalent of std::string substr function but works on actual
         * characters rathen than bytes
         */
        std::string substr(size_t start, size_t size) const;

        /**
         * Get the size of the string, in term of number of actual character
         * not size in bytes
         */
        size_t size() const;
};

/**
 * Operator overloading so that we can print a Utf8String
 * using normal cout << like operations
 */
std::ostream& operator<< (std::ostream& stream, const Utf8String& utf8String);
