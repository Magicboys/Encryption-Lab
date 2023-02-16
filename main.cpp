/* Encryption Lab
 * CSC4220
 * Authored by Tyler Watson and Ammanuel Beyene
*/

//Imports
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <bitset>

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
void WriteToFile(string fileName, string fileContents);
void EncryptString(string originalString, string& encryptedString);
void DecryptString(string encryptedString, string& decryptedString);

//Encryption Step Methods
string** AddRoundKey(string** state, string** roundKey);
string** SubByte(string** state, string fromList);
string** ShiftRows(string** state, string direction);
void HexToBinary(int original, int& binaryValue);
void HexToText(string hexFile, string &stringFile );

//Helper methods
string** KeyScheduler(string** keySchedule);
void HexToBinary(int original, int& binaryValue);
string PruneBinaryNumber(string original);
string BinaryToHex(const string& binaryString);
void PrintArrayDebug(string** state);

//Global Constants
//AES Key
const string AES_KEY [4][4] = {{"73", "73", "69", "72"},
                               {"61", "68", "73", "69"},
                               {"74", "63", "62", "6e"},
                               {"69", "62", "6f", "67"}};


//S-BOX
const string SBOX [16][16] = {{"63","7c","77","7b","f2","6b","6f",	"c5",	"30",	"01",	"67",	"2b",	"fe",	"d7",	"ab",	"76"},
                              {"ca", "82", "c9", "7d", "fa", "59", "47", "f0", "ad", "d4", "a2", "af", "9c", "a4", "72", "c0" },
                              {"b7", "fd", "93", "26", "36", "3f", "f7", "cc", "34", "a5", "e5", "f1", "71", "d8", "31", "15" },
                              {"04", "c7", "23", "c3", "18", "96", "05", "9a", "07", "12", "80", "e2", "eb", "27", "b2", "75" },
                              {"09", "83", "2c", "1a", "1b", "6e", "5a", "a0", "52", "3b", "d6", "b3", "29", "e3", "2f", "84"},
                              {"53", "d1", "00", "ed", "20", "fc", "b1", "5b", "6a", "cb", "be", "39", "4a", "4c", "58", "cf"},
                              {"d0", "ef", "aa", "fb", "43", "4d", "33", "85", "45", "f9", "02", "7f", "50", "3c", "9f", "a8"},
                              {"51", "a3", "40", "8f", "92", "9d", "38", "f5", "bc", "b6", "da", "21", "10", "ff", "f3", "d2" },
                              {"cd", "0c", "13", "ec", "5f", "97", "44", "17", "c4", "a7", "7e", "3d", "64", "5d", "19", "73"},
                              {"60", "81", "4f", "dc", "22", "2a", "90", "88", "46", "ee", "b8", "14", "de", "5e", "0b", "db" },
                              {"e0", "32", "3a", "0a", "49", "06", "24", "5c", "c2", "d3", "ac", "62", "91", "95", "e4", "79" },
                              {"e7", "c8", "37", "6d", "8d", "d5", "4e", "a9", "6c", "56", "f4", "ea", "65", "7a", "ae", "08"},
                              {"ba", "78", "25", "2e", "1c", "a6", "b4", "c6", "e8", "dd", "74", "1f", "4b", "bd", "8b", "8a"},
                              {"70", "3e", "b5", "66", "48", "03", "f6", "0e", "61", "35", "57", "b9", "86", "c1", "1d", "9e" },
                              {"e1", "f8", "98", "11", "69", "d9", "8e", "94", "9b", "1e", "87", "e9", "ce", "55", "28", "df"},
                              {"8c", "a1", "89", "0d", "bf", "e6", "42", "68", "41", "99", "2d", "0f", "b0", "54", "bb", "16"}};



const string ISBOX [16][16] =
        {{"52", "09", "6a", "d5","30", "36", "a5", "38", "bf", "40", "a3", "9e", "81", "f3", "d7", "fb"},
         {"7c", "e3", "39", "82","9b", "2f", "ff", "87", "34", "8e", "43", "44", "c4", "de", "e9", "cb"},
         {"54", "7b", "94", "32", "a6", "c2", "23", "3d", "ee", "4c", "95", "0b", "42", "fa", "c3", "4e" },
         {"08", "2e", "a1", "66", "28", "d9", "24", "b2", "76", "5b", "a2", "49", "6d", "8b", "d1", "25"},
         {"72", "f8", "f6", "64", "86", "68", "98", "16", "d4", "a4", "5c", "cc", "5d", "65", "b6", "92" },
         {"6c", "70", "48", "50", "fd", "ed", "b9", "da", "5e", "15", "46", "57", "a7", "8d", "9d", "84"},
         {"90", "d8", "ab", "00", "8c", "bc", "d3", "0a", "f7", "e4", "58", "05", "b8", "b3", "45", "06" },
         {"d0", "2c", "1e", "8f", "ca", "3f", "0f", "02", "c1", "af", "bd", "03", "01", "13", "8a", "6b" },
         {"3a", "91", "11", "41", "4f", "67", "dc", "ea", "97", "f2", "cf", "ce", "f0", "b4", "e6", "73"},
         {"96", "ac", "74", "22", "e7", "ad", "35", "85", "e2", "f9", "37", "e8", "1c", "75", "df", "6e"},
         {"47", "f1", "1a", "71", "1d", "29", "c5", "89", "6f", "b7", "62", "0e", "aa", "18", "be", "1b" },
         {"fc", "56", "3e", "4b", "c6", "d2", "79", "20", "9a", "db", "c0", "fe", "78", "cd", "5a", "f4" },
         {"1f", "dd", "a8", "33", "88", "07", "c7", "31", "b1", "12", "10", "59", "27", "80", "ec", "5f" },
         {"60", "51", "7f", "a9", "19", "b5", "4a", "0d", "2d", "e5", "7a", "9f", "93", "c9", "9c", "ef" },
         {"a0", "e0", "3b", "4d", "ae", "2a", "f5", "b0", "c8", "eb", "bb", "3c", "83", "53", "99", "61" },
         {"17", "2b", "04", "7e", "ba", "77", "d6", "26", "e1", "69", "14", "63", "55", "21", "0c", "7d"}};




int main(int argc, char* argv[]) {
    bool continueProgram = true;

    while (continueProgram) {
        cout << "===================================" << endl;
        cout << "What option would you like?" << endl;
        cout << "Type '1' to AES Encrypt a file." << endl;
        cout << "Type '2' to AES Decrypt a file." << endl;
        cout << "Type '3' to exit the program. " << endl;

        string userInput;
        cin >> userInput;

        if (userInput == "1") {
            cout << "===================================" << endl;
            cout << "What is the file name?" << endl;
            string fileName;
            cin >> fileName;

            string fileContents;
            ReadFile(fileName, fileContents);

            //Loop through fileContents and encrypt characters in batches of 16
            string encryptedFile = "";
            string encryptedString = "";
            int len = fileContents.length();
            if(fileContents.at(len-1) == '\r' ||fileContents.at(len-1) == '\n'){
                len-= 1;
            }
            for (int i  = 0; i < len; i += 16) {

                string localSubstring = "";
                int diff = fileContents.length()-i;
                if(diff >= 16){
                    localSubstring = fileContents.substr(i, 16);
                }else {
                    localSubstring = fileContents.substr(i, diff);
                }

                int len = localSubstring.length();
                if(localSubstring.length() < 16) {
                    for (int j = 0; j < (16 - len); j++) {
                        localSubstring += " ";
                    }
                }


                //Search for illegal characters
                string localEncryptedResult = "";
                EncryptString(localSubstring, localEncryptedResult);
                encryptedFile += localEncryptedResult;
            }

            cout << "Encrypted File:" << endl;
            cout << encryptedFile << endl;

            WriteToFile( "encrypted_file.txt", encryptedFile);

        } else if (userInput == "2") {
            //Code for decrypting a file


            cout << "===================================" << endl;
            string fileContents;
            ReadFile("encrypted_file.txt", fileContents);

            //Loop through fileContents and encrypt characters in batches of 16
            string decryptedFile = "";
            for (int i  = 0; i < fileContents.length()-1; i += 32) {

                string localSubstring = fileContents.substr(i, 32);

                //Search for illegal characters
                string localEncryptedResult = "";
                DecryptString(localSubstring, localEncryptedResult);
                decryptedFile += localEncryptedResult;

            }

            //Encrypted file contents
            cout << decryptedFile << endl;
            string stringFile = "";
            HexToText( decryptedFile, stringFile );
            WriteToFile("decrypted_file.txt", stringFile);


        } else if (userInput == "3") {
            continueProgram = false;
        } else {
            cout << "Invalid Option, Try Again!" << endl;
        }
    }
}

/*
 * ENCRYPTION METHODS
 * */

//Takes in the original 16 character string and returns an encrypted 16 character string
void EncryptString(string originalString, string& encryptedString) {
    //First make original state matrix
    int rows = 4;
    string** state = new string*[rows];
    for (int i = 0; i < rows; ++i) {
        state[i] = new string[4];
    }

    int indexCounter = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ostringstream oss;
            string hex_representation;
            oss << std::hex << int(originalString[indexCounter]);
            hex_representation = oss.str();
            state[i][j] = hex_representation;
            cout<<state[i][j]<<" ";
            indexCounter++;
        }
        cout<<endl;
    }
    cout<<endl;



    //Allocate Key Data Structures
    string** keySchedule = new string*[4];
    for (int i = 0; i < 4; ++i) {
        keySchedule[i] = new string[44];
    }
    string** roundKey = new string*[4];
    for (int i = 0; i < 4; ++i) {
        roundKey[i] = new string[4];
    }

    //Generate Key Schedule
    keySchedule = KeyScheduler(keySchedule);




    //Before Rounds perform AddRoundKey
    for (int i = 0; i < 4; i++) {
        for (int j  = 0; j < 4; j++) {
            roundKey[i][j] = keySchedule[i][j];
        }
    }



    state = AddRoundKey(state, roundKey);



    cout << "INITIAL ADD ROUND KEY COMPLETE" << endl;
    //Rounds 0-9
    for (int round = 0; round < 10; round++) {
        //Fetch round key
        int currCol = (round+1) * 4;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                roundKey[i][j] = keySchedule[i][currCol+j];
            }
        }


        //Steps
        state = SubByte(state, "SBOX");
        state = ShiftRows(state, "Left");
        state = AddRoundKey(state, roundKey);

        cout << "ROUND " << (round+1) << " COMPLETE" << endl;
        PrintArrayDebug(state);
    }

    //Cipher Text
    string returnString = "";

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            returnString += state[i][j];
        }
    }
    encryptedString = returnString;

    //Deallocate arrays
    for (int i = 0; i < rows; i++) {
        delete [] state[i];
    }
    delete [] state;

    for (int i = 0; i < 4; i++) {
        delete [] keySchedule[i];
    }
    delete [] keySchedule;

    for (int i = 0; i < 4; i++) {
        delete [] roundKey[i];
    }
    delete [] roundKey;
}

//Takes in the original 16 character string and returns an encrypted 16 character string
void DecryptString(string encryptedString, string& decryptedString) {
    //First make original state matrix
    int rows = 4;
    string** state = new string*[rows];
    for (int i = 0; i < rows; ++i) {
        state[i] = new string[4];
    }

    int indexCounter = 0;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            string x (1, encryptedString.at(indexCounter));
            indexCounter++;
            string y (1, encryptedString.at(indexCounter));
            indexCounter++;
            state[i][j] = x+y;
        }
    }


    //indexCounter = 0;

    //Allocate Key Data Structures
    string** keySchedule = new string*[4];
    for (int i = 0; i < 4; ++i) {
        keySchedule[i] = new string[44];
    }
    string** roundKey = new string*[4];
    for (int i = 0; i < 4; ++i) {
        roundKey[i] = new string[4];
    }

    //Generate Key Schedule
    keySchedule = KeyScheduler(keySchedule);

    //Before Rounds perform AddRoundKey
    for (int i = 0; i < 4; i++) {
        for (int j  = 0; j < 4; j++) {
            roundKey[i][j] = keySchedule[i][j+40];
        }
    }



    cout << "INITIAL ADD ROUND KEY COMPLETE" << endl;
    int currCol = 0;
    //Rounds 0-9
    for (int round = 9; round >= 0; round--) {
        //Fetch round key
        currCol = (round+1) * 4;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                roundKey[i][j] = keySchedule[i][currCol+j];
            }
        }

        state = AddRoundKey(state, roundKey);
        state = ShiftRows(state, "Right");
        state = SubByte(state, "ISBOX");

        cout << "ROUND " << (round+1) << " COMPLETE" << endl;
    }



    //Before Rounds perform AddRoundKey
    for (int i = 0; i < 4; i++) {
        for (int j  = 0; j < 4; j++) {
            roundKey[i][j] = keySchedule[i][j];
        }
    }

    state = AddRoundKey(state, roundKey);


    //Cipher Text
    string returnString = "";

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            returnString += state[i][j];
        }
    }


    decryptedString = returnString;



    //Deallocate arrays
    for (int i = 0; i < rows; i++) {
        delete [] state[i];
    }
    delete [] state;

    for (int i = 0; i < 4; i++) {
        delete [] keySchedule[i];
    }
    delete [] keySchedule;

    for (int i = 0; i < 4; i++) {
        delete [] roundKey[i];
    }
    delete [] roundKey;


}

//Performs key scheduling for sub-round key creation via utilizing the programs AES key
string** KeyScheduler(string** keySchedule) {
    //Copy over AES key for columns W0-W3
    for (int i = 0 ; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            keySchedule[i][j] = AES_KEY[i][j];
        }
    }

    //Perform key expansion steps to generate new keys in columns W4-W44;
    int currentCol = 4;
    while (currentCol < 44) {
        //Copy over columns and shift the bits left one index
        string columnOne[4] = {keySchedule[1][(currentCol-4)], keySchedule[2][(currentCol-4)], keySchedule[3][(currentCol-4)], keySchedule[0][(currentCol-4)]};
        string columnTwo[4] = {keySchedule[1][(currentCol-1)], keySchedule[2][(currentCol-1)], keySchedule[3][(currentCol-1)], keySchedule[0][(currentCol-1)]};

        //Perform SubByte with each bit
        for (int i = 0; i < 4; i++) {
            int columnOneX = stoi(columnOne[i].substr(0,1), nullptr, 16);
            int columnOneY = stoi(columnOne[i].substr(1,1), nullptr, 16);
            int columnTwoX = stoi(columnTwo[i].substr(0,1), nullptr, 16);
            int columnTwoY = stoi(columnTwo[i].substr(1,1), nullptr, 16);
            columnOne[i] = SBOX[columnOneX][columnOneY];
            columnTwo[i] = SBOX[columnTwoX][columnTwoY];
        }

        //XOR columnOne and columnTwo then save result
        for (int i = 0; i < 4; i++) {
            int columnOneOriginalHex = stoi(columnOne[i], nullptr, 16);
            int columnTwoOriginalHex = stoi(columnTwo[i], nullptr, 16);
            int colOneBinary = 0;
            int colTwoBinary = 0;

            HexToBinary(columnOneOriginalHex, colOneBinary);
            HexToBinary(columnTwoOriginalHex, colTwoBinary);

            string colOneBinaryString = std::to_string(colOneBinary);
            string colTwoBinaryString = std::to_string(colTwoBinary);

            string xorResult = "";
            for (int j = 0; j < 8; j++) {
                if ((colOneBinaryString[j] == '1') && (colTwoBinaryString[j] == '1')) {
                    xorResult += '0';
                } else if ((colOneBinaryString[j] == '0') && (colTwoBinaryString[j] == '0')) {
                    xorResult += '0';
                } else {
                    xorResult += '1';
                }
            }

            //Save new value to the new column
            keySchedule[i][currentCol] =  BinaryToHex(xorResult);
        }
        currentCol++;
    }

    return keySchedule;
}




//Encryption AddRoundKey Step
string** AddRoundKey(string** state, string** roundKey) {

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
           // Convert both columns from hex to binary strings
            int aBinary = 0;
            int bBinary = 0;
            HexToBinary(stoi(roundKey[j][i], nullptr, 16), aBinary);
            HexToBinary(stoi(state[i][j], nullptr, 16), bBinary);

            string aBinaryString = to_string(aBinary);
            string bBinaryString = to_string(bBinary);
            aBinaryString = PruneBinaryNumber(aBinaryString);
            bBinaryString = PruneBinaryNumber(bBinaryString);

            //Perform XOR of both binary string
            string xorResult = "";
            for (int j = 0; j < 8; j++) {
                if ((aBinaryString[j] == '1') && (bBinaryString[j] == '1')) {
                    xorResult += '0';
                } else if ((aBinaryString[j] == '0') && (bBinaryString[j] == '0')) {
                    xorResult += '0';
                } else {
                    xorResult += '1';
                }
            }
            string hex_representation = BinaryToHex(xorResult);
            state[i][j] = hex_representation;
        }
    }
    return state;
}

//Encryption Sub Byte Step
string** SubByte(string** state, string fromList) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int x = stoi(state[i][j].substr(0,1), nullptr, 16);
            int y = stoi(state[i][j].substr(1,1), nullptr, 16);
            if(fromList == "SBOX") {
                state[i][j] = SBOX[x][y];
            }else{
                state[i][j] = ISBOX[x][y];
            }
        }
    }
    return state;
}

//Encryption Shift Rows Step
string** ShiftRows(string** state, string direction) {
    for (int i = 0; i < 4; i++) {
        //Grab original row
        string originalRow[4];
        for (int j = 0; j < 4; j++) {
            originalRow[j] = state[i][j];
        }
        //Shift the bytes
        string shiftedRow[4];
        int shiftedIndex = 0;
        if(direction == "Left") {
            shiftedIndex = (i);
        }else{
            shiftedIndex = (i*3)%4;
        }
        for (int j = 0; j < 4; j++) {
            shiftedRow[j] = originalRow[shiftedIndex];
            shiftedIndex++;
            if (shiftedIndex >= 4) {
                shiftedIndex = 0;
            }
        }
        //Save the shifted row
        for (int j = 0; j < 4; j++) {
            state[i][j] = shiftedRow[j];
        }
    }
    return state;
}


/*
 * UTILITY METHODS BELOW
 * */

//Prints the state array to console
void PrintArrayDebug(string** state) {
    cout << "=========================" << endl;
    for (int i = 0; i < 4; i++) {
        cout << "[";
        for (int j = 0; j < 4; j++) {
            cout << state[i][j] << ", ";
        }
        cout << "]" << endl;
    }
}

// Hexadecimal to binary conversion
void HexToBinary(int original, int& binaryValue) {

    binaryValue = 0;
    int z = 1;
    int remainder;
    int n = original;
    while(n!=0)
    {
        remainder = n % 2;
        n = n / 2;
        binaryValue = binaryValue+remainder * z;
        z=z*10;
    }


}

//Binary string to hex conversion
string BinaryToHex(const string& binaryString) {
    string hex;
    for (size_t i = 0; i < binaryString.length(); i += 4) {
        std::bitset<4> nibble(binaryString.substr(i,4));
        hex += nibble.to_ulong() < 10 ? char(nibble.to_ulong() + '0') : char(nibble.to_ulong()-10 + 'A');
    }
    return hex;
}

void HexToText(string hexString, string &stringFile ){

    string hexToString = "";
    for (size_t i = 0; i < hexString.length(); i += 2){
        string part = hexString.substr(i, 2);
        char  hexToChar = stoul(part, nullptr, 16);
        hexToString += hexToChar;
    }

    stringFile = hexToString;


}


//Prune binary numbers that may have values that may have other numbers than 1's or 0's from a primitive operation
//Also verifies that the binary string has 8 characters
string PruneBinaryNumber(string original) {
    string prunedPolyProduct = "";
    for (int k = 0; k < (8 - original.length()); k++) {
        prunedPolyProduct += '0';
    }
    prunedPolyProduct += original;

    //Make sure there are no 2s
    for (int k = 0; k < prunedPolyProduct.length(); k++) {
        if (prunedPolyProduct[k] == '2') {
            prunedPolyProduct.replace(k, 1, "0");
        }
    }
    return prunedPolyProduct;
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

void WriteToFile(string fileName, string fileContents) {
    ofstream file;
    file.open(fileName);
    if (file.good()){
        string line;
        file<<fileContents;
    } else {
        cout << fileName << " doesn't exist!" << endl;
        return;
    }
    file.close();
}