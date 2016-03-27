#pragma once


#include "ofMain.h"
#include "nanovg/src/nanovg.h"
#include "nanovg/src/nanovg_gl.h"

NVGcontext *nvgCreateOFX(int flags);
void nvgDeleteOFX(NVGcontext *ctx);
int nvglCreateImageFromHandleOF(NVGcontext* ctx, GLuint textureId, int w, int h, int flags);
GLuint nvglImageHandleOF(NVGcontext* ctx, int image);

inline NVGcolor toNVGcolor(ofColor c) {
    return nvgRGBA(c.r, c.g, c.b, c.a);
}
inline ofColor NVGtoOFcolor(NVGcolor n) {
    return ofColor(n.r, n.g, n.b, n.a);
}


class ofxNanoVG {
protected:
    NVGcontext* ctx;
    int flags;
public:
    ofxNanoVG(int flags = 0):ctx(0), flags(flags) {
        
    }
    void setup() {
        ctx = nvgCreateOFX(flags);
    }
    void setup(int new_flags) {
        flags = new_flags;
        ctx = nvgCreateOFX(new_flags);
    }
    virtual ~ofxNanoVG();
    void beginFrame(int windowWidth, int windowHeight, float devicePixelRatio = 1.0) {
        nvgBeginFrame(ctx, windowWidth, windowHeight, devicePixelRatio);
    }
    void cancelFrame() { nvgCancelFrame(ctx); }
    void endFrame() { nvgEndFrame(ctx); }
    
    void save() { nvgSave(ctx); }
    void restore() { nvgRestore(ctx); }
    void reset() { nvgReset(ctx); }
    
    void strokeColor(ofColor c) { nvgStrokeColor(ctx, toNVGcolor(c)); }
    void strokePaint(NVGpaint paint) { nvgStrokePaint(ctx, paint); }
    void fillColor(ofColor c) { nvgFillColor(ctx, toNVGcolor(c)); }
    void fillPaint(NVGpaint paint) { nvgFillPaint(ctx, paint); }
    
    void miterLimit(float limit) { nvgMiterLimit(ctx, limit); }
    void strokeWidth(float width) { nvgStrokeWidth(ctx, width); }
    
    // line end (cap) draw style: NVG_BUTT, NVG_ROUND, NVG_SQUARE
    void lineCap(int cap) { nvgLineCap(ctx, cap); }
    // sharp corner draw style: NVG_MITER, NVG_ROUND, NVG_BEVEL
    void lineJoin(int join) { nvgLineJoin(ctx, join); }
    
    void globalAlpha(float alpha) { nvgGlobalAlpha(ctx, alpha); }
    
    struct Transform {
        union {
            float a, b, c, d, x, y;
            float f[6];
        };
    };

    void resetTransform() { nvgResetTransform(ctx); }
    void transform(const Transform& t) {
        nvgTransform(ctx, t.a, t.b, t.c, t.d, t.x, t.y);
    }
    void transform(float a, float b, float c, float d, float x, float y) {
        nvgTransform(ctx, a, b, c, d, x, y);
    }
    void translate(float x, float y) {
        nvgTranslate(ctx, x, y);
    }
    void rotate(float angle) {
        nvgRotate(ctx, angle);
    }
    void skewX(float angle) {
        nvgSkewX(ctx, angle);
    }
    void skewY(float angle) {
        nvgSkewY(ctx, angle);
    }
    void scale(float x, float y) {
        nvgScale(ctx, x, y);
    }
    
    
    Transform currentTransform() {
        Transform t;
        nvgCurrentTransform(ctx, t.f);
        return t;
    }
    static Transform transformIdentity() {
        Transform t;
        nvgTransformIdentity(t.f);
        return t;
    }
    static Transform transformTranslate(float x, float y) {
        Transform t;
        nvgTransformTranslate(t.f, x, y);
        return t;
    }
    static Transform transformScale(float x, float y) {
        Transform t;
        nvgTransformScale(t.f, x, y);
        return t;
    }
    static Transform transformRotate(float angle) {
        Transform t;
        nvgTransformRotate(t.f, angle);
        return t;
    }
    static Transform transformSkewX(float angle) {
        Transform t;
        nvgTransformSkewX(t.f, angle);
        return t;
    }
    static Transform transformSkewY(float angle) {
        Transform t;
        nvgTransformSkewY(t.f, angle);
        return t;
    }
    static Transform transformMultiply(Transform a, Transform b) {
        nvgTransformMultiply(a.f, b.f);
        return a;
    }
    static Transform transformPremultiply(Transform a, Transform b) {
        nvgTransformPremultiply(a.f, b.f);
        return a;
    }
    static bool transformInverse(Transform& dst, Transform src) {
        return !!nvgTransformInverse(dst.f, src.f);
    }
    static ofVec2f transformPoint(const Transform& xform, ofVec2f v) {
        nvgTransformPoint(&v.x, &v.y, xform.f, v.x, v.y);
        return v;
    }
    
    int createImage(const string& filename, int imageFlags) {
        return nvgCreateImage(ctx, filename.c_str(), imageFlags);
    }
    int createImageBuf(const ofBuffer& buf, int imageFlags) {
        return nvgCreateImageMem(ctx, imageFlags, (unsigned char*)buf.getData(), buf.size());
    }
    int createImageRGBA(int w, int h, int imageFlags, const unsigned char* data) {
        return nvgCreateImageRGBA(ctx, w, h, imageFlags, data);
    }
    void updateImage(int image, const unsigned char* data) {
        nvgUpdateImage(ctx, image, data);
    }
    void imageSize(int image, int& w, int& h) {
        nvgImageSize(ctx, image, &w, &h);
    }
    void deleteImage(int image) {
        nvgDeleteImage(ctx, image);
    }
    
    NVGpaint linearGradient(float sx, float sy,
                            float ex, float ey,
                            ofColor icol, ofColor ocol) {
        NVGcolor nicol = toNVGcolor(icol);
        NVGcolor nocol = toNVGcolor(ocol);
        return nvgLinearGradient(ctx, sx, sy, ex, ey, nicol, nocol);
    }
    NVGpaint boxGradient(ofRectangle rect, float radius, float feather,
                         ofColor icol, ofColor ocol) {
        NVGcolor nicol = toNVGcolor(icol);
        NVGcolor nocol = toNVGcolor(ocol);
        return nvgBoxGradient(ctx, rect.x, rect.y, rect.width, rect.height,
                              radius, feather, nicol, nocol);
    }
    NVGpaint radialGradient(ofVec2f center, float in_radius, float out_radius,
                            ofColor icol, ofColor ocol) {
        return nvgRadialGradient(ctx, center.x, center.y, in_radius, out_radius,
                                 toNVGcolor(icol), toNVGcolor(ocol));
    }
    NVGpaint imagePattern(ofVec2f top_left, ofVec2f bottom_right,
                          float angle, int image, float alpha) {
        return nvgImagePattern(ctx, top_left.x, top_left.y,
                               bottom_right.x, bottom_right.y,
                               angle, image, alpha);
    }
    
    void scissor(float x, float y, float w, float h) {
        nvgScissor(ctx, x, y, w, h);
    }
    void scissor(ofRectangle rect) {
        scissor(rect.x, rect.y, rect.width, rect.height);
    }
    void intersectScissor(float x, float y, float w, float h) {
        nvgIntersectScissor(ctx, x, y, w, h);
    }
    void intersectScissor(ofRectangle r) {
        intersectScissor(r.x, r.y, r.width, r.height);
    }
    void resetScissor() { nvgResetScissor(ctx); }
    
    void beginPath() { nvgBeginPath(ctx); }
    void moveTo(float x, float y) { nvgMoveTo(ctx, x, y); }
    void moveTo(ofVec2f v) { moveTo(v.x, v.y); }
    void lineTo(float x, float y) { nvgLineTo(ctx, x, y); }
    void lineTo(ofVec2f v) { lineTo(v.x, v.y); }
    void bezierTo(float c1x, float c1y, float c2x, float c2y, float x, float y) {
        nvgBezierTo(ctx, c1x, c1y, c2x, c2y, x, y);
    }
    void bezierto(ofVec2f c1, ofVec2f c2, ofVec2f v) {
        bezierTo(c1.x, c1.y, c2.x, c2.y, v.x, v.y);
    }
    void quadTo(float cx, float cy, float x, float y) {
        nvgQuadTo(ctx, cx, cy, x, y);
    }
    void quadTo(ofVec2f c, ofVec2f v) {
        quadTo(c.x, c.y, v.x, v.y);
    }
    void closePath() { nvgClosePath(ctx); }
    void pathWinding(int dir) { nvgPathWinding(ctx, dir); }

    void arcTo(float x1, float y1, float x2, float y2, float radius) {
        nvgArcTo(ctx, x1, y1, x2, y2, radius);
    }
    void arcTo(ofVec2f v1, ofVec2f v2, float radius) {
        arcTo(v1.x, v1.y, v2.x, v2.y, radius);
    }
    void rect(float x, float y, float w, float h) { nvgRect(ctx, x, y, w, h); }
    void rect(ofRectangle r) { rect(r.x, r.y, r.width, r.height); }
    void roundedRect(float x, float y, float w, float h, float r) {
        nvgRoundedRect(ctx, x, y, w, h, r);
    }
    void roundedRect(ofRectangle rect, float radius) {
        roundedRect(rect.x, rect.y, rect.width, rect.height, radius);
    }
    void ellipse(float cx, float cy, float rx, float ry) {
        nvgEllipse(ctx, cx, cy, rx, ry);
    }
    void ellipse(ofVec2f c, float rx, float ry) { ellipse(c.x, c.y, rx, ry); }
    void circle(float cx, float cy, float r) { nvgCircle(ctx, cx, cy, r); }
    void circle(ofVec2f c, float r) { circle(c.x, c.y, r); }
    void fill() { nvgFill(ctx); }
    void stroke() { nvgStroke(ctx); }
    
    
    int createFont(const string& name, const string& filename) {
        return nvgCreateFont(ctx, name.c_str(), filename.c_str());
    }
    int createFontBuf(const string& name, const ofBuffer& buf);
    int findFont(const string& name) {
        return nvgFindFont(ctx, name.c_str());
    }
    void fontSize(float size) { nvgFontSize(ctx, size); }
    void fontBlur(float blur) { nvgFontBlur(ctx, blur); }
    void textLetterSpacing(float spacing) { nvgTextLetterSpacing(ctx, spacing); }
    void textLineHeight(float height) { nvgTextLineHeight(ctx, height); }
    void textAlgn(int align) { nvgTextAlign(ctx, align); }
    void fontFaceId(int font) { nvgFontFaceId(ctx, font); }
    void fontFace(const string& name) { nvgFontFace(ctx, name.c_str()); }
    float text(float x, float y, const string& str) {
        return nvgText(ctx, x, y, str.c_str(), NULL);
    }
    float text(ofVec2f v, const string& str) { return text(v.x, v.y, str); }
    void textBox(float x, float y, float breakRowWidth, const string& str) {
        nvgTextBox(ctx, x, y, breakRowWidth, str.c_str(), NULL);
    }
    void textBox(ofVec2f v, float breakRowWidth, const string& str) {
        textBox(v.x, v.y, breakRowWidth, str);
    }
    float textBounds(float x, float y, const string& str, ofRectangle& bounds) {
        float b[4];
        float f = nvgTextBounds(ctx, x, y, str.c_str(), NULL, b);
        bounds.x = b[0], bounds.y = b[1], bounds.width = b[2]-b[0], bounds.height = b[3]-b[1];
        return f;
    }
    float textBounds(ofVec2f v, const string& str, ofRectangle& bounds) {
        return textBounds(v.x, v.y, str, bounds);
    }
    void textBoxBounds(float x, float y, float breakRowWidth,
                       const string& str, ofRectangle& bounds) {
        float b[4];
        nvgTextBoxBounds(ctx, x, y, breakRowWidth, str.c_str(), NULL, b);
        bounds.x = b[0], bounds.y = b[1], bounds.width = b[2]-b[0], bounds.height = b[3]-b[1];
        
    }
    void textBoxBounds(ofVec2f v, float breakRowWidth, const string& str, ofRectangle& bounds) {
        textBoxBounds(v.x, v.y, breakRowWidth, str, bounds);
    }
    int textGlyphPositions(float x, float y, const string& str,
                           vector<NVGglyphPosition>& positions) {
        int i = nvgTextGlyphPositions(ctx, x, y, str.c_str(), NULL,
                                      positions.data(), positions.size());
        return i;
    }
    int textGlyphPositions(ofVec2f v, const string& str,
                           vector<NVGglyphPosition>& positions) {
        return textGlyphPositions(v.x, v.y, str, positions);
    }
    void textMetrics(float& ascender, float& descender, float& lineh) {
        nvgTextMetrics(ctx, &ascender, &descender, &lineh);
    }
    int textBreakLines(const string& str, float breakRowWidth,
                       vector<NVGtextRow> rows);
    
    
};

