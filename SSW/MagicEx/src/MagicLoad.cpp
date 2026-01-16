/*  Starshatter: The Open Source Project
    Copyright (c) 2021-2025, Starshatter: The Open Source Project Contributors
    Copyright (c) 2011-2012, Starshatter OpenSource Distribution Contributors
    Copyright (c) 1997-2006, Destroyer Studios LLC.

    AUTHOR:       John DiCamillo
*/

#include <cstring>
#include <iostream>
#include <vector>

#include <MagicLoad.h>

#include <Bitmap.h>
#include <Color.h>
#include <D3DXImage.h>
#include <Geometry.h>
#include <Pcx.h>


int
LoadTexture(const char* fname, Bitmap*& bitmap, int type)
{
   if (fname == nullptr || fname[0] == 0)
      return 0;
   bitmap = Bitmap::CheckCache(fname);
   if (bitmap)
      return 0;  // legacy behaviour
   const bool is_pcx = std::strstr(fname, ".pcx") || std::strstr(fname, ".PCX");
   if (is_pcx) {
      PcxImage pcx;
      if (pcx.Load((char*) fname) != PCX_OK)  // Casting away const!
         return 0;
      bitmap = new Bitmap;
      if (bitmap == nullptr)
         return 0;
      if (pcx.himap) {
         bitmap->CopyHighColorImage(pcx.width, pcx.height, pcx.himap);
      }
      else if (pcx.bitmap) {
         bitmap->CopyImage(pcx.width, pcx.height, pcx.bitmap);
         std::vector<char> buf(std::strlen(fname) + 2);
         std::strcpy(buf.data(), fname);
         if (auto* ext = std::strstr(buf.data(), ".pcx"))
            *ext = 0;
         std::strcat(buf.data(), "+.pcx");
         if (pcx.Load(buf.data()) == PCX_OK && pcx.himap != 0)
            bitmap->CopyHighColorImage(pcx.width, pcx.height, pcx.himap);
      }
      else {
         // legacy no-op
      }
   }
   else {
      D3DXImage d3dx;
      bitmap = new Bitmap;
      if (d3dx.Load((char*) fname)) {
         std::cout << "d3dx done" << std::endl;
         bitmap = new Bitmap;
         bitmap->CopyHighColorImage(d3dx.width, d3dx.height, d3dx.image);
      }
   }
   if (bitmap) {
      LoadAlpha(fname, *bitmap, type);
      bitmap->SetFilename(fname);
      bitmap->SetType(type);
      bitmap->MakeTexture();
      Bitmap::AddToCache(bitmap);
   }
   return 0;
}


int
LoadAlpha(const char* name, Bitmap& bitmap, int type)
{
   PcxImage    pcx;
   D3DXImage   d3dx;
   bool        pcx_file = strstr(name, ".pcx") || strstr(name, ".PCX");
   bool        bmp_file = strstr(name, ".bmp") || strstr(name, ".BMP");
   bool        jpg_file = strstr(name, ".jpg") || strstr(name, ".JPG");
   bool        png_file = strstr(name, ".png") || strstr(name, ".PNG");
   bool        tga_file = strstr(name, ".tga") || strstr(name, ".TGA");

   // check for an associated alpha-only (grayscale) bitmap:
   char filename[256];
   strcpy_s(filename, name);

   char* dot = strrchr(filename, '.');
   if (dot && pcx_file)
      strcpy(dot, "@.pcx");
   else if (dot && bmp_file)
      strcpy(dot, "@.bmp");
   else if (dot && jpg_file)
      strcpy(dot, "@.jpg");
   else if (dot && png_file)
      strcpy(dot, "@.png");
   else if (dot && tga_file)
      strcpy(dot, "@.tga");
   else
      return 0;

   // first try to load from current directory:
   bool loaded = false;

   if (pcx_file)
      loaded = pcx.Load(filename) == PCX_OK;

   else
      loaded = d3dx.Load(filename);

   // now copy the alpha values into the bitmap:
   if (loaded) {
      if (pcx_file && pcx.bitmap) {
         bitmap.CopyAlphaImage(pcx.width, pcx.height, pcx.bitmap);
      }
      else if (pcx_file && pcx.himap) {
         bitmap.CopyAlphaRedChannel(pcx.width, pcx.height, pcx.himap);
      }
      else if (d3dx.image) {
         bitmap.CopyAlphaRedChannel(d3dx.width, d3dx.height, d3dx.image);
      }
   }

   return 0;
}
