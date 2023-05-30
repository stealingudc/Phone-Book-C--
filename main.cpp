#include "./src/Login.h"
#include "./src/Menu.h"

using namespace std;

int main(){
    if(Login::start()){
        Menu::main_menu();
    }
}