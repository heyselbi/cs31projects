#include <iostream>
#include <string>
#include <cctype>
#include <cassert>
using namespace std;

int appendToAll(string a[], int n, string value)
{
    if (n < 0)
        return -1;
    else
    {
        for (int i = 0; i < n; i++)
            a[i] = a[i]+value;
        return n;
    }
}

int lookup(const string a[], int n, string target)
{
    if (n <= 0)
        return -1;
    else
    {
        for (int i = 0; i < n; i++)
            if (a[i] == target)
            {
                return i;
                break;
            }
        return -1;
    }
}

int positionOfMax(const string a[], int n)
{
    if (n <= 0)
        return -1;
    else
    {
        int indexMax = 0;
        for (int i = 1; i < n; i++)
        {
            if (a[i][0] > a[indexMax][0])
                indexMax = i;
        }
        return indexMax;
    }
}

int rotateLeft(string a[], int n, int pos)
{
    if ((n <= 0) || (pos <= 0) || (pos > n))
        return -1;
    else
    {
        string temp = a[pos];
        for (int k = pos; k < (n-1); k++)
            a[k] = a[k+1];
        a[n-1] = temp;
        return pos;
    }
}

int countRuns(const string a[], int n)
{
    if (n < 0)
        return -1;
    else if (n == 0)
        return 0;
    else
    {
        int runs = 1;
        for (int i = 0; i < (n-1); i++)
        {
            if (a[i] != a[i+1])
                runs++;
        }
        return runs;
    }
}

int flip(string a[], int n)
{
    if (n < 0)
        return -1;
    else
    {
        string temp;
        for (int i = 0; i < (n/2); i++)
        {
            temp = a[i];
            a[i] = a[n-1-i];
            a[n-1-i] = temp;
        }
        return n;
    }
}

int differ(const string a1[], int n1, const string a2[], int n2)
{
    if ((n1 <= 0) || (n2 <= 0))
        return -1;
    else
    {
        int shortest;
        if (n1 <= n2)
            shortest = n1;
        else
            shortest = n2;
        for (int i = 0; i < shortest; i++)
        {
            if (a1[i] != a2[i])
                shortest = i;
        }
        return shortest;
    }
}

int subsequence(const string a1[], int n1, const string a2[], int n2)
{
    if ((n1 <= 0) || (n2 <= 0))
        return -1;
    else
    {
        int startIndex = -1;
        for (int i = 0; i < n1; i++)
            {
                if (a2[0] == a1[i])
                {
                    startIndex = i;
                    for (int k = 0; k < n2; k++)
                    {
                        if (a2[k]!=a1[i+k])
                            startIndex = -1;
                    }
                }
            }
        return startIndex;
    }
}

int lookupAny(const string a1[], int n1, const string a2[], int n2)
{
     if ((n1 <= 0) || (n2 <= 0))
         return -1;
     else
     {
         for (int i=0; i < n1; i++)
             for (int k=0; k < n2; k++)
             {
                 if (a1[i] == a2[k])
                     return i;
             }
         return -1;
     }
 }
 
 int divide(string a[], int n, string divider)
 {
     if (n <= 0)
         return -1;
     else
     {
         string temp;
         int tempIndex = n-1, count=0;
         for (int i = 0; i < n; i++)
         {
             if (a[i] < divider)
                 count++;
         }
         if (count == 0)
             return 0;
         for (int k = 0; k < count; k++)
         {
             if (a[k] >= divider)
             {
                 for (int j = tempIndex; j >=count; j--)
                 {
                     if (a[j] < divider)
                     {
                         temp = a[j];
                         a[j] = a[k];
                         a[k] = temp;
                         tempIndex = j-1;
                         break;
                     }
                 }
             }
         }return count;
     }
 }

int main()
{
    string h[7] = { "greg", "gavin", "ed", "xavier", "", "eleni", "fiona" };
    assert(lookup(h, 7, "eleni") == 5);
    assert(lookup(h, 7, "ed") == 2);
    assert(lookup(h, 2, "ed") == -1);
    assert(positionOfMax(h, 7) == 3);
    
    string g[4] = { "greg", "gavin", "fiona", "kevin" };
    assert(differ(h, 4, g, 4) == 2);
    assert(appendToAll(g, 4, "?") == 4 && g[0] == "greg?" && g[3] == "kevin?");
    assert(rotateLeft(g, 4, 1) == 1 && g[1] == "fiona?" && g[3] == "gavin?");
    
    string e[4] = { "ed", "xavier", "", "eleni" };
    assert(subsequence(h, 7, e, 4) == 2);
    
    string d[5] = { "gavin", "gavin", "gavin", "xavier", "xavier" };
    assert(countRuns(d, 5) == 2);
    
    string f[3] = { "fiona", "ed", "john" };
    assert(lookupAny(h, 7, f, 3) == 2);
    assert(flip(f, 3) == 3 && f[0] == "john" && f[2] == "fiona");
    
    assert(divide(h, 7, "fiona") == 3);
    
    cout << "All tests succeeded" << endl;
    
}

