Windows Path Finding application

This is a temporary test application to solve the best path battle units should follow to reach a target position, taking into account walls.

Maps and map images are compatible with the RiskyLab Tilemap online map editor (https://riskylab.com/tilemap/).

Battle units movements history can be exported to a JSON file format.

Please see the repository folder Example Files for more information and examples.

Source code done in 2025 by Sergio Dilonardo - Geneva

_______________________________________________________________________________________________________

System requirements:

Windows 10 or upper (Direct2D)
Ideally a full HD display 1920 x 1080 pixels
_______________________________________________________________________________________________________



For developers:
===============

C++ MFC std 17 based application, done with Visual Studio


Windows Path Finding application GitHub repository:

https://github.com/SergioFrontTest/PathFinding/blob/main/README.md
_______________________________________________________________________________________________________

Third party source code:

JSON support:
nlohmann/json
https://stackoverflow.com/questions/48659797/how-to-setup-json-library-in-c-microsoft-visual-studio
https://github.com/nlohmann/json
_______________________________________________________________________________________________________

To build and run this project using CMake 3.8 or upper:

1) You will need CMake.

2) Create a build directory (if you haven't already):

   Windows Command Prompt:

       md build
       cd build

2) Run CMake with Release configuration, Windows Command Prompt:

       cmake -DCMAKE_BUILD_TYPE=Release ..

   This tells CMake to generate build files with optimizations for Release mode.
   Make sure the .. points to the directory where the CMakeLists.txt is located.

3) Build the project:

       cmake --build . --config Release

   The --config Release flag is especially useful on multi-configuration generators like Visual Studio.
   On single-configuration generators (like Makefiles), the CMAKE_BUILD_TYPE set earlier is sufficient.

4) Find and run the generated executable binary file named:

       PathFinding.exe
_______________________________________________________________________________________________________

The main application entry function is in file PathFinding.cpp:    BOOL CPathFindingApp::InitInstance()
