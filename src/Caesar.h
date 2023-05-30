#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

string alphabet = "abcdefghijklmnopqrstuvwxyz";
string path = "./res/";

class Caesar{
public:
    /// @brief Standard Caesar Cipher encryption. Does not accept numbers or special characters. Takes every letter of the provided string and shifts it to the left by 'x' letters of the alphabet, where 'x' is the provided shift.
    /// @param str The string to encrypt.
    /// @param shift The shift.
    /// @return The encrypted string.
    static string encrypt(string str, int shift){
        string ret;
        if(shift < 26){
            for (int i = 0; i < str.length(); i++){
                int original_position;
                if(i <= alphabet.length()){
                original_position = alphabet.find(str[i]);
                } else if (i > alphabet.length()){
                    original_position = alphabet.find(str[i - 25]);
                }
                ret.push_back(alphabet[original_position - shift]);
            }
        }
        return ret;
    };
    /// @brief Reverse operation of the Caesar::encrypt() method. Check that for more info.
    /// @param str The string to decrypt.
    /// @param shift The shift.
    /// @return The decrypted string.
    static string decrypt(string str, int shift){
        string ret;
        if(shift < 26){
            for (int i = 0; i < str.length(); i++){
                int original_position;
                if(i <= alphabet.length()){
                original_position = alphabet.find(str[i]);
                } else if (i > alphabet.length()){
                    original_position = alphabet.find(str[i - 25]);
                }
                ret.push_back(alphabet[original_position + shift]);
            }
        }
        return ret;
    };
    /// @brief Compares the user input to the decrypted password found in "./res/password.txt".
    /// @param input User input. Get with cin.
    /// @return 0 -> no match || 1 -> match
    static bool verify(string input){
        ifstream ifstr(path + "password.txt");
        string line;
        vector<string> data;
        while(getline(ifstr, line)){
            data.push_back(line);
        }
        ifstr.close();
        int shift = stoi(data[0]);
        string pass = data[1];
        if(decrypt(pass, shift) == input){
            return true;
        } else return false;
    }
};