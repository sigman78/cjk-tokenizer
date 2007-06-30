#ifndef __CJK_TOKENIZER_H__
#define __CJK_TOKENIZER_H__

#include <string>
#include <vector>
#include <unicode.h>

namespace cjk {
    enum tokenizer_type {
        TOKENIZER_DEFAULT,
        TOKENIZER_UNIGRAM
    };
    
    class tokenizer {
        private:
        enum tokenizer_type _type;
        static inline void _convert_unicode_to_char(unicode_char_t &uchar,
                                                    unsigned char *p);
        public:
        tokenizer();
        tokenizer(enum tokenizer_type type);
        ~tokenizer();
        void tokenize(std::string &str,
                      std::vector<std::string> &token_list);
        void tokenize(char *buf, size_t buf_len,
                      std::vector<std::string> &token_list);
        void split(std::string &str,
                   std::vector<std::string> &token_list);
        void split(char *buf, size_t buf_len,
                   std::vector<std::string> &token_list);
    };
};

#endif
