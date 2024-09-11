#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;
std::map<int,char> greyToAscii = {
    {255,'@'}, {240, '&'}, {210, '$'}, {180, '%'}, {'&', 150}, 
    {'*', 120}, {'+', 90}, {'=', 60}, {'-', 30}, {'.', 0}
};


void resizeImage(std::string& input_image_path, std::string& outputImgPath) {
    cv::Mat img = cv::imread(input_image_path, cv::IMREAD_GRAYSCALE);
    if(img.empty()) {
        std::cerr << "Couldn't open input image \n";
        return;

    }
    cv::Size newSize(static_cast<int>(img.cols * 0.23), static_cast<int>(img.rows * 0.1));

    cv::Mat resizedImg;
    cv::resize(img,resizedImg, newSize);

    if(!cv::imwrite(outputImgPath, resizedImg)) {
        std::cerr << "Couldn't output image \n";
        return;
    }
    std::cout << "Image resized succ \n";

}


void convertGrayToAscii(std::string& input_image_path, std::string& outputAsciiPath) {
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
            // if(it == greyToAscii.end()) {
            //     it = std::prev(greyToAscii.end());
            // }
            outFile << it->second;
        }
        outFile << std::endl;
    }
    outFile.close();
    std::cout << "Ascii Art Created Succ \n";

}

int main() {


    std::string unprocessed_image_path = "/home/dawnwalker/playground/ascii-encoding/badapple-framedata/output_frame_0150.png";
    std::string input_resized_image_path = "/home/dawnwalker/playground/ascii-encoding/processed-frames/resized_frame_0001.png";
    std::string output_resized_image_path = "/home/dawnwalker/playground/ascii-encoding/processed-frames/resized_frame_0001.png";
    std::string output_ascii_path = "/home/dawnwalker/playground/ascii-encoding/processed-frames/ascii_frame_0001.txt";

    std::stringstream ss;
    int frameNumber = 1;
    int numberOfFrames = 13144;

    for(int image = 1; image < numberOfFrames; image++) {

        ss << "/home/dawnwalker/playground/ascii-encoding/badapple-framedata/output_frame_" << frameNumber << ".png";
        unprocessed_image_path = ss.str();
        ss.clear();
        ss.str("");

        ss << "/home/dawnwalker/playground/ascii-encoding/processed-frames/resized_frame_" << frameNumber << ".png";
        output_resized_image_path = ss.str();
        ss.clear();
        ss.str("");


        resizeImage(unprocessed_image_path, output_resized_image_path);

        ss << "/home/dawnwalker/playground/ascii-encoding/processed-frames/resized_frame_" << frameNumber << ".png";
        input_resized_image_path = ss.str();
        ss.clear();
        ss.str("");

        ss << "/home/dawnwalker/playground/ascii-encoding/processed-frames/ascii-art/ascii_frame_" << frameNumber << ".txt";
        output_ascii_path = ss.str();
        ss.clear();
        ss.str("");

        std::ifstream file(output_ascii_path);

        if(!file) {
            std::ofstream newFile(output_ascii_path);
            if(!newFile) {
                std::cerr << "Failed to create txt file \n";
                return -1;
            }        
        }
        convertGrayToAscii(input_resized_image_path, output_ascii_path);

        frameNumber++;
        std:: cout << "Frame Number: " << frameNumber << "\n";

    }




    return 0;

}