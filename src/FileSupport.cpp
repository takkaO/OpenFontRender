#include "FileSupport.h"

#if defined(ARDUINO_WIO_TERMINAL) || defined(ENABLE_M5STACK)

fileclass_t fileclass;
fs::FS &fontFS = SD;

void ffsupport_setffs(fs::FS &ffs) {
	fontFS = ffs;
}

fileclass_t *ffsupport_fopen(const char *Filename, const char *mode) {
#if defined(ARDUINO_WIO_TERMINAL)
	// For WioTerminal
	if (strcmp(mode, "r") == 0) {
		fileclass._fstream = fontFS.open(Filename, FA_READ);
	} else if (strcmp(mode, "r+") == 0) {
		fileclass._fstream = fontFS.open(Filename, FA_READ | FA_WRITE);
	} else if (strcmp(mode, "w") == 0) {
		fileclass._fstream = fontFS.open(Filename, FA_CREATE_ALWAYS | FA_WRITE);
	} else if (strcmp(mode, "w+") == 0) {
		fileclass._fstream = fontFS.open(Filename, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
	} else if (strcmp(mode, "a") == 0) {
		fileclass._fstream = fontFS.open(Filename, FA_OPEN_APPEND | FA_WRITE);
	} else if (strcmp(mode, "a+") == 0) {
		fileclass._fstream = fontFS.open(Filename, FA_OPEN_APPEND | FA_WRITE | FA_READ);
	} else if (strcmp(mode, "wx") == 0) {
		fileclass._fstream = fontFS.open(Filename, FA_CREATE_NEW | FA_WRITE);
	} else if (strcmp(mode, "w+x") == 0) {
		fileclass._fstream = fontFS.open(Filename, FA_CREATE_NEW | FA_WRITE | FA_READ);
	} else {
		fileclass._fstream = fontFS.open(Filename, FA_READ);
	}
#else
	// For M5Stack and others
	fileclass._fstream = fontFS.open(Filename, mode);
#endif
	return &fileclass;
}

void ffsupport_fclose(fileclass_t *stream) {
	stream->_fstream.close();
}

size_t ffsupport_fread(void *ptr, size_t size, size_t nmemb, fileclass_t *stream) {
	return stream->_fstream.read((uint8_t *)ptr, size * nmemb);
}

int ffsupport_fseek(fileclass_t *stream, long int offset, int whence) {
	return stream->_fstream.seek(offset, (SeekMode)whence);
}

long int ffsupport_ftell(fileclass_t *stream) {
	return stream->_fstream.position();
}

#endif