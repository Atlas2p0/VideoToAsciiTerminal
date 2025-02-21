#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <map>
#include <unistd.h>
#include <limits.h>
#include <algorithm>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

inline char grayToAscii(uint8_t gray) {
    static const std::string asciiChars = " .'`^\",:;>~+_-?{(itfjrxnzmpdbo*#&8%@$";
    size_t index = static_cast<size_t>(gray) * (asciiChars.size() - 1) / 255;
    return asciiChars[index];
}


char cwd[PATH_MAX];
int numberOfFrames = 0;

cv::Mat resizeImage(const cv::Mat& img) {    
    if (img.empty()) return cv::Mat();

    double scaleX = 0.19, scaleY = 0.08; 
    cv::Size newSize(static_cast<int>(img.cols * scaleX), static_cast<int>(img.rows * scaleY));
    cv::Mat resizedImg;
    cv::resize(img, resizedImg, newSize, 0, 0, cv::INTER_AREA);  
    return resizedImg;
}

void convertGrayImgToAscii(const cv::Mat& grayImg, const std::string& outputAsciiPath) {

    if (grayImg.empty()) {
        std::cerr << "Couldn't open image file \n";
        return;
    }

    std::ofstream outFile(outputAsciiPath);
    if (!outFile) {
        std::cerr << "Couldn't open output file \n";
        return;
    }

    for (int y = 0; y < grayImg.rows; ++y) {
        const uchar* row = grayImg.ptr<uchar>(y);
        std::vector<char> rowStr;
        rowStr.reserve(grayImg.cols); 

        for (int x = 0; x < grayImg.cols; ++x) {
            rowStr.push_back(grayToAscii(row[x]));
        }
        outFile.write(rowStr.data(), rowStr.size()); 
        outFile.put('\n');  
    }
}

void convertVideoToAscii(const std::string& unprocessed_frames_dir, const std::string& ascii_art_dir) {
    fs::path directory_path = fs::current_path() / "../ffmpeg-framedata"; 
    std::cout << "Scanning directory: " << directory_path << std::endl;

    try {
        if (!fs::exists(directory_path) || !fs::is_directory(directory_path)) {
            std::cerr << "The provided path is not a directory or does not exist.\n";
            return;
        }
        numberOfFrames = static_cast<int>(std::distance(fs::directory_iterator(directory_path), fs::directory_iterator{}));
        std::cout << "Number of frames: " << numberOfFrames << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error accessing filesystem: " << e.what() << std::endl;
        return;
    }

    for (int frameNumber = 1; frameNumber < numberOfFrames; ++frameNumber) {

        fs::path unprocessed_image_path = fs::path(unprocessed_frames_dir) / ("frame_" + std::to_string(frameNumber) + ".png");
        cv::Mat unprocessedImage = cv::imread(unprocessed_image_path.string(), cv::IMREAD_GRAYSCALE);

        if (unprocessedImage.empty()) {
            std::cerr << "Warning: Couldn't load image " << unprocessed_image_path.string() << "\n";
            continue;
        }

        cv::Mat resizedImage = resizeImage(unprocessedImage);
        fs::path output_ascii_path = fs::path(ascii_art_dir) / ("ascii_frame_" + std::to_string(frameNumber) + ".txt");
        convertGrayImgToAscii(resizedImage, output_ascii_path.string());

        if (frameNumber % 500 == 0)
            std::cout << "Processed Frame: " << frameNumber << "\n";
    }
}


int main() {

    std::string unprocessed_images_dir, output_ascii_dir;
    std::ostringstream ss;

    if (getcwd(cwd, sizeof(cwd))) {
        std::cout << "Current Working dir: " << cwd << "\n";
    } else {
        std::cerr << "Error getting current working dir \n";
        return -1;
    }

    ss << cwd << "/../ffmpeg-framedata";
    unprocessed_images_dir = ss.str();

    ss.str(""); 
    ss << cwd << "/../processed-frames/ascii-art";
    output_ascii_dir = ss.str();

    convertVideoToAscii(unprocessed_images_dir, output_ascii_dir);
    return 0;
}
