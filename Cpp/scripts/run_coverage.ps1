# Build with coverage, run unit tests, and report line coverage with gcovr.
param(
    [string]$BuildDir = "$PSScriptRoot\..\build"
)

$Root = (Resolve-Path "$PSScriptRoot\..").Path
Set-Location $Root

if (-not (Test-Path $BuildDir)) {
    cmake -B $BuildDir -G Ninja -DCMAKE_CXX_COMPILER=C:/msys64/mingw64/bin/g++.exe -DENABLE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug
}

cmake --build $BuildDir
Get-ChildItem -Path $BuildDir -Recurse -Filter *.gcda -ErrorAction SilentlyContinue | Remove-Item -Force

Set-Location "$BuildDir\test_workdir"
& "$BuildDir\test\unit_tests.exe"
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

Set-Location $Root
python -m gcovr --config gcovr.cfg
