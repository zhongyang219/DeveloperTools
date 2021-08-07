REM 复制common路径下的文件到运行目录

xcopy .\common .\bin\x64_vc15_Unicode_Debug\ /s /f /h
xcopy .\common .\bin\x64_vc15_Unicode_Release\ /s /f /h
pause