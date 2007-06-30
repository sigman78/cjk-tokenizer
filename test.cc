#include <iostream>
#include "cjk-tokenizer.h"

using namespace std;
using namespace cjk;

int main() {
    tokenizer tknzr;
    tokenizer tknzr2(TOKENIZER_UNIGRAM);
    vector<string> token_list;
    vector<string>::iterator token_iter;
    char text[] =
        "美女遊戲等你挑戰周蕙最新鈴搶先下載茄子醬耍可愛一流"
        "Giant Microwave Turns Plastic Back to Oil";
    string text_str = string(text);

    tknzr.tokenize(text_str, token_list);
    cout << "Original string: " << text << endl;
    cout << "Tokenized result: ";
    for (token_iter = token_list.begin();
         token_iter != token_list.end(); token_iter++) {
        cout << "[" << *token_iter << "] ";
    }
    cout << endl << endl;

    token_list.clear();
    tknzr2.tokenize(text_str, token_list);
    cout << "Original string: " << text << endl;
    cout << "Tokenized result: ";
    for (token_iter = token_list.begin();
         token_iter != token_list.end(); token_iter++) {
        cout << "[" << *token_iter << "] ";
    }
    cout << endl << endl;

    token_list.clear();
    tknzr.split(text_str, token_list);
    cout << "Original string: " << text << endl;
    cout << "Tokenized result: ";
    for (token_iter = token_list.begin();
         token_iter != token_list.end(); token_iter++) {
        cout << "[" << *token_iter << "] ";
    }
    cout << endl << endl;

    return 0;
}
