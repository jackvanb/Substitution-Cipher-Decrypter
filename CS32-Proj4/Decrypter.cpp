//
//  Decryptor.cpp
//  CS32-Proj4
//
//  Created by Jack Van Boening on 3/10/18.
//  Copyright © 2018 Jack Van Boening. All rights reserved.
//
#include "provided.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;
const string TOKENIZERS = "  ,;:.!()[]{}-\"#$%^&1234567890";

class DecrypterImpl
{
public:
    DecrypterImpl();
    bool load(string filename);
    vector<string> crack(const string& ciphertext);
private:
    WordList wl;
    Tokenizer tokenizer;
    Translator translator;
    
    void crackHelper(const string& ciphertext, const vector<string>& cipherWords, vector<string>& result);

    int numUnknowns(const string& word)
    {
        int count = 0;
        for (int i = 0; i < word.size(); i++)
        {
            if (word[i] == '?')
                count++;
                
        }
        return count;
    }
};

DecrypterImpl::DecrypterImpl() : tokenizer(TOKENIZERS)
{ }

bool DecrypterImpl::load(string filename)
{
    if (wl.loadWordList(filename))
        return true;
    else
        return false;
}

vector<string> DecrypterImpl::crack(const string& ciphertext)
{
    vector<string> cipherwWords = tokenizer.tokenize(ciphertext);
    vector<string> result;
    crackHelper(ciphertext, cipherwWords, result);
    sort(result.begin(), result.end());
    return result;
}

void DecrypterImpl::crackHelper(const string& ciphertext, const vector<string>& cipherWords, vector<string>& result)
{
    int maxIndex = 0;
    int maxUnknowns = 0;
    
    for (int i = 0; i < cipherWords.size(); i++)
    {
        int count = numUnknowns(translator.getTranslation(cipherWords[i]));
        
        if (count == 0) // FULLY TRANSLATED WORD
        {
            // NOT IN WORD LIST RETURN TO PREVIOUS CALL
            if ( ! wl.contains(translator.getTranslation(cipherWords[i])))
                return;
        }
        
        if (count > maxUnknowns)
        {
            maxUnknowns = count;
            maxIndex = i;
        }
    }
    
    if (maxUnknowns == 0)   // COMPLETE DESCRIPTION, ADD TO RESULT
    {
        result.push_back(translator.getTranslation(ciphertext));
        return;
    }
    
    ////////////////// FIND CANIDATES //////////////
    string currCipher = cipherWords[maxIndex];
    vector<string> canidates = wl.findCandidates(currCipher, translator.getTranslation(currCipher));
    
    // LOOP THROUGH CANIDATES
    for (int i = 0; i < canidates.size(); i++)
    {
        if (translator.pushMapping(currCipher, canidates[i]))
        {
            crackHelper(ciphertext, cipherWords, result);
            translator.popMapping();
        }
    }

}

//******************** Decrypter functions ************************************

// These functions simply delegate to DecrypterImpl's functions.
// You probably don't want to change any of this code.

Decrypter::Decrypter()
{
    m_impl = new DecrypterImpl;
}

Decrypter::~Decrypter()
{
    delete m_impl;
}

bool Decrypter::load(string filename)
{
    return m_impl->load(filename);
}

vector<string> Decrypter::crack(const string& ciphertext)
{
    return m_impl->crack(ciphertext);
}

