#ifndef PYTHONTHREAD_HPP
#define PYTHONTHREAD_HPP

#include "UtilsGlobal.hpp"

//Avoid conflict with Qt slots keyword
#undef slots
#include <Python.h>
#define slots

///@cond INTERNAL

class CPyAllowThreads
{
    public:

        CPyAllowThreads() : _state(PyEval_SaveThread()){}
        ~CPyAllowThreads(){ PyEval_RestoreThread(_state); }

    private:

        PyThreadState* _state;
};

class CPyEnsureGIL
{
    public:

        CPyEnsureGIL() : _state(PyGILState_Ensure()){}
        ~CPyEnsureGIL(){ PyGILState_Release(_state); }

    private:

        PyGILState_STATE _state;
};

///@endcond

#endif // PYTHONTHREAD_HPP
