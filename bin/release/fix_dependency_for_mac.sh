#!/bin/bash
APP="DeveloperTools.app"
EXEC="$APP/Contents/MacOS/DeveloperTools"
FW="$APP/Contents/Frameworks"

# 复制自定义库到 Frameworks
mkdir -p "$FW"
cp libCCommonTools.1.dylib libRibbonFrame.1.dylib "$FW/"

# 修复库的 install_name
for lib in CCommonTools RibbonFrame; do
    install_name_tool -id "@rpath/lib${lib}.1.dylib" "$FW/lib${lib}.1.dylib"
done

# 为主程序添加 rpath
install_name_tool -add_rpath "@executable_path/../Frameworks" "$EXEC"

# 修复主程序的库引用
for lib in CCommonTools RibbonFrame; do
    install_name_tool -change "lib${lib}.1.dylib" "@rpath/lib${lib}.1.dylib" "$EXEC"
done

# 修复库之间的相互依赖
for lib in AddCodeHeader FileRename PixelRuler WallpaperTool; do
    install_name_tool -change libCCommonTools.1.dylib @rpath/libCCommonTools.1.dylib "$FW/lib${lib}.dylib"
done

# 重新签名（开发测试用）
codesign --force --deep -s - "$APP"

echo "✅ 修复完成，请测试运行：open $APP"