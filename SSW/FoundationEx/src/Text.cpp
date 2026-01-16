/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Implementation of the Text class
*/

#include <Text.h>

#include <cctype>
#include <cstdarg>
#include <cstring>

// +-------------------------------------------------------------------+
// SPECIAL TEXT REP FOR NULL STRINGS
// This is used only by the default constructor for the Text object,
// to prevent extra rep allocation when constructing empty strings.

TextRep        TextRep::nullrep;

TextRep::TextRep()
   : ref(1234567), data(0), length(0), hash(0), sensitive(true)
{
   data = new char[4];

   if (data)
      std::memset(data, 0, 4);
}

// +-------------------------------------------------------------------+


TextRep::TextRep(const char* s)
   : ref(1), length(0), sensitive(true)
{
   if (s) length = std::strlen(s);

   data = new char[length+1];

   if (data) {
      if (s) std::strcpy(data, s);
      else   data[length] = '\0';

      dohash();
   }
}

TextRep::TextRep(const char* s, int len)
   : ref(1), length(len), sensitive(true)
{
   if (length < 0) length = 0;

   data = new char[length+1];

   if (data) {
      std::memcpy(data, s, length);
      data[length] = '\0';
      dohash();
   }
}

TextRep::TextRep(char c, int len)
   : ref(1), length(len), sensitive(true)
{
   if (length < 0) length = 0;

   data = new char[length+1];

   if (data) {
      std::memset(data, c, length);
      data[length] = '\0';
      dohash();
   }
}

TextRep::TextRep(const TextRep* rep)
   : ref(1)
{
   length    = rep->length;

   data = new char[length+1];

   hash      = rep->hash;
   sensitive = rep->sensitive;

   if (data)
      std::strcpy(data, rep->data);
}

TextRep::~TextRep()
{
   delete[] data;
}

void
TextRep::addref()
{
   ref++;
}

long
TextRep::deref()
{
   long r = --ref;
   return r;
}

inline static void mash(unsigned& hash, unsigned chars)
{
  hash = (chars ^ ((hash << 5) | (hash >> (8*sizeof(unsigned) - 5))));
}

void
TextRep::dohash()
{
   unsigned hv       = (unsigned)length; // Mix in the string length.
   unsigned i        = length*sizeof(char)/sizeof(unsigned);
   const unsigned* p = (const unsigned*)data;

   while (i--)
      mash(hv, *p++);            // XOR in the characters.

   // XOR in any remaining characters:
   i                 = length*sizeof(char)%sizeof(unsigned);
   if (i) {
      unsigned h = 0;
      const char* c = (const char*)p;
      while (i--)
         h = ((h << 8*sizeof(char)) | *c++);
      mash(hv, h);
   }

   hash = hv;
}

// +-------------------------------------------------------------------+

Text::Text()
{
   rep = &TextRep::nullrep;
   rep->addref();
   sym = rep->data;
}

Text::Text(char c)
{
   char buf[2]; buf[0] = c; buf[1] = '\0';

   rep = new TextRep(buf);

   if (!rep) {
      rep = &TextRep::nullrep;
      rep->addref();
   }

   sym = rep->data;
}

Text::Text(const char* s)
{
   rep = new TextRep(s);

   if (!rep) {
      rep = &TextRep::nullrep;
      rep->addref();
   }

   sym = rep->data;
}

Text::Text(const char* s, int len)
{
   rep = new TextRep(s, len);

   if (!rep) {
      rep = &TextRep::nullrep;
      rep->addref();
   }

   sym = rep->data;
}

Text::Text(char c, int len)
{
   rep = new TextRep(c, len);

   if (!rep) {
      rep = &TextRep::nullrep;
      rep->addref();
   }

   sym = rep->data;
}

Text::Text(const Text& s)
{
   rep = s.rep;
   rep->addref();
   sym = rep->data;
}

Text::~Text()
{
   if (rep->deref() == 0) delete rep;

   rep = &TextRep::nullrep;
   sym = rep->data;
}

Text&
Text::operator=(const char* s)
{
   if (rep->deref() == 0) delete rep;
   rep = new TextRep(s);

   if (!rep)
      rep = &TextRep::nullrep;
   sym = rep->data;
   return *this;
}

Text&
Text::operator=(const Text& s)
{
   s.rep->addref();
   if (rep->deref() == 0) delete rep;
   rep = s.rep;
   sym = rep->data;
   return *this;
}

Text
Text::operator+(char c)
{
   char* buf = new char[rep->length + 2];

   if (buf) {
      std::strcpy(buf, sym);
      buf[rep->length] = c;
      buf[rep->length+1] = '\0';
      Text retval(buf);
      delete [] buf;
      return retval;
   }

   else {
      return *this;
   }
}

Text
Text::operator+(const char* s)
{
   char* buf = new char[std::strlen(s) + rep->length + 1];

   if (buf) {
      std::strcpy(buf, sym);
      std::strcat(buf, s);
      Text retval(buf);
      delete [] buf;
      return retval;
   }

   else {
      return *this;
   }
}

Text
Text::operator+(const Text& s)
{
   char* buf = new char[s.rep->length + rep->length + 1];

   if (buf) {
      std::strcpy(buf, sym);
      std::strcat(buf, s.sym);
      Text retval(buf);
      delete [] buf;
      return retval;
   }

   else {
      return *this;
   }
}

bool
Text::isSensitive() const
{
   return rep->sensitive;
}

void
Text::setSensitive(bool s)
{
   rep->sensitive = s;
}

Text&
Text::append(char c)
{
   char* buf = new char[rep->length + 2];

   if (buf) {
      std::strcpy(buf, sym);
      buf[rep->length] = c;
      buf[rep->length+1] = '\0';
      if (rep->deref() == 0) delete rep;

      rep = new TextRep(buf);

      if (!rep)
         rep = &TextRep::nullrep;

      sym = rep->data;
      delete [] buf;
   }

   return *this;
}

Text&
Text::append(const char* s)
{
   char* buf = new char[std::strlen(s) + rep->length + 1];

   if (buf) {
      std::strcpy(buf, sym);
      std::strcat(buf, s);
      if (rep->deref() == 0) delete rep;

      rep = new TextRep(buf);

      if (!rep)
         rep = &TextRep::nullrep;

      sym = rep->data;
      delete [] buf;
   }

   return *this;
}

Text&
Text::append(const Text& s)
{
   char* buf = new char[s.rep->length + rep->length + 1];

   if (buf) {
      int lenA = rep->length;
      int lenB = s.rep->length;

      std::memcpy(buf, sym, lenA);
      std::memcpy(buf + lenA, s.sym, lenB);
      buf[lenA + lenB] = 0;

      if (rep->deref() == 0) delete rep;

      rep = new TextRep(buf);

      if (!rep)
         rep = &TextRep::nullrep;

      sym = rep->data;
      delete [] buf;
   }

   return *this;
}

void
Text::clone()
{
   if (rep->ref > 1) {
      rep->deref();

      TextRep* t = new TextRep(rep);

      rep = t;

      if (!rep)
         rep = &TextRep::nullrep;

      sym = rep->data;
   }
}

char
Text::operator[](int index) const
{
   if (index < (int) rep->length)
      return sym[index];
   else
      throw "BOUNDS ERROR";

   return '\0';
}

char
Text::operator()(int index) const
{
   return sym[index];
}

char&
Text::operator[](int index)
{
   if (index < (int) rep->length) {
      clone();
      return (char&) sym[index];
   }
   else
      throw "BOUNDS ERROR";

   return (char&) sym[0];
}

char&
Text::operator()(int index)
{
   clone();
   return (char&) sym[index];
}

Text
Text::operator()(int start, int len) const
{
   if (start > rep->length || len <= 0)
      return Text();

   if (start + len > rep->length)
      len = rep->length - start;

   char* buf = new char[len+1];

   if (buf) {
      std::strncpy(buf, sym+start, len);
      buf[len] = '\0';

      Text retval(buf);
      delete [] buf;
      return retval;
   }

   return Text();
}


bool
Text::empty() const
{
    return !rep || rep->length < 1;
}


bool
Text::contains(char c) const
{
   if (rep->length > 0) {
      if (!rep->sensitive) {
         char alt = c;
         if (std::islower(alt))      alt = std::toupper(alt);
         else if (std::isupper(alt)) alt = std::tolower(alt);

         if (std::strchr(rep->data, alt) != 0)
            return true;
      }

      if (std::strchr(rep->data, c) != 0)
         return true;
   }

   return false;
}

bool
Text::contains(const char* pattern) const
{
   if (rep->length > 0 && pattern && *pattern) {
      if (rep->sensitive) {
         if (std::strstr(rep->data, pattern) != 0)
            return true;
      }
      else {
         Text smash1(*this);
         smash1.toLower();
         Text smash2(pattern);
         smash2.toLower();

         if (std::strstr(smash1.data(), smash2.data()) != 0)
            return true;
      }
   }

   return false;
}

bool
Text::containsAnyOf(const char* charSet) const
{
   if (rep->length > 0 && charSet && *charSet) {
      if (rep->sensitive) {
         if (strpbrk(rep->data, charSet) != 0)
            return true;
      }
      else {
         Text smash1(*this);
         smash1.toLower();
         Text smash2(charSet);
         smash2.toLower();

         if (strpbrk(smash1.data(), smash2.data()) != 0)
            return true;
      }
   }

   return false;
}

int
Text::indexOf(char c) const
{
   if (rep->length > 0) {
      if (!rep->sensitive) {
         char alt = c;
         if (std::islower(alt))      alt = std::toupper(alt);
         else if (std::isupper(alt)) alt = std::tolower(alt);

         const char* p = std::strchr(rep->data, alt);

         if (p)
            return (p - rep->data);
      }

      const char* p = std::strchr(rep->data, c);

      if (p)
         return (p - rep->data);
   }

   return -1;
}

int
Text::indexOf(const char* pattern) const
{
   if (rep->length > 0 && pattern && *pattern) {
      if (rep->sensitive) {
         const char* p = std::strstr(rep->data, pattern);
         if (p) return (p - rep->data);
      }
      else {
         Text smash1(*this);
         smash1.toLower();
         Text smash2(pattern);
         smash2.toLower();

         const char* p = std::strstr(smash1.data(), smash2.data());
         if (p) return (p - smash1.data());
      }
   }

   return -1;
}

void
Text::toLower()
{
   clone();
   size_t n = rep->length;
   char* p = (char*) sym;
   while (n--) {
      *p = std::tolower((unsigned char)*p);
      p++;
   }

   rep->dohash();
}

void
Text::toUpper()
{
   clone();
   size_t n = rep->length;
   char* p = (char*) sym;
   while ( n-- ) {
      *p = std::toupper((unsigned char)*p);
      p++;
   }

   rep->dohash();
}

Text
Text::substring(int start, int length) const
{
   Text result;

   if (start >= 0 && start < (int) rep->length && length > 0) {
      if (start + length > (int) rep->length)
         length = (int) rep->length - start;

      const char* s = sym + start;

      result.rep = new TextRep(s, length);

      if (!result.rep)
         result.rep = &TextRep::nullrep;

      result.sym = result.rep->data;
   }

   return result;
}

Text
Text::trim() const
{
   Text result;

   if (rep->length) {
      const char* p = sym;
      const char* q = sym + rep->length-1;

      while (p && *p && isspace(*p))   p++;
      while (q>p && *q && isspace(*q)) q--;

      result = substring(p-sym, q-p+1);
   }

   return result;
}

Text
Text::replace(const char* pattern, const char* substitution) const
{
   Text result;

   if (rep->length && pattern && *pattern) {
      const char* pos = rep->data;
      const char* bound = rep->data + rep->length;
      int skip  = std::strlen(pattern);
      do {
         const char* p = std::strstr(pos, pattern);
         if (p) {
            int len = p - pos;
            result.append(Text(pos, len));
            result.append(substitution);
            pos = p + skip;
         }
         else if (pos < bound) {
            result.append(pos);
            break;
         }
      }
      while (pos < bound);
   }

   return result;
}

Text
Text::concat(const char* tail) const
{
    Text result(*this);
    result.append(tail);
    return result;
}

Text
Text::format(const char* fmt, ...)
{
    char buf[2048];
    std::va_list args;
    va_start(args, fmt);
    int len = std::vsnprintf(buf, 2048, fmt, args);
    Text result(buf, len);
    va_end(args);
    return result;
}
