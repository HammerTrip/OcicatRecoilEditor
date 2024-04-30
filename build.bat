@echo off

if exist build\ (
	rd /s /q "build"
)

mkdir build

cmake -GNinja .
ninja

xcopy .\lib\SFML-2.6.1\bin\sfml-graphics-d-2.dll build\ /D /Y /C /K
xcopy .\lib\SFML-2.6.1\bin\sfml-system-d-2.dll build\ /D /Y /C /K
xcopy .\lib\SFML-2.6.1\bin\sfml-window-d-2.dll build\ /D /Y /C /K