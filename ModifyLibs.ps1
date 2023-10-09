# -------------------------------------------------------
# ModifyLibs.ps1
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

Write-Host "** Start modify FreeType libraries **"

Write-Host "Processing remove some codes and files"
foreach ($f in Get-ChildItem "./src/*/" -Include *.c -Recurse) {
    @(Get-Content $f) -replace "^(#include.*\.c)", "//`$1" | Set-Content $f
}

Remove-Item "./src/freetype/ftmac.h" -Force -ErrorAction Ignore
Remove-Item "./src/base/ftmac.c" -Force -ErrorAction Ignore
Remove-Item "./src/truetype/ttgxvar.h" -Force -ErrorAction Ignore
Remove-Item "./src/truetype/ttgxvar.c" -Force -ErrorAction Ignore

Rename-Item "./src/base/ftsystem.c" -NewName "ftsystem.cpp" -Force -ErrorAction Ignore

$target = "./src/freetype/config/ftoption.h"
Write-Host ("Processing " + $target)
# Disable some options
$(Get-Content $target) -replace "^(#define FT_CONFIG_OPTION_USE_LZW)", "//`$1" | Set-Content $target
$(Get-Content $target) -replace "^(#define FT_CONFIG_OPTION_USE_ZLIB)", "//`$1" | Set-Content $target
$(Get-Content $target) -replace "^(#define FT_CONFIG_OPTION_POSTSCRIPT_NAMES)", "//`$1" | Set-Content $target
$(Get-Content $target) -replace "^(#define FT_CONFIG_OPTION_ADOBE_GLYPH_LIST)", "//`$1" | Set-Content $target
$(Get-Content $target) -replace "^(#define FT_CONFIG_OPTION_MAC_FONTS)", "//`$1" | Set-Content $target
$(Get-Content $target) -replace "^(#define FT_CONFIG_OPTION_INCREMENTAL)", "//`$1" | Set-Content $target
$(Get-Content $target) -replace "^(#define TT_CONFIG_OPTION_EMBEDDED_BITMAPS)", "//`$1" | Set-Content $target
$(Get-Content $target) -replace "^(#define TT_CONFIG_OPTION_COLOR_LAYERS)", "//`$1" | Set-Content $target
$(Get-Content $target) -replace "^(#define TT_CONFIG_OPTION_POSTSCRIPT_NAMES)", "//`$1" | Set-Content $target
$(Get-Content $target) -replace "^(#define TT_CONFIG_OPTION_SFNT_NAMES)", "//`$1" | Set-Content $target
$(Get-Content $target) -replace "^(#define TT_CONFIG_CMAP_FORMAT_0)", "//`$1" | Set-Content $target
$(Get-Content $target) -replace "^(#define TT_CONFIG_CMAP_FORMAT_2)", "//`$1" | Set-Content $target
$(Get-Content $target) -replace "^(#define TT_CONFIG_CMAP_FORMAT_6)", "//`$1" | Set-Content $target
$(Get-Content $target) -replace "^(#define TT_CONFIG_CMAP_FORMAT_8)", "//`$1" | Set-Content $target
$(Get-Content $target) -replace "^(#define TT_CONFIG_CMAP_FORMAT_10)", "//`$1" | Set-Content $target
$(Get-Content $target) -replace "^(#define TT_CONFIG_CMAP_FORMAT_12)", "//`$1" | Set-Content $target
$(Get-Content $target) -replace "^(#define TT_CONFIG_CMAP_FORMAT_13)", "//`$1" | Set-Content $target
$(Get-Content $target) -replace "^(#define TT_CONFIG_CMAP_FORMAT_14)", "//`$1" | Set-Content $target
$(Get-Content $target) -replace "^(#define TT_CONFIG_OPTION_SUBPIXEL_HINTING)", "//`$1" | Set-Content $target
$(Get-Content $target) -replace "^(#define TT_CONFIG_OPTION_GX_VAR_SUPPORT)", "//`$1" | Set-Content $target
$(Get-Content $target) -replace "^(#define TT_CONFIG_OPTION_BDF)", "//`$1" | Set-Content $target
$(Get-Content $target) -replace "^(#define AF_CONFIG_OPTION_CJK)", "//`$1" | Set-Content $target
# Enable some options
$(Get-Content $target) -replace "^.*(#define FT_DEBUG_LEVEL_ERROR).*", "`$1" | Set-Content $target


$target = "./src/freetype/config/ftmodule.h"
Write-Host ("Processing " + $target)
# Disable all modules
$(Get-Content $target) -replace "^(FT_USE_MODULE.*)", "//`$1" | Set-Content $target
# Enable some modules
$(Get-Content $target) -replace "^.*(FT_USE_MODULE.*tt_driver_class.*)", "`$1" | Set-Content $target
$(Get-Content $target) -replace "^.*(FT_USE_MODULE.*sfnt_module_class.*)", "`$1" | Set-Content $target
$(Get-Content $target) -replace "^.*(FT_USE_MODULE.*ft_smooth_renderer_class.*)", "`$1" | Set-Content $target


$target = "./src/freetype/config/ftstdlib.h"
Write-Host ("Processing " + $target)
# Disable some options
$(Get-Content $target) -replace "^(#define FT_FILE.*)", "//`$1" | Set-Content $target
$(Get-Content $target) -replace "^(#define ft_fclose.*)", "//`$1" | Set-Content $target
$(Get-Content $target) -replace "^(#define ft_fopen.*)", "//`$1" | Set-Content $target
$(Get-Content $target) -replace "^(#define ft_fread.*)", "//`$1" | Set-Content $target
$(Get-Content $target) -replace "^(#define ft_fseek.*)", "//`$1" | Set-Content $target
$(Get-Content $target) -replace "^(#define ft_ftell.*)", "//`$1" | Set-Content $target

$(Get-Content $target) -replace "^(#define ft_scalloc.*)", "//`$1" | Set-Content $target
$(Get-Content $target) -replace "^(#define ft_smalloc.*)", "//`$1" | Set-Content $target
$(Get-Content $target) -replace "^(#define ft_srealloc.*)", "//`$1" | Set-Content $target


$target = "./src/ft2build.h"
if ( !($(Get-Content $target) -match "#define FT2_BUILD_LIBRARY") ) {
    # If the required definition does not exist
    Write-Host ("Processing " + $target)
    $(Get-Content -Raw $target) -replace "(?smi)^(#define \w*)([\r\n]+)", "`$1`$2#define FT2_BUILD_LIBRARY`$2" | Set-Content $target
    #$(Get-Content $target) | Set-Content $target
}

$target = "./src/base/ftsystem.cpp"
if ( !($(Get-Content $target) -match "#include `"../FileSupport.h`"") ) {
    # If the required definition does not exist
    Write-Host ("Processing " + $target)
    $(Get-Content -Raw $target) -replace "(?smi)^(#include .*)([\r\n]+)", "#include `"../FileSupport.h`"`$2`$1`$2" | Set-Content $target
    #$(Get-Content $target) | Set-Content $target
}

Write-Host "** Finish modify FreeType libraries **"
