#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

#include <vector>

using namespace std;
int main (int argc,char *argv[]) {

    vector<int> myvector(10);
    for (unsigned i=0; i<myvector.size(); i++)
        myvector.at(i)=i;
    std::cout << "myvector contains:";
    for (unsigned i=0; i<myvector.size(); i++)
        std::cout << ' ' << myvector.at(i);
    std::cout << '\n';
    /*
    char tmp;
    if(argc == 1) {
        ShowHelpInfo(argv[0]);
        exit(1);
    }

    while((tmp = getopt(argc,argv,"t:p:i:o:s:")) != -1) {
        switch(tmp) {
            case 't':
                cout<<"argument entered: "<<optarg<<endl;
                break;

            case 'p':
                cout<<"argument entered: "<<optarg<<endl;
                break;

            case 'i':
                cout<<"argument entered: "<<optarg<<endl;
                break;

            case 'o':
                cout<<"argument entered: "<<optarg<<endl;
                break;

            case 's':
                cout<<"The current version is 0.1"<<endl;
                break;

            default:
                ShowHelpInfo(argv[0]);
                break;
        }
    }
     */
    return 0;
}