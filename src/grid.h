#ifndef GUARD_INCLUDE_GRID_H
#define GUARD_INCLUDE_GRID_H

#ifdef GRID_STATIC
    #define GRID_DEF static
#else
    #define GRID_DEF extern
#endif

#include <glad/glad.h>

#include "m3d.h"
#include "common.h"

struct LineGrid {
    GLuint  vao[3];
    GLuint  vbo[3];
    GLsizei indexCount[3];
    Vec4    lineColors[3];
};

struct LineGridShader {
    GLuint lineProgramId;
    GLuint lineMVP_uniform;
    GLuint lineColor_uniform;
};

GRID_DEF LineGrid makeLineGrid(f32 spacing, Vec4 gridColor, Vec4 tickColor, Vec4 axisColor);
GRID_DEF void     renderLineGrid(LineGrid* grid, LineGridShader* program, Mat4* mvp);

#endif // GUARD_INCLUDE_ELEMENTS_H


#ifdef GRID_IMPLEMENTATION

#include <stdlib.h>
#include <stdio.h>
#include <SDL_log.h>

GRID_DEF LineGrid
makeLineGrid(f32 spacing, Vec4 gridColor, Vec4 tickColor, Vec4 axisColor)
{
    auto grid        = LineGrid{};
    auto lineCnt     = size_t(1000); // per one side of an axis
    auto axisCnt     = 4;
    auto f32PerLine  = 4;
    auto lineByteCnt = lineCnt * axisCnt * f32PerLine * sizeof(f32);
    auto lines       = (f32*) malloc(lineByteCnt);

    if (lines == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
                        "Not enough memory to generate a line grid.\n");
        exit(EXIT_FAILURE);
    }
    defer(free(lines));

    glGenVertexArrays(3, grid.vao);
    glGenBuffers(3, grid.vbo);

    // Ignore the center line for each axis and generate all the other grid lines.
    auto endPt = lineCnt * spacing;

    for (auto idx = 0, lineNum = 1; lineNum <= lineCnt; idx += 16, lineNum++)
    {
        // horizontal positive
        lines[idx + 0] = -endPt;
        lines[idx + 1] = f32(lineNum) * spacing;
        lines[idx + 2] = endPt;
        lines[idx + 3] = f32(lineNum) * spacing;
        // horizontal negative
        lines[idx + 4] = -endPt;
        lines[idx + 5] = -f32(lineNum) * spacing;
        lines[idx + 6] = endPt;
        lines[idx + 7] = -f32(lineNum) * spacing;

        // vertical positive
        lines[idx + 8]  = f32(lineNum) * spacing;
        lines[idx + 9]  = endPt;
        lines[idx + 10] = f32(lineNum) * spacing;
        lines[idx + 11] = -endPt;
        // vertical negative
        lines[idx + 12] = -f32(lineNum) * spacing;
        lines[idx + 13] = endPt;
        lines[idx + 14] = -f32(lineNum) * spacing;
        lines[idx + 15] = -endPt;
    }

    glBindVertexArray(grid.vao[0]);
    glBindBuffer(GL_ARRAY_BUFFER, grid.vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, lineByteCnt, lines, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // Generate the tick marks on the gird lines.
    auto tickPt = 5.0f;
    for (auto idx = 0, lineNum = 1; lineNum <= lineCnt; idx += 16, lineNum++)
    {
        // horizontal positive
        lines[idx + 0] = -tickPt;
        lines[idx + 1] = f32(lineNum) * spacing;
        lines[idx + 2] = tickPt;
        lines[idx + 3] = f32(lineNum) * spacing;
        // horizontal negative
        lines[idx + 4] = -tickPt;
        lines[idx + 5] = -f32(lineNum) * spacing;
        lines[idx + 6] = tickPt;
        lines[idx + 7] = -f32(lineNum) * spacing;

        // vertical positive
        lines[idx + 8]  = f32(lineNum) * spacing;
        lines[idx + 9]  = tickPt;
        lines[idx + 10] = f32(lineNum) * spacing;
        lines[idx + 11] = -tickPt;
        // vertical negative
        lines[idx + 12] = -f32(lineNum) * spacing;
        lines[idx + 13] = tickPt;
        lines[idx + 14] = -f32(lineNum) * spacing;
        lines[idx + 15] = -tickPt;
    }

    glBindVertexArray(grid.vao[1]);
    glBindBuffer(GL_ARRAY_BUFFER, grid.vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, lineByteCnt, lines, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    f32 axes[8] = {
        endPt,    0.0f,     -endPt,    0.0f,
        0.0f ,   endPt,       0.0f,  -endPt,
    };

    glBindVertexArray(grid.vao[2]);
    glBindBuffer(GL_ARRAY_BUFFER, grid.vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof axes, axes, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    grid.indexCount[0] = GLsizei(lineCnt * axisCnt * 2);
    grid.indexCount[1] = GLsizei(lineCnt * axisCnt * 2);
    grid.indexCount[2] = GLsizei(4);

    grid.lineColors[0] = gridColor;
    grid.lineColors[1] = tickColor;
    grid.lineColors[2] = axisColor;

    return grid;
}

GRID_DEF void renderLineGrid(LineGrid* grid, LineGridShader* program, Mat4* mvp)
{
    auto& linePrgm = program->lineProgramId;
    auto& mvpU     = program->lineMVP_uniform;
    auto& lineCol  = program->lineColor_uniform;

    glUseProgram(linePrgm);
    glUniformMatrix4fv(mvpU, 1, GL_FALSE, mvp->data);

    for (auto idx = 0; idx < ARRAY_COUNT(grid->vao); ++idx) {
        auto& color = grid->lineColors[idx];

        glUniform4f(lineCol, color.r, color.g, color.b, color.a);
        glBindVertexArray(grid->vao[idx]);
        glDrawArrays(GL_LINES, 0, grid->indexCount[idx]);
    }
    glBindVertexArray(0);
}

#endif // GRID_IMPLEMENTATION
