#include <map>
#include <string>
#include <utility>
#include <vector>

namespace gwDainese
{   
    class Config {
    public:
        bool init(const std::vector<std::string>& fileContents);
        std::string getValueStr(const std::string configKey);
        int getValueInt(const std::string configKey);
        double getValueDbl(const std::string configKey);
        void logValues();

    private:
        std::map<std::string, std::string> configMap;

        bool isLineValid(const std::string configLine);
    };
}
