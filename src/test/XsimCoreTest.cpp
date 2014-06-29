#include <gtest/gtest.h>
#include <iostream>

#include "XsimCore.h"

using namespace xsim;

namespace {
XsimCore* xsimCorePtr;
int sampleSignal;
void sampleFunction() {
    std::cout << "Sample Function" << std::endl;
    xsimCorePtr->signalUpdate(&sampleSignal, BLOCKING);
}
}

TEST(DummyTest, Dummy) {
    std::cout << "Hello Google Test!" << std::endl;
    EXPECT_EQ(1, 1);
}

TEST(XsimCoreTest, Ctor) {
    XsimCore xsimCore;
    xsimCorePtr = &xsimCore;
    void* signals[1];
    signals[0] = &sampleSignal;
    xsimCore.registerFunction(sampleFunction, 1, signals);
    xsimCore.startSim();
}
