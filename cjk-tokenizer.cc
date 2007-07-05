#include <cassert>
#include <unicode.h>
#include "cjk-tokenizer.h"

// 2E80..2EFF; CJK Radicals Supplement
// 3000..303F; CJK Symbols and Punctuation
// 3040..309F; Hiragana
// 30A0..30FF; Katakana
// 3100..312F; Bopomofo
// 3130..318F; Hangul Compatibility Jamo
// 3190..319F; Kanbun
// 31A0..31BF; Bopomofo Extended
// 31C0..31EF; CJK Strokes
// 31F0..31FF; Katakana Phonetic Extensions
// 3200..32FF; Enclosed CJK Letters and Months
// 3300..33FF; CJK Compatibility
// 3400..4DBF; CJK Unified Ideographs Extension A
// 4DC0..4DFF; Yijing Hexagram Symbols
// 4E00..9FFF; CJK Unified Ideographs
// A700..A71F; Modifier Tone Letters
// AC00..D7AF; Hangul Syllables
// F900..FAFF; CJK Compatibility Ideographs
// FE30..FE4F; CJK Compatibility Forms
// FF00..FFEF; Halfwidth and Fullwidth Forms
// 20000..2A6DF; CJK Unified Ideographs Extension B
// 2F800..2FA1F; CJK Compatibility Ideographs Supplement
#define UTF8_IS_CJK(p)                                                  \
    (((p) >= 0x2E80 && (p) <= 0x2EFF)                                   \
     || ((p) >= 0x3000 && (p) <= 0x303F)                                \
     || ((p) >= 0x3040 && (p) <= 0x309F)                                \
     || ((p) >= 0x30A0 && (p) <= 0x30FF)                                \
     || ((p) >= 0x3100 && (p) <= 0x312F)                                \
     || ((p) >= 0x3130 && (p) <= 0x318F)                                \
     || ((p) >= 0x3190 && (p) <= 0x319F)                                \
     || ((p) >= 0x31A0 && (p) <= 0x31BF)                                \
     || ((p) >= 0x31C0 && (p) <= 0x31EF)                                \
     || ((p) >= 0x31F0 && (p) <= 0x31FF)                                \
     || ((p) >= 0x3200 && (p) <= 0x32FF)                                \
     || ((p) >= 0x3300 && (p) <= 0x33FF)                                \
     || ((p) >= 0x3400 && (p) <= 0x4DBF)                                \
     || ((p) >= 0x4DC0 && (p) <= 0x4DFF)                                \
     || ((p) >= 0x4E00 && (p) <= 0x9FFF)                                \
     || ((p) >= 0xA700 && (p) <= 0xA71F)                                \
     || ((p) >= 0xAC00 && (p) <= 0xD7AF)                                \
     || ((p) >= 0xF900 && (p) <= 0xFAFF)                                \
     || ((p) >= 0xFE30 && (p) <= 0xFE4F)                                \
     || ((p) >= 0xFF00 && (p) <= 0xFFEF)                                \
     || ((p) >= 0x20000 && (p) <= 0x2A6DF)                              \
     || ((p) >= 0x2F800 && (p) <= 0x2FA1F)                              \
     || ((p) >= 0x2F800 && (p) <= 0x2FA1F))

using namespace std;
using namespace cjk;

tokenizer::tokenizer() : ngram_size(2),
                         max_token_count(0) {
    unicode_init();
}

tokenizer::~tokenizer() {
}

inline
unsigned char* tokenizer::_unicode_to_char(unicode_char_t &uchar,
                                           unsigned char *p) {
    assert(p != NULL);
    memset(p, 0, sizeof(unicode_char_t) + 1);
    if (uchar < 0x80) {
        p[0] = uchar;
    }
    else if (uchar < 0x800) {
        p[0] = (0xC0 | uchar >> 6);
        p[1] = (0x80 | uchar & 0x3F);
    }
    else if (uchar < 0x10000) {
        p[0] = (0xE0 | uchar >> 12);
        p[1] = (0x80 | uchar >> 6 & 0x3F);
        p[2] = (0x80 | uchar & 0x3F);
    }
    else if (uchar < 0x200000) {
        p[0] = (0xF0 | uchar >> 18);
        p[1] = (0x80 | uchar >> 12 & 0x3F);
        p[2] = (0x80 | uchar >> 6 & 0x3F);
        p[3] = (0x80 | uchar & 0x3F);
    }
    return p;
}

void tokenizer::tokenize(const char *buf, size_t buf_len,
                         vector<string> &token_list) {
    assert(buf != NULL);
    string str = string(buf, buf_len);
    this->tokenize(str, token_list);
}

void tokenizer::tokenize(const string &str, vector<string> &token_list) {
    string token_str;
    vector<string> temp_token_list;
    vector<unicode_char_t> temp_uchar_list;

    this->split(str, temp_token_list);
    this->split(str, temp_uchar_list);

    for (unsigned int i = 0; i < temp_token_list.size();) {
        if (this->max_token_count > 0
            && token_list.size() >= this->max_token_count) {
            break;
        }
        token_str.clear();
        if (UTF8_IS_CJK(temp_uchar_list[i])) {
            for (unsigned int j = i; j < i + this->ngram_size; j++) {
                if (j == temp_token_list.size()) {
                    break;
                }
                if (UTF8_IS_CJK(temp_uchar_list[j])) {
                    token_str += temp_token_list[j];
                    token_list.push_back(token_str);
                }
            }
            i++;
        }
        else {
            unsigned int j = i;
            while (j < temp_token_list.size()) {
                unsigned char *p
                    = (unsigned char*) temp_token_list[j].c_str();
                if (*p == ' ' || UTF8_IS_CJK(temp_uchar_list[j])) {
                    j++;
                    break;
                }
                token_str += temp_token_list[j];
                j++;
            }
            i = j;
            if (this->max_token_count > 0
                && token_list.size() >= this->max_token_count) {
                break;
            }
            if(token_str.length() > 0) {
                token_list.push_back(token_str);
            }
        }
    }
}

void tokenizer::split(const char *buf, size_t buf_len, 
                      vector<string> &token_list) {
    assert(buf != NULL);
    string str = string(buf, buf_len);
    this->split(str, token_list);
}

void tokenizer::split(const string &str, vector<string> &token_list) {
    unicode_char_t uchar;
    char *str_ptr = (char*) str.c_str();
    int str_utf8_len = unicode_strlen(str_ptr, str.length());
    unsigned char p[sizeof(unicode_char_t) + 1];

    for (int i = 0; i < str_utf8_len; i++) {
        str_ptr = unicode_get_utf8((const char*) str_ptr, &uchar);
        if (str_ptr == NULL) {
            break;
        }

        token_list.push_back(string((const char*)_unicode_to_char(uchar, p)));
    }
}

void tokenizer::split(const char *buf, size_t buf_len, 
                      vector<unicode_char_t> &token_list) {
    assert(buf != NULL);
    string str = string(buf, buf_len);
    this->split(str, token_list);
}

void tokenizer::split(const string &str, vector<unicode_char_t> &token_list) {
    unicode_char_t uchar;
    char *str_ptr = (char*) str.c_str();
    int str_utf8_len = unicode_strlen(str_ptr, str.length());

    for (int i = 0; i < str_utf8_len; i++) {
        str_ptr = unicode_get_utf8((const char*) str_ptr, &uchar);
        if (str_ptr == NULL) {
            break;
        }
        token_list.push_back(uchar);
    }
}

bool tokenizer::has_cjk(const std::string &str) {
    vector<unicode_char_t> temp_uchar_list;

    this->split(str, temp_uchar_list);
    
    for (unsigned int i = 0; i < temp_uchar_list.size(); i++) {
        if (UTF8_IS_CJK(temp_uchar_list[i])) {
            return true;
        }
    }
    return false;
}

bool tokenizer::has_cjk(const char *buf, size_t buf_len) {
    assert(buf != NULL);
    string str = string(buf, buf_len);
    return this->has_cjk(str);
}

bool tokenizer::has_cjk_only(const std::string &str) {
    vector<unicode_char_t> temp_uchar_list;

    this->split(str, temp_uchar_list);
    
    for (unsigned int i = 0; i < temp_uchar_list.size(); i++) {
        if (!(UTF8_IS_CJK(temp_uchar_list[i]))) {
            return false;
        }
    }
    return true;
}

bool tokenizer::has_cjk_only(const char *buf, size_t buf_len) {
    assert(buf != NULL);
    string str = string(buf, buf_len);
    return this->has_cjk_only(str);
}
