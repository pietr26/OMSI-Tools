@echo off
echo Setting up environment...
echo.
echo.
set PATH=C:\Qt\6.3.0\mingw_64\bin;C:/Qt/Tools/mingw1120_64/bin;%PATH%

cd C:\Qt\6.3.0\mingw_64\bin
lupdate-pro.exe C:\Users\pietr\OneDrive\Dev\OMSI-Tools\OMSI-Tools\OMSI-Tools.pro -locations absolute -no-obsolete

echo ------------------------------------------------------------------------------------------
echo Finished.
timeout 4