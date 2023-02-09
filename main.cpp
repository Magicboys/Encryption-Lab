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
    string SBOX [16][16] = {{"", "", "", "","", "", "", "", "", "", "", "", "", "", "", ""},
                            {"", "", "", "","", "", "", "", "", "", "", "", "", "", "", ""},
                            {"", "", "", "","", "", "", "", "", "", "", "", "", "", "", ""},
                            {"", "", "", "","", "", "", "", "", "", "", "", "", "", "", ""},
                            {"", "", "", "","", "", "", "", "", "", "", "", "", "", "", ""},
                            {"", "", "", "","", "", "", "", "", "", "", "", "", "", "", ""},
                            {"", "", "", "","", "", "", "", "", "", "", "", "", "", "", ""},
                            {"", "", "", "","", "", "", "", "", "", "", "", "", "", "", ""},
                            {"", "", "", "","", "", "", "", "", "", "", "", "", "", "", ""},
                            {"", "", "", "","", "", "", "", "", "", "", "", "", "", "", ""},
                            {"", "", "", "","", "", "", "", "", "", "", "", "", "", "", ""},
                            {"", "", "", "","", "", "", "", "", "", "", "", "", "", "", ""},
                            {"", "", "", "","", "", "", "", "", "", "", "", "", "", "", ""},
                            {"", "", "", "","", "", "", "", "", "", "", "", "", "", "", ""},
                            {"", "", "", "","", "", "", "", "", "", "", "", "", "", "", ""},
                            {"", "", "", "","", "", "", "", "", "", "", "", "", "", "", ""}};
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