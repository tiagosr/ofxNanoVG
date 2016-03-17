#pragma once


#include "nanovg/src/nanovg.h"
#include "ofMain.h"

NVGcontext *nvgCreateOFX(int flags);
void nvgDeleteOFX(NVGcontext *ctx);
int nvglCreateImageFromHandleOF(NVGcontext* ctx, GLuint textureId, int w, int h, int flags);
GLuint nvglImageHandleOF(NVGcontext* ctx, int image);


