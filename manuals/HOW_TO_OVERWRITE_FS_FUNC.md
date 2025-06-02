
# How to load font files from file system {#how_to_load_font_files_from_file_system}

Control of file systems such as SD/TF cards and SPIFFS is strongly hardware-dependent.  
OpenFontRender supports the following reads as presets.  

-   M5Stack
    -   SD/TF (`ofrfs/M5Stack_SD_Preset.h`)
    -   SPIFFS (`ofrfs/M5Stack_SPIFFS_Preset.h`)
-   Wio Terminal
    -   SD/TF (`ofrfs/WioTerminal_SD_Preset.h`)

## How to use preset

If you only use a preset, simply include a header in your code.  
However, the file system initialization code must be written in the user code.  

See [example](https://github.com/takkaO/OpenFontRender/tree/master/examples) for complete code.

```cpp
// (omitted)

#include "OpenFontRender.h"
#include "ofrfs/M5Stack_SD_Preset.h" // Use preset

OpenFontRender render;

void setup()
{
	// put your setup code here, to run once:
	auto cfg = M5.config();
	M5.begin(cfg);
	M5.Display.fillScreen(TFT_BLACK);

	// File system initialization code
	SD.begin(GPIO_NUM_4, SPI, 40000000);

	render.setSerial(Serial);	  // Need to print render library message
	render.showFreeTypeVersion(); // print FreeType version
	render.showCredit();		  // print FTL credit

	if (render.loadFont("/JKG-M_3_Tiny.ttf"))
	{
		Serial.println("Render initialize error");
		return;
	}

	render.setDrawer(M5.Display); // Set drawer object

// (omitted)
```

## How to read from a file system without presets

If you want to use a microcomputer or file system for which no presets are provided, you have to write a custom code.  
There are five global functions you have to write.  

-   `OFR_fclose (fclose)`
-   `OFR_fopen (fopen)`
-   `OFR_fread (fread)`
-   `OFR_fseek (fseek)`
-   `OFR_ftell (ftell)`

The processing performed by these is the same as the C language functions inside the parentheses.  
A step-by-step implementation flow is shown below.  
In this explain, the hardware will use M5Stack and assume reading from an SD card.  

**1. Declare file object**

First, declare the file object in global scope.  
Since the file object will continue to be used for the entire time the file is open, we declare it in global scope, not local scope.  

```cpp
File myFile;
```

**2. Implement the `OFR_fopen`**

Next, implement the `OFR_fopen` function, which handles file opening.  
The arguments and return values are as follows.  

|    type    | variable | mean                               |
| :--------: | -------- | ---------------------------------- |
|  argument  | filename | The path to the file to be opened. |
|  argument  | mode     | File access mode.                  |
| **return** |          | Pointer to the file object.        |

The details of the arguments `filename` and `mode` are the same as for the C language `fopen` function.  

The reference implementation below opens a file with `SD.open` and returns a pointer to the file object.  

```cpp
FT_FILE *OFR_fopen(const char *filename, const char *mode)
{
	myFile = SD.open(filename, mode);
	return &myFile;
}
```

**3. Implement the `OFR_fclose`**

Next, implement the `OFR_close` function, which handles file closing.  

The arguments and return values are as follows.  

|    type    | variable | mean                                                                          |
| :--------: | -------- | ----------------------------------------------------------------------------- |
|  argument  | stream   | Pointer to the file object.<br/>In this example, it is a pointer to `myFile`. |
| **return** |          | None                                                                          |

\note
File object pointer that given as argument must **ALWAYS** be cast before use in a function.  

The reference implementation below.  
Also `((File *)stream)->close()` has the same meaning as `myFile.close()`.  

```cpp
void OFR_fclose(FT_FILE *stream)
{
	((File *)stream)->close();
}
```

**4. Implement the `OFR_fread`**

Next, implement the `OFR_fread` function, which handles file reading.  
The arguments and return values are as follows.  

|    type    | variable | mean                                                    |
| :--------: | -------- | ------------------------------------------------------- |
|  argument  | ptr      | Pointer to the array where the read objects are stored. |
|  argument  | size     | Size of each object in bytes.                           |
|  argument  | nmemb    | The number of the objects to be read.                   |
|  argument  | stream   | Pointer to the file object.                             |
| **return** |          | Number of objects read successfully.                    |

The details of the arguments are the same as for the C language `fread` function.  

\note
File object pointer that given as argument must **ALWAYS** be cast before use in a function.  

The reference implementation below.  

```cpp
size_t OFR_fread(void *ptr, size_t size, size_t nmemb, FT_FILE *stream)
{
	return ((File *)stream)->read((uint8_t *)ptr, size * nmemb);
}
```

**5. Implement the `OFR_fseek`**

Next, implement the `OFR_fseek` function, which handles moving file pointer.  
The arguments and return values are as follows.  

|    type    | variable | mean                                                           |
| :--------: | -------- | -------------------------------------------------------------- |
|  argument  | stream   | Pointer to the file object.                                    |
|  argument  | offset   | Number of characters to shift the position relative to origin. |
|  argument  | whence   | Position to which offset is added.                             |
| **return** |          | 0​ upon success, nonzero value otherwise.                      |

The details of the arguments are the same as for the C language `fseek` function.  

\note
File object pointer that given as argument must **ALWAYS** be cast before use in a function.  

The reference implementation below.  

```cpp
int OFR_fseek(FT_FILE *stream, long int offset, int whence)
{
	return ((File *)stream)->seek(offset, (SeekMode)whence);
}
```

**6. Implement the `OFR_ftell`**

Finally, implement the `OFR_ftell` function, which get current position of the file pointer within a file.  
The arguments and return values are as follows.  

|    type    | variable | mean                                                         |
| :--------: | -------- | ------------------------------------------------------------ |
|  argument  | stream   | Pointer to the file object.                                  |
| **return** |          | File position indicator on success or -1L if failure occurs. |

The details of the arguments are the same as for the C language `ftell` function.  

\note
File object pointer that given as argument must **ALWAYS** be cast before use in a function.  

The reference implementation below.  

```cpp
long int OFR_ftell(FT_FILE *stream)
{
	return ((File *)stream)->position();
}
```

#### Conclusion

All implementation is now complete. 
The entire sample program is shown. 
However, unnecessary parts before and after are omitted (...).  
See [example](https://github.com/takkaO/OpenFontRender/tree/master/examples) for complete code.

```cpp
// (omitted)

#include "OpenFontRender.h" // Include after M5Unified.h

OpenFontRender render;

/*=== Overwrite SD (File) operation methods BEGIN ===*/
File myFile;

FT_FILE *OFR_fopen(const char *filename, const char *mode)
{
	myFile = SD.open(filename, mode);
	return &myFile;
}

void OFR_fclose(FT_FILE *stream)
{
	// myFile.close()
	((File *)stream)->close();
}

size_t OFR_fread(void *ptr, size_t size, size_t nmemb, FT_FILE *stream)
{
	// myFile.read()
	return ((File *)stream)->read((uint8_t *)ptr, size * nmemb);
}

int OFR_fseek(FT_FILE *stream, long int offset, int whence)
{
	// myFile.seek()
	return ((File *)stream)->seek(offset, (SeekMode)whence);
}

long int OFR_ftell(FT_FILE *stream)
{
	// myFile.position()
	return ((File *)stream)->position();
}
/*=== Overwrite SD (File) operation methods END ===*/

void setup()
{

// (omitted)
```
