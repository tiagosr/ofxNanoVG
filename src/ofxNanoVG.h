#pragma once


#include "ofMain.h"
#include "nanovg/src/nanovg.h"
#include "nanovg/src/nanovg_gl.h"

NVGcontext *nvgCreateOFX(int flags);
void nvgDeleteOFX(NVGcontext *ctx);
int nvglCreateImageFromHandleOF(NVGcontext* ctx, GLuint textureId, int w, int h, int flags);
GLuint nvglImageHandleOF(NVGcontext* ctx, int image);


