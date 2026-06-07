#!/bin/bash
mkdir -p ./bin/debug/
mkdir -p ./bin/release/
cp -r ./common/* ./bin/debug/DeveloperTools.app/Contents/Resources/
cp -r ./common/* ./bin/release/DeveloperTools.app/Contents/Resources/
