#ifndef GUARD_INCLUDE_FONT_H
#define GUARD_INCLUDE_FONT_H

#ifdef FONT_STATIC
    #define FONT_DEF static
#else
    #define FONT_DEF extern
#endif

#include "common.h"

/*
 * Represents ASCII text from SPACE to '@' (from indices 32 to 64 inclusive).
 * That is all punctuation and numbers.
 */
struct Font {
    stbtt_bakedchar data[32];
    i32             dataCount       = ARRAY_COUNT(((Font*)0)->data);
    i32             firstAsciiIndex = 32;
    f32             pixelHeight     = 16.0f;
    i32             texDim;
    GLuint          texId;
};

/*
 * Fill the data memory of font with the characters from fontFile.  A
 * temporary bitmap is created and loaded into GPU memory as a texture
 * with the texture ID from font (the ID must have been previously generated).
 *
 * Returns 0 on error.  Returns a positive number on success and a negative
 * number for the number of characters of font data that can fit into the
 * allocated bitmap (you would take the absolute value of the negative number).
 */
FONT_DEF int fillFontData(char const *fontFile, Font *font);

#endif // GUARD_INCLUDE_FONT_H


#ifdef FONT_IMPLEMENTATION

#include <stdlib.h>
#include <stdio.h>
#include <SDL_log.h>
#include <glad/glad.h>

FONT_DEF int fillFontData(char const *fontFile, Font *font)
{
    auto log = [=](char const *fmt) -> int {
        SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, fmt, fontFile);
        return 0;
    };

    auto file = fopen(fontFile, "rb");
    if (!file)
        return log("Failed to open file %s for reading.\n");
    defer(fclose(file));

    if (fseek(file, 0, SEEK_END) != 0)
        return log("Failed to seek the end of file %s.\n");

    auto fileSz = ftell(file);
    if (fileSz == EOF)
        return log("Failed to seek end of file %s.\n");
    if (fseek(file, 0, SEEK_SET))
        return log("Failed to seek beginning of file %s.\n");

    auto ttfBuf = (u8*) malloc(fileSz);
    defer(free(ttfBuf));

    if (fread(ttfBuf, 1, fileSz, file) < fileSz)
        return log("Could not read file %s.\n");

    // Find the size in bytes needed for the temporary font bitmap.
    auto bmSize = font->pixelHeight * font->pixelHeight * ARRAY_COUNT(font->data);
    auto bmPow  = i32(1);

    while (((1 << bmPow) * (1 << bmPow)) < bmSize)
        ++bmPow;

    auto bmDim  = 1 << bmPow;
    auto bitmap = (u8*) malloc(bmDim * bmDim);
    defer(free(bitmap));

    auto result = stbtt_BakeFontBitmap(ttfBuf,
                                       0,
                                       font->pixelHeight,
                                       bitmap, bmDim, bmDim,
                                       font->firstAsciiIndex,
                                       font->dataCount,
                                       font->data);

    if (result == 0)
        return log("No characters could fit in bitmap for file %s.\n");

    font->texDim = bmDim;
    glBindTexture(GL_TEXTURE_2D, font->texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, bmDim, bmDim, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return result;
}

#endif // FONT_IMPLEMENTATION
