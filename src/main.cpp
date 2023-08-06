#include "AsciiArt.hpp"

bool isInteger(const std::string_view& str) {
    for (auto &i : str)
        if (!(i >= '0' && i < '9'))
            return false;
    return true;
}

bool isDouble(const std::string_view& str) {
    long long cnt = 0;
    for (auto &i : str) {
        if (!(i >= '0' && i < '9')) {
            if (i == '.')cnt++;
            else return false;
        }
    }
    return cnt <= 1;
}

int main(int argc, const char* argv[]) {
    
    using stdPath = std::filesystem::path;
    const stdPath currentWorkingDirectory = std::filesystem::current_path();
    
    std::unordered_map<std::string, std::any> config {
        {"-i", currentWorkingDirectory},
        {"-width", -1},
        {"-height", -1},
        {"-scale", 1.0},
        {"-pixelWidth", 2},
        {"-o", currentWorkingDirectory}
    };
    
    for (size_t i = 1; i < argc; i+=2) {
        const std::string flag = argv[i];
        
        if (config.find(flag) == config.end())
            throw std::invalid_argument(flag + " is not a valid flag");
        
        if (i + 1 >= argc)
            throw std::invalid_argument(flag + " flag missing value");
        
        const std::string value = argv[i+1];
        
        if (flag == "-i" || flag == "-o") {
            config[flag] = (stdPath(currentWorkingDirectory) /= (stdPath(value)));
        } else {
            
            if (flag == "-scale") {
                if (!isDouble(value))
                    throw std::invalid_argument("Invalid " + flag + " value: " + value);
                
                config[flag] = std::stod(value);
                
            } else {
                if (!isInteger(value))
                    throw std::invalid_argument("Invalid " + flag + " value: " + value);
                
                config[flag] = std::stoi(value);
            }
        }
    }
    
    AsciiArt art(config);
    art.process();
    
}
