@echo off

:: Check and set DISCO prompt if defined
if defined DISCO_PROMPT (
    PROMPT $e[32;40m$t$h$h$h$h$h$h $e[31m$l$e[1;39m$p$e[31m$g$e[36m
)

:: Check for Python 3.11 install path
if not defined PYPATH FOR /F "SKIP=2 TOKENS=1,2,*" %%G IN ('REG QUERY "HKEY_LOCAL_MACHINE\SOFTWARE\Python\PythonCore\3.11\InstallPath" /ve 2^>NUL') DO SET PYPATH=%%~sfI

:: Check for Python 3.8 install path if 3.11 is not found
if not defined PYPATH FOR /F "SKIP=2 TOKENS=1,2,*" %%G IN ('REG QUERY "HKEY_LOCAL_MACHINE\SOFTWARE\Python\PythonCore\3.8\InstallPath" /ve 2^>NUL') DO SET PYPATH=%%~sfI

:: Add Python to PATH if found
if defined PYPATH SET PATH=%PYPATH%;%PYPATH%Scripts;%PATH%

:: Set environment variables
SET PYTHONPATH=
SET PYTHON_VENV_PATH=%userprofile%\envs\SimTestLab_dev
SET PIP_FILE=%APPDATA%\pip\pip.ini
SET PIP_RETRIES=10
SET PIP_DEFAULT_TIMEOUT=180
SET PYTHONUTF8=1

:: Check if Python is installed
python -V 2>NUL 1>NUL
IF NOT %ERRORLEVEL%==0 (
    echo Error: Python is not installed.
    set FAULT=1
)

:: Check and update pip config if needed
if exist %PIP_FILE% (
    >nul findstr /c:"%NEXUS_URL%" %PIP_FILE% && (
        echo %PIP_FILE% seems to be ok. Continue...
    ) || (
        echo %PIP_FILE% does not have correct repo set. Will be updated.
        del /F /Q %PIP_FILE%
    )
)

:: Setup virtual environment
echo Setting up virtual environment for Python
python -m venv %PYTHON_VENV_PATH%

:: Activate virtual environment
echo Activating virtual environment for Python
call %PYTHON_VENV_PATH%\Scripts\activate.bat

:: Install requirements
echo Installing requirements
python -m pip install --upgrade pip
pip install -r requirements.txt

IF NOT %ERRORLEVEL%==0 (
    echo Error: Python dependencies error.
    set FAULT=1
)

:: Handle wxPython installation for Python 3.11
python --version > python_version.txt
::findstr /C:"3.11" python_version.txt > nul
::if %errorlevel%==0 (
::    echo Python version is 3.11, installing wxPython from wheel
::    pip install wheels/wxPython-4.2.1a1.dev5577+af85b581-cp311-cp311-win_amd64.whl
::)
del python_version.txt

:: Final message if no errors
IF NOT DEFINED FAULT (
    cls
    echo Welcome to the Simtestlab Python-dev environment
    echo --------------------------------------
    echo.
)
