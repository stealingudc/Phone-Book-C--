#pragma once

#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include "Subscriber.h"

using namespace std;
namespace fs = std::filesystem;

string dir_path = "./res/subscribers/";

class PhoneBook{
private:
    /// @brief | Private member: PhoneBook.h | Gets all files in "./res/subscribers/".
    /// @return vector<string> of all file paths.
    static vector<string> get_files(){
        vector<string> files;
        for(const auto &entry : fs::directory_iterator(dir_path)){
            //cout << entry.path().string() << endl;
            files.push_back(entry.path().string());
        }
        return files;
    }
    /// @brief | Private member: PhoneBook.h | Gets the first unused index of the whole list of subscribers. Acts as a failsafe for PhoneBook::update_indexes().
    /// @return int - the first unused index.
    static int get_index(){
        /*
            I have no idea how this works. All I did was trial and error. It just works - good enough. I'm tired, boss.
        */
        vector<string> files = get_files();
        vector<int> all_indexes;
        int n = 0;
        int index = 0;
        for(int i = 0; i < files.size(); i++){
            string path_cut = files[i].replace(files[i].find("."), 18, "");
            int remainder = stoi(path_cut.replace(path_cut.find("."), 4, ""));
            
            all_indexes.push_back(remainder);
        }
        for(int i = 0; i < all_indexes.size(); i++){
            if(n > all_indexes.size()){
                index = 0;
                break;
            };
            if(all_indexes[i] == n){
                n+=1;
            } else {
                index = n;
            }
        }
        if(index == 0){
            index = n;
        }
        return index;
    }
    /// @brief Upon being called, copies the data in all of the files to memory, then rewrites the files with new, ordered indexes. Might be an issue on a massive scale - has not been tested on large numbers.
    static void update_indexes(){
        vector<string> file_paths = get_files();
        vector<vector<string>> file_instances;
        ifstream ifs;
        for(int i = 0; i < file_paths.size(); i++){
            vector<string> data;
            string line;
            ifs.open(file_paths[i]);
            while(getline(ifs, line)){
                data.push_back(line);
            }
            file_instances.push_back(data);
            ifs.close();
        }
        for(int i = 0; i < file_instances.size(); i++){
            file_instances[i][0] = to_string(i);
            fs::remove(file_paths[i]);
            ofstream ofs(dir_path + to_string(i) + ".txt");
            for(int j = 0; j < file_instances[i].size(); j++){
                ofs << file_instances[i][j] << endl;
            }
            ofs.close();
        }
    }
    
public:
    /// @brief Iterates through all the files in ./res/subscribers/ to find the one that matches the index.
    /// @param index The index to match.
    /// @return The path to the file as a string.
    static string find(int index){
        vector<string> files = get_files();
        string file;
        bool found = false;
        while(found == false){
            for(int i = 0; i < files.size(); i++){
                if(files[i].find(to_string(index)) != string::npos){
                    found = true;
                    file = files[i];
                    break;
                }
            }
        }
        return file;
    }
    /// @brief Iterates through all the files in ./res/subscribers/ and checks their contents to see if it finds a match to the provided string.
    /// @param str The string to match.
    /// @return The path to the file as a string.
    static string find(string str){
        vector<string> files = get_files();
        string file;
        bool found = false;
        ifstream ifstr;
        string line;

        for(int i = 0; i < files.size(); i++){
            ifstr.open(files[i]);
            while(found == false && getline(ifstr, line)){
                if(line.find(str) != string::npos){
                    found = true;
                    file = files[i];
                    break;
                }
            }
            ifstr.close();
        }
        return file;
    }
    /// @brief Internally uses the PhoneBook::find() method to get a file based on the index provided. Reads the file, returns an instance of Subscriber based on the data read.
    /// @param ind index
    /// @return A Subscriber object instance.
    static Subscriber get(int ind){
        string file = find(ind);
        ifstream ifstr(file);
        string line;
        vector<string> data;
        vector<string> codes; vector<string> numbers;
        while(getline(ifstr, line)){
            data.push_back(line);
        }
        int index = stoi(data[0]); string name = data[1]; string address = data[2];
        for(int i = 3; i < data.size(); i+=2){
            codes.push_back(data[i]);
            numbers.push_back(data[i+1]);
        }
        return Subscriber(index, name, address, codes, numbers);
    }
    /// @brief Internally uses the PhoneBook::find() method to get a file based on the string provided. Reads the file, returns an instance of Subscriber based on the data read.
    /// @param str string
    /// @return A Subscriber object instance.
    static Subscriber get(string str){
        string file = find(str);
        ifstream ifstr(file);
        string line;
        vector<string> data;
        vector<string> codes; vector<string> numbers;
        while(getline(ifstr, line)){
            data.push_back(line);
        }
        int index = stoi(data[0]); string name = data[1]; string address = data[2];
        for(int i = 3; i < data.size(); i+=2){
            codes.push_back(data[i]);
            numbers.push_back(data[i+1]);
        }
        return Subscriber(index, name, address, codes, numbers);
    }
    /// @brief Runs get() for each of the files in "./res/subscribers".
    /// @return vector<Subscriber>
    static vector<Subscriber> get_all(){
        vector<string> files = get_files();
        vector<Subscriber> ret;
        for(int i = 0; i < files.size(); i++){
            Subscriber sub = get(i);
            ret.push_back(sub);
        }
        return ret;
    }
    /// @brief Adds a subscriber to the quote-unquote database by appending the data in the Subscriber object to a .txt file.
    /// @param sub The Subscriber object instance.
    static void add(Subscriber sub){
        int index = get_index();
        ofstream ofstr(dir_path + to_string(index) + ".txt");
        sub.set_index(index);
        ofstr << sub.get_index() << "\n"; ofstr << sub.get_name() << "\n"; ofstr << sub.get_address() << "\n";
        for(int i = 0; i < sub.get_codes().size(); i++){
            ofstr << sub.get_codes()[i] << "\n";
            ofstr << sub.get_numbers()[i] << "\n";
        }
        ofstr.close();
    }
    /// @brief Removes the file of the same index provided from ./res/subscribers/, therefore also removing any record of said subscriber. Irreversible.
    /// @param index Index to be matched.
    static void remove(int index){
        string file = find(index);
        if(file.length() > 0){
            fs::remove(file);
            update_indexes();
        }
    }
    /// @brief Removes the file of the same index as the Subscriber instance provided from ./res/subscribers/, therefore also removing any record of said subscriber. Irreversible.
    /// @param sub Subscriber object instance for their index to be matched. Index of the object must be non-null.
    static void remove(Subscriber sub){
        string file = find(sub.get_index());
        if(file.length() > 0){
            fs::remove(file);
            update_indexes();
        }
    }
    /// @brief Removes the record for the existing sub, whilst also adding a new record of the data provided in the Subscriber object instance.
    /// @param sub The Subscriber object instance.
    static void update(Subscriber sub){
        remove(sub);
        add(sub);
    }
    static int get_quantity(){
        vector<string> files = get_files();
        int ret;
        for(int i = 0; i < files.size(); i++){
            ret = i;
        }
        return ret;
    }
};