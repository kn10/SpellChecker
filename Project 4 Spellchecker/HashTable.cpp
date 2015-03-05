/*
Nguyen, Kelvin
August 4, 2014
603892561


Creating a Spellchecker
*/

#include "HashTable.h"

HashTable::HashTable() // default constructor
{
	m_bucketSize = MAXIMUM_BUCKETS;
	m_buckets.resize(MAXIMUM_BUCKETS);
}

HashTable::HashTable(int totalWords) // constructor
{
	m_bucketSize = totalWords;
	m_buckets.resize(MAXIMUM_BUCKETS);
}

int HashTable::getMaxBuckets() const // returns size of bucket (int at first line of textfile)
{
	return m_bucketSize;
}

void HashTable::insert(std::string toInsert) // pushes toInsert into m_buckets, and uses hash function to generate key
{
	if (hasKey(toInsert))
	{
		return;
	}
	m_buckets[HashTable::hash(toInsert)].push_back(toInsert);
}

void HashTable::print()
{
	for (size_t i = 0; i < m_buckets.size(); i++) // iterates through m_buckets vector
	{
		if (m_buckets[i].empty())
		{
			continue;
		}
		std::list<std::string>::iterator listIt; // iterates through m_buckets[i] list, and prints its data
		for (listIt = m_buckets[i].begin(); listIt != m_buckets[i].end(); listIt++)
		{
			std::cout << i << " " << *listIt << std::endl;
		}
	}
}

bool HashTable::hasKey(std::string query)
{
	// Store a pointer to the bucket's list
	std::list<std::string>* lPtr = &m_buckets[hash(query)];

	// Start an iterator at it's beginning
	std::list<std::string>::iterator it = lPtr->begin();

	// Search linearly for the query item
	while (it != lPtr->end())
	{
		if (*it == query)
		{
			break;
		}
		it++;
	}
	// Return whether the iterator found a
	// match or not
	return it != lPtr->end();
}