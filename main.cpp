/* Encryption Lab
 * CSC4220
 * Authored by Tyler Watson and Ammanuel Beyene
*/

//Imports
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;
using std::cout;
using std::endl;
using std::string;
using std::cin;
using std::ifstream;
using std::stringstream;
using std::ostringstream;

//Method Headers
void ReadFile(string fileName, string& fileContents);
void WriteToFile(string fileContents);
void EncryptString(string originalString, string& encryptedString);
void decryptString(string encryptedString, string& decryptedString);

//Helper Methods


int main(int argc, char* argv[]) {
    bool continueProgram = true;
    while (continueProgram) {
        cout << "===================================" << endl;
        cout << "What option would you like?" << endl;
        cout << "Type '1' to encrypt a file." << endl;
        cout << "Type '2' to decrypt a file." << endl;
        cout << "Type '3' to exit the program." << endl;

        string userInput;
        cin >> userInput;

        if (userInput == "1") {
//            cout << "===================================" << endl;
//            cout << "What is the file name?" << endl;
//            string fileName;
//            cin >> fileName;
//
//            string fileContents;
//            ReadFile(fileName, fileContents);
//            cout << fileContents;

            string fileContents = "hii friendos bla";
            string encryptedString = "";
            EncryptString(fileContents, encryptedString);
        } else if (userInput == "2") {
            //Code for decrypting a file

        } else if (userInput == "3") {
            continueProgram = false;
        } else {
            cout << "Invalid Option, Try Again!" << endl;
        }
    }
}


void EncryptString(string originalString, string& encryptedString) {
    //First make original state matrix
    string state[4][4];
    int indexCounter = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ostringstream oss;
            string hex_representation;
            oss << std::hex << int(originalString[indexCounter]);
            hex_representation = oss.str();
            state[i][j] = hex_representation;
            indexCounter++;
        }
    }

    //ADD ROW KEY LOGIC
    //Later we should make the key matrix, but this is simple for now
    string key[4] = {"c", "62", "61", "50"};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int a = stoi(key[j], nullptr, 16);
            int b = stoi(state[i][j], nullptr, 16);
            ostringstream oss;
            string hex_representation;
            oss << std::hex << int((a+b));
            string hex_sum = oss.str();
            state[i][j] = hex_sum;
        }
    }

    //TODO: WITHIN A 10 rep loop
    //TODO: FILL IN S-BOX *sigh* https://en.wikipedia.org/wiki/Rijndael_S-box
    string SBOX [16][16] = {{"63", "7c", "77", "7b","f2", "6b", "6f", "c5", "30", "01", "67", "2b", "fe", "d7", "ab", "76"},
                            {"ca", "82", "c9", "7d","fa", "59", "47", "f0", "ad", "d4", "a2", "af", "9c", "a4", "72", "c0"},
                            {"b7", "fd", "93", "26","36", "3f", "f7", "cc", "34", "a5", "e5", "f1", "71", "d8", "31", "15"},
                            {"04", "c7", "23", "c3","18", "96", "05", "9a", "07", "12", "80", "e2", "eb", "27", "b2", "75"},
                            {"09", "83", "2c", "1a","1b", "6e", "5a", "a0", "52", "3b", "d6", "b3", "29", "e3", "2f", "84"},
                            {"53", "d1", "00", "ed","20", "fc", "b1", "5b", "6a", "cb", "be", "39", "4a", "4c", "58", "cf"},
                            {"d0", "ef", "aa", "fb","43", "4d", "33", "85", "45", "f9", "02", "7f", "50", "3c", "9f", "a8"},
                            {"51", "a3", "40", "8f","92", "9d", "38", "f5", "bc", "b6", "da", "21", "10", "ff", "f3", "d2"},
                            {"cd", "0c", "13", "ec","5f", "97", "44", "17", "c4", "a7", "7e", "3d", "64", "5d", "19", "73"},
                            {"60", "81", "4f", "dc","22", "2a", "90", "88", "46", "ee", "b8", "14", "de", "5e", "0b", "db"},
                            {"e0", "37", "3a", "0a","49", "06", "24", "5c", "c2", "d3", "ac", "62", "91", "95", "e4", "79"},
                            {"e7", "c8", "37", "6d","8d", "d5", "4e", "a9", "6c", "56", "f4", "ea", "65", "7a", "ae", "08"},
                            {"ba", "78", "25", "2e","1c", "a6", "b4", "c6", "e8", "dd", "74", "1f", "4b", "bd", "8b", "8a"},
                            {"70", "3e", "b5", "66","48", "03", "f6", "0e", "61", "35", "57", "b9", "86", "c1", "1d", "9e"},
                            {"e1", "f8", "98", "11","69", "d9", "8e", "94", "9b", "1e", "87", "e9", "cd", "55", "28", "df"},
                            {"8c", "a1", "89", "0d","bf", "e6", "42", "68", "41", "99", "2d", "0f", "b0", "54", "bb", "16"}};
    //TODO: 1. SUB BYTE STEP


    //TODO: 2. SHIFT ROW STEP

    //TODO: 3. MIX COLUMNS STEP

    //TODO: 4. ADD ROUND KEY

    //Debug
//    for (int i = 0; i < 4; i++) {
//        cout << "[";
//        for (int j = 0; j < 4; j++) {
//            cout << state[i][j] << ", ";
//        }
//        cout << "]" << endl;
//    }

}

//Purpose: takes in the name of a file and reads its contents (if the file exists)
//Returns: returns a string which is representative of the file contents
void ReadFile(string fileName, string& fileContents) {
    ifstream file;
    file.open(fileName);
    if (file.good()){
        string line;
        stringstream result("");
        while(getline(file, line)) {
            result << line;
            result << '\n';
        }
        fileContents = result.str();
    } else {
        cout << fileName << " doesn't exist!" << endl;
        return;
    }
    file.close();
}