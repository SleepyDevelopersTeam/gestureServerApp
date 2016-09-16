#include "accumulator.h"
#include <QDebug>
Accumulator::Accumulator()
{
    meanAccumulator = 0;
    n = 0;
    dispAccumulator = 0;
    force_learn = false;
    frame_counter = 0;
}

void Accumulator::forceLearn()
{
    frame_counter = 0;
    force_learn = true;
}

void Accumulator::needForceLearn()
{
    frame_counter++;
    if (frame_counter == MAX_FRAME_AMOUNT)
    {
        force_learn = false;
    }
}

float Accumulator::accumulate(int next)
{
    needForceLearn();
    float mean = 0;
    float disp = 0;

    if (n!=0)
    {
        mean = meanAccumulator / (float) n;
        disp = dispAccumulator / (float) n;
    }
    qDebug()<<mean<<" "<<disp;
    int diff = (int)(mean - next);

    if (diff*diff < disp || n<MAX_FRAME_AMOUNT/2 || force_learn)
    {
        int newMeanValue = meanAccumulator + next;
        uint newDispValue = dispAccumulator + diff*diff;

        if (n == MAX_FRAME_AMOUNT)
        {
            // normalizing accumulators
            newMeanValue = (int) ((float)(newMeanValue) / (n + 1) * n);
            newDispValue = (uint) ((float)(newDispValue) / (n + 1) * n); // TODO: is this a correct normalization?
        }
        else
        {
            n++;
        }

        meanAccumulator = newMeanValue;
        dispAccumulator = newDispValue;
        return false;
    }
    return true;
}

void Accumulator::reset()
{
    meanAccumulator = 0;
    n = 0;
    dispAccumulator = 0;
    force_learn = false;
    frame_counter = 0;
}

float Accumulator::getDisp()
{
    if (n == 0) return 0.0F;
    return dispAccumulator / (float) n;
}
