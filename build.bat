@echo off
IF NOT EXIST bin	mkdir bin
pushd bin
cl -Zi -FC -W3 -WX -wd4577 -I"C:\Users\Chris\dev\stb"^
 -I"C:\Users\Chris\dev\glm"^
 -I"C:\Users\Chris\dev\glfw\include"^
 -I"C:\Users\Chris\dev\glew\include"^
 ..\tetris.cpp^
 -link opengl32.lib^
 -LIBPATH:"C:\Users\Chris\dev\glfw\lib-vc2015" glfw3dll.lib^
 -LIBPATH:"C:\Users\Chris\dev\glew\lib\Release\x64" glew32.lib^
 -SUBSYSTEM:CONSOLE
popd bin