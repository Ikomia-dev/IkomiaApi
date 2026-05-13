@echo off
if "%PYTHON%"=="" set PYTHON=python
%PYTHON% gen_stubs.py -m ikomia.core.pycore -o %~dp0..\
%PYTHON% gen_stubs.py -m ikomia.dataio.pydataio -o %~dp0..\
%PYTHON% gen_stubs.py -m ikomia.dataprocess.pydataprocess -o %~dp0..\
%PYTHON% gen_stubs.py -m ikomia.utils.pyutils -o %~dp0..\
