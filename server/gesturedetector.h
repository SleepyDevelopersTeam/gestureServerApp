#ifndef GESTUREDETECTOR_H
#define GESTUREDETECTOR_H
#include <opencv/cv.h>
#include "accumulator.h"

class GestureDetector
{
public:
    GestureDetector();
    bool detect(cv::Mat& frame);
    int critDeltaWidth;
    int critHeight;
    int prevWidth;
    int chilloutcounter;
    Accumulator accumulator;
    float minDisp;
    float minWidthRate;
};

#endif // GESTUREDETECTOR_H
