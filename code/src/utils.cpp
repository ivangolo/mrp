//
// Created by ivanedo on 12-06-15.
//

#include <iostream>
#include "utils.h"

using namespace std;

void ShowHelpInfo(char *s) {
    cout<<"Usage:   "<<s<<" [-option] [argument]"<<endl;
    cout<<"option:  "<<"-h  show help information"<<endl;
    cout<<"         "<<"-u username"<<endl;
    cout<<"         "<<"-p  password"<<endl;
    cout<<"         "<<"-s  save the password: 0(save password) 1(forget password)"<<endl;
    cout<<"         "<<"-v  show version infomation"<<endl;
    cout<<"example: "<<s<<" -uusername -ppassword -s1"<<endl;
}