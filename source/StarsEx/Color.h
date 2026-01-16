/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Universal Color Format class
*/

#ifndef Color_h
#define Color_h

#include <starshatter/ColorFormat.h>

#include "Types.h"

// +--------------------------------------------------------------------+

class Color
{
    friend class ColorIndex;
    friend class ColorValue;

public:
    static const char* TYPENAME() { return "Color"; }

    enum Misc  { SHADE_LEVELS = 64 }; // max 128!
    enum Mask  { RMask   = 0x00ff0000,
        GMask   = 0x0000ff00,
        BMask   = 0x000000ff,
        AMask   = 0xff000000,
        RGBMask = 0x00ffffff };
    enum Shift { RShift  = 16,
        GShift  =  8,
        BShift  =  0,
        AShift  = 24 };

    Color();
    Color(const Color& c);
    Color(BYTE r, BYTE g, BYTE b, BYTE a=0xff);
    Color(BYTE index);

    Color& operator=(const Color& c);
    bool operator==(const Color& c) const;
    bool operator!=(const Color& c) const;

    Color& operator+=(const Color& c);
    Color operator+(DWORD d) const;

    Color operator+(const Color& c) const;  // color alpha blending
    Color operator*(const Color& c) const;  // color modulation
    Color operator*(double scale) const;
    Color dim(double scale) const;

    void Set(DWORD value);
    void Set(BYTE r, BYTE g, BYTE b, BYTE a=0xff);

    DWORD Value() const;

    void SetRed(BYTE r);
    void SetGreen(BYTE g);
    void SetBlue(BYTE b);
    void SetAlpha(BYTE a);

    DWORD Red() const;
    DWORD Green() const;
    DWORD Blue() const;
    DWORD Alpha() const;

    float fRed() const;
    float fGreen() const;
    float fBlue() const;
    float fAlpha() const;

    BYTE Index() const;

    static DWORD FormattedBlend(DWORD c1, DWORD c2);

    DWORD Formatted() const;
    Color Faded() const;
    Color ShadeColor(int shade) const;
    DWORD Shaded(int shade) const;

    // some useful colors
    static   Color          White;
    static   Color          Black;
    static   Color          Gray;
    static   Color          LightGray;
    static   Color          DarkGray;
    static   Color          BrightRed;
    static   Color          BrightBlue;
    static   Color          BrightGreen;
    static   Color          DarkRed;
    static   Color          DarkBlue;
    static   Color          DarkGreen;
    static   Color          Yellow;
    static   Color          Cyan;
    static   Color          Magenta;
    static   Color          Tan;
    static   Color          Brown;
    static   Color          Violet;
    static   Color          Orange;

    static void UseFormat(const starshatter::ColorFormat& cf);
    static void SetPalette(PALETTEENTRY* pal, int palsize, BYTE* invpal=0);
    static void SetFade(double f);

    static const starshatter::ColorFormat& GetScreenFormat();

    // indexed color initialization:
    static void BuildShadeTable();

    static double GetFade();

    static Color Scale(const Color& c1, const Color& c2, double scale);
    static Color Unformat(DWORD formatted_color);

private:
    DWORD    rgba;

    static   bool           standard_format;
    static   starshatter::ColorFormat    format;
    static double fade;
};

// +--------------------------------------------------------------------+

class ColorValue
{
    friend class Color;

public:
    static const char* TYPENAME() { return "ColorValue"; }

    ColorValue()                     : r(0),   g(0),   b(0),   a(0)      { }
    ColorValue(const ColorValue& c)  : r(c.r), g(c.g), b(c.b), a(c.a)    { }
    ColorValue(const Color& c)       : r( c.fRed()   ),
    g( c.fGreen() ),
    b( c.fBlue()  ),
    a( c.fAlpha() )                   { }
    ColorValue(float ar,
    float ag,
    float ab,
    float aa=1.0f)        : r(ar),  g(ag),  b(ab),  a(aa)     { }

    int         operator==(const ColorValue& c) const { return r==c.r && g==c.g && b==c.b && a==c.a; }
    int         operator!=(const ColorValue& c) const { return r!=c.r || g!=c.g || b!=c.b || a!=c.a; }
    ColorValue& operator+=(const ColorValue& c);       // simple summation

    ColorValue  operator+(const ColorValue& c)   const;  // color alpha blending
    ColorValue  operator*(const ColorValue& c)   const;  // color modulation
    ColorValue  operator*(double scale)          const;
    ColorValue  dim(double scale)                const;

    void     Set(float ar, float ag, float ab, float aa=1.0f) { r=ar; g=ag; b=ab; a=aa; }

    void     SetRed(float ar)    { r=ar; }
    void     SetGreen(float ag)  { g=ag; }
    void     SetBlue(float ab)   { b=ab; }
    void     SetAlpha(float aa)  { a=aa; }

    float    Red()    const { return r; }
    float    Green()  const { return g; }
    float    Blue()   const { return b; }
    float    Alpha()  const { return a; }

    Color    ToColor()                        const;
    DWORD    Formatted()                      const { return ToColor().Formatted();                }
    Color    ShadeColor(int shade)            const { return ToColor().ShadeColor(shade);          }
    DWORD    Shaded(int shade)                const { return ToColor().Shaded(shade);              }
    Color    Faded()                          const { return ToColor().Faded();                    }

private:
    float    r, g, b, a;
};

// +--------------------------------------------------------------------+

class ColorIndex
{
    friend class Color;

public:
    static const char* TYPENAME() { return "ColorIndex"; }

    ColorIndex()                        : index(0)        { }
    ColorIndex(const ColorIndex& c)     : index(c.index)  { }
    ColorIndex(BYTE r, BYTE g, BYTE b)                    { index = Color(r,g,b).Index(); }
    ColorIndex(BYTE i)                  : index(i)        { }

    ColorIndex& operator= (const ColorIndex& c)       { index = c.index; return *this; }
    int         operator==(const ColorIndex& c) const { return index == c.index; }
    int         operator!=(const ColorIndex& c) const { return index != c.index; }

    BYTE     Index()  const { return index; }

    DWORD    Red()    const { return Color(index).Red(); }
    DWORD    Green()  const { return Color(index).Green(); }
    DWORD    Blue()   const { return Color(index).Blue(); }

    float    fRed()   const { return (float)(Red()  /255.0); }
    float    fGreen() const { return (float)(Green()/255.0); }
    float    fBlue()  const { return (float)(Blue() /255.0); }

    DWORD    Formatted()       const { return formatted_palette[index];  }
    DWORD    Shaded(int shade) const { return shade_table[shade*256+index]; }
    ColorIndex Faded()         const { return ColorIndex(index);         }

    // note: this will only work in 8-bit color mode...
    ColorIndex ShadeColor(int s) const { return ColorIndex((BYTE)(shade_table[s*256+index])); }

private:
    BYTE     index;

    static   DWORD    formatted_palette[256];
    static   DWORD    shade_table[256*256];
};

#endif  // Color_h
