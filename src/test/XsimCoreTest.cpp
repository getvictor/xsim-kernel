#include <gtest/gtest.h>
#include <iostream>

#include "XsimCore.h"

using namespace xsim;

TEST(DummyTest, Dummy) {
    std::cout << "Hello Google Test!" << std::endl;
    EXPECT_EQ(1, 1);
}

// Test fixture.
class XsimCoreTest : public testing::Test {
public:
    XsimCore* xsimCorePtr;
    int sampleSignal1 = 1;
    int sampleSignal2 = 2;
    int executionCounter = 0;

    void sampleFunction1() {
        std::cout << "Sample Function 1" << std::endl;
        executionCounter++;
    }
    static void sampleFunction1Wrapper(void* userData) {
        ((XsimCoreTest*)userData)->sampleFunction1();
    }
    void sampleFunction2() {
        std::cout << "Sample Function 2" << std::endl;
        xsimCorePtr->signalUpdate(&sampleSignal2, BLOCKING);
        executionCounter += 10;
    }
    static void sampleFunction2Wrapper(void* userData) {
        ((XsimCoreTest*)userData)->sampleFunction2();
    }

    /**
     * Set up test environment.
     */
    virtual void SetUp() {
        executionCounter = 0;
        xsimCorePtr = new XsimCore();
    }

    /**
     * Tear down the test environment.
     */
    virtual void TearDown() {
        delete xsimCorePtr;
    }

};

TEST_F(XsimCoreTest, Ctor) {

    void* signals1[1];
    signals1[0] = &sampleSignal2;
    void* signals2[1];
    signals2[0] = &sampleSignal1;

    xsimCorePtr->registerFunction(XsimCoreTest::sampleFunction1Wrapper, this, 1, signals1);
    xsimCorePtr->registerFunction(XsimCoreTest::sampleFunction2Wrapper, this, 1, signals2);
    xsimCorePtr->startSim();
    EXPECT_EQ(12, executionCounter);
}

// TODO: 1 signal triggers multiple functions
