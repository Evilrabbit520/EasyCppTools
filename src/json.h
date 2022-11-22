#ifndef EASYCPPTOOLS_SRC_JSON_H__
#define EASYCPPTOOLS_SRC_JSON_H__

#include <string>
#include <vector>
#include <map>

namespace ECT{
namespace json{

class Json
{
private:
    union Value
    {
        bool m_bool;
        int m_int;
        double m_double;
        std::string *m_string;
        std::vector<Json> *m_array;
        std::map<std::string, Json> *m_object;
    };
    
public:
    enum Type
    {
        json_null = 0,
        json_bool,
        json_int,
        json_double,
        json_string,
        json_array,
        json_object
    };

    Json();
    Json(bool value);
    Json(int value);
    Json(double value);
    Json(const char* value);    // Compatible with C
    Json(const std::string & value);
    Json(Type type);
    Json(const Json & other);

    // operator overloading
    operator bool();
    operator int();
    operator double();
    operator std::string();

    Json &operator [](int index);
    void append(const Json & other);

    std::string str() const;

private:
    Type m_type;
    Value m_value;
};


}}

#endif //EASYCPPTOOLS_SRC_JSON_H__