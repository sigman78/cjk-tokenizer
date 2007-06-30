#include <cassert>
#include "unicode.h"
#include "cjk-tokenizer.h"

#define UTF8_TAIL(p)                            \
    ((p) != NULL                                \
     && *(p) >= 0x80 && *(p) <= 0xBF)

#define UTF8_1(p)                               \
    ((p) != NULL                                \
     && *(p) && *(p) <= 0x7F)

#define UTF8_2(p)                               \
    ((p) != NULL && (p+1) != NULL               \
     && (*(p) >= 0xC2 && *(p) <= 0xDF )         \
     && (p+1) != NULL                           \
     && UTF8_TAIL(p+1))

#define UTF8_3(p)                                       \
    ((p) != NULL && (p+1) != NULL && (p+2) != NULL      \
     && ((*(p) == 0xE0                                  \
          && (*(p+1) >= 0xA0 && *(p+1) <= 0xBF )        \
          && UTF8_TAIL(p+2))                            \
         || ((*(p) == 0xED                              \
              && (*(p+1) >= 0x80 && *(p+1) <= 0x9F )    \
              && UTF8_TAIL(p+2))                        \
             || ((*(p) >= 0xE1 && *(p) <= 0xEC)         \
                 && UTF8_TAIL(p+1)                      \
                 && UTF8_TAIL(p+2))                     \
             || ((*(p) >= 0xEE && *(p) <= 0xEF)         \
                 && UTF8_TAIL(p+1)                      \
                 && UTF8_TAIL(p+2)))))

#define UTF8_4(p)                                       \
    ((p) != NULL && (p+1) != NULL && (p+2) != NULL      \
     && (p+3) != NULL                                   \
     && (((*(p) == 0xF0                                    \
           && (*(p+1) >= 0x90 && *(p+1) <= 0xBF )           \
           && UTF8_TAIL(p+2))                               \
          && UTF8_TAIL(p+3))                             \
         || ((*(p) >= 0xF1 && *(p) <= 0xF3)                 \
             && UTF8_TAIL(p+1)                              \
             && UTF8_TAIL(p+2)                              \
             && UTF8_TAIL(p+3))                             \
         || ((*(p) == 0xF4                                  \
              && (*(p+1) >= 0x80 && *(p+1) <= 0x8F )        \
              && UTF8_TAIL(p+2))                            \
             && UTF8_TAIL(p+3))))                           \

#define UTF8_IS_CJK(p)                                  \
    ((p) != NULL && (p+1) != NULL && (p+2) != NULL      \
     && (*(p) >= 0xE4 && *(p) <= 0xE9)                  \
     && (*(p+1) >= 0x80 && *(p+1) <= 0xBF)              \
     && (*(p+2) >= 0x80 && *(p+2) <= 0xBF))

#define UTF8_IS_CJK_PUNCT(p)                            \
    ((p) != NULL && (p+1) != NULL && (p+2) != NULL      \
     && (*(p) >= 0xE4 && *(p) <= 0xE9)                  \
     && (*(p+1) >= 0x95 && *(p+1) <= 0xBF)              \
     && (*(p+2) >= 0x80 && *(p+2) <= 0xBF))             

using namespace std;
using namespace cjk;

tokenizer::tokenizer() {
    unicode_init();
}

tokenizer::tokenizer(enum tokenizer_type type) : _type(type) {
    unicode_init();
}

tokenizer::~tokenizer() {
}

inline
void tokenizer::_convert_unicode_to_char(unicode_char_t &uchar,
                                         unsigned char *p) {
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
        p[0] = (0x80 | uchar >> 12 & 0x3F);
        p[0] = (0x80 | uchar >> 6 & 0x3F);
        p[0] = (0x80 | uchar & 0x3F);
    }
}

void tokenizer::tokenize(char *buf, size_t buf_len,
                         vector<string> &token_list) {
    assert(buf != NULL);
    string str = string(buf, buf_len);
    this->tokenize(str, token_list);
}

void tokenizer::tokenize(string &str, vector<string> &token_list) {
    string token_str;
    vector<string> temp_token_list;

    this->split(str, temp_token_list);

    for (unsigned int i = 0; i < temp_token_list.size();) {
        token_str.clear();
        if (UTF8_IS_CJK((unsigned char*) temp_token_list[i].c_str())) {
            token_list.push_back(temp_token_list[i]);
            if (this->_type != TOKENIZER_UNIGRAM
                && i + 1 < temp_token_list.size()) {
                unsigned char *p
                    = (unsigned char*) temp_token_list[i+1].c_str();
                if (UTF8_IS_CJK(p)) {
                    token_list.push_back(temp_token_list[i]
                                         + temp_token_list[i+1]);
                }
            }
            i++;
        }
        else {
            unsigned int j = i;
            while (j < temp_token_list.size()) {
                unsigned char *p
                    = (unsigned char*) temp_token_list[j].c_str();
                if (*p == ' ' || UTF8_IS_CJK(p)) {
                    j++;
                    break;
                }
                token_str += temp_token_list[j];
                j++;
            }
            i = j;
            token_list.push_back(token_str);
        }
    }
}

void tokenizer::split(char *buf, size_t buf_len, 
                      vector<string> &token_list) {
    assert(buf != NULL);
    string str = string(buf, buf_len);
    this->split(str, token_list);
}

void tokenizer::split(string &str, vector<string> &token_list) {
    unicode_char_t uchar;
    char *str_ptr = (char*) str.c_str();
    int str_utf8_len = unicode_strlen(str_ptr, str.length());
    unsigned char p[sizeof(unicode_char_t) + 1];

    for (int i = 0; i < str_utf8_len; i++) {
        str_ptr = unicode_get_utf8((const char*) str_ptr, &uchar);
        if (str_ptr == NULL) {
            break;
        }
        _convert_unicode_to_char(uchar, p);
        token_list.push_back(string((const char*) p));
    }
}
