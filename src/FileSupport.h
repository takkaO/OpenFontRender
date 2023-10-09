// -------------------------------------------------------
//  FileSupport.h
//
//  Copyright (c) 2021 takkaO
//
//  If you use, modify or redistribute this file as part of
//  the original repository, please follow the repository's license.
//
// -------------------------------------------------------

#ifndef FILE_SUPPORT_H
#define FILE_SUPPORT_H

#include <cstddef>

#define FT_FILE void
#define ft_fclose OFR_fclose
#define ft_fopen OFR_fopen
#define ft_fread OFR_fread
#define ft_fseek OFR_fseek
#define ft_ftell OFR_ftell

void OFR_fclose(FT_FILE *stream);
FT_FILE *OFR_fopen(const char *filename, const char *mode);
size_t OFR_fread(void *ptr, size_t size, size_t nmemb, FT_FILE *stream);
int OFR_fseek(FT_FILE *stream, long int offset, int whence);
long int OFR_ftell(FT_FILE *stream);

#ifdef CONFIG_SPIRAM_SUPPORT
	#define ft_scalloc ps_calloc
	#define ft_smalloc ps_malloc
	#define ft_srealloc ps_realloc
#else
	#define ft_scalloc calloc
	#define ft_smalloc malloc
	#define ft_srealloc realloc
#endif

#endif