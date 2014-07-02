#include <gtest/gtest.h>
#include <iostream>

#include "XsimCore.h"

using namespace xsim;

// Test fixture.
class XsimCoreTest : public testing::Test {
public:
    XsimCore* xsimCorePtr;
    int sampleSignal1 = 1;
    int sampleSignal2 = 2;
    int sampleSignal3 = 3;
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
    void sampleFunction3() {
        std::cout << "Sample Function 3" << std::endl;
        xsimCorePtr->signalUpdate(&sampleSignal3, BLOCKING);
        executionCounter += 100;
    }
    static void sampleFunction3Wrapper(void* userData) {
        ((XsimCoreTest*)userData)->sampleFunction3();
    }
    void sampleCoroutine1() {
        std::cout << "Sample Coroutine 1" << std::endl;
        executionCounter += 1000;
    }
    static void sampleCoroutine1Wrapper(void* userData) {
        ((XsimCoreTest*)userData)->sampleCoroutine1();
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

/**
 * Basic test with 2 functions. Expected execution order:
 * - sampleFunction1
 * - sampleFunction2
 * - sampleFunction1
 */
TEST_F(XsimCoreTest, basic_2functions) {

    void* signals1[1];
    signals1[0] = &sampleSignal2;
    void* signals2[1];
    signals2[0] = &sampleSignal1;

    xsimCorePtr->registerFunction(XsimCoreTest::sampleFunction1Wrapper, this, 1, signals1);
    xsimCorePtr->registerFunction(XsimCoreTest::sampleFunction2Wrapper, this, 1, signals2);
    xsimCorePtr->startSim();
    ASSERT_EQ(12, executionCounter);
}

/**
 * Basic test with 3 functions. Expected execution order:
 * - sampleFunction1
 * - sampleFunction2
 * - sampleFunction3
 * - sampleFunction1
 * - sampleFunction2
 */
TEST_F(XsimCoreTest, basic_3functions) {

    void* signals1[2];
    signals1[0] = &sampleSignal3;
    signals1[1] = &sampleSignal1;
    void* signals2[2];
    signals2[0] = &sampleSignal1;
    signals2[1] = &sampleSignal3;

    xsimCorePtr->registerFunction(XsimCoreTest::sampleFunction1Wrapper, this, 2, signals1);
    xsimCorePtr->registerFunction(XsimCoreTest::sampleFunction2Wrapper, this, 2, signals2);
    xsimCorePtr->registerFunction(XsimCoreTest::sampleFunction3Wrapper, this, 0, nullptr);
    xsimCorePtr->startSim();
    ASSERT_EQ(122, executionCounter);
}

/**
 * Basic test with 2 functions and 1 coroutine. Expected execution order:
 * - sampleFunction1
 * - sampleFunction2
 * - sampleFunction1
 */
TEST_F(XsimCoreTest, basic_2functions_1coroutine) {

    void* signals1[1];
    signals1[0] = &sampleSignal2;
    void* signals2[1];
    signals2[0] = &sampleSignal1;

    xsimCorePtr->registerCoroutine(XsimCoreTest::sampleCoroutine1Wrapper, this, 1, signals1);
    xsimCorePtr->registerFunction(XsimCoreTest::sampleFunction1Wrapper, this, 1, signals1);
    xsimCorePtr->registerFunction(XsimCoreTest::sampleFunction2Wrapper, this, 1, signals2);
    xsimCorePtr->startSim();
    ASSERT_EQ(1012, executionCounter);
}
