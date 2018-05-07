#include <iostream>
#include<vector>
#include"algorithm"
#include "core/core.hpp"
#include "highgui/highgui.hpp"
#include "imgproc/imgproc.hpp"
#include <iomanip>
using namespace std;
using namespace cv;

int main(int argc,char *argv[])
{
    Mat image,imageGray,imageGuussian;
    Mat imageSobelX,imageSobelY,imageSobelOut;
    image=imread("/home/ubuntu/图片/bu.png",1);
    int width = image.cols;
    int height = image.rows;
    cout<<width<< "good"<<height<<endl;
    //1. 原图像大小调整，提高运算效率
    //resize(image,image,Size(500 ,300));
    imshow("1.原图像",image);

    imageGray=image;
   // 2. 转化为灰度图
    cvtColor(image,imageGray,CV_RGB2GRAY);
    imshow("2.灰度图",imageGray);

    //3. 高斯平滑滤波
     GaussianBlur(imageGray,imageGuussian,Size(3,3),0);
    imshow("3.高斯平衡滤波",imageGuussian);

    //4.求得水平和垂直方向灰度图像的梯度差,使用Sobel算子
    Mat imageX16S,imageY16S;
    Sobel(imageGuussian,imageX16S,CV_16S,1,0,3,1,0,4);
    Sobel(imageGuussian,imageY16S,CV_16S,0,1,3,1,0,4);
    convertScaleAbs(imageX16S,imageSobelX,1,0);
    convertScaleAbs(imageY16S,imageSobelY,1,0);
    imageSobelOut=imageSobelX-imageSobelY;
    imshow("4.X方向梯度",imageSobelX);
    imshow("4.Y方向梯度",imageSobelY);
    imshow("4.XY方向梯度差",imageSobelOut);
    //4.开运算,消除亮度较高的细小区域,先腐蚀后膨胀
    Mat  element1=getStructuringElement(0,Size(3,3));
    morphologyEx(imageSobelOut,imageSobelOut,MORPH_CLOSE,element1);
    imshow("5.开运算",imageSobelOut);

    //6.均值滤波，消除高频噪声
    blur(imageSobelOut,imageSobelOut,Size(3,3));
    imshow("6.均值滤波",imageSobelOut);

    //7.二值化
    Mat imageSobleOutThreshold;
    threshold(imageSobelOut,imageSobleOutThreshold,180,255,CV_THRESH_BINARY);
    imshow("7.二值化",imageSobleOutThreshold);


    //8. 膨胀，填充空隙，根据核的大小，有可能需要4次膨胀操作
    Mat  element=getStructuringElement(0,Size(7,7));
    dilate(imageSobleOutThreshold,imageSobleOutThreshold,element);
    dilate(imageSobleOutThreshold,imageSobleOutThreshold,element);
    dilate(imageSobleOutThreshold,imageSobleOutThreshold,element);

    dilate(imageSobleOutThreshold,imageSobleOutThreshold,element);



    imshow("8.膨胀",imageSobleOutThreshold);

    //9.通过findContours找到条形码区域的矩形边界
    vector< vector<Point> > contours;
    vector<Vec4i> hiera;
    findContours(imageSobleOutThreshold,contours,hiera,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
    for(int i=0;i < contours.size();i++)
    {
        Rect rect=boundingRect((Mat)contours[i]);
        float c=(float)rect.width/rect.height;//矩形的长宽比,(float)强制转化
       // printf("%.1f%\n",c*100);
        if( 0.9<c && c<1.2 &&rect.area()>4000  )
        {
          rectangle(image,rect,Scalar(255),2);
          //cout<<"width:"<<rect.width<<"height:"<<rect.height<<endl;
          //cout<<"c:"<<setprecision(2)<<c<<endl;//setprecision(2)精度为2
        }

    }
    imshow("9.找出二维码矩形区域",image);

    waitKey();
}
