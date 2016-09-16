#include "foneaccumulator.h"
#include <iostream>
int FoneAccumulator::dispThreshold;
int FoneAccumulator::maxN;
int FoneAccumulator::forceLearnDuration = 60;

FoneAccumulator::FoneAccumulator(size_t width, size_t height)
{
    dispThreshold = 20;
	maxN = 100;

    trackedPixelsThreshold = 0.5F;
	
	meanAccumulator = new cv::Mat(height, width, CV_32F);
	dispAccumulator = new cv::Mat(height, width, CV_32F);
	
	n = new cv::Mat(height, width, CV_8UC1);
	tracked = new cv::Mat(height, width, CV_8UC1);
	
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
		{
			meanAccumulator->at<uchar>(y, x) = 0;
			dispAccumulator->at<uchar>(y, x) = 0;
			n->at<uchar>(y, x) = 0;
			tracked->at<uchar>(y, x) = 0;
		}
	
	this->width = width;
	this->height = height;
	
	forceFoneAccumulating = false;
    forceLearnFrameCounter = 0;
}

void FoneAccumulator::accumulate(cv::Mat *nextFrame)
{
    int foregroundPixelsCount = 0;
	// each pixel
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			uint accumulatedMean = meanAccumulator->at<uint>(y, x);
			uint accumulatedDisp = dispAccumulator->at<uint>(y, x);
			uchar N = n->at<uchar>(y, x);
			uint mean = 0;
			uint disp = 0;
			if (N!=0)
			{
				mean = accumulatedMean / N;
				disp = accumulatedDisp / N;
			}
			
			uchar addition = nextFrame->at<uchar>(y, x);
			uchar diff = abs(mean - addition);
			
			// first condition is for cases when we haven't collected much data yet and disp is too small
			// second defines, if variance `diff` can be explained with fone picture noise
			// third is for the case of the first frame, when we don't have any data and have to collect it
			// fourth is for ones who would like to accumulate fone with no conditions
			if (diff < dispThreshold || diff*diff < disp || N == 0 || forceFoneAccumulating)
			{
				tracked->at<uchar>(y, x) = 0;
				
				uint newMeanValue = accumulatedMean + addition;
				uint newDispValue = accumulatedDisp + diff*diff;
				
				if (N == maxN)
				{
					// normalizing accumulators
					newMeanValue = (uint) ((float)(newMeanValue) / (N + 1) * N);
					newDispValue = (uint) ((float)(newDispValue) / (N + 1) * N); // TODO: is this a correct normalization?
				}
				else
				{
					n->at<uchar>(y, x) = N + 1;
				}
				
				meanAccumulator->at<uint>(y, x) = newMeanValue;
				dispAccumulator->at<uint>(y, x) = newDispValue;
			}
			else // pixel tends not to belong the background
			{
                ++foregroundPixelsCount;
				tracked->at<uchar>(y, x) = 255;
			}
		}
	}

    if (foregroundPixelsCount > nextFrame->size().area() * trackedPixelsThreshold)
    {
        enableForceAccumulating();
    }

    if (forceFoneAccumulating)
    {
        ++forceLearnFrameCounter;
        if (forceLearnFrameCounter >= forceLearnDuration)
        {
            disableForceAccumulating();
        }
    }
}

void FoneAccumulator::getForegroundMask(cv::Mat& thresholded)
{
    threshold(*dispAccumulator, thresholded, dispThreshold, (uchar) 255, 0);
}

void FoneAccumulator::enableForceAccumulating()
{
	forceFoneAccumulating = true;
    forceLearnFrameCounter = 0;
}
void FoneAccumulator::disableForceAccumulating()
{
	forceFoneAccumulating = false;
}

FoneAccumulator::~FoneAccumulator()
{
    delete meanAccumulator;
    delete dispAccumulator;
	delete n;
    delete tracked;
}
