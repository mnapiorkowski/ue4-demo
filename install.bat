@echo off
set runtimeDir=Plugins\UnrealMono\Runtime\Win64\lib\
set monoLib=mono-2.0-sgen.lib

if not exist %runtimeDir% (mkdir %runtimeDir%)
if not exist %runtimeDir%\%monoLib% for %%p in ("%PATH:;=";"%") do (
    if exist %%~p\..\lib\%monoLib% (xcopy "%%~p\..\lib\%monoLib%" "%runtimeDir%")  
)

if not exist Managed\ (mkdir Managed\)
mcs Test\UnrealMonoTest.cs -target:library -out:Managed\test.dll -unsafe