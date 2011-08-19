
/*
    Copyright 2011 Aevum Software aevum @ aevumlab.com

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

    @author Victor Vicente de Carvalho victor.carvalho@aevumlab.com
    @author Ozires Bortolon de Faria ozires@aevumlab.com
*/

#include "BitmapFontCache.hpp"

using namespace gdx_cpp::graphics::g2d;

void BitmapFontCache::setPosition (float x,float y) {
    translate(x - this.x, y - this.y);
}

void BitmapFontCache::translate (float xAmount,float yAmount) {
    if (xAmount == 0 && yAmount == 0) return;
    if (integer) {
        xAmount = (int)xAmount;
        yAmount = (int)yAmount;
    }
    x += xAmount;
    y += yAmount;
    float[] vertices = this.vertices;
    for (int i = 0, n = idx; i < n; i += 5) {
        vertices[i] += xAmount;
        vertices[i + 1] += yAmount;
    }
}

void BitmapFontCache::setColor (const gdx_cpp::graphics::Color& tint) {
    final float color = tint.toFloatBits();
    if (color == this.color) return;
    this.color = color;
    float[] vertices = this.vertices;
    for (int i = 2, n = idx; i < n; i += 5)
        vertices[i] = color;
}

void BitmapFontCache::setColor (float r,float g,float b,float a) {
    int intBits = ((int)(255 * a) << 24) | ((int)(255 * b) << 16) | ((int)(255 * g) << 8) | ((int)(255 * r));
    float color = NumberUtils.intBitsToFloat((intBits & 0xfeffffff));
    if (color == this.color) return;
    this.color = color;
    float[] vertices = this.vertices;
    for (int i = 2, n = idx; i < n; i += 5)
        vertices[i] = color;
}

void BitmapFontCache::draw (const SpriteBatch& spriteBatch) {
    spriteBatch.draw(font.getRegion().getTexture(), vertices, 0, idx);
}

void BitmapFontCache::draw (const SpriteBatch& spriteBatch,float alphaModulation) {
    Color color = getColor();
    float oldAlpha = color.a;
    color.a *= alphaModulation;
    setColor(color);
    draw(spriteBatch);
    color.a = oldAlpha;
    setColor(color);
}

gdx_cpp::graphics::Color& BitmapFontCache::getColor () {
    float floatBits = color;
    int intBits = NumberUtils.floatToRawIntBits(color);
    Color color = tmpColor;
    color.r = (intBits & 0xff) / 255f;
    color.g = ((intBits >>> 8) & 0xff) / 255f;
    color.b = ((intBits >>> 16) & 0xff) / 255f;
    color.a = ((intBits >>> 24) & 0xff) / 255f;
    return color;
}

void BitmapFontCache::reset (int glyphCount) {
    x = 0;
    y = 0;
    idx = 0;

    int vertexCount = glyphCount * 20;
    if (vertices == null || vertices.length < vertexCount) vertices = new float[vertexCount];
}

float BitmapFontCache::addToCache (const CharSequence& str,float x,float y,int start,int end) {
    float startX = x;
    BitmapFont font = this.font;
    Glyph lastGlyph = null;
    if (font.data.scaleX == 1 && font.data.scaleY == 1) {
        while (start < end) {
            lastGlyph = font.data.getGlyph(str.charAt(start++));
            if (lastGlyph != null) {
                addGlyph(lastGlyph, x + lastGlyph.xoffset, y + lastGlyph.yoffset, lastGlyph.width, lastGlyph.height);
                x += lastGlyph.xadvance;
                break;
            }
        }
        while (start < end) {
            char ch = str.charAt(start++);
            Glyph g = font.data.getGlyph(ch);
            if (g != null) {
                x += lastGlyph.getKerning(ch);
                lastGlyph = g;
                addGlyph(lastGlyph, x + g.xoffset, y + g.yoffset, g.width, g.height);
                x += g.xadvance;
            }
        }
    } else {
        float scaleX = font.data.scaleX, scaleY = font.data.scaleY;
        while (start < end) {
            lastGlyph = font.data.getGlyph(str.charAt(start++));
            if (lastGlyph != null) {
                addGlyph(lastGlyph, //
                         x + lastGlyph.xoffset * scaleX, //
                         y + lastGlyph.yoffset * scaleY, //
                         lastGlyph.width * scaleX, //
                         lastGlyph.height * scaleY);
                x += lastGlyph.xadvance * scaleX;
                break;
            }
        }
        while (start < end) {
            char ch = str.charAt(start++);
            Glyph g = font.data.getGlyph(ch);
            if (g != null) {
                x += lastGlyph.getKerning(ch) * scaleX;
                lastGlyph = g;
                addGlyph(lastGlyph, //
                         x + g.xoffset * scaleX, //
                         y + g.yoffset * scaleY, //
                         g.width * scaleX, //
                         g.height * scaleY);
                x += g.xadvance * scaleX;
            }
        }
    }
    return x - startX;
}

void BitmapFontCache::addGlyph (const gdx_cpp::graphics::g2d::BitmapFont::Glyph& glyph,float x,float y,float width,float height) {
    final float x2 = x + width;
    final float y2 = y + height;
    final float u = glyph.u;
    final float u2 = glyph.u2;
    final float v = glyph.v;
    final float v2 = glyph.v2;

    final float[] vertices = this.vertices;
    if (!integer) {
        vertices[idx++] = x;
        vertices[idx++] = y;
        vertices[idx++] = color;
        vertices[idx++] = u;
        vertices[idx++] = v;

        vertices[idx++] = x;
        vertices[idx++] = y2;
        vertices[idx++] = color;
        vertices[idx++] = u;
        vertices[idx++] = v2;

        vertices[idx++] = x2;
        vertices[idx++] = y2;
        vertices[idx++] = color;
        vertices[idx++] = u2;
        vertices[idx++] = v2;

        vertices[idx++] = x2;
        vertices[idx++] = y;
        vertices[idx++] = color;
        vertices[idx++] = u2;
        vertices[idx++] = v;
    } else {
        vertices[idx++] = (int)x;
        vertices[idx++] = (int)y;
        vertices[idx++] = color;
        vertices[idx++] = u;
        vertices[idx++] = v;

        vertices[idx++] = (int)x;
        vertices[idx++] = (int)y2;
        vertices[idx++] = color;
        vertices[idx++] = u;
        vertices[idx++] = v2;

        vertices[idx++] = (int)x2;
        vertices[idx++] = (int)y2;
        vertices[idx++] = color;
        vertices[idx++] = u2;
        vertices[idx++] = v2;

        vertices[idx++] = (int)x2;
        vertices[idx++] = (int)y;
        vertices[idx++] = color;
        vertices[idx++] = u2;
        vertices[idx++] = v;
    }
}

gdx_cpp::graphics::g2d::BitmapFont::TextBounds& BitmapFontCache::setText (const CharSequence& str,float x,float y) {
    return setText(str, x, y, 0, str.length());
}

gdx_cpp::graphics::g2d::BitmapFont::TextBounds& BitmapFontCache::setText (const CharSequence& str,float x,float y,int start,int end) {
    reset(end - start);
    y += font.data.ascent;
    textBounds.width = addToCache(str, x, y, start, end);
    textBounds.height = font.data.capHeight;
    return textBounds;
}

gdx_cpp::graphics::g2d::BitmapFont::TextBounds& BitmapFontCache::setMultiLineText (const CharSequence& str,float x,float y) {
    return setMultiLineText(str, x, y, 0, HAlignment.LEFT);
}

gdx_cpp::graphics::g2d::BitmapFont::TextBounds& BitmapFontCache::setMultiLineText (const CharSequence& str,float x,float y,float alignmentWidth,const gdx_cpp::graphics::g2d::BitmapFont::HAlignment& alignment) {
    BitmapFont font = this.font;

    int length = str.length();
    reset(length);

    y += font.data.ascent;
    float down = font.data.down;

    float maxWidth = 0;
    float startY = y;
    int start = 0;
    int numLines = 0;
    while (start < length) {
        int lineEnd = BitmapFont.indexOf(str, '\n', start);
        float xOffset = 0;
        if (alignment != HAlignment.LEFT) {
            float lineWidth = font.getBounds(str, start, lineEnd).width;
            xOffset = alignmentWidth - lineWidth;
            if (alignment == HAlignment.CENTER) xOffset /= 2;
        }
        float lineWidth = addToCache(str, x + xOffset, y, start, lineEnd);
        maxWidth = Math.max(maxWidth, lineWidth);
        start = lineEnd + 1;
        y += down;
        numLines++;
    }
    textBounds.width = maxWidth;
    textBounds.height = font.data.capHeight + (numLines - 1) * font.data.lineHeight;
    return textBounds;
}

gdx_cpp::graphics::g2d::BitmapFont::TextBounds& BitmapFontCache::setWrappedText (const CharSequence& str,float x,float y,float wrapWidth) {
    return setWrappedText(str, x, y, wrapWidth, HAlignment.LEFT);
}

gdx_cpp::graphics::g2d::BitmapFont::TextBounds& BitmapFontCache::setWrappedText (const CharSequence& str,float x,float y,float wrapWidth,const gdx_cpp::graphics::g2d::BitmapFont::HAlignment& alignment) {
    BitmapFont font = this.font;

    int length = str.length();
    reset(length);

    y += font.data.ascent;
    float down = font.data.down;

    float maxWidth = 0;
    int start = 0;
    int numLines = 0;
    while (start < length) {
        int lineEnd = start + font.computeVisibleGlyphs(str, start, BitmapFont.indexOf(str, '\n', start), wrapWidth);
        if (lineEnd < length) {
            while (lineEnd > start) {
                char ch = str.charAt(lineEnd);
                if (ch == ' ' || ch == '\n') break;
                lineEnd--;
            }
        }
        if (lineEnd == start) lineEnd++;
        float xOffset = 0;
        if (alignment != HAlignment.LEFT) {
            float lineWidth = font.getBounds(str, start, lineEnd).width;
            xOffset = wrapWidth - lineWidth;
            if (alignment == HAlignment.CENTER) xOffset /= 2;
        }
        float lineWidth = addToCache(str, x + xOffset, y, start, lineEnd);
        maxWidth = Math.max(maxWidth, lineWidth);
        start = lineEnd + 1;
        y += down;
        numLines++;
    }
    textBounds.width = maxWidth;
    textBounds.height = font.data.capHeight + (numLines - 1) * font.data.lineHeight;
    return textBounds;
}

gdx_cpp::graphics::g2d::BitmapFont::TextBounds& BitmapFontCache::getBounds () {
    return textBounds;
}

float BitmapFontCache::getX () {
    return x;
}

float BitmapFontCache::getY () {
    return y;
}

BitmapFont& BitmapFontCache::getFont () {
    return font;
}

void BitmapFontCache::dispose () {
    font.dispose();
}

void BitmapFontCache::setUseIntegerPositions (bool use) {
    this.integer = use;
}

bool BitmapFontCache::usesIntegerPositions () {
    return integer;
}

