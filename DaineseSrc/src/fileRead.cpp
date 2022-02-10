#include <fstream>
#include <iostream>
#include <span>

#include "fileRead.h"

using namespace std;

namespace gwDainese
{
    template <class T> char* as_bytes(T& x) {
        return &reinterpret_cast<char&>(x);
    }


    string readTextFile(const string& fileName, const int len) {
        string fileContents = "";

        vector<string> contentsVec = readTextFileToVector(fileName);
        for (size_t i = 0; i < len && i < contentsVec.size(); i++) {
            fileContents += contentsVec[i] + "\n";
        }
        return fileContents;
    }

    vector<string> readTextFileToVector(const std::string& fileName) {
        vector<string> fileContents;

        ifstream ist(fileName);
        if (ist) {
            string line;
            cout << "opened it\n";
            while (getline (ist, line)) {
                //cout << line << '\n';
                fileContents.push_back(line);
            }
            ist.close();
        }
        else cout << "can't open input file " << fileName << "\n";

        return fileContents;
    }

    // Function puts together full path and name of file to read from, verifies
    // provided type against expected type for this type of file, then calls read function.
    template<typename TfileContents, typename TreturnData> std::vector<TreturnData>
        FileRead::readFile(string dataDir, 
                            string fileBaseName, 
                            DaineseDataTypes dataType) {
        
        vector<TreturnData> adjustedData;

        // Verify that am passed in expected contents type for this file type. Ugly but best
        // that I could come up with to verify.
        string returnType = filesData.GetReturnType(dataType);
        if (typeid(TreturnData).name() == returnType) {
            string fileType = filesData.GetContentsType(dataType);
            if (typeid(TfileContents).name() == fileType) {
                string fileExt = filesData.GetExtension(dataType);
                string fileName = dataDir.length() == 0 ?  fileBaseName : dataDir + "\\" + fileBaseName;
                fileName += fileExt;
                
                vector<TfileContents> fileContents = readNumericFile<TfileContents>(fileName);

                double multiplier = filesData.GetMultiplier(dataType);
                double offset = filesData.GetOffset(dataType);
                if ( multiplier != 1 || offset != 0) {
                    auto tempData = adjustData(fileContents, multiplier, offset);
                    std::copy(tempData.begin(), tempData.end(), std::back_inserter(adjustedData));
                }
                else {
                    // Need to move fileContents to adjustedData.
                    std::copy(fileContents.begin(), fileContents.end(), std::back_inserter(adjustedData));
                }
            }
            else {
                cout << "Mismatched file contents type in FileRead::readFile, expected: " 
                    << fileType << ", received: " << typeid(TfileContents).name() << "\n";
            }
        }
        else {
            cout << "Unexpected return type in FileRead::readFile, expected: " << returnType
                << " but received: " << typeid(TreturnData).name() << "\n";
        }

        return adjustedData;
    }

    template<typename T> vector<double> FileRead::adjustData(const vector<T>& srcData, 
                                                            double multiplier, double offset) {

        vector<double> vec (srcData.size(), 0);
        for (int i = 0; i < srcData.size(); i++) {
            vec[i] = (srcData[i] + offset) * multiplier;
        }

        return vec;
    }

    template<typename T> std::vector<T> readNumericFile(const string& fileName, 
                                                        const size_t maxValuesToRead) {
        std::vector<T> vec;
        
        ifstream file(fileName, ios::binary);
        if (file) {
            file.unsetf(ios::skipws);   // Ensure that cr/lf are interpreted as numbers, not cr/lf.
            std::streamsize fileSize;

            // Get size of file.
            file.seekg(0, ios::end);
            fileSize = file.tellg();
            size_t vecSize = fileSize / sizeof(T);
            if (vecSize > maxValuesToRead) vecSize = maxValuesToRead;

            file.seekg(0, ios::beg);
            cout << "file size:" << fileSize << ", vector size: " << vecSize << "\n";

            vec.reserve(vecSize);
            
            size_t valsRead = 0;
            for(T x; file.read(as_bytes(x), sizeof(T)); ) {
                vec.push_back(x);
                if (++valsRead >= maxValuesToRead) break;
            }        
            file.close();
        }
        else cout << "can't open input file " << fileName << "\n";

        return vec;
    }

    template vector<short> readNumericFile<short>(const string&, const size_t);
    template vector<unsigned short> readNumericFile<unsigned short>(const string&, const size_t);
    template vector<int> readNumericFile<int>(const string&, const size_t);
    template vector<float> readNumericFile<float>(const string&, const size_t);
    template vector<long> readNumericFile<long>(const string&, const size_t);
    template vector<unsigned long> readNumericFile<unsigned long>(const string&, const size_t);
    template vector<double> readNumericFile<double>(const string&, const size_t);

    template vector<double> FileRead::readFile<double, double>(string, string, DaineseDataTypes);
    template vector<short> FileRead::readFile<short, short>(string, string, DaineseDataTypes);
    template vector<double> FileRead::readFile<unsigned short, double>(string, string, DaineseDataTypes);
    template vector<unsigned short> FileRead::readFile<unsigned short, unsigned short>(string, string, DaineseDataTypes);
    template vector<double> FileRead::readFile<unsigned long, double>(string, string, DaineseDataTypes);
    template vector<float> FileRead::readFile<float, float>(string, string, DaineseDataTypes);

    template vector<double> FileRead::adjustData(const vector<short>&, double, double);
    template vector<double> FileRead::adjustData(const vector<unsigned short>&, double, double);
    template vector<double> FileRead::adjustData(const vector<int>&, double, double);
    template vector<double> FileRead::adjustData(const vector<long>&, double, double);
    template vector<double> FileRead::adjustData(const vector<unsigned long>&, double, double);
    template vector<double> FileRead::adjustData(const vector<float>&, double, double);
    template vector<double> FileRead::adjustData(const vector<double>&, double, double);
}
