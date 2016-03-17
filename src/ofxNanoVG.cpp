#include "ofxNanoVG.h"

#define NANOVG_GL2
#define NANOVG_GL3
#include "nanovg/src/nanovg_gl.h"

NVGcontext *nvgCreateOFX(int flags) {
	if (ofIsGLProgrammableRenderer()) {
		return nvgCreateGL3(flags);
	} else {
		return nvgCreateGL2(flags);
	}
}
void nvgDeleteOFX(NVGcontext *ctx) {
	if (ofIsGLProgrammableRenderer()) {
		return nvgDeleteGL3(ctx);
	} else {
		return nvgDeleteGL2(ctx);
	}
}
int nvglCreateImageFromHandleOF(NVGcontext* ctx, GLuint textureId, int w, int h, int flags) {
	if (ofIsGLProgrammableRenderer()) {
		return nvglCreateImageFromHandleGL3(ctx, textureId, w, h, flags);
	} else {
		return nvglCreateImageFromHandleGL2(ctx, textureId, w, h, flags);
	}
}
GLuint nvglImageHandleOF(NVGcontext* ctx, int image) {
	if (ofIsGLProgrammableRenderer()) {
		return nvglImageHandleGL3(ctx, image);
	} else {
		return nvglImageFromHandleGL2(ctx, image);
	}
}