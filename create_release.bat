@echo off

set RELEASE_EXE_PATH=build\Desktop_Qt_6_8_0_MinGW_64_bit-Release\chemical-searcher.exe

rmdir /s /q release

mkdir release
mkdir release\files

windeployqt --release --dir release\files %RELEASE_EXE_PATH%
copy /y "lib\discord\discord_game_sdk.dll" "release\files\discord_game_sdk.dll"

copy /y "%RELEASE_EXE_PATH%" "release\files\"

cd release\files
7z a -tzip ..\ChemicalSearcher.zip *
7z a -tzip ..\ChemicalSearcher.7z *

cd ..\..