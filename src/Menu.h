#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "PhoneBook.h"
#include "Subscriber.h"

using namespace std;

string _dir_path = "./res/ui/";

class Menu{
private:
    /// @brief Method for getting multiple user inputs in a sequence without breaking the proccess loop. Can be broken by entering the provided escape character. vector<string> passed as argument because I don't like pointers.
    /// @param prompts vector<string> containing all of the prompts to be printed before each input.
    /// @param escape The escape character.
    /// @param ret Data provided by the user as vector<string>.
    /// @return 
    static vector<string> get_multiple_inputs(vector<string> prompts, string escape, vector<string> ret) {
        bool did_escape = false;
        while(!did_escape){
            for(int i = 0; i < prompts.size(); i++){
                cout << prompts[i] + ": ";
                ret = getQuery(ret);
                if(ret[i] == escape){
                    did_escape = true;
                    ret = vector<string>();
                    main_menu();
                    break;
                }
            }
            if(!did_escape){
                return ret;
                break;
            }
        }
    }
    /// @brief Method for selecting one of the subscribers in the whole list through user input. By index.
    /// @param escape The escape character used for breaking the loop.
    /// @return The Subscriber object instance selected by the user. // Subscriber object with index = -1 if bad input.
    static Subscriber select_sub(string escape){
        bool did_escape = false;
        string input;
        Subscriber ret;
        while(!did_escape){
            display("select");
            input = getQuery(input);
            if(input == escape){
                did_escape = true;
                main_menu();
                break;
            } else {
                try{
                    int index = stoi(input);
                    vector<Subscriber> subs = PhoneBook::get_all();
                    for(int i = 0; i < subs.size(); i++){
                        if(index == subs[i].get_index()){
                            ret = subs[i];
                            break;
                        } else {
                            ret.set_index(-1);
                        }
                    }
                } catch(const std::exception& e){
                    ret.set_index(-1);
                }
            }
            if(!did_escape){
                return ret;
                break;
            }
        }
    }
    /// @brief Method that allows the user to input any character that is not included in a list of options i.e. a switch statement.
    /// @param escape The escape character.
    /// @param min Minimum value the user can input that is considered valid.
    /// @param max Maximum value.
    /// @return The user input, parsed as int. Recursive if bad input.
    static int input_ambiguous(string escape, int min, int max){
        bool did_escape = false;
        string input;
        int selected;
        while(!did_escape){
            input = getQuery(input);
            if(input == escape){
                did_escape = true;
                main_menu();
                break;
            } else {
                try{
                    int i = stoi(input);
                    if(i < min || i > max){
                        cout << "Invalid input. Please try again." << endl;
                        input_ambiguous(escape, min, max);
                        break;
                    } else {
                        selected = i;
                    }
                    
                } catch(const std::exception& e){
                    cout << "Invalid input. Please try again." << endl;
                    input_ambiguous(escape, min, max);
                    break;
                }
            }
            if(!did_escape){
                return selected;
                break;
            }
        }
    }
    /// @brief Method that allows the user to input any character or string that is not included in a list of options.
    /// @param escape The escape character.
    /// @return The user input, parsed as string. Recursive if bad input.
    static string input_ambiguous(string escape){
        bool did_escape = false;
        string input;
        string ret;
        while(!did_escape){
            input = getQuery(input);
            if(input == escape){
                did_escape = true;
                main_menu();
                break;
            } else {
                if(input.length() > 0){
                    ret = input;
                } else {
                    input_ambiguous(escape);
                    break;
                }
            }
            if(!did_escape){
                return ret;
                break;
            }
        }
    }
    /// @brief Print method for a Subscriber object instance.
    /// @param sub The Subscriber object.
    static void print_sub(Subscriber sub){
        cout << "#" + to_string(sub.get_index()) + ": ";
        cout << sub.get_name() + ", ";
        cout << sub.get_address() << endl;
        cout << "Phone number(s) for " + sub.get_name() + ": ";
        for(int i = 0; i < sub.get_codes().size(); i++){
            cout << "+" + sub.get_codes_str()[i] + sub.get_numbers_str()[i] + ", ";
        }
        cout << endl;
    }
public:
    /// @brief Gets user input. No escape character.
    /// @param data Used to distinguish method overloads. Doesn't actually do anything.
    /// @return The user input, parsed as int.
    static int getQuery(int data){
        string input;
        getline(cin, input);
        try{
            int i = stoi(input);
            data = i;
            return data;
        }
        catch(const std::exception& e){
            return -1;
        }
    }
    /// @brief Gets user input. No escape character.
    /// @param data Used to distinguish method overloads. Doesn't actually do anything.
    /// @return The user input, parsed as string.
    static string getQuery(string data){
        getline(cin, data);
        return data;
    }
    /// @brief Gets the user input. No escape character.
    /// @param data vector<string> to add the user input to. I still don't like pointers :(
    /// @return The vector<string> provided as argument, enhanced.
    static vector<string> getQuery(vector<string> data){
        string input;
        getline(cin, input);
        data.push_back(input);
        return data;
    }
    /// @brief Simply prints the text in the given .txt, from ./res/ui/ - used to avoid code bloat with lots of "cout << str << endl;" lines.
    /// @param src Name of the .txt, path and/or extension name excluded.
    static void display(string src){
        string line;
        vector<string> text;
        ifstream ifstr(_dir_path + src + ".txt");
        while(getline(ifstr, line)){
            text.push_back(line);
        }
        for(int i = 0; i < text.size(); i++){
            cout << text[i] << endl;
        }
    }
    /// @brief The main menu. Central navigation unit. B)
    static void main_menu(){
        int input;
        system("cls");
        display("main_menu"); cout << endl;
        cin >> input;

        switch(input){
            case 1:
                new_sub();
                break;
            case 2:
                search_by_index();
                break;
            case 3:
                search_by_string("NAME");
                break;
            case 4:
                search_by_string("PHONE NUMBER");
                break;
            case 5:
                search_by_string("COUNTRY CODE");
                break;
            case 6:
                search_by_string("ADDRESS");
                break;
            case 7:
                list_all();
                break;
            case 9:
                break;
        }
    }
    /// @brief Method for adding a new Subscriber, step-by-step, through the console. Processes user input.
    static void new_sub(){
        vector<string> prompts = {"First Name", "Last Name", "Address", "Country Code", "Phone Number"};
        vector<string> data; 
        //Flushing 'cin' prevents skipping of the user's input.
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        system("cls");
        data = get_multiple_inputs(prompts, "8", data);
        string name = data[0] + " " + data[1]; string address = data[2];
        vector<string> codes = {data[3]}; vector<string> numbers = {data[4]};
        cout << endl; display("new_sub"); cout << endl;
        int input;
        cin >> input;
        Subscriber sub(name, address, codes, numbers);

        switch(input){
            case 1:
                PhoneBook::add(sub);
                main_menu();
                break;
            case 2:
                new_phone(sub);
                break;
            case 3:
                new_sub();
                break;
            case 4:
                main_menu();
                break;
        }
        
    }
    /// @brief Following Menu::new_sub(), provides the user a prompt to append a new country-code/phone-number pair to the Subscriber object before writing it to disk.
    /// @param sub The Subscriber object instance.
    static void new_phone(Subscriber sub){
        int choice;
        vector<string> prompts = {"Country Code", "Phone Number"};
        vector<string> data; vector<string> codes = sub.get_codes_str(); vector<string> numbers = sub.get_numbers_str();
        //Flushing 'cin' prevents skipping of the user's input.
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        system("cls");
        data = get_multiple_inputs(prompts, "#", data);
        
        codes.push_back(data[0]); numbers.push_back(data[1]);
        sub.set_codes(codes); sub.set_numbers(numbers);
        cout << endl; print_sub(sub); cout << endl;
        display("new_phone"); cout << endl;

        cin >> choice;

        switch(choice){
            case 1:
                PhoneBook::add(sub);
                main_menu();
                break;
            case 2:
                new_phone(sub);
                break;
            case 3:
                main_menu();
                break;
        }
    }
    /// @brief Allows the user to select the option to edit any record of any Subscriber, by passing the Subscriber object as an argument, then making use of subsequent methods.
    /// @param sub The subscriber object instance.
    /// @return Subscriber - the object to be edited.
    static Subscriber edit(Subscriber sub){
        int choice;
        system("cls");
        print_sub(sub);
        display("edit");

        cin >> choice;
        switch(choice){
            case 1:
                add_phone(sub);
                break;
            case 2:
                remove_number(sub);
                break;
            case 3:
                edit_name(sub);
                break;
            case 4:
                edit_address(sub);
                break;
            case 5:
                remove(sub);
                break;
            case 9:
                main_menu();
                break;
        }
    }
    /// @brief Method to allow searching for a Subscriber by index through user input.
    static void search_by_index(){
        system("cls");
        int input;
        display("search"); cout << "INDEX" << endl;
        cout << "Enter a subscriber's index (max." + to_string(PhoneBook::get_quantity()) + "):" << endl;
        //Flushing 'cin' prevents skipping of the user's input.
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        input = input_ambiguous("#", 0, PhoneBook::get_quantity());
        edit(PhoneBook::get(input));
    }
    /// @brief Method to allow searching for a Subscriber by file content through user input.
    /// @param type The type of input to be printed out in the console.
    static void search_by_string(string type){
        system("cls");
        string input;
        display("search"); cout << type << endl;
        cout << "Enter a subscriber's index (max." + to_string(PhoneBook::get_quantity()) + "):" << endl;
        //Flushing 'cin' prevents skipping of the user's input.
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        input = input_ambiguous("#");
        edit(PhoneBook::get(input));
    }
    /// @brief Prints out all Subscribers in ./res/subscribers/. Allows selection of one of them for further editing.
    static void list_all(){
        vector<Subscriber> subs = PhoneBook::get_all();
        Subscriber selected;
        for(int i = 0; i < subs.size(); i++){
            print_sub(subs[i]);
        }
        cout << endl;
        //Flushing 'cin' prevents skipping of the user's input.
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        selected = select_sub("#");
        if(selected.get_index() != -1){
            edit(selected);
        }
    }
    /// @brief Allows the user to select one of any Subscriber's phone numbers for deletion. Must be confirmed.
    /// @param sub The Subscriber object instance.
    static void remove_number(Subscriber sub){
        int selected;
        cout << "#" + to_string(sub.get_index()) + ": ";
        cout << sub.get_name() + ", ";
        cout << sub.get_address() << endl;
        cout << "Phone numbers:" << endl;
        for(int i = 0; i < sub.get_codes().size(); i++){
            cout << to_string(i) + ") " + "+" + sub.get_codes_str()[i] + sub.get_numbers_str()[i] << endl;
        }
        cout << endl << "Select a phone number to remove OR enter '#' to return to the main menu." << endl;
        //Flushing 'cin' prevents skipping of the user's input.
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        selected = input_ambiguous("#", 0, sub.get_codes().size());
        vector<string> codes = sub.get_codes_str(); vector<string> numbers = sub.get_numbers_str();
        vector<string>::iterator iter;
        iter = codes.begin() + selected; codes.erase(iter);
        iter = numbers.begin() + selected; numbers.erase(iter);
        sub.set_codes(codes); sub.set_numbers(numbers);
        confirm(sub);
        
    }
    /// @brief Confirm prompt for any operation done on any Subscriber object. Updates said Subscriber's file on the disk.
    /// @param sub The Subscriber object instance.
    static void confirm(Subscriber sub){
        system("cls");
        int choice;
        print_sub(sub);
        display("confirm");
        cin >> choice;
        switch(choice){
            case 1:
                PhoneBook::update(sub);
                main_menu();
                break;
            case 2:
                main_menu();
                break;
            default:
                confirm(sub);
                break;
        }
    }
    /// @brief Deletion prompt for a selected Subscriber. Deletes said Subscriber's file from the disk. Yikes.
    /// @param sub The Subscriber object instance.
    static void remove(Subscriber sub){
        system("cls");
        int choice;
        print_sub(sub);
        cout << "Are you sure you wish to remove this subscriber?" << endl << endl;
        cout << "1) Confirm" << endl; cout << "2) Cancel" << endl;
        cout<< "WARNING: This action is irreversible. Use caution." << endl;
        cin >> choice;
        switch(choice){
            case 1:
                PhoneBook::remove(sub);
                main_menu();
                break;
            case 2:
                main_menu();
                break;
            default:
                remove(sub);
                break;
        }
    }
    /// @brief Provides the user a prompt to append a new country-code/phone-number pair to the Subscriber object. Differs from Menu::new_phone in that it updates an already existing Subscriber's information, instead of creating a new file on the disk.
    /// @param sub The Subscriber object instance.
    static void add_phone(Subscriber sub){
        int choice;
        vector<string> prompts = {"Country Code", "Phone Number"};
        vector<string> data; vector<string> codes = sub.get_codes_str(); vector<string> numbers = sub.get_numbers_str();
        //Flushing 'cin' prevents skipping of the user's input.
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        system("cls");
        data = get_multiple_inputs(prompts, "#", data);
        
        codes.push_back(data[0]); numbers.push_back(data[1]);
        sub.set_codes(codes); sub.set_numbers(numbers);
        cout << endl; print_sub(sub); cout << endl;
        display("new_phone"); cout << endl;

        cin >> choice;

        switch(choice){
            case 1:
                PhoneBook::update(sub);
                main_menu();
                break;
            case 2:
                add_phone(sub);
                break;
            case 3:
                main_menu();
                break;
        }
    }
    /// @brief Gives the user a prompt to overwrite an existing Subscriber object's "name" property. Must be confirmed.
    /// @param sub The Subscriber object instance.
    static void edit_name(Subscriber sub){
        string input;
        system("cls");
        cout << "Edit name for: " << endl;
        print_sub(sub);
        cout << "or enter '#' to go back to the main menu.";
        cout << endl << "Enter a new name:" << endl;
        //Flushing 'cin' prevents skipping of the user's input.
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        input = input_ambiguous("#");
        sub.set_name(input);
        confirm(sub);
    }
    /// @brief Gives the user a prompt to overwrite an existing Subscriber object's "address" property. Must be confirmed.
    /// @param sub The Subscriber object instance.
    static void edit_address(Subscriber sub){
        string input;
        system("cls");
        cout << "Edit address for: " << endl;
        print_sub(sub);
        cout << "or enter '#' to go back to the main menu.";
        cout << endl << "Enter a new address:" << endl;
        //Flushing 'cin' prevents skipping of the user's input.
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        input = input_ambiguous("#");
        sub.set_address(input);
        confirm(sub);
    }
};