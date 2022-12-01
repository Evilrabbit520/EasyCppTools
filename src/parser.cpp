#include "parser.h"

using namespace ECT::json;


Parser::Parser() : m_str(""), m_idx(0)
{

}
void Parser::load(const std::string &str)
{
    m_str = str;
    m_idx = 0;
}

void Parser::skip_white_space()
{
    while(m_str[m_idx] == ' ' || m_str[m_idx] == '\n' || m_str[m_idx] == '\r' || m_str[m_idx] == '\t')
        m_idx++;
}

char Parser::get_next_token()
{
    skip_white_space();
    return m_str[m_idx++];
}

Json Parser::parse()
{
    switch (get_next_token())
    {
    case 'n':
        m_idx--;
        return parse_null();
    case 't':
    case 'f':
        m_idx--;
        return parse_bool();
    case '-':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return parse_number();
    case '"':
        return Json(pares_string());
    case '[':
        return pares_array();
    case '{':
        return pares_object();
    default:
        break;
    }
    throw new std::logic_error("unexpected char");
}

Json Parser::parse_null()
{
    if(m_str.compare(m_idx, 4, "null") == 0)
    {
        return Json();
    }
    throw new std::logic_error("paser 'null' error");
}
Json Parser::parse_bool()
{
    if(m_str.compare(m_idx, 4, "true") == 0)
    {
        return Json(true);
    }
    else if (m_str.compare(m_idx, 5, "false") == 0)
    {
        return Json(false);
    }
    throw new std::logic_error("paser bool error");
}
Json Parser::parse_number()
{
    int pos = m_idx;
    if(m_str[m_idx] == '-')
    {
        m_idx++;
    }
    if(m_str[m_idx] < '0' || m_str[m_idx] > '9')
    {
        throw new std::logic_error("paser number error");
    }
    while (m_str[m_idx] >= '0' && m_str[m_idx] <= '9')
    {
        m_idx++;
    }
    if(m_str[m_idx] != '.')
    {
        int i = std::atoi(m_str.c_str() + pos);
        return Json(i);
    }
    m_idx++;
    if(m_str[m_idx] < '0' || m_str[m_idx] > '9')
    {
        throw new std::logic_error("paser number error");
    }
    while (m_str[m_idx] >= '0' && m_str[m_idx] <= '9')
    {
        m_idx++;
    }
    double f = std::atof(m_str.c_str() + pos);
    return Json(f);
}
std::string Parser::pares_string()
{
    std::string out;
    while(true)
    {
        char ch = m_str[m_idx++];
        if(ch == '"')
        {
            break;
        }
        if(ch == '\\')
        {
            ch = m_str[m_idx++];
            switch (ch)
            {
            case '\n':
                out += '\n';
                break;
            case '\r':
                out += '\t';
                break;
            case '\t':
                out += '\t';
                break;
            case '\b':
                out += '\b';
                break;
            case '\f':
                out += '\f';
                break;
            case '"':
                out += "\\\"";
                break;
            case '\\':
                out += "\\\\";
                break;
            case 'u':
                out += "\\u";
                for(int i = 0; i < 4; i++)
                {
                    out += m_str[m_idx++];
                }
                break;
            default:
                break;
            }
        }
        else
        {
            out += ch;
        }
    }
    return out;
}
Json Parser::pares_array()
{
    Json arr(Json::json_array);
    char ch = get_next_token();
    if(ch == ']')
    {
        return arr;
    }
    m_idx--;
    while(true)
    {
        arr.append(parse());
        ch = get_next_token();
        if(ch == ']')
        {
            break;
        }
        if(ch == ',')
        {
            throw new std::logic_error("pares array error");
        }
        m_idx++;
    }
    return arr;
}
Json Parser::pares_object()
{
    return Json();
}