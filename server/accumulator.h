#ifndef ACCUMULATOR_H
#define ACCUMULATOR_H
#define MAX_FRAME_AMOUNT 30
#include <iostream>

class Accumulator
{
public:
    bool force_learn;
    int meanAccumulator;
    int n;
    uint dispAccumulator;
    Accumulator();
    float accumulate(int next);
    void forceLearn();
    int frame_counter = 0;
    void needForceLearn();
    void reset();

    float getDisp();
};

#endif // ACCUMULATOR_H
