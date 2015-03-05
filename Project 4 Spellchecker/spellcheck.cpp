/*
Nguyen, Kelvin
August 4, 2014
603892561


Creating a Spellchecker
*/

// Include directories
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iomanip>
#include <vector> // STL container
#include <list> // STL container
#include "HashTable.h"

using namespace std;

// Constants
const string DICTIONARY = "wordlist.txt";
const string TEST_DICTIONARY = "kwordlist.txt";
const string TEST_FILE = "test.txt";
const int INDENT = 25;
const int PRINT_VECTOR_INDENT = 18;
char ALPHABET_LETTERS[26] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
const int TOTAL_ALPHABET_LETTERS = 26;

// Spellcheck Function:
void spellCheck(istream& inf, istream& wordlistfile, ostream& outf);

// Spellcheck techniques:
vector<string> swapAdjacentChars(string& word); // Swapping each adjacent pair of characters in the word.
vector<string> insertChars(string& word); // In between each adjacent pair of characters in the word (also before the first character and after the last character), each letter from 'A' through 'Z' is inserted.
vector<string> deleteChars(string& word); // Deleting each character from the word.
vector<string> replaceChars(string& word); // Replacing each character in the word with each letter from 'A' through 'Z'
vector<string> splitWord(string& word); // Splitting the word into a pair of words by adding a space in between each adjacent pair of characters in the word. It should be noted that this will only generate a suggestion if both words in the pair are found in the wordlist.

// Helper functions:
void printVector(vector<string>& v, ostream& outf); // prints vector into ostream
void pushCorrectWords(vector<string>& possibleWords, vector<string>& suggestions, HashTable& wordList); // checks if word exists in dictionary, then parses into a vector
void pushCorrectWordsForSplittedWords(vector<string>& possibleWords, vector<string>& suggestions, HashTable& wordList); // for split words: checks if both words exist in dictionary, then parses into a vector

int main()
{
	ifstream wordList(DICTIONARY.c_str());
	ifstream file(TEST_FILE.c_str());

	spellCheck(file, wordList, cout);

	//Testing:
	//ifstream kwordList(TEST_DICTIONARY.c_str());
	//spellCheck(file, kwordList, cout);

	return 0;
}

void spellCheck(istream& inf, istream& wordlistfile, ostream& outf)
{
	// Declarations:
	string word; // to parse into vectors
	string dummyLine; // for first line
	int totalWords; // to set HashTable constructor
	vector<string> check; // vector of words to check for errors
	vector<string> checkLowercase; // converts words in check to lowercase
	vector<string> misspelledWords; // incorrectly spelled words

	wordlistfile >> totalWords; // read in first line to store total words in wordlist

	HashTable wordList(totalWords); // hashtable for wordList

	getline(wordlistfile, dummyLine);
	while (getline(wordlistfile, word)) // reads rest of the file;
	{
		wordList.insert(word);
	}

	while (inf >> word) // stores original sentence into check vector
	{
		check.push_back(word);
	}

	for (size_t i = 0; i < check.size(); i++) // transforms all words in check vector to lowercase, then parses these words into checkLowercase vector
	{
		string word = check[i];
		transform(word.begin(), word.end(), word.begin(), ::tolower);
		checkLowercase.push_back(word);
	}

	for (size_t i = 0; i < check.size(); i++) // if the word is not in our wordList, then it is misspelled; thus, parse this word into misspelledWords vector
	{
		if (!wordList.hasKey(checkLowercase[i]))
		{
			misspelledWords.push_back(checkLowercase[i]);
		}
	}

	// Applying spellchecking techniques to the misspelled words
	for (size_t i = 0; i < misspelledWords.size(); i++)
	{
		// Declarations
		string word = misspelledWords[i];
		vector<string> permutations = swapAdjacentChars(word); // permutations of word;
		vector<string> insertions = insertChars(word); // words with chars inserted into each position
		vector<string> deletedWords = deleteChars(word); // words where each character is deleted from the word
		vector<string> replacedWords = replaceChars(word); // words where each character is replaced by a letter in the alphabet
		vector<string> splittedWords = splitWord(word); // words are splitted, with a space between each character
		vector<string> suggestions; // provides suggestions for word

		transform(word.begin(), word.end(), word.begin(), ::toupper); // transform each word to uppercase

		// Parsing correctly spelled words into suggestions vector
		pushCorrectWords(insertions, suggestions, wordList);
		pushCorrectWords(permutations, suggestions, wordList);
		pushCorrectWords(deletedWords, suggestions, wordList);
		pushCorrectWords(replacedWords, suggestions, wordList);
		pushCorrectWordsForSplittedWords(splittedWords, suggestions, wordList);

		// Printing out original sentence
		for (size_t i = 0; i < check.size(); i++)
		{
			outf << check[i] << " ";
		}
		outf << endl;
		outf << setw(INDENT) << "word not found: " << word << endl;
		outf << setw(INDENT) << "perhaps you meant: " << endl;
		std::sort(suggestions.begin(), suggestions.end()); // sort the suggestions vector in alphabetical order
		printVector(suggestions, outf);
		outf << endl;
	}
}

vector<string> swapAdjacentChars(string& word) // swap each adjacent pair of characters in the word
{
	// word = abcd -> bacd, acbd, abdc,
	vector<string> swappedWords;

	string temp = word;

	for (size_t i = 0; i < temp.size() - 1; i++)
	{
		swap(temp[i], temp[i + 1]);
		swappedWords.push_back(temp);
		temp = word;
	}

	return swappedWords;
}

vector<string> insertChars(string& word)
{/*
 in between each adjacent pair of characters in the word
 (also before the first character and after the last character),
 each letter from 'A' through 'Z' is inserted.*/

	vector<string> possibleWords;
	string temp = word;

	for (size_t i = 0; i <= temp.size(); i++)
	{
		for (int j = 0; j < TOTAL_ALPHABET_LETTERS; j++)
		{
			temp.insert(i, 1, ALPHABET_LETTERS[j]);
			possibleWords.push_back(temp);
			temp = word;
		}
	}

	return possibleWords;
}

vector<string> deleteChars(string& word)
{
	/*
	Deleting each character from the word.
	abc -> bc, ac, ab
	*/
	vector<string> deletedWords;
	string temp = word;

	for (size_t i = 0; i < temp.size(); i++)
	{
		temp.erase(i, 1);
		deletedWords.push_back((temp));
		temp = word;
	}

	return deletedWords;
}

vector<string> replaceChars(string& word) // Replacing each character in the word with each letter from 'A' through 'Z'
{
	vector<string> replacedWords;
	string temp = word;
	for (size_t i = 0; i < temp.size(); i++)
	{
		for (int j = 0; j < TOTAL_ALPHABET_LETTERS; j++)
		{
			temp[i] = ALPHABET_LETTERS[j];
			replacedWords.push_back(temp);
			temp = word;
		}
	}

	return replacedWords;
}

vector<string> splitWord(string& word)
{
	// Splitting the word into a pair of words by adding a space in between each adjacent pair of characters in the word.
	// It should be noted that this will only generate a suggestion if both words in the pair are found in the wordlist.
	/*
	word = abc;
	a bc
	ab c
	*/
	vector<string> splittedWords;
	string temp = word;
	for (size_t i = 1; i < temp.size(); i++)
	{
		temp.insert(i, 1, ' ');
		splittedWords.push_back(temp);
		temp = word;
	}

	return splittedWords;
}

void printVector(vector<string>& v, ostream& outf)
{
	for (size_t i = 0; i < v.size(); i++)
	{
		outf << '\t' << '\t' << v[i] << endl;
	}
}

void pushCorrectWords(vector<string>& possibleWords, vector<string>& suggestions, HashTable& wordList)
{
	for (size_t i = 0; i < possibleWords.size(); i++)
	{
		string temp = possibleWords[i];
		transform(temp.begin(), temp.end(), temp.begin(), ::tolower); // transform word to lowercase;

		if (wordList.hasKey(temp))
		{
			transform(temp.begin(), temp.end(), temp.begin(), ::toupper); // transform temp to uppercase;

			if (find(suggestions.begin(), suggestions.end(), temp) == suggestions.end())
			{
				suggestions.push_back(temp);
			}
		}
	}
}

void pushCorrectWordsForSplittedWords(vector<string>& possibleWords, vector<string>& suggestions, HashTable& wordList)
{
	for (size_t i = 0; i < possibleWords.size(); i++)
	{
		string temp = possibleWords[i];
		transform(temp.begin(), temp.end(), temp.begin(), ::tolower); // transform word to lowercase;
		size_t blankSpace = temp.find(" "); // find position of blankSpace
		string firstWord = temp.substr(0, blankSpace);
		string secondWord = temp.substr(blankSpace + 1);

		if (wordList.hasKey(firstWord) && wordList.hasKey(secondWord)) // if firstWord and secondWord are real words in our dictionary
		{
			transform(temp.begin(), temp.end(), temp.begin(), ::toupper); // transform temp to uppercase;

			if (find(suggestions.begin(), suggestions.end(), temp) == suggestions.end())
			{
				suggestions.push_back(temp); // push the original word (temp) into suggestions vector
			}
		}
	}
}