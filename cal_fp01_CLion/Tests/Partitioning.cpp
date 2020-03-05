/*
 * Partioning.cpp
 */

#include "Partitioning.h"


int s_recursive(int n,int k)
{
   if (k == 1 || k == n) return 1;
   else return s_recursive(n-1,k-1) + k*s_recursive(n-1,k);
}

int s_dynamic(int n,int k)
{
    int s[k][n];

    for (int i=0;i<n;i++)
    {
        for (int i2=0;i2<k;i2++)
        {
            if (i==i2) s[i2][i] = 1;
            else if (i2 > i) continue;
            else if (i2 == 0) s[i2][i] = 1;
            else s[i2][i] = s[i2-1][i-1] + (i2+1)*s[i2][i-1];
        }
    }
    return s[k-1][n-1];
}


int b_recursive(int n)
{
    int result=0;
    for (int i=1;i<=n;i++)
    {
        result+=s_recursive(n,i);
    }
    return result;
}

int b_dynamic(int n)
{
    int result=0;
    for (int i=1;i<=n;i++)
    {
        result+=s_dynamic(n,i);
    }
    return result;
}


