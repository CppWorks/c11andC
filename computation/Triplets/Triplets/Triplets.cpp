// Triplets.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <iostream> 
#include <algorithm>
using namespace std;

int MaxBound = 12;
size_t numDigis = 5;
// I should totally order the vector...
// show triplemts first.....worry about sum
int counter = 0;
vector<int> elements;
// so we do I get now?
void PrintList(vector<int>&currentsolution)
{
    vector<int>::iterator a;
    for  (a = currentsolution.begin(); a != currentsolution.end(); a++)
    {
        cout << *a << "\t";
    }
    cout << endl;
}

// now my next job is to sort the vector...easy;



// Optimise...we should never buid solution more than 3 mits.

// I am noly learning to how to terminate recursion at one go..
// return false means recursion is still going
bool RecursiveFindTiplets(int sum, vector<int>&currentsolution, int start)
{
    if (currentsolution.size() == 3 && sum < MaxBound)
    {
        // we have a valid soulution
        counter++;
        PrintList(currentsolution);
        return false;
        
    }
    else if ( sum >= MaxBound)
    {
        // Given our list is ordered, we can terminate
        return true;
    }
    vector<int> previousSolution = currentsolution;
    int previousSum = sum;
    for (size_t i = start; i < elements.size(); i++) {
      if (currentsolution.size() == numDigis)
        return false;
      currentsolution.push_back(elements[i]);
      sum += elements[i];

      if (RecursiveFindTiplets(sum, currentsolution, i + 1) == true) {
          cout << "total count is " << counter << endl;
          exit(0); // I use exit to return.
      }
      sum = previousSum;           // This restore process is very important..
      currentsolution = previousSolution;
    }
    return false;
}


int countTriplets(int arr[], int n, int sum)
{
    // Sort input array
    sort(arr, arr + n);

    // Initialize result
    int ans = 0;

    // Every iteration of loop counts triplet with
    
    // first element as arr[i].
    // The idea is enumarate in a smart fashion.
    for (int i = 0; i < n - 2; i++)
    {
        // Initialize other two elements as corner elements
        // of subarray arr[j+1..k]
        int j = i + 1, k = n - 1;

        // Use Meet in the Middle concept
        while (j < k)
        {
            // If sum of current triplet is more or equal,
            // move right corner to look for smaller values
            if (arr[i] + arr[j] + arr[k] >= sum)
                k--;

            // Else move left corner
            else
            {
                // This is important. For current i and j, there
                // can be total k-j third elements.
                ans += (k - j);
                j++;
            }
        }
    }
    return ans;
}

int _tmain(int argc, _TCHAR* argv[])
{
    int sum = 0;
    elements.push_back(5);
    elements.push_back(1);
    elements.push_back(3);
    elements.push_back(4);
    elements.push_back(7);
    // using default comparison (operator <):
    // using default comparison (operator <):
    std::sort(elements.begin(), elements.end());
    vector<int> currentSolution;
    RecursiveFindTiplets(sum, currentSolution, 0);
    return 0;
}

// In in this solution, 
//I have utlised the optimise sturcture of the sorted array. if minimum and maxium value from i to n-1 with tripilets is smaller than target..just move the stat value to the right of 1
// continue;