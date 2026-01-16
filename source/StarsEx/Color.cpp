/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo


    OVERVIEW
    ========
    Universal Color Format class
*/

#include "Color.h"

#include <vector>

#include <starshatter/ColorFormat.h>

#include "ColorPalette.h"


using starshatter::ColorFormat;
using starshatter::ColorPalette;


// +--------------------------------------------------------------------+

Color          Color::White      = Color(255,255,255);
Color          Color::Black      = Color(  0,  0,  0);
Color          Color::Gray       = Color(128,128,128);
Color          Color::LightGray  = Color(192,192,192);
Color          Color::DarkGray   = Color( 64, 64, 64);
Color          Color::BrightRed  = Color(255,  0,  0);
Color          Color::BrightBlue = Color(  0,  0,255);
Color          Color::BrightGreen= Color(  0,255,  0);
Color          Color::DarkRed    = Color(128,  0,  0);
Color          Color::DarkBlue   = Color(  0,  0,128);
Color          Color::DarkGreen  = Color(  0,128,  0);
Color          Color::Yellow     = Color(255,255,  0);
Color          Color::Cyan       = Color(  0,255,255);
Color          Color::Magenta    = Color(255,  0,255);
Color          Color::Tan        = Color(180,150,120);
Color          Color::Brown      = Color(128,100, 80);
Color          Color::Violet     = Color(128,  0,128);
Color          Color::Orange     = Color(255,150, 20);

bool           Color::standard_format = false;
ColorFormat    Color::format = ColorFormat(256);
double         Color::fade = 1.0;
DWORD          ColorIndex::formatted_palette[256];
DWORD          ColorIndex::shade_table[256*256];

static const Color FADE_COLOR {Color::Black};

static ColorPalette g_palette;

// +--------------------------------------------------------------------+

Color::Color() :
    rgba(0)
{
}

Color::Color(const Color& c) :
    rgba {c.rgba}
{
}

Color::Color(BYTE r, BYTE g, BYTE b, BYTE a)
{
    Set(r, g, b, a);
}

Color::Color(BYTE index)
{
    *this = g_palette.Forward(index);
}

// +--------------------------------------------------------------------+

Color&
Color::operator=(const Color& c)
{
    rgba = c.rgba;
    return *this;
}

// +--------------------------------------------------------------------+

bool
Color::operator==(const Color& c) const
{
    return rgba == c.rgba;
}

bool
Color::operator!=(const Color& c) const
{
    return rgba != c.rgba;
}

// +--------------------------------------------------------------------+

Color&
Color::operator+=(const Color& c)
{
    int  r = Red()   + c.Red();     if (r > 255) r = 255;
    int  g = Green() + c.Green();   if (g > 255) g = 255;
    int  b = Blue()  + c.Blue();    if (b > 255) b = 255;

    Set((BYTE) r, (BYTE) g, (BYTE) b);
    return *this;
}

Color
Color::operator+(DWORD d) const
{
    int r = Red()   + ((d & RMask) >> RShift);   if (r > 255) r = 255;
    int g = Green() + ((d & GMask) >> GShift);   if (g > 255) g = 255;
    int b = Blue()  + ((d & BMask) >> BShift);   if (b > 255) b = 255;

    return Color((BYTE) r,(BYTE) g,(BYTE) b);
}

// +--------------------------------------------------------------------+

Color
Color::operator+(const Color& c) const
{
    float src_alpha = c.fAlpha();
    float dst_alpha = 1.0f - src_alpha;

    BYTE  r = (BYTE)((fRed()  *dst_alpha + c.fRed()  *src_alpha)*255.0f);
    BYTE  g = (BYTE)((fGreen()*dst_alpha + c.fGreen()*src_alpha)*255.0f);
    BYTE  b = (BYTE)((fBlue() *dst_alpha + c.fBlue() *src_alpha)*255.0f);

    return Color(r, g, b);
}

// +--------------------------------------------------------------------+

Color
Color::operator*(const Color& c) const
{
    BYTE  r = (BYTE) ((fRed()   * c.fRed())   *255.0f);
    BYTE  g = (BYTE) ((fGreen() * c.fGreen()) *255.0f);
    BYTE  b = (BYTE) ((fBlue()  * c.fBlue())  *255.0f);

    return Color(r, g, b);
}

// +--------------------------------------------------------------------+

Color
Color::operator*(double scale) const
{
    int r = static_cast<int>(Red()   * scale); if (r > 255) r = 255;
    int g = static_cast<int>(Green() * scale); if (g > 255) g = 255;
    int b = static_cast<int>(Blue()  * scale); if (b > 255) b = 255;
    int a = static_cast<int>(Alpha() * scale); if (a > 255) a = 255;

    return Color((BYTE) r, (BYTE) g, (BYTE) b, (BYTE) a);
}

Color
Color::dim(double scale) const
{
    int r = static_cast<int>(Red()   * scale);
    int g = static_cast<int>(Green() * scale);
    int b = static_cast<int>(Blue()  * scale);

    return Color((BYTE) r, (BYTE) g, (BYTE) b, (BYTE) Alpha());
}

// +--------------------------------------------------------------------+

void
Color::Set(DWORD value)
{
    rgba = value;
}

void
Color::Set(BYTE r, BYTE g, BYTE b, BYTE a)
{
    rgba = (r<<RShift) | (g<<GShift) | (b<<BShift) | (a<<AShift);
}

// +--------------------------------------------------------------------+

DWORD
Color::Value() const
{
    return rgba;
}

// +--------------------------------------------------------------------+

void
Color::SetRed(BYTE r)
{
    rgba = (rgba & ~RMask) | (r << RShift);
}

void
Color::SetGreen(BYTE g)
{
    rgba = (rgba & ~GMask) | (g << GShift);
}

void
Color::SetBlue(BYTE b)
{
    rgba = (rgba & ~BMask) | (b << BShift);
}

void
Color::SetAlpha(BYTE a)
{
    rgba = (rgba & ~AMask) | (a << AShift);
}

// +--------------------------------------------------------------------+

DWORD
Color::Red() const
{
    return (rgba & RMask) >> RShift;
}

DWORD
Color::Green() const
{
    return (rgba & GMask) >> GShift;
}

DWORD
Color::Blue() const
{
    return (rgba & BMask) >> BShift;
}

DWORD
Color::Alpha() const
{
    return (rgba & AMask) >> AShift;
}

// +--------------------------------------------------------------------+

float
Color::fRed() const
{
    return static_cast<float>(Red() / 255.0f);
}

float
Color::fGreen() const
{
    return static_cast<float>(Green() / 255.0f);
}

float
Color::fBlue() const
{
    return static_cast<float>(Blue() / 255.0f);
}

float
Color::fAlpha() const
{
    return static_cast<float>(Alpha() / 255.0f);
}

// +--------------------------------------------------------------------+

BYTE
Color::Index() const
{
    return g_palette.Reverse(*this).Index();
}

// +--------------------------------------------------------------------+


DWORD
Color::FormattedBlend(DWORD c1, DWORD c2)
{
    if (format.pal)
        return g_palette.Blend(static_cast<BYTE>(c1), static_cast<BYTE>(c2)).Index();
    Color left;
    left.Set(c1);
    const auto blend = left + c2;
    return blend.Value();
}


// +--------------------------------------------------------------------+

DWORD
Color::Formatted() const
{
    if (format.pal) {
        return Index();
    }

    else {
        if (fade != 1.0) {
            double step = (1.0 - fade);

            DWORD r = ((int) ((fRed()   - (fRed()   - FADE_COLOR.fRed())  * step)*255.0)) >> format.rdown;
            DWORD g = ((int) ((fGreen() - (fGreen() - FADE_COLOR.fGreen())* step)*255.0)) >> format.gdown;
            DWORD b = ((int) ((fBlue()  - (fBlue()  - FADE_COLOR.fBlue()) * step)*255.0)) >> format.bdown;
            DWORD a = Alpha()>>format.adown;

            return (r<<format.rshift)|(g<<format.gshift)|(b<<format.bshift)|(a<<format.ashift);

        }

        else if (standard_format) {
            return rgba;
        }

        else {
            DWORD r = Red()  >>format.rdown;
            DWORD g = Green()>>format.gdown;
            DWORD b = Blue() >>format.bdown;
            DWORD a = Alpha()>>format.adown;

            return (r<<format.rshift)|(g<<format.gshift)|(b<<format.bshift)|(a<<format.ashift);
        }
    }
}

// +--------------------------------------------------------------------+

Color
Color::Faded() const
{
    if (fade != 1.0) {
        double step = (1.0 - fade);

        DWORD r = ((int) ((fRed()   - (fRed()   - FADE_COLOR.fRed())  * step)*255.0));
        DWORD g = ((int) ((fGreen() - (fGreen() - FADE_COLOR.fGreen())* step)*255.0));
        DWORD b = ((int) ((fBlue()  - (fBlue()  - FADE_COLOR.fBlue()) * step)*255.0));
        DWORD a = Alpha();

        return Color((BYTE) r, (BYTE) g, (BYTE) b, (BYTE) a);

    }

    else {
        return *this;
    }
}

// +--------------------------------------------------------------------+

Color
Color::ShadeColor(int shade) const
{
    double   fr = fRed(),      sr = fr;
    double   fg = fGreen(),    sg = fg;
    double   fb = fBlue(),     sb = fb;
    double   range = SHADE_LEVELS;

    // first shade:
    if (shade < SHADE_LEVELS) {          // shade towards black
        sr = fr * (shade/range);
        sg = fg * (shade/range);
        sb = fb * (shade/range);
    }
    else if (shade > SHADE_LEVELS) {     // shade towards white
        double step = (shade - range)/range;

        sr = fr - (fr - 1.0) * step;
        sg = fg - (fg - 1.0) * step;
        sb = fb - (fb - 1.0) * step;
    }

    return Color((BYTE) (sr*255.0), (BYTE) (sg*255.0), (BYTE) (sb*255.0), (BYTE) Alpha());
}

// +--------------------------------------------------------------------+

DWORD
Color::Shaded(int shade) const
{
    return ShadeColor(shade).Formatted();
}

// +--------------------------------------------------------------------+

Color
Color::Unformat(DWORD formatted_color)
{
    if (format.pal) {
        return Color((BYTE) formatted_color);
    }
    else if (standard_format) {
        Color c;
        c.Set(formatted_color);
        return c;
    }
    else {
        BYTE r = (BYTE) ((formatted_color & format.rmask)>>format.rshift) << format.rdown;
        BYTE g = (BYTE) ((formatted_color & format.gmask)>>format.gshift) << format.gdown;
        BYTE b = (BYTE) ((formatted_color & format.bmask)>>format.bshift) << format.bdown;
        BYTE a = (BYTE) ((formatted_color & format.amask)>>format.ashift) << format.adown;

        return Color(r,g,b,a);
    }
}

// +--------------------------------------------------------------------+

Color
Color::Scale(const Color& c1, const Color& c2, double scale)
{
    BYTE r = (BYTE) ((c1.fRed()   + (c2.fRed()   - c1.fRed()  )*scale) * 255);
    BYTE g = (BYTE) ((c1.fGreen() + (c2.fGreen() - c1.fGreen())*scale) * 255);
    BYTE b = (BYTE) ((c1.fBlue()  + (c2.fBlue()  - c1.fBlue() )*scale) * 255);
    BYTE a = (BYTE) ((c1.fAlpha() + (c2.fAlpha() - c1.fAlpha())*scale) * 255);

    return Color(r,g,b,a);
}

// +--------------------------------------------------------------------+

void
Color::UseFormat(const ColorFormat& cf)
{
    format = cf;

    if (format.rmask == RMask && format.gmask == GMask && format.bmask == BMask)
    standard_format = true;
    else
    standard_format = false;

    if (cf.pal) {
        for (int i = 0; i < 256; i++)
            ColorIndex::formatted_palette[i]   = i;
    }
    else {
        for (int i = 0; i < 256; i++)
            ColorIndex::formatted_palette[i] = Color(i).Formatted();
    }
}

// +--------------------------------------------------------------------+

void
Color::SetPalette(PALETTEENTRY* pal, int palsize, BYTE* invpal)
{
    const std::vector<PALETTEENTRY> forward {pal, pal + palsize};
    if (invpal) {
        g_palette = ColorPalette(forward, {invpal, invpal + 32768});
    }
    else {
        g_palette = ColorPalette(forward);
    }
    UseFormat(format);

    // set up shade table:
    const double old_fade = fade;
    fade = 1.0;
    BuildShadeTable();
    fade = old_fade;
}

// +--------------------------------------------------------------------+

void
Color::SetFade(double f)
{
    fade = f;
}

// +--------------------------------------------------------------------+

const ColorFormat&
Color::GetScreenFormat()
{
    return format;
}

// +--------------------------------------------------------------------+

void
Color::BuildShadeTable()
{
    for (int shade = 0; shade < SHADE_LEVELS*2; shade++)
    for (int index = 0; index < 256; index++)
    ColorIndex::shade_table[shade*256+index] = Color(index).Shaded(shade);
}

// +--------------------------------------------------------------------+

double
Color::GetFade()
{
    return fade;
}

// +--------------------------------------------------------------------+
// +--------------------------------------------------------------------+
// +--------------------------------------------------------------------+


ColorValue&
ColorValue::operator+=(const ColorValue& c)
{
    r += c.r;
    g += c.g;
    b += c.b;

    return *this;
}

ColorValue
ColorValue::operator+(const ColorValue& c) const
{
    float src_alpha = c.a;
    float dst_alpha = 1.0f - a;

    float fr = (r * dst_alpha) + (c.r * src_alpha);
    float fg = (g * dst_alpha) + (c.g * src_alpha);
    float fb = (b * dst_alpha) + (c.b * src_alpha);

    return ColorValue(fr, fg, fb);
}

ColorValue
ColorValue::operator*(const ColorValue& c) const
{
    return ColorValue(r*c.r, g*c.g, b*c.b);
}

ColorValue
ColorValue::operator*(double scale) const
{
    return ColorValue((float) (r*scale),
    (float) (g*scale),
    (float) (b*scale),
    (float) (a*scale));
}

ColorValue
ColorValue::dim(double scale) const
{
    return ColorValue((float) (r*scale),
    (float) (g*scale),
    (float) (b*scale));
}

// +--------------------------------------------------------------------+

inline BYTE bclamp(float x) { return (BYTE) ((x<0) ? 0 : (x>255) ? 255 : x); }

Color
ColorValue::ToColor() const
{
    return Color(bclamp(r * 255.0f),
    bclamp(g * 255.0f),
    bclamp(b * 255.0f),
    bclamp(a * 255.0f));
}
