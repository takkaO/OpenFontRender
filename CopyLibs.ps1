# -------------------------------------------------------
# CopyLibs.sh
# 
# Copyright (c) 2021 takkaO
#
# If you use, modify, or redistribute this file independently　
# of the original repository, this program is licensed under 
# the MIT License.
# 
# If you use, modify or redistribute this file as part of 
# the original repository, please follow the repository's license.
# 
# -------------------------------------------------------

$targetDirs = Get-ChildItem -Directory | Where-Object { $_.Name -match "^freetype.*"}

if ($targetDirs.Length -eq 0) {
	Write-Host "Not Found FreeType project directory." -ForegroundColor Red
	exit
}

$title = "Delete a FreeType libraries in OpenFontRender that already exists."
$message = "Are you sure you want to proceed ?"
$typename = "System.Management.Automation.Host.ChoiceDescription"
$yes = new-object $typename("&Yes","Delete")
$no  = new-object $typename("&No","Exit")
$assembly= $yes.getType().AssemblyQualifiedName
$choice = new-object "System.Collections.ObjectModel.Collection``1[[$assembly]]"
$choice.add($yes)
$choice.add($no)
$answer = $host.ui.PromptForChoice($title,$message,$choice,1)

if ($answer -eq 1) {
	Write-Host "The process has been suspended by user."
	exit
}

Remove-Item "./src/freetype" -Recurse -Force -ErrorAction Ignore
Remove-Item "./src/base" -Recurse -Force -ErrorAction Ignore
Remove-Item "./src/cache" -Recurse -Force -ErrorAction Ignore
Remove-Item "./src/sfnt" -Recurse -Force -ErrorAction Ignore
Remove-Item "./src/smooth" -Recurse -Force -ErrorAction Ignore
Remove-Item "./src/truetype" -Recurse -Force -ErrorAction Ignore
Remove-Item "./src/ft2build.h" -Recurse -Force -ErrorAction Ignore

$FreeTypeDir = $targetDirs[0]
Write-Output "** Start Copying the necessary files **"


## Source files
$childDir = "freetype"
$targetPath = $FreeTypeDir.Name + "/include/" + $childDir
Write-Output ("Copying [" + $targetPath + "]")
if (!(Test-Path $targetPath)) {
	Write-Host "Not Found " + $targetPath -ForegroundColor Red
	exit
}
Copy-Item $targetPath -destination ("./src/" + $childDir) -recurse

$childDir = "base"
$targetPath = $FreeTypeDir.Name + "/src/" + $childDir
Write-Output ("Copying [" + $targetPath + "]")
if (!(Test-Path $targetPath)) {
	Write-Host "Not Found " + $targetPath -ForegroundColor Red
	exit
}
Copy-Item $targetPath -destination ("./src/" + $childDir) -recurse

$childDir = "cache"
$targetPath = $FreeTypeDir.Name + "/src/" + $childDir
Write-Output ("Copying [" + $targetPath + "]")
if (!(Test-Path $targetPath)) {
	Write-Host "Not Found " + $targetPath -ForegroundColor Red
	exit
}
Copy-Item $targetPath -destination ("./src/" + $childDir) -recurse

$childDir = "sfnt"
$targetPath = $FreeTypeDir.Name + "/src/" + $childDir
Write-Output ("Copying [" + $targetPath + "]")
if (!(Test-Path $targetPath)) {
	Write-Host "Not Found " + $targetPath -ForegroundColor Red
	exit
}
Copy-Item $targetPath -destination ("./src/" + $childDir) -recurse

$childDir = "smooth"
$targetPath = $FreeTypeDir.Name + "/src/" + $childDir
Write-Output ("Copying [" + $targetPath + "]")
if (!(Test-Path $targetPath)) {
	Write-Host "Not Found " + $targetPath -ForegroundColor Red
	exit
}
Copy-Item $targetPath -destination ("./src/" + $childDir) -recurse

$childDir = "truetype"
$targetPath = $FreeTypeDir.Name + "/src/" + $childDir
Write-Output ("Copying [" + $targetPath + "]")
if (!(Test-Path $targetPath)) {
	Write-Host "Not Found " + $targetPath -ForegroundColor Red
	exit
}
Copy-Item $targetPath -destination ("./src/" + $childDir) -recurse

## Header file
$childFile = "ft2build.h"
$targetPath = $FreeTypeDir.Name + "/include/" + $childFile
Write-Output ("Copying [" + $targetPath + "]")
if (!(Test-Path $targetPath)) {
	Write-Host "Not Found " + $targetPath -ForegroundColor Red
	exit
}
Copy-Item $targetPath -destination ("./src/" + $childFile)

Write-Host "** Success to all progress **"