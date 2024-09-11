#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <map>
#include <unistd.h>
#include <limits.h>

namespace fs = std::filesystem;

std::map<int,char> greyToAscii = {
    {255,'@'}, {240, '&'}, {210, '$'}, {180, '%'}, {'&', 150}, 
    {'*', 120}, {'+', 90}, {'=', 60}, {'-', 30}, {'.', 0}
};

char cwd[PATH_MAX];
const int numberOfFrames = 13144;

void resizeImage(std::string& input_image_path, std::string& outputImgPath) {

    cv::Mat img = cv::imread(input_image_path, cv::IMREAD_GRAYSCALE);
    if(img.empty()) {
        std::cerr << "Couldn't open input image \n";
        return;

    }

    cv::Size newSize(static_cast<int>(img.cols * 0.25), static_cast<int>(img.rows * 0.14));
    cv::Mat resizedImg;
    cv::resize(img,resizedImg, newSize);

    if(!cv::imwrite(outputImgPath, resizedImg)) {
        std::cerr << "Couldn't output image \n";
        return;
    }

    std::cout << "Image resized succ \n";
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
            auto it = greyToAscii.lower_bound(pixelValue);
            outFile << it->second;
        }
        outFile << std::endl;
    }
    outFile.close();
    std::cout << "Ascii Art Created Succ \n";

}

void convertVideoToAscii(std::string unprocessed_frames_dir, std::string resized_frames_dir, std::string ascii_art_dir) {

    std::stringstream ss;
    int frameNumber = 1;
    std::string unprocessed_image_path = "";
    std::string input_resized_image_path = "";
    std::string output_resized_image_path = "";
    std::string output_ascii_path = "";

    for(int image = 1; image < numberOfFrames; image++) {

        ss << unprocessed_frames_dir << "/output_frame_" << frameNumber << ".png";
        unprocessed_image_path = ss.str();
        ss.clear();
        ss.str("");

        ss << resized_frames_dir << "/resized_frame_" << frameNumber << ".png";
        output_resized_image_path = ss.str();
        ss.clear();
        ss.str("");


        resizeImage(unprocessed_image_path, output_resized_image_path);

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
        convertGrayImgToAscii(input_resized_image_path, output_ascii_path);

        frameNumber++;
        std:: cout << "Frame Number: " << frameNumber << "\n";
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

    ss << cwd << "/../badapple-framedata";
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