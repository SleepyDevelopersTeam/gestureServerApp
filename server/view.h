#ifndef VIEW_H
#define VIEW_H
#include <server.h>

#include <QVector>
#include <QWidget>
#include <opencv2/opencv.hpp>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <QDebug>
#include "foneaccumulator.h"
#include "gesturedetector.h"

class View : public QWidget
{
     Q_OBJECT

public:
    View();
    cv::Mat img;
    ~View();
private:
    QVector<char> rgbPicture;
    Server* w;
    void YUV_NV21_TO_RGB(QVector<char> &argb, const QVector<char> &yuv, const int width, const int height);
    void rgbToMat();
    FoneAccumulator* acc;
    GestureDetector* gestDetector;

private slots:
    void showImage();
};

#endif // VIEW_H
