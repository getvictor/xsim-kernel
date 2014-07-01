#ifndef COROUTINE_H_
#define COROUTINE_H_

#include <boost/coroutine/v2/coroutine.hpp>

#include "XsimTypedefs.h"

namespace xsim {

/**
 * The coroutine, which can take time.
 */
class Coroutine {
public:

    /**
     * The coroutine source. Used to start/resume the coroutine.
     */
    boost::coroutines::coroutine<int>::pull_type *source;

    /**
     * The coroutine sink. Used to pause the coroutine.
     */
    boost::coroutines::coroutine<int>::push_type *sink;

private:
    /**
     * Flag indicating whether the coroutine has started.
     */
    bool started = false;

    /**
     * Flag indicating whether the coroutine is paused.
     */
    bool paused = false;

    /**
     * Flag indicating whether the coroutine is finished.
     */
    bool finished = false;

public:

    /**
     * Constructor.
     * @param coroutine The coroutine function.
     * @param userData The user data for coroutine function.
     */
    Coroutine(CoroutinePtr coroutine, void* userData);

    /**
     * Destructor.
     */
    ~Coroutine();

    /**
     * Pause the coroutine.
     */
    void pause();

    /**
     * Start the coroutine.
     */
    void start();

    /**
     * Has coroutine execution started?
     */
    bool isStarted() {
        return started;
    }

    /**
     * Has coroutine execution finished?
     */
    bool isFinished() {
        return finished;
    }

};
}



#endif /* COROUTINE_H_ */
