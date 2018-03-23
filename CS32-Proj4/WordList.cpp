//
//  WordList.cpp
//  CS32-Proj4
//
//  Created by Jack Van Boening on 3/10/18.
//  Copyright © 2018 Jack Van Boening. All rights reserved.
//
#include "provided.h"
#include "MyHash.h"
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;

class WordListImpl
{
public:
    bool loadWordList(string filename);
    bool contains(string word) const;
    vector<string> findCandidates(string cipherWord, string currTranslation) const;
    
private:
    MyHash<string, string> wordHash;        // WORD TO WORD
    MyHash<string , list<string>> patternHash;    // PATTERN TO LIST OF WORDS
    string createWordPattern(string word) const;
    
};

bool WordListImpl::loadWordList(string filename)
{
    wordHash.reset();
    patternHash.reset();
    
    ifstream infile(filename);
    if ( ! infile )  // Did opening the file fail?
    {
        cerr << "Error: Cannot open data.txt!" << endl;
        return false;
    }
    
    string s;
    vector<string> wordList;
        // getline returns infile; the while tests its success/failure state
        while (getline(infile, s))
        {
            for (int i = 0; i < s.size(); i++)
            {
                if (! isalpha(s[i]) && s[i] != '\'')  // APOSTROPHE
                    continue;
            }
            
            string temp;
            for (int j = 0; j < s.size(); j++)  // CONVERT ALL TO LOWER CASE
            {
                temp += tolower(s[j]);
            }
            s = temp;
            wordHash.associate(s, s);   // WORD HASH FUNCTION
            
            string wordPattern = createWordPattern(s);
            // ADD LOGIC FOR ADDING TO PATTERN HASH
            if (patternHash.find(wordPattern) == nullptr)
            {
                // CREATE LINKED LIST
                list<string> word;
                word.push_back(s);
                patternHash.associate(wordPattern, word);
            }
            else
            {
                // ADD TO LINKED LIST
                list<string> * wordList = patternHash.find(wordPattern);
                wordList->push_back(s);
            }
        }
    return true;
}

bool WordListImpl::contains(string word) const
{
    // CONVERT TO LOWER
    string temp;
    for (int j = 0; j < word.size(); j++)
    {
        temp += tolower(word[j]);
    }
    word = temp;

    
    if (wordHash.find(word) == nullptr)
    {
        return false;
    }
    else
    {
        return true;
    }
    
}

vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const
{
    vector<string> result;
    
    // PRE-CONDITION CHECKS
    if (cipherWord.size() != currTranslation.size())
        return vector<string>();  // empty vector
    
    
    for (int i = 0; i < cipherWord.size(); i++)
    {
        
        if (! isalpha(cipherWord[i]) && cipherWord[i] != '\'')  // APOSTROPHE
            return vector<string>();
            
        if (! isalpha(currTranslation[i]) && currTranslation[i] != '\''
            && currTranslation[i] != '?')
            return vector<string>();
        
        if (isalpha(currTranslation[i] && !isalpha(cipherWord[i])))
            return vector<string>();
        
        if (currTranslation[i] == '?' && !isalpha(cipherWord[i]))
            return vector<string>();
        
        if (currTranslation[i] == '\'' && cipherWord[i] != '\'')
            return vector<string>();
    }
    
    //////////////////////////
    // CONVERT TO LOWER
    string temp;
    for (int j = 0; j < cipherWord.size(); j++)
    {
        temp += tolower(cipherWord[j]);
    }
    cipherWord = temp;
    
    temp.clear();
    for (int j = 0; j < currTranslation.size(); j++)
    {
        temp += tolower(currTranslation[j]);
    }
    currTranslation = temp;
    
    ///////////////////////////
    // ADD WORDS WITH SAME WORD PATTERN AS CIPHER TO VECTOR
    string cipherPattern = createWordPattern(cipherWord);

    if (patternHash.find(cipherPattern) == nullptr)
        return vector<string>(); // EMPTY VECTOR
    else
    {
        list<string>::const_iterator it = patternHash.find(cipherPattern)->begin();
        while (it != patternHash.find(cipherPattern)->end())
        {
            result.push_back((*it));
            it++;
        }
    }
    
    ////////////////////////////
    // CHECK FOR MATCHING LETTER IN CURRTRANSLATION
    for (int j = 0; j < currTranslation.size(); j++)
    {
        vector<string>::const_iterator it = result.begin();
        while (it != result.end())
        {
            string word = *it;
            
            if (isalpha(currTranslation[j]) && word[j] != currTranslation[j])
                it = result.erase(it);
            
            else if (currTranslation[j] == '\'' && word[j] != '\'')
                it = result.erase(it);
            
            else
                it++;
        }
    }
    ////////////////////////////
    
    
    return result; 
}

string WordListImpl::createWordPattern(string word) const
{
    // FIRST LETTER CONSIDERED '0'
    MyHash<char, string> patternHash;
    patternHash.associate(word[0], "0");
    string result = "0";
    
    if (word.size() == 1)
        return result;
    
    for (int i = 1; i != word.size(); i++)
    {
        if (patternHash.find(word[i]) == nullptr)
        {
            // ADD NEW LETTER TO PATTERN
            if (word[i] == '\'')
            {
                patternHash.associate(word[i], "\'");
                result += "\'";
            }
            else
            {
                patternHash.associate(word[i],to_string(i));
                result += to_string(i);
            }
        }
        else    // REPEATING LETTER
        {
            result += (*patternHash.find(word[i]));
        }
    }
    return result;
}

//***** hash functions for string, int, and char *****

unsigned int hash(const std::string& s)
{
    return std::hash<std::string>()(s);
}

unsigned int hash(const int& i)
{
    return std::hash<int>()(i);
}

unsigned int hash(const char& c)
{
    return std::hash<char>()(c);
}

//******************** WordList functions ************************************

// These functions simply delegate to WordListImpl's functions.
// You probably don't want to change any of this code.

WordList::WordList()
{
    m_impl = new WordListImpl;
}

WordList::~WordList()
{
    delete m_impl;
}

bool WordList::loadWordList(string filename)
{
    return m_impl->loadWordList(filename);
}

bool WordList::contains(string word) const
{
    return m_impl->contains(word);
}

vector<string> WordList::findCandidates(string cipherWord, string currTranslation) const
{
    return m_impl->findCandidates(cipherWord, currTranslation);
}

