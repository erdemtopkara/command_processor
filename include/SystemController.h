#pragma once
#include "Algorithm.h"

class SystemController {
public:
    void init();
    void run();    // infinite 10 ms loop

private:
    Algorithm algorithm;
};
