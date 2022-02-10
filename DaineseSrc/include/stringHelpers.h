#include <string>
#include <utility>

namespace gwDainese
{   
    std::string trim(const std::string origStr);
    std::pair<std::string, std::string> split(const std::string configLine);
}
