/*
* Change.cpp
*/

#include "Change.h"

string calcChange(int m, int numCoins, int *coinValues)
{
    string result;

    if (m==0) return "";

    int rows=numCoins+1,cols=m+1;

    int minCoins[rows][cols];
    int lastCoin[rows][cols];

    int mod_coinValues[rows];
    mod_coinValues[0]=0;
    for (int i=1;i<rows;i++)
    {
        mod_coinValues[i]=coinValues[i-1];
    }

    for (int i=0;i<rows;i++)
    {
        for (int k=0;k<cols;k++)
        {
            if (k==0)
            {
                minCoins[i][k] = 0;
                lastCoin[i][k] = 0;
            }
            else if (mod_coinValues[i]==0 && k>0)
            {
                minCoins[i][k] = 99999;
                lastCoin[i][k] = 0;
            }
            else if (k>=mod_coinValues[i] && (minCoins[i][k-mod_coinValues[i]]+1) < minCoins[i-1][k])
            {
                minCoins[i][k] = minCoins[i][k - mod_coinValues[i]] + 1;
                lastCoin[i][k] = mod_coinValues[i];
            }
            else
            {
                minCoins[i][k]=minCoins[i-1][k];
                lastCoin[i][k]=lastCoin[i-1][k];
            }
        }
    }

    int i=numCoins,k=m;

    while (true)
    {
        while(i > 0 && k > 0)
        {
            if (minCoins[i][k]==minCoins[i-1][k]  && lastCoin[i][k]==lastCoin[i-1][k])i--;
            else break;
        }
        if (i==0 || k==0) break;
        result+=to_string(lastCoin[i][k])+";";
        if ((minCoins[i][k-mod_coinValues[i]]+1)==minCoins[i][k]) k-=mod_coinValues[i];
    }

    if (result.empty())result="-";


    return result;
}

