#!/bin/bash

# -------------------------------------------------------
# ModifyLibs.sh
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

echo "** Start modify FreeType libraries **"

echo "Processing remove some codes and files"
for eachValue in `find ./src -name *.c` ; do
	sed -i -e "s|^\(#include.*\.c\)|//\1|" ${eachValue}
done

rm -f "./src/freetype/ftmac.h"
rm -f "./src/base/ftmac.c"
rm -f "./src/truetype/ttgxvar.h"
rm -f "./src/truetype/ttgxvar.c"

mv -f "./src/base/ftsystem.c" "./src/base/ftsystem.cpp"

target="./src/freetype/config/ftoption.h"
echo "Processing ${target}"
# Disable some options
sed -i \
-e "s|^\(#define FT_CONFIG_OPTION_USE_LZW\)|//\1|" \
-e "s|^\(#define FT_CONFIG_OPTION_USE_ZLIB\)|//\1|" \
-e "s|^\(#define FT_CONFIG_OPTION_POSTSCRIPT_NAMES\)|//\1|" \
-e "s|^\(#define FT_CONFIG_OPTION_ADOBE_GLYPH_LIST\)|//\1|" \
-e "s|^\(#define FT_CONFIG_OPTION_MAC_FONTS\)|//\1|" \
-e "s|^\(#define FT_CONFIG_OPTION_INCREMENTAL\)|//\1|" \
-e "s|^\(#define TT_CONFIG_OPTION_EMBEDDED_BITMAPS\)|//\1|" \
-e "s|^\(#define TT_CONFIG_OPTION_COLOR_LAYERS\)|//\1|" \
-e "s|^\(#define TT_CONFIG_OPTION_POSTSCRIPT_NAMES\)|//\1|" \
-e "s|^\(#define TT_CONFIG_OPTION_SFNT_NAMES\)|//\1|" \
-e "s|^\(#define TT_CONFIG_CMAP_FORMAT_0\)|//\1|" \
-e "s|^\(#define TT_CONFIG_CMAP_FORMAT_2\)|//\1|" \
-e "s|^\(#define TT_CONFIG_CMAP_FORMAT_6\)|//\1|" \
-e "s|^\(#define TT_CONFIG_CMAP_FORMAT_8\)|//\1|" \
-e "s|^\(#define TT_CONFIG_CMAP_FORMAT_10\)|//\1|" \
-e "s|^\(#define TT_CONFIG_CMAP_FORMAT_12\)|//\1|" \
-e "s|^\(#define TT_CONFIG_CMAP_FORMAT_13\)|//\1|" \
-e "s|^\(#define TT_CONFIG_CMAP_FORMAT_14\)|//\1|" \
-e "s|^\(#define TT_CONFIG_OPTION_SUBPIXEL_HINTING\)|//\1|" \
-e "s|^\(#define TT_CONFIG_OPTION_GX_VAR_SUPPORT\)|//\1|" \
-e "s|^\(#define TT_CONFIG_OPTION_BDF\)|//\1|" \
-e "s|^\(#define AF_CONFIG_OPTION_CJK\)|//\1|" \
${target}
# Enable some options
sed -i -e "s|^.*\(#define FT_DEBUG_LEVEL_ERROR\).*|\1|" \
${target}


target="./src/freetype/config/ftmodule.h"
echo "Processing ${target}"
# Disable all modules
sed -i \
-e "s|^\(FT_USE_MODULE.*\)|//\1|" \
$target
# Enable some modules
sed -i \
-e "s|^.*\(FT_USE_MODULE.*tt_driver_class.*\)|\1|" \
-e "s|^.*\(FT_USE_MODULE.*sfnt_module_class.*\)|\1|" \
-e "s|^.*\(FT_USE_MODULE.*ft_smooth_renderer_class.*\)|\1|" \
$target


target="./src/freetype/config/ftstdlib.h"
echo "Processing ${target}"
# Disable all modules
sed -i \
-e "s|^\(#define FT_FILE.*\)|//\1|" \
-e "s|^\(#define ft_fclose.*\)|//\1|" \
-e "s|^\(#define ft_fopen.*\)|//\1|" \
-e "s|^\(#define ft_fread.*\)|//\1|" \
-e "s|^\(#define ft_fseek.*\)|//\1|" \
-e "s|^\(#define ft_ftell.*\)|//\1|" \
-e "s|^\(#define ft_scalloc.*\)|//\1|" \
-e "s|^\(#define ft_smalloc.*\)|//\1|" \
-e "s|^\(#define ft_srealloc.*\)|//\1|" \
$target


target="./src/ft2build.h"
if ! grep -q "^#define FT2_BUILD_LIBRARY" ${target}; then
	echo "Processing ${target}"
	sed -i -z -e "s|\(.*\)\(#define [a-zA-Z_0-9]\+\)\([\r\n]\+\)\(.*\)|\1\2\3#define FT2_BUILD_LIBRARY\3\4|" ${target}
fi


target="./src/base/ftsystem.cpp"
if ! grep -q "^#include \"../FileSupport.h\"" ${target}; then
	echo "Processing ${target}"
	sed -i -z -e "s|\(.*\)\(#include [a-zA-Z_0-9]\+\)\([\r\n]\+\)\(.*\)|\1#include \"../FileSupport.h\"\3\2\3\4|" ${target}
fi


echo "** Finish modify FreeType libraries **"