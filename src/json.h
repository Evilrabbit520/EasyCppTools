/**
 * @file json.h
 * @author Evilrabbit (evilrabbit520@gmail.com)
 * @brief json header file
 * @version 1.0.0
 * @date 2022-11-21
 * 
 * @copyright Copyright (c) 2021-2022 Evilrabbit. All rights reserved.
 * 
 */

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

    /// @brief C object overloading
    /// @param key 
    /// @return 
    Json &operator [](const char *key);
    /// @brief C++ object overloading
    /// @param key 
    /// @return 
    Json &operator [](const std::string &key);

    void operator = (const Json &other);
    bool operator == (const Json &other);
    bool operator != (const Json &other);

    std::string str() const;

    void PublicCopy(const Json &other);
    /// @brief dynamic memory release
    void clear();

    // array traversal
    typedef std::vector<Json>::iterator iterator;
    iterator begin()
    {
        return m_value.m_array->begin();
    }
    iterator end()
    {
        return m_value.m_array->end();
    }

    bool isNull() const
    {
        return m_type == json_null;
    }
    bool isBool() const
    {
        return m_type == json_bool;
    }
    bool isInt() const
    {
        return m_type == json_int;
    }
    bool isDouble() const
    {
        return m_type == json_double;
    }
    bool isString() const
    {
        return m_type == json_string;
    }
    bool isArray() const
    {
        return m_type == json_array;
    }
    bool isObject() const
    {
        return m_type == json_object;
    }

    // type conversion
    bool asBool() const;
    int asInt() const;
    double asDouble() const;
    std::string asString() const;

    //determine array index
    bool has(int index);
    bool has(const char *key);
    bool has(const std::string &key);

    void remove(int index);
    void remove(const char *key);
    void remove(const std::string &key);

    void parse(const std::string & str);

private:
    Type m_type;
    Value m_value;
};


}}

#endif //EASYCPPTOOLS_SRC_JSON_H__