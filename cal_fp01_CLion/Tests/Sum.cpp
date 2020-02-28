/*
 * Sum.cpp
 */

#include "Sum.h"
#include <chrono>

using namespace std;

string calcSum(int* sequence, int size)
{
    string result;



    for (int i=1;i<=size;i++)
    {
        int currentMin = 9999;
        int currentSum = 0;

        for (int i2=0;i2<i;i2++)
        {
            currentSum+=sequence[i2];
        }
        if (currentSum < currentMin)

    }


    return result;
}
