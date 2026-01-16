/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Declaration of the Text class
*/

#ifndef Text_h
#define Text_h

#include <atomic>
#include <cstring>
#include <ostream>

#ifndef _WIN32
#include <strings.h>
#endif

// +-------------------------------------------------------------------+

class TextRep
{
   friend class Text;

public:
   TextRep();
   ~TextRep();

private:
   TextRep(const char* s);
   TextRep(const char* s, int len);
   TextRep(char c, int len);
   TextRep(const TextRep* rep);

   void        addref();
   long        deref();

   void        dohash();

   char*       data;
   std::atomic<long> ref;
   int         length;
   unsigned    hash;
   bool        sensitive;

   static TextRep    nullrep;
};

// +-------------------------------------------------------------------+

class Text
{
public:
   static const char* TYPENAME() { return "Text"; }

   Text();
   Text(char c);
   Text(const char*  s);
   Text(const char*  s, int len);
   Text(char c, int len);
   Text(const Text&  s);
   ~Text();

   friend std::ostream& operator<<(std::ostream& out, const Text& text);

   // case sensitivity
   bool  isSensitive()     const;
   void  setSensitive(bool s);

   // comparison
   int compare(const char* s) const;
   int compare(const Text& s) const;

   // assignment
   Text& operator=(const char*   s);
   Text& operator=(const Text&   s);

   // catenation
   Text& append(char c);
   Text& append(const char*      s);
   Text& append(const Text&      s);

   Text  operator+(char          c);
   Text  operator+(const char*   s);
   Text  operator+(const Text&   s);

   Text& operator+=(char         c) { return append(c); }
   Text& operator+=(const char*  s) { return append(s); }
   Text& operator+=(const Text&  s) { return append(s); }

   // indexing
   char     operator[](int index) const;
   char     operator()(int index) const;
   char&    operator[](int index);
   char&    operator()(int index);

   Text  operator()(int start, int len) const;

   // access
   int         length() const    { return rep->length; }
   unsigned    hash() const      { return rep->hash;   }

   const char* data() const      { return sym; }
   operator const char* () const { return sym; }

   bool empty() const;

   bool        contains(char        c) const;
   bool        contains(const char* s) const;

   bool        containsAnyOf(const char* charSet) const;

   int         indexOf(char         c) const;
   int         indexOf(const char*  s) const;

   // mutation
   void        toLower();
   void        toUpper();

   // substring
   Text        substring(int start, int length) const;
   Text        trim() const;
   Text        replace(const char* pattern, const char* substitution) const;
   Text        concat(const char* tail) const;
   static Text format(const char* fmt, ...);

private:
   void        clone();

   const char* sym;
   TextRep*    rep;
};

// +-------------------------------------------------------------------+

inline int Text::compare(const char* s) const
{
   if (rep->sensitive)
      return strcmp(sym, s);
   else
#ifdef _WIN32
      return _stricmp(sym, s);
#else
      return strcasecmp(sym, s);
#endif
}

inline int Text::compare(const Text& s) const
{
   if (rep->sensitive && s.rep->sensitive)
      return strcmp(sym, s.sym);
   else
#ifdef _WIN32
      return _stricmp(sym, s.sym);
#else
      return strcasecmp(sym, s.sym);
#endif
}

// +-------------------------------------------------------------------+

inline std::ostream& operator<<(std::ostream& out, const Text& text) {
   out << text.data(); return out; }
inline int operator==(const Text& l, const Text& r) {
   return (l.length() == r.length()) && (l.compare(r) == 0); }
inline int operator!=(const Text& l, const Text& r) { return l.compare(r) != 0; }
inline int operator< (const Text& l, const Text& r) { return l.compare(r) <  0; }
inline int operator<=(const Text& l, const Text& r) { return l.compare(r) <= 0; }
inline int operator> (const Text& l, const Text& r) { return l.compare(r) >  0; }
inline int operator>=(const Text& l, const Text& r) { return l.compare(r) >= 0; }

inline int operator==(const char* l, const Text& r) { return r.compare(l) == 0; }
inline int operator!=(const char* l, const Text& r) { return r.compare(l) != 0; }
inline int operator< (const char* l, const Text& r) { return r.compare(l) <  0; }
inline int operator<=(const char* l, const Text& r) { return r.compare(l) <= 0; }
inline int operator> (const char* l, const Text& r) { return r.compare(l) >  0; }
inline int operator>=(const char* l, const Text& r) { return r.compare(l) >= 0; }

inline int operator==(      char* l, const Text& r) { return r.compare(l) == 0; }
inline int operator!=(      char* l, const Text& r) { return r.compare(l) != 0; }
inline int operator< (      char* l, const Text& r) { return r.compare(l) <  0; }
inline int operator<=(      char* l, const Text& r) { return r.compare(l) <= 0; }
inline int operator> (      char* l, const Text& r) { return r.compare(l) >  0; }
inline int operator>=(      char* l, const Text& r) { return r.compare(l) >= 0; }

inline int operator==(const Text& l, const char* r) { return l.compare(r) == 0; }
inline int operator!=(const Text& l, const char* r) { return l.compare(r) != 0; }
inline int operator< (const Text& l, const char* r) { return l.compare(r) <  0; }
inline int operator<=(const Text& l, const char* r) { return l.compare(r) <= 0; }
inline int operator> (const Text& l, const char* r) { return l.compare(r) >  0; }
inline int operator>=(const Text& l, const char* r) { return l.compare(r) >= 0; }

inline int operator==(const Text& l,       char* r) { return l.compare(r) == 0; }
inline int operator!=(const Text& l,       char* r) { return l.compare(r) != 0; }
inline int operator< (const Text& l,       char* r) { return l.compare(r) <  0; }
inline int operator<=(const Text& l,       char* r) { return l.compare(r) <= 0; }
inline int operator> (const Text& l,       char* r) { return l.compare(r) >  0; }
inline int operator>=(const Text& l,       char* r) { return l.compare(r) >= 0; }

inline Text operator+(const char* l, const Text& r) { return Text(l) + r; }
inline Text operator+(      char* l, const Text& r) { return Text(l) + r; }

// +-------------------------------------------------------------------+

#endif  // Text_h
