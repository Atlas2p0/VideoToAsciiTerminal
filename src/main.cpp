#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <map>
#include <unistd.h>
#include <limits.h>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;


char grayToAscii(uint8_t gray) {
    // Define ASCII characters from lightest to darkest.
    // You can reverse or modify this string based on your preference.
    // const std::string asciiChars = " .'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
    const std::string asciiChars = " .'`^\",:;>~+_-?{(itfjrxnzmpdbo*#&8%@$";
    // const std::string asciiChars = " .:`;i~+_-?1tao*%@";
    // const std::string asciiChars = " .:-=+*#%@";
    
    // Compute an index by scaling the grayscale value to the range of asciiChars indices
    size_t index = static_cast<size_t>(gray) * (asciiChars.size() - 1) / 255;
    return asciiChars[index];
}

char cwd[PATH_MAX];
int numberOfFrames = 0;
void resizeImage(std::string& input_image_path, std::string& outputImgPath) {

    
    cv::Mat img = cv::imread(input_image_path, cv::IMREAD_GRAYSCALE);
    if(img.empty()) {
        std::cerr << "Couldn't open input image \n";
        return;

    }
    

    cv::Size newSize(static_cast<int>(img.cols * 0.19), static_cast<int>(img.rows * 0.08));
    cv::Mat resizedImg;
    cv::resize(img,resizedImg, newSize);

    if(!cv::imwrite(outputImgPath, resizedImg)) {
        std::cerr << "Couldn't output image \n";
        return;
    }

}


void convertGrayImgToAscii(std::string& input_image_path, std::string& outputAsciiPath) {

    cv::Mat grayImg = cv::imread(input_image_path, cv::IMREAD_GRAYSCALE);
    
    if(grayImg.empty()) {
        std::cerr << "Couldn't open image file \n";
        return;
    }

    std::ofstream outFile(outputAsciiPath);

    if(!outFile.is_open()) {
        std::cerr << "Couldn't open output file \n";
        return;
    }

    for(int y = 0; y < grayImg.rows; y++) {
        for(int x = 0; x < grayImg.cols; x++) {
            uchar pixelValue = grayImg.at<uchar>(y,x);
            auto it = grayToAscii(static_cast<uint8_t>(pixelValue));
            outFile << it;
        }
        outFile << std::endl;
    }
    outFile.close();
    std::cout << "Ascii Art Created Succesful \n";

}

void convertVideoToAscii(std::string unprocessed_frames_dir, std::string resized_frames_dir, std::string ascii_art_dir) {

    fs::path directory_path = fs::current_path() / "../ffmpeg-framedata"; 
    std::cout << directory_path;
    size_t fileCount = 0;

    try {
        if (fs::exists(directory_path) && fs::is_directory(directory_path)) {
            for (const auto& entry : fs::directory_iterator(directory_path)) {
                if (fs::is_regular_file(entry.status())) {
                    ++fileCount;
                }
            }
            std::cout << "Number of files: " << fileCount << std::endl;
        } else {
            std::cerr << "The provided path is not a directory or does not exist." << std::endl;
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "General exception: " << e.what() << std::endl;
    }

    numberOfFrames = fileCount;
    int frameNumber = 1;

    std::stringstream ss;
    std::string unprocessed_image_path = "";
    std::string input_resized_image_path = "";
    std::string output_resized_image_path = "";
    std::string output_ascii_path = "";

    for(int image = 1; image < numberOfFrames; image++) {

        ss << unprocessed_frames_dir << "/frame_" << frameNumber << ".png";
        unprocessed_image_path = ss.str();
        ss.clear();
        ss.str("");

        ss << resized_frames_dir << "/resized_frame_" << frameNumber << ".png";
        output_resized_image_path = ss.str();
        ss.clear();
        ss.str("");


        resizeImage(unprocessed_image_path, output_resized_image_path);



        frameNumber++;
    }
    frameNumber= 1;
    for(int image = 1; image < numberOfFrames; image++) {
        
        ss << resized_frames_dir << "/resized_frame_" << frameNumber << ".png";
        input_resized_image_path = ss.str();
        ss.clear();
        
        ss.str("");
        ss << ascii_art_dir << "/ascii_frame_" << frameNumber << ".txt";
        output_ascii_path = ss.str();
        ss.clear();
        ss.str("");

        std::ifstream file(output_ascii_path);

        if(!file) {
            std::ofstream newFile(output_ascii_path);
            if(!newFile) {
                std::cerr << "Failed to create txt file \n";
                return;
            }        
        }
        std::cout << "Frame Number: " << frameNumber << "\n";
        frameNumber++;
        convertGrayImgToAscii(input_resized_image_path, output_ascii_path);
    }
}


int main() {


    std::string unprocessed_images_dir = "";
    std::string output_resized_images_dir = "";
    std::string output_ascii_dir = "";
    std::stringstream ss;

    if(getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::cout << "Current Working dir: " << cwd << "\n";
    }
    else {
        std::cerr << "Error getting curr working dir \n";
        return -1;
    }

    ss << cwd << "/../ffmpeg-framedata";
    unprocessed_images_dir = ss.str();
    ss.clear();
    ss.str("");

    ss << cwd << "/../processed-frames";
    output_resized_images_dir = ss.str();
    ss.clear();
    ss.str("");

    ss << cwd << "/../processed-frames/ascii-art";
    output_ascii_dir = ss.str();
    ss.clear();
    ss.str("");


    convertVideoToAscii(unprocessed_images_dir, output_resized_images_dir, output_ascii_dir);

    return 0;
}