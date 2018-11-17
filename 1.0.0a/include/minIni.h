#ifndef MININI_H
#define MININI_H

#include "minGlue.h"

#define char char

#if !defined INI_BUFFERSIZE
  #define INI_BUFFERSIZE  512
#endif

#if defined __cplusplus
  extern "C" {
#endif

int   ini_getbool(char *Section, char *Key, int DefValue, char *Filename);
long  ini_getl(char *Section, char *Key, long DefValue, char *Filename);
int   ini_gets(char *Section, char *Key, char *DefValue, char *Buffer, int BufferSize, char *Filename);
int   ini_getsection(int idx, char *Buffer, int BufferSize, char *Filename);
int   ini_getkey(char *Section, int idx, char *Buffer, int BufferSize, char *Filename);

#if defined INI_REAL
INI_REAL ini_getf(char *Section, char *Key, INI_REAL DefValue, char *Filename);
#endif

#if !defined INI_READONLY
int   ini_putl(char *Section, char *Key, long Value, char *Filename);
int   ini_puts(char *Section, char *Key, char *Value, char *Filename);
#if defined INI_REAL
int   ini_putf(char *Section, char *Key, INI_REAL Value, char *Filename);
#endif
#endif /* INI_READONLY */

#if !defined INI_NOBROWSE
typedef int (*INI_CALLBACK)(char *Section, char *Key, char *Value, void *UserData);
int  ini_browse(INI_CALLBACK Callback, void *UserData, char *Filename);
#endif /* INI_NOBROWSE */

#if defined __cplusplus
  }
#endif


#if defined __cplusplus

#if defined __WXWINDOWS__
	#include "wxMinIni.h"
#else
  #include <string>

  /* The C++ class in minIni.h was contributed by Steven Van Ingelgem. */
  class minIni
  {
  public:
    minIni(std::string& filename) : iniFilename(filename)
      { }

    bool getbool(std::string& Section, std::string& Key, bool DefValue=false) const
      { return ini_getbool(Section.c_str(), Key.c_str(), int(DefValue), iniFilename.c_str()) != 0; }

    long getl(std::string& Section, std::string& Key, long DefValue=0) const
      { return ini_getl(Section.c_str(), Key.c_str(), DefValue, iniFilename.c_str()); }

    int geti(std::string& Section, std::string& Key, int DefValue=0) const
      { return static_cast<int>(this->getl(Section, Key, long(DefValue))); }

    std::string gets(std::string& Section, std::string& Key, std::string& DefValue="") const
      {
        char buffer[INI_BUFFERSIZE];
        ini_gets(Section.c_str(), Key.c_str(), DefValue.c_str(), buffer, INI_BUFFERSIZE, iniFilename.c_str());
        return buffer;
      }

    std::string getsection(int idx) const
      {
        char buffer[INI_BUFFERSIZE];
        ini_getsection(idx, buffer, INI_BUFFERSIZE, iniFilename.c_str());
        return buffer;
      }

    std::string getkey(std::string& Section, int idx) const
      {
        char buffer[INI_BUFFERSIZE];
        ini_getkey(Section.c_str(), idx, buffer, INI_BUFFERSIZE, iniFilename.c_str());
        return buffer;
      }

#if defined INI_REAL
    INI_REAL getf(std::string& Section, std::string& Key, INI_REAL DefValue=0) const
      { return ini_getf(Section.c_str(), Key.c_str(), DefValue, iniFilename.c_str()); }
#endif

#if ! defined INI_READONLY
    bool put(std::string& Section, std::string& Key, long Value) const
      { return ini_putl(Section.c_str(), Key.c_str(), Value, iniFilename.c_str()) != 0; }

    bool put(std::string& Section, std::string& Key, int Value) const
      { return ini_putl(Section.c_str(), Key.c_str(), (long)Value, iniFilename.c_str()) != 0; }

    bool put(std::string& Section, std::string& Key, bool Value) const
      { return ini_putl(Section.c_str(), Key.c_str(), (long)Value, iniFilename.c_str()) != 0; }

    bool put(std::string& Section, std::string& Key, std::string& Value) const
      { return ini_puts(Section.c_str(), Key.c_str(), Value.c_str(), iniFilename.c_str()) != 0; }

    bool put(std::string& Section, std::string& Key, char* Value) const
      { return ini_puts(Section.c_str(), Key.c_str(), Value, iniFilename.c_str()) != 0; }

#if defined INI_REAL
    bool put(std::string& Section, std::string& Key, INI_REAL Value) const
      { return ini_putf(Section.c_str(), Key.c_str(), Value, iniFilename.c_str()) != 0; }
#endif

    bool del(std::string& Section, std::string& Key) const
      { return ini_puts(Section.c_str(), Key.c_str(), 0, iniFilename.c_str()) != 0; }

    bool del(std::string& Section) const
      { return ini_puts(Section.c_str(), 0, 0, iniFilename.c_str()) != 0; }
#endif

  private:
    std::string iniFilename;
  };

#endif /* __WXWINDOWS__ */
#endif /* __cplusplus */

#endif /* MININI_H */
