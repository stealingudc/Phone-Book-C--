#include <string>
#include "Caesar.h"

using namespace std;

class Login{
public:
    static bool start(){
        string input;
        system("cls");
        cout << "Enter password:" << endl << endl;
        cin >> input;
        if(Caesar::verify(input)){
            return true;
        } else return false;
    }
};