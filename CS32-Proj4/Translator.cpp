//
//  Translator.cpp
//  CS32-Proj4
//
//  Created by Jack Van Boening on 3/10/18.
//  Copyright © 2018 Jack Van Boening. All rights reserved.
//
#include "provided.h"
#include <map>
#include <unordered_map>
#include <list>
#include <string>
#include <iostream>
using namespace std;

class TranslatorImpl
{
public:
    TranslatorImpl();
    bool pushMapping(string ciphertext, string plaintext);
    bool popMapping();
    string getTranslation(const string& ciphertext) const;
private:
    // ALL MAPPINGS ARE STORED AS LOWER CASE LETTERS
    unordered_map<char, char> cipherToPlain; // CHIPHER_TEXT TO PLAIN_TEXT
    unordered_map<char, char> plainToCiper; // PLAIN_TEXT TO CHIPHER_TEXT
    //  LINKED LIST OF ALL PREVIOUS MAPPINGS
    list<string> cipherChanges;
    list<string> plaintextChanges;
    
    bool sameMapping(string ciphertext, string plaintext)
    {
        for (int i = 0; i < ciphertext.size(); i++)
        {
            char c = tolower(ciphertext[i]);
            char p = tolower(plaintext[i]);
            
            // SAME MAPPING ALREADY EXISTS
            unordered_map<char, char>::const_iterator cToP = cipherToPlain.find(c);
            if (cToP->second == p)
                continue;
            else
                return false;
        }
        return true;
    }
    
};

TranslatorImpl::TranslatorImpl()
{
    // INITILIZE CIPHER TEXT
    string alphabet = "abcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < alphabet.size(); i++)
    {
        pair<char, char> pair1(alphabet[i], '?');
        cipherToPlain.insert(pair1);
    }
    
}

bool TranslatorImpl::pushMapping(string ciphertext, string plaintext)
{
    // PRE-CONDITIONS
    if (ciphertext.size() != plaintext.size())
        return false;
    
    for (int i = 0; i < ciphertext.size(); i++)
    {
        char c = tolower(ciphertext[i]);
        char p = tolower(plaintext[i]);
        
        
        if (!isalpha(ciphertext[i]) || !isalpha(plaintext[i]))
            return false;
       
        // ADD INCONSISTENT CHECKS - DIFFERNT MAPPING ALREADY EXISTS
        unordered_map<char, char>::const_iterator cToP = cipherToPlain.find(c);
        unordered_map<char, char>::const_iterator pToC = plainToCiper.find(p);
        if (cToP->second != '?' && cToP->second != p) // DIFFERNT MAPPING EXISTS FOR CIPHER LETTER
            return false;
        if (pToC != plainToCiper.end() && pToC->second != c)
            return false;
    }
    ////////////////////////////
    
    if (sameMapping(ciphertext, plaintext))
        return true;
    else
    {
        // VALID MAPPING - ADD TO MAPPING
        cipherChanges.push_back(ciphertext);
        plaintextChanges.push_back(plaintext);
    
        for (int i = 0; i < ciphertext.size(); i++)
        {
            char c = tolower(ciphertext[i]);
            char p = tolower(plaintext[i]);
            pair<char, char> pair1(p, c);
            cipherToPlain[c] = p;
            plainToCiper[p] = c;
        }
    }
    
    return true;
}

bool TranslatorImpl::popMapping()
{
    if (cipherChanges.empty())
        return false;
    else
    {
        string cipher = cipherChanges.back();
        string plaintext = plaintextChanges.back();
        for (int i = 0; i < cipher.size(); i++)
        {
            // LOOK AT LAST CHANGES, REMOVE CHANGES
            char c = tolower(cipher[i]);
            char p = tolower(plaintext[i]);
            cipherToPlain[c] = '?'; // set back to ?
            plainToCiper.erase(p);   // remove from map
        }
        
        // REMOVE STRINGS FROM LINKED LIST
        cipherChanges.pop_back();
        plaintextChanges.pop_back();
    }
    return true;
}

string TranslatorImpl::getTranslation(const string& ciphertext) const
{
    string result;
    for (int i = 0; i < ciphertext.size(); i++)
    {
        char c = tolower(ciphertext[i]);
        unordered_map<char, char>::const_iterator cToP = cipherToPlain.find(c);
        // IF NOT A LETTER, DON'T CHANGE
        if (! isalpha(c))
        {
            result += ciphertext[i];
            continue;
        }
        // NOT IN MAPPING
        if (cToP->second == '?')
        {
            result += '?';
            continue;
        }
        // MAPS TO A LETTER
        else
        {
            // NEED TO KNOW CASE
            if (isupper(ciphertext[i]))
            {
                char r = toupper(cToP->second);
                result += r;
            }
            else
                result += cToP->second;
        }
        
    }
    
    return result;
}

//******************** Translator functions ************************************

// These functions simply delegate to TranslatorImpl's functions.
// You probably don't want to change any of this code.

Translator::Translator()
{
    m_impl = new TranslatorImpl;
}

Translator::~Translator()
{
    delete m_impl;
}

bool Translator::pushMapping(string ciphertext, string plaintext)
{
    return m_impl->pushMapping(ciphertext, plaintext);
}

bool Translator::popMapping()
{
    return m_impl->popMapping();
}

string Translator::getTranslation(const string& ciphertext) const
{
    return m_impl->getTranslation(ciphertext);
}

