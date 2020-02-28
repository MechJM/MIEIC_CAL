/*
 * Sum.cpp
 */

#include "Sum.h"
#include <chrono>
#include <vector>
#include <iostream>

using namespace std;

string calcSum(int* sequence, int size)
{
    string result;

    int sums[size][size];

    for (int m=1;m<=size;m++)
    {
        for (int i=0;i<size;i++)
        {
            if (m==1) sums[m][i] = sequence[i];
            else sums[m][i] = sums[m-1][i] + sums[1][m+i-1];
        }
    }




	return result;
}

