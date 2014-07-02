#include <gtest/gtest.h>
#include <iostream>

#include "Coroutine.h"

using namespace xsim;

// Test fixture.
class CoroutineTest : public testing::Test {
public:

    /**
     * The instance under test.
     */
    Coroutine* coroutinePtr;

    /**
     * The execution counter that keeps track where we are in the coroutine.
     */
    int executionCounter = 0;

    /**
     * Set up test environment.
     */
    virtual void SetUp() {
    }

    /**
     * Tear down the test environment.
     */
    virtual void TearDown() {
        if (coroutinePtr != nullptr) {
            delete coroutinePtr;
        }
    }

    void sampleCoroutine() {
        std::cout << "Sample Coroutine" << std::endl;
        for (int i = 0; i < 10; i++) {
            executionCounter++;
            coroutinePtr->pause();
        }
    }
    static void sampleCoroutineWrapper(void* userData) {
        ((CoroutineTest*)userData)->sampleCoroutine();
    }

};

/**
 * Basic coroutine test where we run the coroutine once.
 */
TEST_F(CoroutineTest, basic) {
    coroutinePtr = new Coroutine(CoroutineTest::sampleCoroutineWrapper, this);
    coroutinePtr->start();
    int expectedCount = 0;
    for (int i = 0; i < 10; i++) {
        ASSERT_TRUE(coroutinePtr->isStarted());
        ASSERT_FALSE(coroutinePtr->isFinished());
        ASSERT_EQ(++expectedCount, executionCounter);
        coroutinePtr->start();
    }
    ASSERT_TRUE(coroutinePtr->isFinished());
}

/**
 * Coroutine test where we run the same coroutine twice.
 */
TEST_F(CoroutineTest, reentrant) {
    coroutinePtr = new Coroutine(CoroutineTest::sampleCoroutineWrapper, this);
    coroutinePtr->start();
    int expectedCount = 0;
    for (int i = 0; i < 10; i++) {
        ASSERT_TRUE(coroutinePtr->isStarted());
        ASSERT_FALSE(coroutinePtr->isFinished());
        ASSERT_EQ(++expectedCount, executionCounter);
        coroutinePtr->start();
    }
    ASSERT_TRUE(coroutinePtr->isFinished());

    std::cout << "Play it again." << std::endl;

    // start it again
    executionCounter = 0;
    coroutinePtr->reset();
    coroutinePtr->start();
    expectedCount = 0;
    for (int i = 0; i < 10; i++) {
        ASSERT_TRUE(coroutinePtr->isStarted());
        ASSERT_FALSE(coroutinePtr->isFinished());
        ASSERT_EQ(++expectedCount, executionCounter);
        coroutinePtr->start();
    }
    ASSERT_TRUE(coroutinePtr->isFinished());
}
