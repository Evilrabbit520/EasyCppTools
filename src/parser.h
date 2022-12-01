#ifndef EASYCPPTOOLS_SRC_PARSER_H__
#define EASYCPPTOOLS_SRC_PARSER_H__

#include "json.h"
#include <string>

namespace ECT{
namespace json{

class Parser
{
private:
    //Data
    std::string m_str;
    int m_idx;
public:
    Parser(/* args */);
    void load(const std::string &str);
    Json parse();
    
private:
    /// @brief 忽略回车制表空格
    void skip_white_space();
    /// @brief 获取下个字符
    /// @return 下个字符索引
    char get_next_token();
private:
    Json parse_null();
    Json parse_bool();
    Json parse_number();
    std::string pares_string();
    Json pares_array();
    Json pares_object();
};


}}

#endif // EASYCPPTOOLS_SRC_PARSER_H__