#ifndef __CJK_TOKENIZER_H__
#define __CJK_TOKENIZER_H__

#include <string>
#include <vector>
#include <unicode.h>

namespace cjk {
    class tokenizer {
        private:
        static inline unsigned char* _unicode_to_char(unicode_char_t &uchar,
                                                      unsigned char *p);
        public:
        unsigned int ngram_size;
        unsigned int max_token_count;
        tokenizer();
        ~tokenizer();
        void tokenize(const std::string &str,
                      std::vector<std::string> &token_list);
        void tokenize(const char *buf, size_t buf_len,
                      std::vector<std::string> &token_list);
        void split(const std::string &str,
                   std::vector<std::string> &token_list);
        void split(const char *buf, size_t buf_len,
                   std::vector<std::string> &token_list);
        void split(const std::string &str,
                   std::vector<unicode_char_t> &token_list);
        void split(const char *buf, size_t buf_len,
                   std::vector<unicode_char_t> &token_list);
        bool has_cjk(const std::string &str);
        bool has_cjk(const char *buf, size_t buf_len);
        bool has_cjk_only(const std::string &str);
        bool has_cjk_only(const char *buf, size_t buf_len);
    };
};

#endif
