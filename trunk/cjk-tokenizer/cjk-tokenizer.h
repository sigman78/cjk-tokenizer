#ifndef __CJK_TOKENIZER_H__
#define __CJK_TOKENIZER_H__

#include <string>
#include <vector>
#include <unicode.h>

namespace cjk {
    class tokenizer_handler {
    public:
        virtual void handle_token(const std::string &tok, bool is_cjk) {}
    };

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
        void tokenize(const std::string &str,
                      tokenizer_handler &handler);

        void split(const std::string &str,
                   std::vector<std::string> &token_list);
        void split(const std::string &str,
                   std::vector<unicode_char_t> &token_list);

        void segment(std::string str,
                     std::vector<std::string> &token_segment);

        bool has_cjk(const std::string &str);
        bool has_cjk_only(const std::string &str);
    };
};

#endif
