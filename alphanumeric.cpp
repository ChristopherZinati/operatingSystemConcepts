#include <iostream>
#include <pthread.h>
#include <vector>
#include <sstream>
using namespace std;

vector<string> alphaAndNum;
bool alphaChecked = false;
bool numericChecked = true;

void* alpha(void* arg){
    vector<string>* alphaNum = static_cast<vector<string>*>(arg);
    for(auto it = alphaNum->begin(); it != alphaNum->end(); ++it){
        const string& str = *it;
        while(!numericChecked){

        }
        if(!isdigit(str[0])){
            cout << "alpha: " << str << endl;
        }
        alphaChecked = true;
        numericChecked = false;
    }
    alphaChecked = true;
    return nullptr;
}

void* numeric(void* arg){
    vector<string>* alphaNum = static_cast<vector<string>*>(arg);
    for(auto it = alphaNum->begin(); it != alphaNum->end(); ++it){
        const string& str = *it;
        while(!alphaChecked){
            
        }
        if(isdigit(str[0])){
            cout << "numeric: " << str << endl;
        }
        numericChecked = true;
        alphaChecked = false;
    }
    numericChecked = true;
    return nullptr;
}
int main()
{
    //tokenize the input and store in global vector
    string line = " ";
    cout << "enter a string: " << endl;
    getline(cin, line);

    stringstream check(line);
    string temp;

    while(getline(check, temp, ' ')){
        alphaAndNum.push_back(temp);
    }

    pthread_t alphaThread, numericThread;

    pthread_create(&alphaThread, nullptr, alpha, &alphaAndNum);
    pthread_create(&numericThread, nullptr, numeric, &alphaAndNum);

    pthread_join(alphaThread, nullptr);
    pthread_join(numericThread, nullptr);
}



