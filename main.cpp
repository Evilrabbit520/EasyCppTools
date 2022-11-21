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

using namespace ECT::json;

int main()
{
    Json v1;
    Json v2 = true;
    Json v3 = 123;
    Json v4 = 1.23;
    Json v5 = "hello word";
    return 0;
}