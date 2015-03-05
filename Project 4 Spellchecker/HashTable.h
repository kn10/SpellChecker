/*
Nguyen, Kelvin
August 4, 2014
603892561


Creating a Spellchecker
*/

/*
Implementing my own HashTable, using vector<list<string>>,
to store words list, and check if a word exists in the dictionary
or not.
*/

#ifndef HASHTABLE_INCLUDED
#define HASHTABLE_INCLUDED

// Include directories
#include <iostream>
#include <vector> // STL container
#include <list> // STL container
#include <string>

const int MAXIMUM_BUCKETS = 100000; // no more than 100000 max

class HashTable
{
public:
	HashTable(); // default constructor
	HashTable(int totalWords); // constructor
	void insert(std::string toInsert);
	void print();
	bool hasKey(std::string query);
	int getMaxBuckets() const;

private:
	std::vector< std::list<std::string> > m_buckets;
	int m_bucketSize;

	// hash function:
	unsigned int hash(std::string toHash)
	{
		unsigned int result = 0;
		for (size_t i = 0; i < toHash.length(); i++)
		{
			result = result * 101 + toHash[i];
		}
		return result % m_bucketSize;
	}
};

#endif // HashTable.h