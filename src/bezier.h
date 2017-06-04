#ifndef GUARD_BEZIER_H 
#define GUARD_BEZIER_H 

#ifdef BEZIER_STATIC
    #define BEZIER_DEF static
#else
    #define BEZIER_DEF extern
#endif

#include <glad/glad.h>

enum class BezierProperty : i32 {
    Line  = 0,      // line that connects control points
    Curve = 1,
    Point = 2,
    Text  = 3,
};

struct Bezier {
    Vec2 cp[4];
    Vec4 colors[4];
    f32  lineWidth[4];

    /* Don't change after making the quad. */
    u32 segments = 24;

    GLuint  vao[4];
    GLuint  vbo[4];
    GLsizei indexCount[4];
    GLsizei textIndexCount[4];
    GLsizei textCharCount[4];
    GLenum  drawType[4];

    Bezier& setLineColor(f32 r, f32 g, f32 b, f32 a) {
        colors[i32(BezierProperty::Line)] = vec4(r,g,b,a);
        return *this;
    }

    Bezier& setLineSize(f32 size) {
        lineWidth[i32(BezierProperty::Line)] = size;
        return *this;
    }

    Bezier& setCurveColor(f32 r, f32 g, f32 b, f32 a) {
        colors[i32(BezierProperty::Curve)] = vec4(r,g,b,a);
        return *this;
    }

    Bezier& setCurveSize(f32 size) {
        lineWidth[i32(BezierProperty::Curve)] = size;
        return *this;
    }

    Bezier& setPointColor(f32 r, f32 g, f32 b, f32 a) {
        colors[i32(BezierProperty::Point)] = vec4(r,g,b,a);
        return *this;
    }

    Bezier& setPointSize(f32 size) {
        lineWidth[i32(BezierProperty::Point)] = size;
        return *this;
    }

    Bezier& setTextColor(f32 r, f32 g, f32 b, f32 a) {
        colors[i32(BezierProperty::Text)] = vec4(r,g,b,a);
        return *this;
    }
};

struct BezierShader {
    GLuint lineProgramId;
    GLuint lineMVP_uniform;
    GLuint lineColor_uniform;

    GLuint textProgramId;
    GLuint textMVP_uniform;
    GLuint textOffset_uniform;
    GLuint textColor_uniform;
    GLuint textTexture_uniform;
};

struct Font;

BEZIER_DEF Bezier makeBezier(u32 segments);
BEZIER_DEF void   loadBezierVertices(Bezier *bez, Font *font);
BEZIER_DEF void   renderBezier(Bezier* bezier, BezierShader* shader, Font* font, Mat4* lineMVP, Mat4* textMVP);

#endif // GUARD_BEZIER_H 




#ifdef BEZIER_IMPLEMENTATION

#include "font.h"

static Vec2 bezier3(f32 scale, Vec2 c1, Vec2 c2, Vec2 c3, Vec2 c4)
{
    auto a = lerp(scale, c1, c2);
    auto b = lerp(scale, c2, c3);
    auto c = lerp(scale, c3, c4);
    auto d = lerp(scale, a, b);
    auto e = lerp(scale, b, c);
    auto f = lerp(scale, d, e);

    return f;
}

BEZIER_DEF Bezier makeBezier(u32 segments)
{
    constexpr i32 LINE   = i32(BezierProperty::Line);
    constexpr i32 CURVE  = i32(BezierProperty::Curve);
    constexpr i32 POINT  = i32(BezierProperty::Point);
    constexpr i32 TEXT   = i32(BezierProperty::Text);
    constexpr i32 VTX_SZ = 2 * sizeof(f32);

    auto quad   = Bezier{};
    auto bufCnt = ARRAY_COUNT(quad.vao);

    quad.segments = segments;

    quad.cp[0] = vec2(120.0f, 160.0f);
    quad.cp[1] = vec2(35.0f,  200.0f);
    quad.cp[2] = vec2(220.0f, 260.0f);
    quad.cp[3] = vec2(220.0f, 40.0f);

    quad.lineWidth[LINE]  = 3.0f;
    quad.lineWidth[CURVE] = 3.0f;
    quad.lineWidth[POINT] = 3.0f;
    quad.lineWidth[TEXT]  = 1.0f;

    glGenVertexArrays(bufCnt, quad.vao);
    glGenBuffers(bufCnt, quad.vbo);

    // vertex array for the control point lines
    quad.indexCount[LINE] = 3 * 2; // 3 lines, 6 vertices
    quad.drawType[LINE]   = GL_LINES;
    glBindVertexArray(quad.vao[LINE]);
    glBindBuffer(GL_ARRAY_BUFFER, quad.vbo[LINE]);
    glBufferData(GL_ARRAY_BUFFER, quad.indexCount[LINE] * VTX_SZ, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // vertex array for the bezier curve itself
    quad.indexCount[CURVE] = segments + 1;
    quad.drawType[CURVE]   = GL_LINE_STRIP;
    glBindVertexArray(quad.vao[CURVE]);
    glBindBuffer(GL_ARRAY_BUFFER, quad.vbo[CURVE]);
    glBufferData(GL_ARRAY_BUFFER, quad.indexCount[CURVE] * VTX_SZ, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // vertex array for the control point end points
    quad.indexCount[POINT] = 4; // 4 vertices
    quad.drawType[POINT]   = GL_POINTS;
    glBindVertexArray(quad.vao[POINT]);
    glBindBuffer(GL_ARRAY_BUFFER, quad.vbo[POINT]);
    glBufferData(GL_ARRAY_BUFFER, quad.indexCount[POINT] * VTX_SZ, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // vertex array for the control point text
    // TODO: Track max number of characters?  How to enforce at app level?
    quad.indexCount[TEXT] = 4 * 4 * 19; // 4 vertices for 4 sets where each set is 19 characters each.
    quad.drawType[TEXT]   = GL_QUADS;
    glBindVertexArray(quad.vao[TEXT]);
    glBindBuffer(GL_ARRAY_BUFFER, quad.vbo[TEXT]);
    glBufferData(GL_ARRAY_BUFFER, quad.indexCount[TEXT] * VTX_SZ * 2, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof f32, 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof f32, (GLvoid*) (2 * sizeof f32));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    return quad;
}

BEZIER_DEF void loadBezierVertices(Bezier *bez, Font *font)
{
    constexpr i32 VTX_SZ = 2 * sizeof(f32);

    { // control point lines
    auto access  = i32(BezierProperty::Line);
    auto bufSz   = bez->indexCount[access] * VTX_SZ;
    auto cpLines = (f32*)malloc(bufSz);
    defer(free(cpLines));

    for (auto cp1 = 0; cp1 < ARRAY_COUNT(bez->cp) - 1; ++cp1) {
        auto cp2 = cp1 + 1;
        auto idx = cp1 * 4;

        cpLines[idx + 0] = bez->cp[cp1].x;
        cpLines[idx + 1] = bez->cp[cp1].y;
        cpLines[idx + 2] = bez->cp[cp2].x;
        cpLines[idx + 3] = bez->cp[cp2].y;
    }

    glBindBuffer(GL_ARRAY_BUFFER, bez->vbo[access]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, bufSz, (GLvoid*)cpLines);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    } // end control point lines

    { // the actual bezier curve
    auto access   = i32(BezierProperty::Curve);
    auto bufSz    = bez->indexCount[access] * VTX_SZ;
    auto segments = (f32*)malloc(bufSz);
    defer(free(segments));

    segments[0]                     = bez->cp[0].x;
    segments[1]                     = bez->cp[0].y;
    segments[bez->segments * 2 + 0] = bez->cp[3].x;
    segments[bez->segments * 2 + 1] = bez->cp[3].y;

    for (u32 seg = 1; seg < bez->segments; ++seg) {
        auto idx   = seg * 2;
        auto scale = f32(seg) / f32(bez->segments);
        auto c1    = bez->cp[0];
        auto c2    = bez->cp[1];
        auto c3    = bez->cp[2];
        auto c4    = bez->cp[3];
        auto pt    = bezier3(scale, c1, c2, c3, c4);

        segments[idx + 0] = pt.x;
        segments[idx + 1] = pt.y;
    }

    glBindBuffer(GL_ARRAY_BUFFER, bez->vbo[access]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, bufSz, (GLvoid*)segments);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    } // end bezier curve

    { // control points
    auto access = i32(BezierProperty::Point);
    auto bufSz  = bez->indexCount[access] * VTX_SZ;
    auto cntPts = (f32*)malloc(bufSz);
    defer(free(cntPts));

    for (auto cp = 0; cp < ARRAY_COUNT(bez->cp); ++cp) {
        auto idx = cp * 2;

        cntPts[idx + 0] = bez->cp[cp].x;
        cntPts[idx + 1] = bez->cp[cp].y;
    }

    glBindBuffer(GL_ARRAY_BUFFER, bez->vbo[access]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, bufSz, (GLvoid*)cntPts);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    } // end control points

    { // control point text
    constexpr i32 TXT_BUF_SZ   = 20;
    constexpr i32 VTX_PER_QUAD = 4;
    constexpr i32 ACCESS       = i32(BezierProperty::Text);

    char txtBuf[TXT_BUF_SZ * ARRAY_COUNT(bez->cp)];
    f32  text[TXT_BUF_SZ * VTX_PER_QUAD * 2 * 2]; // *2 for two f32 per vtx, * 2 for texture coordinates

    bez->indexCount[ACCESS] = 0;
    glBindBuffer(GL_ARRAY_BUFFER, bez->vbo[ACCESS]);

    auto offset = i32(0);

    for (auto idx = 0; idx < ARRAY_COUNT(bez->cp); ++idx) {
        constexpr f32 MAX_CP =  100000.0f;
        constexpr f32 MIN_CP = -100000.0f;

        auto& cp = bez->cp[idx];
        auto  x  = cp.x;
        auto  y  = cp.y;
        /*
         * Keep x and y in the interval of [-99999.9f, 99999.9f] to
         * limit the number of rendered text.
         */
        if (x > MAX_CP) x = MAX_CP - 0.1f;
        if (x < MIN_CP) x = MIN_CP + 0.1f;
        if (y > MAX_CP) y = MAX_CP - 0.1f;
        if (y < MIN_CP) y = MIN_CP + 0.1f;

        auto cnt   = snprintf(txtBuf, TXT_BUF_SZ, "%.1f, %.1f", x, y);
        auto bufSz = cnt * VTX_PER_QUAD * VTX_SZ * 2;
        auto txt_x = 0.0f;
        auto txt_y = 0.0f;
        auto quad  = stbtt_aligned_quad{};

        bez->textCharCount[idx]  = cnt;
        bez->textIndexCount[idx] = cnt * VTX_PER_QUAD;

        for (auto i = 0; txtBuf[i] != '\0'; ++i) {
            auto txtIdx = i * 16;

            stbtt_GetBakedQuad(font->data, font->texDim, font->texDim,
                               txtBuf[i] - font->firstAsciiIndex,
                               &txt_x, &txt_y,
                               &quad,
                               true);

            // top left
            text[txtIdx + 0]  = quad.x0;
            text[txtIdx + 1]  = -quad.y0;
            text[txtIdx + 2]  = quad.s0;
            text[txtIdx + 3]  = quad.t0;
            // top right
            text[txtIdx + 4]  = quad.x1;
            text[txtIdx + 5]  = -quad.y0;
            text[txtIdx + 6]  = quad.s1;
            text[txtIdx + 7]  = quad.t0;
            // bottom right
            text[txtIdx + 8]  = quad.x1;
            text[txtIdx + 9]  = -quad.y1;
            text[txtIdx + 10] = quad.s1;
            text[txtIdx + 11] = quad.t1;
            // bottom left
            text[txtIdx + 12] = quad.x0;
            text[txtIdx + 13] = -quad.y1;
            text[txtIdx + 14] = quad.s0;
            text[txtIdx + 15] = quad.t1;
        }

        glBufferSubData(GL_ARRAY_BUFFER, offset, bufSz, (GLvoid*) text);
        offset += bufSz;
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    } // control point text
}

BEZIER_DEF void
renderBezier(Bezier*       bezier,
             BezierShader* shader,
             Font*         font,
             Mat4*         lineMVP,
             Mat4*         textMVP)
{
    GLfloat lineWidth, pointSize;
    glGetFloatv(GL_LINE_WIDTH, &lineWidth);
    glGetFloatv(GL_POINT_SIZE, &pointSize);
    glEnable(GL_LINE_SMOOTH);
    glUseProgram(shader->lineProgramId);
    glUniformMatrix4fv(shader->lineMVP_uniform, 1, GL_FALSE, lineMVP->data);

    for (auto idx = 0; idx < ARRAY_COUNT(bezier->vao) - 1; ++idx) {
        auto& lineColor = bezier->colors[idx];
        glLineWidth(bezier->lineWidth[idx]);
        glPointSize(bezier->lineWidth[idx]);
        glUniform4f(shader->lineColor_uniform, lineColor.r, lineColor.g, lineColor.b, lineColor.a);
        glBindVertexArray(bezier->vao[idx]);
        glDrawArrays(bezier->drawType[idx], 0, bezier->indexCount[idx]);
    }
    glLineWidth(lineWidth);
    glPointSize(pointSize);
    glDisable(GL_LINE_SMOOTH);

    constexpr i32 TEXT = i32(BezierProperty::Text);

    glUseProgram(shader->textProgramId);
    glUniform1i(shader->textTexture_uniform, 0);
    glUniformMatrix4fv(shader->textMVP_uniform, 1, GL_FALSE, textMVP->data);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font->texId);
    glUniform4f(shader->textColor_uniform,
                bezier->colors[TEXT].r,
                bezier->colors[TEXT].g,
                bezier->colors[TEXT].b,
                bezier->colors[TEXT].a);

    for (i32 idx = 0, idxOffset = 0; idx < ARRAY_COUNT(bezier->cp); ++idx) {
        auto& cp       = bezier->cp[idx];
        auto  chCnt    = bezier->textCharCount[idx];
        auto  txt_x    = chCnt * font->pixelHeight * 0.25f;
        auto  txt_y    = font->pixelHeight + 10.0f;
        auto  txtNudge = vec4(txt_x, txt_y, 0.0f, 1.0f);
        auto  offset   = vec4(cp.x, cp.y, 0.0f, 1.0f);

        txtNudge = (*textMVP) * txtNudge;
        offset   = (*lineMVP) * offset;
        offset   = offset - txtNudge;

        glUniform4f(shader->textOffset_uniform, offset.x, offset.y, offset.z, offset.w);
        glBindVertexArray(bezier->vao[TEXT]);
        glDrawArrays(GL_QUADS, idxOffset, bezier->textIndexCount[idx]);
        idxOffset += bezier->textIndexCount[idx];
    }
    glBindVertexArray(0);
}

#endif // BEZIER_IMPLEMENTATION
