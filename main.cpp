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
void WriteToFile(string fileContents);
void EncryptString(string originalString, string& encryptedString);
void decryptString(string encryptedString, string& decryptedString);

//Encryption Step Methods
string** AddRoundKey(string** state, string** roundKey);
string** SubByte(string** state);
string** ShiftRows(string** state);
string** MixedColumns(string** state);

//Helper methods
string** KeyScheduler(string** keySchedule);
void HexToBinary(int original, int& binaryValue);
string BinaryToHex(const string& binaryString);
string IrreduciblePolynomialTheorem(string original);
void PrintArrayDebug(string** state);
string GalosisFieldBinaryMultiplication(unsigned int a, unsigned int b);

//Global Constants
//AES Key
const string AES_KEY [4][4] = {{"73", "73", "69", "72"},
                               {"61", "68", "73", "69"},
                               {"74", "63", "62", "6e"},
                               {"69", "62", "6f", "67"}};

//S-BOX
const string SBOX [16][16] = {{"63", "7c", "77", "7b","f2", "6b", "6f", "c5", "30", "01", "67", "2b", "fe", "d7", "ab", "76"},
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


int main(int argc, char* argv[]) {
    bool continueProgram = true;

    //TODO: Remove test case
    IrreduciblePolynomialTheorem("111100000000");

    /*
    while (continueProgram) {
        cout << "===================================" << endl;
        cout << "What option would you like?" << endl;
        cout << "Type '1' to AES Encrypt a file." << endl;
        cout << "Type '2' to AES Decrypt a file." << endl;
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
            //TODO: Loop through file and pass file contents to be encrypted in 16 character batches
            EncryptString(fileContents, encryptedString);
            cout << encryptedString << endl;
        } else if (userInput == "2") {
            //Code for decrypting a file

        } else if (userInput == "3") {
            continueProgram = false;
        } else {
            cout << "Invalid Option, Try Again!" << endl;
        }
    }
     */
}


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
            indexCounter++;
        }
    }

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
    cout << "===DEBUG=====" << endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 44; j++) {
            cout << keySchedule[i][j] << " ";
        }
        cout << endl;
    }
    cout << "===========" << endl;

    //Before Rounds perform AddRoundKey
    for (int i = 0; i < 4; i++) {
        for (int j  = 0; j < 4; j++) {
            roundKey[i][j] = keySchedule[i][j];
            cout << roundKey[i][j] << " ";
        }
        cout << endl;
    }
    state = AddRoundKey(state, roundKey);
    cout << "INITIAL ADD ROUND KEY COMPLETE" << endl;

    //Rounds 0-9
    for (int round = 0; round < 9; round++) {
        //Fetch round key
        int currCol = (round+1) * 4;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                roundKey[i][j] = keySchedule[j][currCol+i];
            }
        }

//        cout << "ROUND: " << (round+1) << " DEBUG" << endl;
//        for (int i = 0; i < 4; i++) {
//            for (int j = 0; j < 4; j++) {
//                cout << roundKey[i][j] << " ";
//            }
//            cout << endl;
//        }

        //Steps
        state = SubByte(state);
        state = ShiftRows(state);
        //state = MixedColumns(state);
//        state = AddRoundKey(state, roundKey);
        cout << "ROUND " << (round+1) << " COMPLETE" << endl;
        PrintArrayDebug(state);
    }

    //Round 10
    //Fetch Round 10 Key
    int currCol = (9) * 4;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            roundKey[i][j] = keySchedule[j][currCol+i];
        }
    }
    //Round 10 Steps
    state = SubByte(state);
    state = ShiftRows(state);
    state = AddRoundKey(state, roundKey);
    cout << "ROUND 10 COMPLETE" << endl;

    //Cipher Text
    string returnString = "";

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            returnString += state[i][j] + " ";
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

string** MixedColumns(string** state) {
    string mixColKey[4][4] = {{"02", "03", "01", "01"},
                              {"01", "02", "03", "01"},
                              {"01", "01", "02", "03"},
                              {"03", "01", "01", "02"}};

    for (int i = 0; i < 4; i++) {
        //Grab original col and turn into a binary number
        int originalCol[4];
        int mixColumnKeys[4];
        string multipliedResults[4];
        string mixedCol[4];

        //Perform the DOT product and do GalosisField binary multiplication for the binary multiplication
        for (int k = 0; k < 4; k++) {
            for (int j = 0; j < 4; j++) {
                originalCol[j] = stoi(state[j][i], nullptr, 16);
                mixColumnKeys[j] = stoi(mixColKey[k][j], nullptr, 16);
                int mixColBinary = 0;
                int originalColBinary = 0;

                HexToBinary(originalCol[j], originalColBinary);
                HexToBinary(mixColumnKeys[j], mixColBinary);
                multipliedResults[j] = GalosisFieldBinaryMultiplication(mixColBinary, originalColBinary);
            }

            //Do XOR for the four multiplied binary products
            string binaryXORResult = "";
            for (int z = 0; z < 8; z++) {
                int oneCounter = 0;
                for (int q = 0; q < 4; q++) {
                    if (multipliedResults[q][z] == '1') {
                        oneCounter++;
                    }
                }
                if ((oneCounter % 2) == 0) {
                    binaryXORResult += '0';
                } else {
                    binaryXORResult += '1';
                }
            }
            mixedCol[k] = BinaryToHex(binaryXORResult);
        }
        for (int j = 0; j < 4; j++) {
            state[j][i] = mixedCol[j];
        }
    }
    return state;
}

// Galois Field (2^8) Binary Multiplication
string GalosisFieldBinaryMultiplication(unsigned int numOne, unsigned int numTwo) {
    string product = std::to_string((numOne*numTwo));
    string newProduct = "";
    for (int i = 0; i < product.length(); i++) {
        if (product[i] == '0') {
            newProduct += product[i];
        } else if (product[i] == '1') {
            newProduct += product[i];
        } else {
            if ((std::stoi(std::to_string(product[i])) % 2) == 0) {
                newProduct += "0";
            } else {
                newProduct += "1";
            }
        }
    }

    //Store return value in the result variable
    string result = "";

    //If the binary number is overflowed then apply the irreducible polynomial theorem GF(2^3)
    if (stoi(newProduct) > 11111111) {
        newProduct = newProduct.substr(1,8);
        string polyTheorem = "00011011"; //x^4+x^3+x+1
        string result = "";
        for (int i = 0; i < 8; i++) {
            if ((polyTheorem[i] == '1') && (newProduct[i] == '1')) {
                result += "0";
            } else if (((polyTheorem[i] == '0') && (newProduct[i] == '1')) || ((polyTheorem[i] == '1') && (newProduct[i] == '0'))) {
                result += "1";
            } else {
                result += "0";
            }
        }

        if (result.length() != 8) {
            string temp = "";
            for(int i = 0; i <= (8-result.length()); i++) {
                temp += "0";
            }
            temp += result;
            result = temp;
        }
        return result;
    } else {
        if (newProduct.length() < 8) {
            string temp = "";
            for(int i = 0; i < (8-newProduct.length()); i++) {
                temp += "0";
            }
            temp += newProduct;
            newProduct = temp;
        }
        return newProduct;
    }
}

//Takes a binary string larger then 256 and reduces it via irreducibile polynomial formula GF(2^3)
string IrreduciblePolynomialTheorem(string original) {
    //Save the first 8 bits & get overflowed section
//    cout << "length: " << original.length() << endl;
    string overflowed = original.substr(0,original.length()-8);
    original = original.substr(original.length()-8,8);

    cout << "Original: " << original << endl;
    cout << "Overflowed: " << overflowed << endl;
//    cout << "Overflowed length: " << overflowed.length() << endl;

    int degree = 1;
    //string polyTheoremConstant = "00011011"; //x^4+x^3+x+1
    string polyTheoremConstant = "00011011";
    string degreeBinary = "";
    string resultsToAddToOriginal[8] = {"","", "", "", "", "", "", ""};
    for (int i = overflowed.length()-1; i >= 0; i--) {
//        cout << "DEGREE: " << degree << endl;
        if (overflowed[i] == '1') {
            degreeBinary = '1';
            for (int k = 0; k < degree-1; k++) {
                degreeBinary += '0';
            }
//            cout << "OVERFLOW X: " << degreeBinary << endl;
//            cout << "CONSTANT: " << polyTheoremConstant << endl;
            string polyTheoremProduct = std::to_string((stoi(polyTheoremConstant)*stoi(degreeBinary)));
//            cout << "POLY THEROEM PRODUCT:" << polyTheoremProduct << endl;


            if (polyTheoremProduct.length() <= 8) {
                string polyTheroemProduct =  std::to_string((stoi(polyTheoremProduct)+stoi(original)));
                string prunedPolyProduct = "";
                for (int k = 0; k < (8 - polyTheoremProduct.length()); k++) {
                    prunedPolyProduct += '0';
                }
                prunedPolyProduct += polyTheoremProduct;
//                cout << "2's Length: " << prunedPolyProduct.length() << endl;
                for (int k = 0; k < polyTheoremProduct.length(); k++) {
                    if (polyTheoremProduct[k] == '2') {
                        polyTheoremProduct.replace(k, 1, "0");
                    }
                }

                resultsToAddToOriginal[degree-1] = prunedPolyProduct;
//                cout << "newORIGINAL: " << original << endl;
            } else {
                cout << "Poly Theorem Product too long! Needs to be reduced" << endl;
            }
        }
        degree++;
    }

    //Add all the binary string's together
    string binaryXORResult = "";
    for (int z = 0; z < 8; z++) {
        int oneCounter = 0;
        if (resultsToAddToOriginal[z] != "") {
            cout << "ADDING: " << resultsToAddToOriginal[z] << endl;
            for (int q = 0; q < 8; q++) {
                if ((resultsToAddToOriginal[z][q] == '1')) {
                    oneCounter++;
                }
            }
        }

        if ((oneCounter % 2) == 0) {
            binaryXORResult += '0';
        } else {
            binaryXORResult += '1';
        }
    }

    cout << binaryXORResult << endl;

    return "";
}

//Encryption AddRoundKey Step
string** AddRoundKey(string** state, string** roundKey) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int a = stoi(roundKey[j][i], nullptr, 16);
            int b = stoi(state[i][j], nullptr, 16);
            ostringstream oss;
            string hex_representation;
            oss << std::hex << int((a+b));
            string hex_sum = oss.str();
            state[i][j] = hex_sum;
        }
    }
    return state;
}

//Encryption Sub Byte Step
string** SubByte(string** state) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int x = stoi(state[i][j].substr(0,1), nullptr, 16);
            int y = stoi(state[i][j].substr(1,1), nullptr, 16);
            state[i][j] = SBOX[x][y];
        }
    }
    return state;
}

//Encryption Shift Rows Step
string** ShiftRows(string** state) {
    for (int i = 0; i < 4; i++) {
        //Grab original row
        string originalRow[4];
        for (int j = 0; j < 4; j++) {
            originalRow[j] = state[i][j];
        }
        //Shift the bytes
        string shiftedRow[4];
        int shiftedIndex = (i);
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

string BinaryToHex(const string& binaryString) {
    string hex;
    for (size_t i = 0; i < binaryString.length(); i += 4) {
        std::bitset<4> nibble(binaryString.substr(i,4));
        hex += nibble.to_ulong() < 10 ? char(nibble.to_ulong() + '0') : char(nibble.to_ulong()-10 + 'A');
    }
    return hex;
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