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

#include <list>

#if defined(ARDUINO_M5Stack_Core_ESP32) || defined(ARDUINO_M5STACK_FIRE) || defined(ARDUINO_M5STACK_Core2)
#define ENABLE_M5STACK
#endif

#if defined(ARDUINO_WIO_TERMINAL)
#include <SPI.h>
#include <Seeed_Arduino_FS.h>

#else if defined(ENABLE_M5STACK)
#include <SD.h>
#include <SPIFFS.h>
#endif

#if defined(ARDUINO_WIO_TERMINAL) || defined(ENABLE_M5STACK)

// TODO: want to make the program more abstract and common.
#define FT_FILE fileclass_t
#define ft_fclose ffsupport_fclose
#define ft_fopen ffsupport_fopen
#define ft_fread ffsupport_fread
#define ft_fseek ffsupport_fseek
#define ft_ftell ffsupport_ftell

typedef struct
{
	File _fstream;
} fileclass_t;

void ffsupport_setffs(fs::FS &ffs);
fileclass_t *ffsupport_fopen(const char *Filename, const char *mode);
void ffsupport_fclose(fileclass_t *stream);
size_t ffsupport_fread(void *ptr, size_t size, size_t nmemb, fileclass_t *stream);
int ffsupport_fseek(fileclass_t *stream, long int offset, int whence);
long int ffsupport_ftell(fileclass_t *stream);

#else

#define FT_FILE FILE
#define ft_fclose fclose
#define ft_fopen fopen
#define ft_fread fread
#define ft_fseek fseek
#define ft_ftell ftell

#endif

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