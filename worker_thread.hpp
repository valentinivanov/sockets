#ifndef VI_WORKER_THREAD_HPP
#define VI_WORKER_THREAD_HPP

#include "common_inc.h"

namespace vi
{
    template<class T>
    class worker_thread
    {
    private:

        T mTask;

        pthread_t mThread;
        bool mThreadCreated;

    public:

        worker_thread(const T& task) : mTask(task), 
                                       mThread(0),
                                       mThreadCreated(false)
        {
        }

        void start()
        {
            mThreadCreated = (0 == pthread_create(&mThread, NULL, do_work, this));
        }

        void join()
        {
            if (mThreadCreated)
            {
                pthread_join(mThread, NULL);
            }
        }

    private:

        static void* do_work(void* pVoidThis)
        {
            worker_thread<T>* pThis = reinterpret_cast<worker_thread<T>*>(pVoidThis);

            pThis->mTask.begin();

            while(pThis->mTask.step())
            {
            }

            pThis->mTask.end();

            return NULL;
        }
    };
};

#endif