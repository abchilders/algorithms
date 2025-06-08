// translated to C++ from Adam's lumber.py 

#include <cstdlib>
#include <unordered_map>
#include <iostream>

using namespace std;
// cut, max price
int calculate_price(int length, unordered_map<int, int>& mem)
{
	// base cases
	if (length < 6)
	{
		return 0;
	}
	if (length < 8)
	{
		return 2;
	}
	if (length < 10)
	{
		return 3;
	}
	if (length < 12)
	{
		return 4;
	}
	if (length < 16)
	{
		return 5;
	}
	if (length < 18)
	{
		return 7;
	}
	if (length < 20)
	{
		return 9;
	}
	if (length < 24)
	{
		return 10;
	}
	if (length == 24)
	{
		return 12;
	}

	// goal: find max of all possibilities 

	// use parallel vectors 
	vector<int> prices = { 2, 3, 4, 5, 7, 9, 10, 12 };
	vector<int> cuts = { 6, 8, 10, 12, 16, 18, 20, 24 };

	vector<int> result{};

	for (int i = 0; i < cuts.size(); i++)
	{
		int cut = cuts[i];
		int price = prices[i];

		int result_price = 0;
		unordered_map<int, int>::const_iterator find = mem.find(length - cut); 
		
		// if current cut isn't in memory yet, calculate it 
		if (find == mem.end())
		{
			result_price = calculate_price(length - cut, mem); 
			mem[length - cut] = result_price; 
		}
		else
		{
			result_price = mem[length - cut]; 
		}
		result.push_back(result_price + price); 
		
		// get largest value in result 
		int largest = INT_MIN;
		for (auto val : result)
		{
			if (val > largest)
			{
				largest = val; 
			}
		}
		return largest; 
	}
}

int main(void)
{
	unordered_map<int, int> mem{}; 
	cout << "max price of length 36 is " << calculate_price(36, mem) << endl;
	return 0; 
}