#ifndef EXTINPUT_H
#define EXTINPUT_H
#include <SerialStream.h>
#include <thread>
#include <iostream>
#include <string>
#include <stdio.h>

class Extinput
{
    public:
        int m_val = 0;
        std::thread* m_pt;
        Extinput();
};

#endif // EXTINPUT_H
