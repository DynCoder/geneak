cd scripts

echo building...
call build_debug.bat

cd ../bin

echo running...
echo.

geneak.exe

echo.
cd ..

@pause