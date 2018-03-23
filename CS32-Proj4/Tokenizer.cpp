//
//  Tokenizer.cpp
//  CS32-Proj4
//
//  Created by Jack Van Boening on 3/10/18.
//  Copyright © 2018 Jack Van Boening. All rights reserved.
//
#include "provided.h"
#include <string>
#include <vector>


using namespace std;

class TokenizerImpl
{
public:
    TokenizerImpl(string separators);
    vector<string> tokenize(const std::string& s) const;
private:
    string m_seperators;
    
    bool isSeperator(char c) const {
        for (int i = 0; i < m_seperators.size(); ++i)
        {
            if (m_seperators[i] == c)
                return true;
        }
        return false;
    }
};

TokenizerImpl::TokenizerImpl(string separators) : m_seperators(separators)
{
    
}

vector<string> TokenizerImpl::tokenize(const std::string& s) const
{
    vector<string> result;
    
    string word;
    for (int i = 0; i < s.size(); i++)
    {
        if (isSeperator(s[i]))
        {
            if (!word.empty())
            {
                result.push_back(word);
                word.clear();
            }
        }
        else
            word += s[i];
    }
    if (!word.empty())
        result.push_back(word);
    
    return result;
}

//******************** Tokenizer functions ************************************

// These functions simply delegate to TokenizerImpl's functions.
// You probably don't want to change any of this code.

Tokenizer::Tokenizer(string separators)
{
    m_impl = new TokenizerImpl(separators);
}

Tokenizer::~Tokenizer()
{
    delete m_impl;
}

vector<string> Tokenizer::tokenize(const std::string& s) const
{
    return m_impl->tokenize(s);
}
