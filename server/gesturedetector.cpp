#include "gesturedetector.h"
#include <QDebug>
#define TIME_TO_RESET 20

GestureDetector::GestureDetector()
{
    critDeltaWidth = 60;
    critHeight = 30;
    prevWidth = INT_MAX;
    accumulator = Accumulator();
    chilloutcounter = 0;
    minDisp = 100.0F;
    minWidthRate = 0.05F;
}

bool GestureDetector::detect(cv::Mat& frame)
{
    if (chilloutcounter!=0)
    {
        chilloutcounter++;
        if (chilloutcounter == TIME_TO_RESET)
        {
            qDebug()<<"chilout ended";
            chilloutcounter = 0;
        }
        return false;
    }
    int currentHorWhite = 0;
    int MaxHorWhite = 0;
    for (int x = 0; x < frame.cols; ++x)
    {
        int maxWhite = 0;
        int currentWhite = 0;
        for (int y = 0; y < frame.rows; ++y)
        {
            if (frame.at<uchar>(y,x) > 0)
            {
                currentWhite++;
            }
            else
            {
                if (maxWhite<currentWhite)
                {
                    maxWhite = currentWhite;
                }
                currentWhite = 0;
            }
        }
        if (maxWhite > critHeight)
        {
            currentHorWhite++;
        }
        else
        {
            if (MaxHorWhite<currentHorWhite)
            {
                MaxHorWhite = currentHorWhite;
            }
            currentHorWhite = 0;
        }
    }

    prevWidth = MaxHorWhite;

    if (accumulator.accumulate(MaxHorWhite))
    {
        // gesture detected
        if (frame.size().width * minWidthRate > MaxHorWhite)
        {
            // too short body width
            accumulator.forceLearn();
            return false;
        }
        if (accumulator.getDisp() < minDisp)
        {
            // too little disp accumulated, don't make such unconsidered decisions
            accumulator.forceLearn();
            return false;
        }
        chilloutcounter++;
        accumulator.reset();
        return true;
    }
    else
    {
        return false;
    }
}
