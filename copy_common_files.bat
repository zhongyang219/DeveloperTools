REM 复制common路径下的文件到运行目录

xcopy /y /e .\common .\bin\x64_vc15_Debug\
xcopy /y /e .\common .\bin\x64_vc15_Release\
pause