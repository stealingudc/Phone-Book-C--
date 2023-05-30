#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

class Subscriber{
private:
    int index;
    string name, address;
    vector<int> country_codes;
    vector<long> phone_numbers;
    
    /// @brief | Private member: Subscriber.h | Parses any country codes of type string as type int. Argument must be a vector<string>. Non-int values return -1.
    /// @param strs vector<string> - The vector to be parsed.
    /// @return vector<int> - The parsed vector<string> as vector<int>.
    static vector<int> parse_codes(vector<string> strs){
        vector<int> ret;
        for(int i = 0; i < strs.size(); i++){
            try{
                if(strs[i].find("+") != string::npos){
                    ret.push_back(stoi(strs[i].replace(strs[i].find("+"), 1, "")));
                } else {
                    ret.push_back(stoi(strs[i]));
                }
            }
            catch(const std::exception& e){
                ret.push_back(-1);
            } 
        }
        return ret;
    }
    /// @brief | Private member: Subscriber.h | Parses any phone numbers of type string as type long. Argument must be a vector<string>. Non-long values return -1.
    /// @param strs vector<string> - The vector to be parsed.
    /// @return vector<long> - The parsed vector<string> as vector<long>.
    static vector<long> parse_numbers(vector<string> strs){
        vector<long> ret;
        for(int i = 0; i < strs.size(); i++){
            try{
                long parsed = stol(strs[i]);
                ret.push_back(parsed);
            }
            catch(const std::exception& e){
                ret.push_back(-1);
            }
        }
        return ret;
    }

public:
        /*Constructors*/
    Subscriber(){};
    Subscriber(string nm, string ad, vector<int> cds, vector<long> nrs){
        name = nm; address = ad; country_codes = cds; phone_numbers = nrs;
    };
    Subscriber(string nm, string ad, vector<string> cds, vector<string> nrs){
        name = nm; address = ad; country_codes = parse_codes(cds); phone_numbers = parse_numbers(nrs);
    };
    Subscriber(int in, string nm, string ad, vector<int> cds, vector<long> nrs){
        index = in; name = nm; address = ad; country_codes = cds; phone_numbers = nrs;
    };
    Subscriber(int in, string nm, string ad, vector<string> cds, vector<string> nrs){
        index = in; name = nm; address = ad; country_codes = parse_codes(cds); phone_numbers = parse_numbers(nrs);
    };
        /*Getters*/
    int get_index(){return index;}
    string get_name(){return name;}
    string get_address(){return address;}
    vector<int> get_codes(){return country_codes;}
    /// @brief Runs to_string() on every country code contained in the class and pushes them to a new vector.
    /// @return The "stringified" country codes as a vector.
    vector<string> get_codes_str(){
        vector<string> ret;
        vector<int> codes = get_codes();
        for(int i = 0; i < codes.size(); i++){
            ret.push_back(to_string(codes[i]));
        }
        return ret;
    }
    vector<long> get_numbers(){return phone_numbers;}
    /// @brief Runs to_string() on every phone number contained in the class and pushes them to a new vector.
    /// @return The "stringified" phone numbers as a vector.
    vector<string> get_numbers_str(){
        vector<string> ret;
        vector<long> numbers = get_numbers();
        for(int i = 0; i < numbers.size(); i++){
            ret.push_back(to_string(numbers[i]));
        }
        return ret;
    }
        /*Setters*/
    void set_index(int ind){index = ind;}
    void set_name(string nm){name = nm;}
    void set_address(string ad){address = ad;}
    void set_codes(vector<int> cds){country_codes = cds;}
    void set_codes(vector<string> cds){country_codes = parse_codes(cds);}
    void set_numbers(vector<long> nrs){phone_numbers = nrs;}
    void set_numbers(vector<string> nrs){phone_numbers = parse_numbers(nrs);}
};