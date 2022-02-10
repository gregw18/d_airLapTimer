#include <string>
#include <vector>

#include "FileTypesData.h"


namespace gwDainese
{
    std::string readTextFile(const std::string&, const int);
    std::vector<std::string> readTextFileToVector(const std::string& fileName);

    template<typename T> std::vector<T> readNumericFile(const std::string& fileName,
                                                        const size_t maxValuesToRead = SIZE_MAX);

    class FileRead{
        public:
            template<typename TfileContents, typename TreturnData> std::vector<TreturnData> readFile(std::string dataDir, 
                                                        std::string fileBaseName, 
                                                        DaineseDataTypes dataType);
        private:
            FileTypesData filesData;

            template<typename T> std::vector<double> adjustData(const std::vector<T>& srcData, 
                                                            double multipler, double offset);
    };
}
