# Unreal Engine 4.27 demo - integration with embedded Mono Runtime

## Licenses

### Game
Game Assets: Licensed for use with the Unreal Engine only. Without a custom license you cannot use to create sequels, remasters, or otherwise emulate the original game or use the original game’s trademarks, character names, or other IP to advertise or name your game. (Unreal Engine EULA applies) (Please note this applies to the Game Assets that refer to Epic's Paragon, you can still use the project code and content to build your own Unreal Engine game)

### Mono Runtime
In general, the runtime and its class libraries are licensed under the terms of the MIT license, and some third party code is licensed under the 3-clause BSD license.
Check details [here](https://github.com/mono/mono/blob/main/LICENSE).

## Prerequisites
Current version works only on Windows. You need to have Mono x64 installed and copy 'lib' directory to Plugins/UnrealMono/Runtime/Win64/ (in fact, just 'lib/mono-2.0-sgen.lib' should be enough).

## Test
From the root directory of your game run:
```
mcs Test/UnrealMonoTest.cs -target:library -out:Managed/test.dll
```
This DLL will be loaded into Mono Runtime after you push Enter while playing the game. Then player character's jump height will be multiplied by the factor returned from the compiled function.