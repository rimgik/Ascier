#include "AsciiArt.hpp"

std::string AsciiArt::grayScaleToString (const uint8_t& c, const int& count) {
//    static const std::string grayscale =
//    " .'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
    static constexpr std::string_view grayscale { " .:-=+*#%@" };
    const uint8_t length = (c * grayscale.length()) / 250;
    
    return std::string(count, grayscale[length]);
}

cv::Mat AsciiArt::readFromStdin() const {
    if (inputWidth == -1 || inputHeight == -1)
        throw std::invalid_argument("Stdin: missing input image width and height");
    
    cv::Mat img {cv::Size(inputWidth, inputHeight), CV_8UC3, cv::Scalar(0, 0, 0)};
    
    for (int rows = 0; rows < inputHeight; ++rows) {
        for (int cols = 0; cols < inputWidth; ++cols) {
            cv::Vec3b color{};
            
            // red
            color[2] = std::cin.get();
            // green
            color[1] = std::cin.get();
            // blue
            color[0] = std::cin.get();
            
            img.at<cv::Vec3b>(rows, cols) = color;
            
            if (!std::cin) {
                return cv::Mat();
            }
            
        }
    }
    return img;
}

// img must be an BGR color coded img
// convert an BGR image into GrayScale Image (scaled according to
// scaling factor) then into ascii art
std::string AsciiArt::imageToText(const cv::Mat& img) const {
    
    if (img.data == NULL)
        return "";
    
    cv::Mat grayscaleImage{};
    cv::cvtColor(img, grayscaleImage, cv::COLOR_BGR2GRAY);
    
    cv::Mat scaled;
    
    try {
        cv::resize(grayscaleImage, scaled, cv::Size(0, 0), scale, scale);
    } catch (cv::Exception) {
        std::cerr << "Invalid scaling factor" << std::endl;
        std::terminate();
    }
    
    std::string result{};
    
    for (int row = 0; row < scaled.rows; ++row) {
        for (int col = 0; col < scaled.cols; ++col) {
            result += grayScaleToString(scaled.at<uint8_t>(row, col), pixelWidth);
        }
        result += '\n';
    }
    
    return result;
}

void AsciiArt::display(const std::string_view& text, int count) const {
    
    if (text == "")
        return;
    
    if (writeToStdout) {
        std::cout << text << std::endl;
    } else {
        std::ofstream file;
        if (count == -1) {
            std::filesystem::create_directories(outputPath);
            file.open(outputPath, std::ios::out | std::ios::in | std::ios::trunc);
        } else {
            
            const char* cstring = outputPath.c_str();
            int requiredSize = snprintf(nullptr, 0, cstring, count);
            char* result = static_cast<char*>(malloc(requiredSize + 1)); // +1 for null-terminator
            snprintf(result, requiredSize + 1, cstring, count);
            
            // make sure all the directories exists
            std::filesystem::create_directories(std::filesystem::path(result).parent_path());
            
            file.open(result, std::ios::out | std::ios::in | std::ios::trunc);
            
            free(result);
        }
        file << text << '\n';
        file.close();
    }
}

AsciiArt::AsciiArt(std::unordered_map<std::string, std::any>& c) :
    currentWorkingDirectory {std::filesystem::current_path()},
    outputPath {std::any_cast<stdPath>(c["-o"])},
    inputPath {std::any_cast<stdPath>(c["-i"])},
    writeToStdout {outputPath == currentWorkingDirectory},
    readingFromStdin {inputPath == currentWorkingDirectory},
    pixelWidth {std::any_cast<int>(c["-pixelWidth"])},
    scale {std::any_cast<double>(c["-scale"])},
    inputWidth {std::any_cast<int>(c["-width"])},
    inputHeight {std::any_cast<int>(c["-height"])}
{
}

void AsciiArt::process() const {
    if (readingFromStdin) {
        int cnt = 0;
        while (std::cin) {
            display(imageToText(readFromStdin()), cnt);
            cnt++;
        }
    } else {
        display(imageToText(cv::imread(inputPath, cv::IMREAD_COLOR)));
    }
}
