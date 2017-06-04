#include <SDL.h>
#include <stdlib.h>
#include <math.h>
#include <glad/glad.h>
#include <glad.c>

#include "stb_truetype.h"

#include "m3d.h"
#include "gl_util.h"
#include "grid.h"
#include "bezier.h"
#include "font.h"
#include "common.h"

char const *VTX2D_SHADER =
#include "shaders/vtx2d.glsl"
"";

char const *FRAG2D_SHADER =
#include "shaders/frag2d.glsl"
"";

char const *VTX_TEXT_SHADER =
#include "shaders/vtxText.glsl"
"";

char const *FRAG_TEXT_SHADER =
#include "shaders/fragText.glsl"
"";

struct Input {
    bool zoomingIn   = false;
    bool zoomingOut  = false;
    bool isPanMode   = false;
    bool action_1    = false;
    bool cursorMoved = false;
    Vec2 cursorRel   = vec2(0, 0);
    Vec2 cursor      = vec2(0, 0);
};

bool isNearPoint(Vec2 src, Vec2 pt, f32 radius)
{
    auto minCorner = vec2(pt.x - radius, pt.y - radius);
    auto maxCorner = vec2(pt.x + radius, pt.y + radius);

    return src.x <= maxCorner.x
        && src.x >= minCorner.x
        && src.y <= maxCorner.y
        && src.y >= minCorner.y;
}

Vec4 mapToWorldCoord(Mat4 const &view, f32 x, f32 y)
{
    auto eye = inverse(view);
    return eye * vec4(x, y, 0.0f, 1.0f);
}

int main(int argc, char *argv[])
{
    //SDL_SetMainReady();
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
                        "Failed to initialize SDL: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    defer(SDL_Quit());

    int screen_w = 1024;
    int screen_h = 768;

    auto window = SDL_CreateWindow("Bezier",
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   screen_w,
                                   screen_h,
                                   SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
                        "Failed to create SDL Window: %s\n",
                        SDL_GetError());
        return EXIT_FAILURE;
    }
    defer(SDL_DestroyWindow(window));

    auto glCtx = SDL_GL_CreateContext(window);
    if (glCtx == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
                        "Failed to create OpenGL Context: %s\n",
                        SDL_GetError());
        return EXIT_FAILURE;
    }
    defer(SDL_GL_DeleteContext(glCtx));
    SDL_GL_MakeCurrent(window, glCtx);

    gladLoadGL();

    // Setup for late swap tearing and fallback to vsync if that is not supported.
    if (SDL_GL_SetSwapInterval(-1) == -1)
        SDL_GL_SetSwapInterval(1);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto vtx2d    = glCreateShader(GL_VERTEX_SHADER);
    auto frag2d   = glCreateShader(GL_FRAGMENT_SHADER);
    auto vtxTxt   = glCreateShader(GL_VERTEX_SHADER);
    auto fragTxt  = glCreateShader(GL_FRAGMENT_SHADER);
    auto linePrgm = glCreateProgram();
    auto textPrgm = glCreateProgram();

    if (!util::buildShader(vtx2d,   VTX2D_SHADER))     return EXIT_FAILURE;
    if (!util::buildShader(frag2d,  FRAG2D_SHADER))    return EXIT_FAILURE;
    if (!util::buildShader(vtxTxt,  VTX_TEXT_SHADER))  return EXIT_FAILURE;
    if (!util::buildShader(fragTxt, FRAG_TEXT_SHADER)) return EXIT_FAILURE;
    glAttachShader(linePrgm, vtx2d);
    glAttachShader(linePrgm, frag2d);
    glAttachShader(textPrgm, vtxTxt);
    glAttachShader(textPrgm, fragTxt);
    if (!util::linkProgram(linePrgm)) return EXIT_FAILURE;
    if (!util::linkProgram(textPrgm)) return EXIT_FAILURE;
    glDeleteShader(vtx2d);
    glDeleteShader(frag2d);
    glDeleteShader(vtxTxt);
    glDeleteShader(fragTxt);
    defer(glDeleteProgram(linePrgm));
    defer(glDeleteProgram(textPrgm));

    auto findOrtho = [&]{
        return orthoGL(0, screen_w, 0, screen_h, 0.0f, 1.0f);
    };

    auto ortho        = findOrtho();
    auto flipY        = scale(1.0f, -1.0f, 1.0f);
    auto screenCenter = translate(screen_w / 2.0f, screen_h / 2.0f, 0.0f);
    auto screenZoom   = identity();
    auto screenMove   = identity();
    auto mvp          = ortho * screenCenter * screenZoom * flipY;
    auto gridShader   = LineGridShader{};
    auto bezierShader = BezierShader{};

    gridShader.lineProgramId     = linePrgm;
    gridShader.lineMVP_uniform   = glGetUniformLocation(linePrgm, "MVP");
    gridShader.lineColor_uniform = glGetUniformLocation(linePrgm, "LineColor");

    bezierShader.lineProgramId     = gridShader.lineProgramId;
    bezierShader.lineColor_uniform = gridShader.lineColor_uniform;
    bezierShader.lineMVP_uniform   = gridShader.lineMVP_uniform;

    bezierShader.textProgramId       = textPrgm;
    bezierShader.textMVP_uniform     = glGetUniformLocation(textPrgm, "MVP");
    bezierShader.textOffset_uniform  = glGetUniformLocation(textPrgm, "TextOffset");
    bezierShader.textColor_uniform   = glGetUniformLocation(textPrgm, "TextColor");
    bezierShader.textTexture_uniform = glGetUniformLocation(textPrgm, "TextTexture");

    constexpr char const *FONT_FILE = "assets/Roboto-Regular.ttf";

    auto font = Font{};
    glGenTextures(1, &font.texId);
    fillFontData(FONT_FILE, &font);

    auto black  = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    auto grid   = makeLineGrid(25.0f, vec4(0.1f, 0.35f, 0.8f, 0.4f), black, black);
    auto bezier = makeBezier(64);

    loadBezierVertices(&bezier, &font);
    bezier.setLineSize(1.0f).setLineColor(0.7f, 0.3f, 0.05f, 1.0f);
    bezier.setCurveSize(3.0f).setCurveColor(0.1f, 0.9f, 0.25f, 1.0f);
    bezier.setPointSize(6.0f).setPointColor(0.1f, 0.3f, 0.85f, 1.0f);
    bezier.setTextColor(0.05f, 0.05f, 0.05f, 1.0f);

    constexpr i32 CONTROL_PT_NOT_MOVING = -1;

    auto running    = true;
    auto prevInput  = Input{};
    auto movedCntPt = CONTROL_PT_NOT_MOVING;
    auto cursor     = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    defer(SDL_FreeCursor(cursor));

    SDL_SetCursor(cursor);

    while (running) {
        auto event   = SDL_Event{};
        auto input   = Input{};
        auto resized = false;
        auto mouse_x = i32(0);
        auto mouse_y = i32(0);

        SDL_GetMouseState(&mouse_x, &mouse_y);

        input.cursor    = vec2(f32(mouse_x), f32(mouse_y));
        input.isPanMode = prevInput.isPanMode;
        input.action_1  = prevInput.action_1;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {

            case SDL_QUIT: {
                running = false;
            } break;

            case SDL_WINDOWEVENT: {
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    resized = true;
                    SDL_GL_GetDrawableSize(window, &screen_w, &screen_h);
                }
            } break;

            case SDL_MOUSEMOTION: {
                input.cursor      = vec2(f32(event.motion.x),    f32(event.motion.y));
                input.cursorRel   = vec2(f32(event.motion.xrel), f32(event.motion.yrel));
                input.cursorMoved = true;
            } break;

            case SDL_MOUSEBUTTONDOWN: {
                auto& btn = event.button;
                if (btn.state == SDL_PRESSED) {
                    if (btn.button == SDL_BUTTON_LEFT)  input.action_1  = true;
                    if (btn.button == SDL_BUTTON_RIGHT) input.isPanMode = true;
                }
                input.cursor = vec2(f32(btn.x), f32(btn.y));
            } break;

            case SDL_MOUSEBUTTONUP: {
                auto& btn = event.button;
                if (btn.state == SDL_RELEASED) {
                    if (btn.button == SDL_BUTTON_LEFT)  input.action_1  = false;
                    if (btn.button == SDL_BUTTON_RIGHT) input.isPanMode = false;
                }
            } break;

            case SDL_MOUSEWHEEL: {
                auto& wheel = event.wheel;
                if (wheel.y > 0) input.zoomingIn  = true;
                if (wheel.y < 0) input.zoomingOut = true;
            } break;

            default: {
            } break;

            } // end switch event.type
        }

        if (resized) {
            ortho        = findOrtho();
            screenCenter = translate(screen_w / 2.0f, screen_h / 2.0f, 0.0f);
            glViewport(0, 0, screen_w, screen_h);
        }

        if (input.isPanMode) {
            if (!prevInput.isPanMode) {
                SDL_FreeCursor(cursor);
                cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
                SDL_SetCursor(cursor);
            }
            if (input.cursorMoved) {
                auto trans = translate(input.cursorRel.x, input.cursorRel.y, 0.0f);
                screenMove = screenMove * trans;
            }
        }
        if (!input.isPanMode && prevInput.isPanMode) {
            SDL_FreeCursor(cursor);
            cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
            SDL_SetCursor(cursor);
        }

        auto view = screenCenter * screenMove * screenZoom * flipY;
        auto zoom = [&](f32 scaling) {
            auto in_x = input.cursor.x;
            auto in_y = input.cursor.y;
            auto posA = mapToWorldCoord(view, in_x, in_y);

            screenZoom = screenZoom * scale(scaling, scaling, scaling);

            auto nextView = screenCenter * screenMove * screenZoom * flipY;
            auto posB     = mapToWorldCoord(nextView, in_x, in_y);
            auto rel      = screenZoom.data[0] * (posB - posA);
            rel.y = -rel.y;

            screenMove = screenMove * translate(rel.x, rel.y, 0.0f);
            view       = screenCenter * screenMove * screenZoom * flipY;
        };

        if (input.zoomingIn && screenZoom.data[0] < 1024.0f) {
            zoom(2.0f);
        }
        if (input.zoomingOut && screenZoom.data[0] > 0.25f) {
            zoom(0.5f);
        }

        if (input.action_1 && !prevInput.action_1) {
            auto pos = mapToWorldCoord(view, input.cursor.x, input.cursor.y);

            for (auto idx = 0; idx < ARRAY_COUNT(bezier.cp); ++idx) {
                auto& cp = bezier.cp[idx];

                if (isNearPoint(vec2(pos.x, pos.y), cp, 8.0f)) {
                    movedCntPt = idx;
                    break;
                }
            }
        }

        if (!input.action_1 && prevInput.action_1) {
            movedCntPt = CONTROL_PT_NOT_MOVING;
        }

        if (movedCntPt > CONTROL_PT_NOT_MOVING && input.cursorMoved) {
            auto pos = mapToWorldCoord(view, input.cursor.x, input.cursor.y);

            bezier.cp[movedCntPt].x = pos.x;
            bezier.cp[movedCntPt].y = pos.y;

            loadBezierVertices(&bezier, &font);
        }

        prevInput = input;
        mvp       = ortho * view;

        auto textMvp = ortho * screenCenter * screenMove * flipY;

        glClearColor(0.98f, 0.98f, 0.98f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderLineGrid(&grid, &gridShader, &mvp);
        renderBezier(&bezier, &bezierShader, &font, &mvp, &textMvp);

        SDL_GL_SwapWindow(window);
    }

    return EXIT_SUCCESS;
}
