/**
 * @file main.cpp
 * @author Evilrabbit (evilrabbit520@gmail.com)
 * @brief main file
 * @version 1.0.0
 * @date 2022-11-21
 * 
 * @copyright Copyright (c) 2021-2022 Evilrabbit. All rights reserved.
 * 
 */
#include <iostream>
#include "src/json.h"
#include <fstream>
#include <sstream>
#include "src/md5.h"

using namespace ECT::json;
using namespace Hash;

int main()
{
    // Json v1;
    // Json v2 = true;
    // Json v3 = 123;
    // Json v4 = 1.23;
    // Json v5 = "hello word";

    // bool b = v2;
    // int i = v3;
    // double f = v4;
    // const std::string &s = v5;

    // Json arr;
    // arr[0] = true;
    // arr[1] = 123;

    // arr.append(1.23);
    // arr.append("hello word");

    // bool b = arr[0];
    // int i = arr[1];
    // double f = arr[2];
    // const std::string &s = arr[3];

    // std::cout << arr.str() << std::endl;

    // Json object;
    // object["bool"] = true;
    // object["int"] = 123;
    // object["double"] = 1.23;
    // object["string"] = "hello word";

    //std::cout << object.has("string") << std::endl;
    // std::cout << arr.has(3) << std::endl;

    //object.remove("string");
    // std::cout << object.str() << std::endl;
    // object.clear();

    std::ifstream file("./test/test.json");
    std::stringstream ss;
    ss << file.rdbuf();
    const std::string &str = ss.str();

    Json v;   
    v.parse(str);

    std::string name = v["data"]["name"];

    std::cout << name << std::endl;

    MD5 md5 = MD5(name); 
    std::cout << md5.hexdigest() << std::endl;


    return 0;
}