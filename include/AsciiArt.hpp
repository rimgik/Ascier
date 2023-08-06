#ifndef AsciiArt_hpp
#define AsciiArt_hpp

// C++ std
#include <algorithm>
#include <string>
#include <iostream>
#include <filesystem>
#include <any>
#include <fstream>
#include <stdio.h>

// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using stdPath = std::filesystem::path;

class AsciiArt final {
private:
    const stdPath currentWorkingDirectory;
    const stdPath outputPath;
    const stdPath inputPath;
    const bool writeToStdout;
    const bool readingFromStdin;
    const int pixelWidth;
    const double scale;
    const int inputWidth;
    const int inputHeight;
    
    static std::string grayScaleToString (const uint8_t& c, const int& count);
    
    cv::Mat readFromStdin() const;
    std::string imageToText(const cv::Mat& img) const;
    void display(const std::string_view& text, int count = -1) const;
    
public:
    
    AsciiArt() = delete;
    AsciiArt(std::unordered_map<std::string, std::any>& c);
    
    void process() const;
    
};

#endif /* AsciiArt_hpp */
