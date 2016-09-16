#include "view.h"


View::View()
{
    w = new Server();
    w->show();
    cvNamedWindow("capture", CV_WINDOW_AUTOSIZE);
    connect(w,SIGNAL(imageObtained()),this,SLOT(showImage()));
    acc = 0;
    gestDetector = 0;
}

cv::Mat gauss_Filter(const cv::Mat& image)
{
    cv::Mat result_image;
    double sigma=3;
    cv::GaussianBlur(image,result_image,cv::Size(3*sigma,3*sigma),sigma);
    return result_image;
}

void View::YUV_NV21_TO_RGB(QVector<char> &argb,const QVector<char> &yuv,const int width,const int height)
{
    int frameSize = width * height;
    argb.clear();
    int ii = 0;
    int ij = 0;
    int di = +1;
    int dj = +1;

    for (int i = 0, ci = ii; i < height; ++i, ci += di) {
        for (int j = 0, cj = ij; j < width; ++j, cj += dj) {
            int y = (0xff & ((int) yuv[ci * width + cj]));
            int v = (0xff & ((int) yuv[frameSize + (ci >> 1) * width + (cj & ~1) + 0]));
            int u = (0xff & ((int) yuv[frameSize + (ci >> 1) * width + (cj & ~1) + 1]));
            y = y < 16 ? 16 : y;

            int r = (int) (1.164f * (y - 16) + 1.596f * (v - 128));
            int g = (int) (1.164f * (y - 16) - 0.813f * (v - 128) - 0.391f * (u - 128));
            int b = (int) (1.164f * (y - 16) + 2.018f * (u - 128));

            r = r < 0 ? 0 : (r > 255 ? 255 : r);
            g = g < 0 ? 0 : (g > 255 ? 255 : g);
            b = b < 0 ? 0 : (b > 255 ? 255 : b);

            argb.push_back(b);//0xff000000 | (r << 16) | (g << 8) | b;
            argb.push_back(g);
            argb.push_back(r);
        }
    }
}

void View::rgbToMat()
{
    int k=0;
    for( int i = 0; i < img.rows; i++ )
        for( int j = 0; j < img.cols; j++ )
        {
            img.at<cv::Vec3b>(i,j)[0]=rgbPicture[k++];
            img.at<cv::Vec3b>(i,j)[1]=rgbPicture[k++] ;
            img.at<cv::Vec3b>(i,j)[2]=rgbPicture[k++] ;
        }
}

void View::showImage()
{
    YUV_NV21_TO_RGB(rgbPicture,w->data,w->width,w->height);
    img.create(w->height,w->width,CV_8UC3);
    if (acc == 0)
    {
        acc = new FoneAccumulator(w->width, w->height);
        gestDetector = new GestureDetector();
    }
    rgbToMat();

    int frame_number = 0;
    cv::cvtColor(img, img, CV_BGR2GRAY,1);
    img = gauss_Filter(img);
    //img.copyTo(humanShowableImage);
    acc->accumulate(&img);

    if (acc->forceFoneAccumulating)
    {
        // fone is being resetting now, let's reset body width accumulator too
        gestDetector->accumulator.forceLearn();
    }

    //acc.getForegroundMask(img);
    cv::bitwise_and(img, *(acc->tracked), img);
    //img.copyTo(img, *acc.tracked);
    cv::imshow("capture", img);
    if (gestDetector->detect(*(acc->tracked)))
    {
        qDebug()<<"Gesture detected!!!!!!!!!";
    }
//    char c = cvWaitKey(33);
//    if (c == 27)
//    {
//        break;
//    }
//    frame_number++;


    //    cv::imshow("lol",img);
}

View::~View()
{
    cvDestroyWindow("capture");
}
