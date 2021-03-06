#include <iostream>
#include <assert.h>
#include <atomic>
#include "opencv2/core/core.hpp"

using namespace std;

void method1(cv::Mat);
void method2(cv::Mat);
void method3(cv::Mat);
void method4(cv::Mat);
void method5(cv::Mat);
void method5_2(cv::Mat);
void method5_3(cv::Mat); // 最快的
void method6(cv::Mat);
void method7(cv::Mat);
void method8(cv::Mat);

int main(int argc, char* argv[])
{
    cv::Size imgSize(6400,4800);
    cv::Mat image = cv::Mat(imgSize, CV_8UC3, cv::Scalar(1,1,1));
    method1(image);
    method2(image);
    method3(image);
    method4(image);
    method5(image);
    method5_2(image);
    method5_3(image);
    method6(image);
    method7(image);
    method8(image);
}

void method1(cv::Mat img){
    // at access with Vec3b Vector
    double t0 = (double) cv::getTickCount();
    int height = img.rows;
    int width = img.cols;
    int sum = 0;
    for(int row=0; row < height; row++){
        for(int col=0; col < width; col++){
            cv::Vec3b uc_pixel = img.at<cv::Vec3b>(row, col);
            int a = uc_pixel[0];
            int b = uc_pixel[1];
            int c = uc_pixel[2];
            sum += a + b + c;
        }
    }
    assert(sum==3*height*width);
    double time = ((double) cv::getTickCount() - t0) / cv::getTickFrequency();
    std::cout << "Time for method1: " << time << std::endl;
}

void method2(cv::Mat img){
    // direct at access 
    double t0 = (double) cv::getTickCount();
    int height = img.rows;
    int width = img.cols;
    int sum = 0;
    for(int row=0; row < height; row++){
        for(int col=0; col < width; col++){
            int a = img.at<cv::Vec3b>(row, col)[0];
            int b = img.at<cv::Vec3b>(row, col)[1];
            int c = img.at<cv::Vec3b>(row, col)[2];
            sum += a + b + c;
        }
    }
    assert(sum==3*height*width);
    double time = ((double) cv::getTickCount() - t0) / cv::getTickFrequency();
    std::cout << "Time for method2: " << time << std::endl;
}

void method3(cv::Mat img){
    // pointer + Vec3b vector
    double t0 = (double) cv::getTickCount();
    int height = img.rows;
    int width = img.cols;
    int sum = 0;
    for(int row=0; row < height; row++){
        cv::Vec3b *ptr = img.ptr<cv::Vec3b>(row);
        for(int col=0; col < width; col++){
            cv::Vec3b pixel = ptr[col];
            int a = pixel[0];
            int b = pixel[1];
            int c = pixel[2];
            sum += a + b + c;
        }
    }
    assert(sum==3*height*width);
    double time = ((double) cv::getTickCount() - t0) / cv::getTickFrequency();
    std::cout << "Time for method3: " << time << std::endl;
}

void method4(cv::Mat img){
    // pointer
    double t0 = (double) cv::getTickCount();
    int height = img.rows;
    int width = img.cols;
    int sum = 0;
    for(int row=0; row < height; row++){
        cv::Vec3b *ptr = img.ptr<cv::Vec3b>(row);
        for(int col=0; col < width; col++){
            int a = ptr[col][0];
            int b = ptr[col][1];
            int c = ptr[col][2];
            sum += a + b + c;
        }
    }
    assert(sum==3*height*width);
    double time = ((double) cv::getTickCount() - t0) / cv::getTickFrequency();
    std::cout << "Time for method4: " << time << std::endl;
}

void method5(cv::Mat img){
    // raw pointer 
    double t0 = (double) cv::getTickCount();
    int height = img.rows;
    int width = img.cols;
    int sum=0;
    for(int row=0; row < height; row++){
        const uchar *ptr = img.ptr(row);
        for(int col=0; col < width; col++){
            const uchar *uc_pixel = ptr;
            int a = uc_pixel[0];
            int b = uc_pixel[1];
            int c = uc_pixel[2];
            // int a = ptr[0];
            // int b = ptr[1];
            // int c = ptr[2];
            sum += a + b + c;
            ptr += 3;
        }
    }
    assert(sum==3*height*width);
    double time = ((double) cv::getTickCount() - t0) / cv::getTickFrequency();
    std::cout << "Time for method5: " << time << std::endl;
}

void method5_2(cv::Mat img){
    // raw pointer 
    double t0 = (double) cv::getTickCount();
    int height = img.rows;
    int width = img.cols;
    int sum=0;
    for(int row=0; row < height; row++){
        const uchar *ptr = img.ptr(row);
        for(int col=0; col < width; col++){
            // const uchar *uc_pixel = ptr;
            // int a = uc_pixel[0];
            // int b = uc_pixel[1];
            // int c = uc_pixel[2];
            // 不使用中间指针
            int a = ptr[0];
            int b = ptr[1];
            int c = ptr[2];
            sum += a + b + c;
            ptr += 3;
        }
    }
    assert(sum==3*height*width);
    double time = ((double) cv::getTickCount() - t0) / cv::getTickFrequency();
    std::cout << "Time for method5_2: " << time << std::endl;
}

void method5_3(cv::Mat img){
    // raw pointer
    double t0 = (double) cv::getTickCount();
    int height = img.rows;
    int width = img.cols;

    // 如果内存连续排列，当做单行处理
    if(img.isContinuous()) {
       width = width * height;
       height = 1;
    } else {
       printf("img.isContinuous() is false\n");
    }

    int sum = 0;
    for(int row = 0; row < height; row++){
        const uchar *ptr = img.ptr(row);
        for(int col = 0; col < width; col++){
            sum += ptr[0] + ptr[1] + ptr[2];
            ptr += 3;
        }
    }
    assert(sum==3*height*width);
    double time = ((double) cv::getTickCount() - t0) / cv::getTickFrequency();
    std::cout << "Time for method5_3: " << time << std::endl;
}

void method6(cv::Mat img){
    // raw pointer + raw step
    // 必须保证Mat在内存的存储是连续的
    double t0 = (double) cv::getTickCount();
    int height = img.rows;
    int width = img.cols;
    int sum = 0;
    const uchar *uc_pixel = img.data;
    for(int row=0; row < height; row++){
        uc_pixel = img.data + row*img.step;
        for(int col=0; col < width; col++){
            int a = uc_pixel[0];
            int b = uc_pixel[1];
            int c = uc_pixel[2];
            sum += a + b + c;
            uc_pixel += 3;
        }
    }
    assert(sum==3*height*width);
    double time = ((double) cv::getTickCount() - t0) / cv::getTickFrequency();
    std::cout << "Time for method6: " << time << std::endl;
}

void method7(cv::Mat image){
    double t0 = (double) cv::getTickCount();
    int height = image.rows;
    int width = image.cols;
    cv::MatConstIterator_<cv::Vec3b> it = image.begin<cv::Vec3b>(), it_end = image.end<cv::Vec3b>();
    int sum = 0;
    for(; it != it_end; ++it){
        int a = (*it)[0];
        int b = (*it)[1];
        int c = (*it)[2];           
        sum += a + b + c;
    }
    assert(sum==3*height*width);
    double time = ((double) cv::getTickCount() - t0) / cv::getTickFrequency();
    std::cout << "Time for method7: " << time << std::endl;
}

// C++11 Lambda
//typedef cv::Point3_<uint8_t> Pixel;
typedef cv::Vec3b Pixel;

void method8(cv::Mat image){
    if(image.isContinuous()) {
       printf("img.isContinuous() is true\n");
    } else {
       printf("img.isContinuous() is false\n");
    }
    double t0 = (double) cv::getTickCount();
    int height = image.rows;
    int width = image.cols;
    // 用forEach，要小心Data Race!所以不一定高效的，反而有可能是最慢的
#if 1
    std::atomic<int> num;
    std::atomic<int> sum;
#else
    // 这个是不能用的，会有Data Race!
    int num, sum;
#endif
    num = 0;
    sum = 0;
    image.forEach<Pixel>(
         [&](Pixel &pixel, const int * position) -> void {
           if(pixel[0] != 1 or pixel[1] != 1 or pixel[2] != 1) {
              printf("pixel[0] = %d, pixel[1] = %d, pixel[2] = %d\n", pixel[0], pixel[1], pixel[2]);
           }
           //sum += pixel.x + pixel.y + pixel.z;
           sum += pixel[0] + pixel[1] + pixel[2];
           num++;
         }
    );
#if 1
    printf("num = %d, sum = %d, sum/3 = %d, 3*height*width = %d\n", num.load(), sum.load(), sum.load()/3, 3*height*width);
    assert(sum.load()==3*height*width);
#else
    printf("num = %d, sum = %d, sum/3 = %d, 3*height*width = %d\n", num, sum, sum/3, 3*height*width);
    assert(sum==3*height*width);
#endif
    double time = ((double) cv::getTickCount() - t0) / cv::getTickFrequency();
    std::cout << "Time for method8: " << time << std::endl;
}
