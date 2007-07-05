#ifndef __CJK_TOKENIZER_H__
#define __CJK_TOKENIZER_H__

#include <string>
#include <vector>
#include <unicode.h>

namespace cjk {
    class tokenizer {
        private:
        static inline void _convert_unicode_to_char(unicode_char_t &uchar,
                                                    unsigned char *p);
        public:
        unsigned int ngram_size;
        unsigned int max_token_count;
        tokenizer();
        ~tokenizer();
        void tokenize(std::string &str,
                      std::vector<std::string> &token_list);
        void tokenize(char *buf, size_t buf_len,
                      std::vector<std::string> &token_list);
        void split(std::string &str,
                   std::vector<std::string> &token_list);
        void split(char *buf, size_t buf_len,
                   std::vector<std::string> &token_list);
        void split(std::string &str,
                   std::vector<unicode_char_t> &token_list);
        void split(char *buf, size_t buf_len,
                   std::vector<unicode_char_t> &token_list);
    };
};

#endif
