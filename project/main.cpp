#include <iostream>
#include "Management.h"
#include <utility>

using namespace std;

int main(int argc, char *argv[]) {      //Per gli int che vivono solo dentro le funzioni e che non dovrebbero superare 2^16 usa short

    bool err=false;

    if(argc!=4){
        cerr<<"Error in the number of input from command line";
        err=true;
    }
    else{

        Management social_network;
        if(!social_network.controls(argv[1],argv[2],argv[3])) err=true;
        else{
                cout<<"Analysis of input files completed successfully."<<endl;
                social_network.execution();
            }
    }

    if(err){
        cout<<"Press 'q' to exit."<<endl; //Not really important which key
        char q;
        cin >>q;
    }

    return 0;
}
