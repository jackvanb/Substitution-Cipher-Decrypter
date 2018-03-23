//
//  main.cpp
//  CS32-Proj4
//
//  Created by Jack Van Boening on 3/10/18.
//  Copyright © 2018 Jack Van Boening. All rights reserved.
//
#include "provided.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

const string WORDLIST_FILE = "/Users/jvb/Downloads/skeleton/wordlist.txt";

int main()
{
    cout << "================" << endl;
    cout << "EXAMPLE SENTENCES TO DECRYPT:" << endl;
    cout << endl;
    cout << "Trcy oyc koon oz rweelycbb vmobcb, wyogrcn oecyb; hjg ozgcy tc moox bo moya wg grc vmobck koon grwg tc ko yog bcc grc oyc trlvr rwb hccy oecyck zon jb. -Rcmcy Xcmmcn" << endl;
    cout << endl;
    cout << "Jxwpjq qwrla glcu pcx qcn xkvv dw uclw ekarbbckpjwe dq jzw jzkpta jzrj qcn ekep'j ec jzrp dq jzw cpwa qcn eke ec. -Urls Jxrkp" << endl;
    cout << "================" << endl;
    cout << endl;
    
    


    Decrypter d;
    d.load(WORDLIST_FILE);
    string message = "";
    vector<string> result;
    
    message = "Trcy oyc koon oz rweelycbb vmobcb, wyogrcn oecyb; hjg ozgcy tc moox bo moya wg grc vmobck koon grwg tc ko yog bcc grc oyc trlvr rwb hccy oecyck zon jb. -Rcmcy Xcmmcn";
    result = d.crack(message);
    for (int i = 0; i < result.size(); i++)
    {
        cout << result[i] << endl;
    }
    
    result.clear();
    message.clear();
    cout << endl;
    
    message = "Jxwpjq qwrla glcu pcx qcn xkvv dw uclw ekarbbckpjwe dq jzw jzkpta jzrj qcn ekep'j ec jzrp dq jzw cpwa qcn eke ec. -Urls Jxrkp";
    result = d.crack(message);
    for (int i = 0; i < result.size(); i++)
    {
        cout << result[i] << endl;
    }
    
}

