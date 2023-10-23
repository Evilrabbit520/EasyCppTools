/**
 * @file DataSorting.h
 * @author Evilrabbit (evilrabbit520@gmail.com)
 * @brief Several commonly used sorting.
 * @version 1.0.0
 * @date 2023-10-06
 *
 * @copyright Copyright (c) 2021-2023 Evilrabbit. All rights reserved.
 *
 */


#ifndef EASYCPPTOOLS_SRC_DATASORTING_H__
#define EASYCPPTOOLS_SRC_DATASORTING_H__

#include <iostream>
#include <string>
#include <vector>

namespace ECT{

class Sorting
{
private:
    template<typename T>
    void printData(std::vector<T> arr)
    {
        //print arry
        for (auto item : arr)
        {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }
public:
    Sorting(/* args */);

    ~Sorting();
public:
    template<typename T>
    void bubbleSort(std::vector<T>& arrVec)
    {
        int arrSize = arrVec.size();
        for(int i = 0; i < arrSize - 1; ++i)
        {
            for(int j = 0; j < arrSize - i - 1; ++j)
            {
                if(arrVec[j] > arrVec[j + 1])
                {
                    std::swap(arrVec[j], arrVec[j + 1]);
                }
            }
        }
        printData(arrVec);
    }

    template<typename T>
    void selectSort(std::vector<T>& arrVec)
    {
        int arrSize = arrVec.size();
        for(int i = 0; i < arrSize - 1; i++)
        {
            int index = 0;
            for(int j = 1; j < arrSize - i; j++)
            {
                if(arrVec[j] > arrVec[index])
                {
                    index = j;
                }
            }
            std::swap(arrVec[index], arrVec[arrSize - 1 - i]);
        }
        printData(arrVec);
    }

};

}

#endif // EASYCPPTOOLS_SRC_DATASORTING_H__