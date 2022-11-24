# -------------------------------------------------------
# Export_CMakeLists.ps1
# 
# Copyright (c) 2022 takkaO
#
# If you use, modify, or redistribute this file independently　
# of the original repository, this program is licensed under 
# the MIT License.
# 
# If you use, modify or redistribute this file as part of 
# the original repository, please follow the repository's license.
# 
# -------------------------------------------------------

Write-Host "** Start create CMakeLists.txt **"

## Clean CMakeLists.txt
Write-Host "Processing clear old CMakeLists.txt"
Write-Output "# This file was automatically created by `"Export_CMakeLists`" script.`n" | Set-Content CMakeLists.txt

## Search source files
Write-Host "Processing search C/C++ source files"
$lst = Get-ChildItem -Path "./src" -File -Recurse -Include *.c,*.cpp | Resolve-Path -Relative
$lst = $lst -replace '\\', "/"

Write-Output "set(srcs " | Add-Content CMakeLists.txt
foreach ($f in $lst) {
    # Space for indent
    Write-Output "    `"$($f)`"" | Add-Content CMakeLists.txt
}
Write-Output ")" | Add-Content CMakeLists.txt

## Search include directory
Write-Host "Processing search include directories"
$lst = Get-ChildItem -Path "./src" -Directory -Recurse | Resolve-Path -Relative
$lst = $lst -replace '\\', "/"

Write-Output "idf_component_register(SRCS `${srcs}" | Add-Content CMakeLists.txt
Write-Output "                       INCLUDE_DIRS" | Add-Content CMakeLists.txt
Write-Output "                       `"./src`"" | Add-Content CMakeLists.txt
foreach ($f in $lst) {
    # Space for indent
    Write-Output "                       `"$($f)`"" | Add-Content CMakeLists.txt
}
Write-Output ")" | Add-Content CMakeLists.txt

Write-Host "** Finish create CMakeLists.txt **"