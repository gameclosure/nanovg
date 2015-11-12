// emcc -s USE_GLFW=3 -s FULL_ES2=1 -I../src NanoVGContext.cpp rgba.c ../src/nanovg.c --post-js glue.js -o nanovg.js -DNANOVG_GLEW -DEMSCRIPTEN -O3

#include <emscripten/emscripten.h>
#define GLFW_INCLUDE_ES2 
#include <GLFW/glfw3.h>
#include <assert.h> 
#include "nanovg.h"
#define NANOVG_GLES2_IMPLEMENTATION
#include "nanovg_gl.h"
#include "nanovg_gl_utils.h"

#include "NanovgContext.h"

#include "rgba.h"

NVGcontext *vg;
GLFWwindow * window;

//#define TEST_CASE 1

//TODO: remove GLFW ????
#define GLFW 1

NanoVGContext::NanoVGContext(int rWidth, int rHeight) {
#ifdef GLFW
  glfwInit();
  rgba_init();
  width = rWidth;
  height = rHeight;
  window = glfwCreateWindow(width, height, "NanoVG", NULL, NULL);
  glfwMakeContextCurrent(window);

#else
  rgba_init();
EM_ASM({
  var canvas = document.getElementById("canvas");
  Module.GL = canvas.getContext('webgl');
});
  
#endif
  vg = nvgCreateGLES2(NVG_ANTIALIAS);
  displayingImage=0;
  globalAlpha=1.0f;
  savestate=false;
}

NanoVGContext::~NanoVGContext() {
  nvgDeleteGLES2(vg);
#ifdef GLFW
  glfwTerminate();
  glfwDestroyWindow(window);
#endif
}

void NanoVGContext::glBeginLoop(){

  if(savestate) return;
  savestate = true;

  double mx, my, t, dt;
  int winWidth, winHeight;
  int fbWidth, fbHeight;
  float pxRatio;

  glfwGetCursorPos(window, &mx, &my);
  glfwGetWindowSize(window, &winWidth, &winHeight);
  glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
  // Calculate pixel ration for hi-dpi devices.
  pxRatio = (float)fbWidth / (float)winWidth;

  nvgBeginFrame(vg, width, height, pxRatio);
}

void NanoVGContext::glEndLoop(){
  if(!savestate) return;
  savestate = false;
  nvgEndFrame(vg);
  glfwSwapBuffers(window);
  glfwPollEvents();
}

void NanoVGContext::clear() {
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
}

void NanoVGContext::save(){
  nvgSave(vg);
}

void NanoVGContext::restore(){
  nvgRestore(vg);
}

void NanoVGContext::translate(int x, int y){
  nvgTranslate(vg, x, y);
}

static int renderCreateTextureJS(void* uptr, int type, int w, int h, int imageFlags)
{
	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	GLNVGtexture* tex = glnvg__allocTexture(gl);

	if (tex == NULL) return 0;

#ifdef NANOVG_GLES2
	if (glnvg__nearestPow2(w) != (unsigned int)w || glnvg__nearestPow2(h) != (unsigned int)h) {
		if ((imageFlags & NVG_IMAGE_REPEATX) != 0 || (imageFlags & NVG_IMAGE_REPEATY) != 0) {
			printf("Repeat X/Y is not supported for non power-of-two textures (%d x %d)\n", w, h);
			imageFlags &= ~(NVG_IMAGE_REPEATX | NVG_IMAGE_REPEATY);
		}
		if (imageFlags & NVG_IMAGE_GENERATE_MIPMAPS) {
			printf("Mip-maps is not support for non power-of-two textures (%d x %d)\n", w, h);
			imageFlags &= ~NVG_IMAGE_GENERATE_MIPMAPS;
		}
	}
#endif

	glGenTextures(1, &tex->tex);
	tex->width = w;
	tex->height = h;
	tex->type = type;
	tex->flags = imageFlags;
	glnvg__bindTexture(gl, tex->tex);

	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
#ifndef NANOVG_GLES2
	glPixelStorei(GL_UNPACK_ROW_LENGTH, tex->width);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
#endif

#if defined (NANOVG_GL2)
	// GL 1.4 and later has support for generating mipmaps using a tex parameter.
	if (imageFlags & NVG_IMAGE_GENERATE_MIPMAPS) {
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	}
#endif

	if (type == NVG_TEXTURE_RGBA)
#ifdef TEST_CASE
		EM_ASM({
			GLctx.texImage2D(GLctx.TEXTURE_2D, 0, GLctx.RGBA, GLctx.RGBA, GLctx.UNSIGNED_BYTE, window.context.currentImage);
        });
#else
		EM_ASM({
			var _ctx = exports.get();
			GLctx.texImage2D(GLctx.TEXTURE_2D, 0, GLctx.RGBA, GLctx.RGBA, GLctx.UNSIGNED_BYTE, _ctx.currentImage);
        });
#endif
	else
#if defined(NANOVG_GLES2)
		EM_ASM({
			var _ctx = exports.get();
			GLctx.texImage2D(GLctx.TEXTURE_2D, 0, GLctx.LUMINANCE, GLctx.LUMINANCE, GLctx.UNSIGNED_BYTE, _ctx.currentImage);
	    });
#elif defined(NANOVG_GLES3)
        EM_ASM({
			var _ctx = exports.get();
			GLctx.texImage2D(GLctx.TEXTURE_2D, 0, GLctx.R8, GLctx.RED, GLctx.UNSIGNED_BYTE, _ctx.currentImage);
		});
#else
        EM_ASM({
			var _ctx = exports.get();
			GLctx.texImage2D(GLctx.TEXTURE_2D, 0, GLctx.RED, GLctx.RED, GLctx.UNSIGNED_BYTE, _ctx.currentImage);
		});
#endif

	if (imageFlags & NVG_IMAGE_GENERATE_MIPMAPS) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (imageFlags & NVG_IMAGE_REPEATX)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	if (imageFlags & NVG_IMAGE_REPEATY)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
#ifndef NANOVG_GLES2
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
#endif

	// The new way to build mipmaps on GLES and GL3
#if !defined(NANOVG_GL2)
	if (imageFlags & NVG_IMAGE_GENERATE_MIPMAPS) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}
#endif

	glnvg__checkError(gl, "create tex");
	glnvg__bindTexture(gl, 0);

	return tex->id;
}

static int renderCreateTextTextureJS(void* uptr, int type, int w, int h, int imageFlags)
{
	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	GLNVGtexture* tex = glnvg__allocTexture(gl);

	if (tex == NULL) return 0;

#ifdef NANOVG_GLES2
	if (glnvg__nearestPow2(w) != (unsigned int)w || glnvg__nearestPow2(h) != (unsigned int)h) {
		if ((imageFlags & NVG_IMAGE_REPEATX) != 0 || (imageFlags & NVG_IMAGE_REPEATY) != 0) {
			printf("Repeat X/Y is not supported for non power-of-two textures (%d x %d)\n", w, h);
			imageFlags &= ~(NVG_IMAGE_REPEATX | NVG_IMAGE_REPEATY);
		}
		if (imageFlags & NVG_IMAGE_GENERATE_MIPMAPS) {
			printf("Mip-maps is not support for non power-of-two textures (%d x %d)\n", w, h);
			imageFlags &= ~NVG_IMAGE_GENERATE_MIPMAPS;
		}
	}
#endif

	glGenTextures(1, &tex->tex);
	tex->width = w;
	tex->height = h;
	tex->type = type;
	tex->flags = imageFlags;
	glnvg__bindTexture(gl, tex->tex);

	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
#ifndef NANOVG_GLES2
	glPixelStorei(GL_UNPACK_ROW_LENGTH, tex->width);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
#endif

#if defined (NANOVG_GL2)
	// GL 1.4 and later has support for generating mipmaps using a tex parameter.
	if (imageFlags & NVG_IMAGE_GENERATE_MIPMAPS) {
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	}
#endif

	if (type == NVG_TEXTURE_RGBA)
#ifdef TEST_CASE
		EM_ASM({
			GLctx.texImage2D(GLctx.TEXTURE_2D, 0, GLctx.RGBA, GLctx.RGBA, GLctx.UNSIGNED_BYTE, window.context.currentText);
        });
#else
		EM_ASM({
			var _ctx = exports.get();
			GLctx.texImage2D(GLctx.TEXTURE_2D, 0, GLctx.RGBA, GLctx.RGBA, GLctx.UNSIGNED_BYTE, _ctx.currentText);
        });
#endif
	else
#if defined(NANOVG_GLES2)
		EM_ASM({
			var _ctx = exports.get();
			GLctx.texImage2D(GLctx.TEXTURE_2D, 0, GLctx.LUMINANCE, GLctx.LUMINANCE, GLctx.UNSIGNED_BYTE, _ctx.currentText);
	    });
#elif defined(NANOVG_GLES3)
        EM_ASM({
			var _ctx = exports.get();
			GLctx.texImage2D(GLctx.TEXTURE_2D, 0, GLctx.R8, GLctx.RED, GLctx.UNSIGNED_BYTE, _ctx.currentText);
		});
#else
        EM_ASM({
			var _ctx = exports.get();
			GLctx.texImage2D(GLctx.TEXTURE_2D, 0, GLctx.RED, GLctx.RED, GLctx.UNSIGNED_BYTE, _ctx.currentText);
		});
#endif

	if (imageFlags & NVG_IMAGE_GENERATE_MIPMAPS) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (imageFlags & NVG_IMAGE_REPEATX)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	if (imageFlags & NVG_IMAGE_REPEATY)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
#ifndef NANOVG_GLES2
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
#endif

	// The new way to build mipmaps on GLES and GL3
#if !defined(NANOVG_GL2)
	if (imageFlags & NVG_IMAGE_GENERATE_MIPMAPS) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}
#endif

	glnvg__checkError(gl, "create tex");
	glnvg__bindTexture(gl, 0);

	return tex->id;
}

#define NVG_MAX_STATES 32
#define NVG_MAX_FONTIMAGES 4    

struct NVGstate {
	NVGpaint fill;
	NVGpaint stroke;
	float strokeWidth;
	float miterLimit;
	int lineJoin;
	int lineCap;
	float alpha;
	float xform[6];
	NVGscissor scissor;
	float fontSize;
	float letterSpacing;
	float lineHeight;
	float fontBlur;
	int textAlign;
	int fontId;
};
typedef struct NVGstate NVGstate;

struct NVGpoint {
	float x,y;
	float dx, dy;
	float len;
	float dmx, dmy;
	unsigned char flags;
};
typedef struct NVGpoint NVGpoint;

struct NVGpathCache {
	NVGpoint* points;
	int npoints;
	int cpoints;
	NVGpath* paths;
	int npaths;
	int cpaths;
	NVGvertex* verts;
	int nverts;
	int cverts;
	float bounds[4];
};
typedef struct NVGpathCache NVGpathCache;

struct NVGcontext {
	NVGparams params;
	float* commands;
	int ccommands;
	int ncommands;
	float commandx, commandy;
	NVGstate states[NVG_MAX_STATES];
	int nstates;
	NVGpathCache* cache;
	float tessTol;
	float distTol;
	float fringeWidth;
	float devicePxRatio;
	struct FONScontext* fs;
	int fontImages[NVG_MAX_FONTIMAGES];
	int fontImageIdx;
	int drawCallCount;
	int fillTriCount;
	int strokeTriCount;
	int textTriCount;
};


int NanoVGContext::createTextureFromImage(int sWidth, int sHeight) {
  return drawImageMapID = renderCreateTextureJS(vg->params.userPtr, NVG_TEXTURE_RGBA, sWidth, sHeight, 0);
}

int NanoVGContext::createTextureFromText(int sWidth, int sHeight) {
  return drawTextMapID = renderCreateTextTextureJS(vg->params.userPtr, NVG_TEXTURE_RGBA, sWidth, sHeight, 0);
}


void NanoVGContext::drawImage(int mTexture, int sx, int sy, int sWidth, int sHeight, int dx, int dy, int dWidth, int dHeight, int tWidth, int tHeight, float alpha) {
  float scaleX = (float)dWidth/(float)sWidth;
  float scaleY = (float)dHeight/(float)sHeight;
  NVGpaint imgPaint = nvgImagePattern(vg, (-sx*scaleX)+dx, (-sy*scaleY)+dy, tWidth*scaleX, tHeight*scaleY, 0, mTexture, alpha);
  nvgBeginPath(vg);
  nvgRect(vg, dx, dy, dWidth, dHeight);
  nvgFillPaint(vg, imgPaint);
  nvgFill(vg);
}

void NanoVGContext::beginPath() {
  nvgBeginPath(vg);
}

void NanoVGContext::closePath() {
  nvgClosePath(vg);
}

void NanoVGContext::stroke() {
  nvgStroke(vg);
}

void NanoVGContext::moveTo(int x, int y) {
  nvgMoveTo(vg, x, y);
}

void NanoVGContext::lineTo(int x, int y) {
  nvgLineTo(vg, x, y);
}

void NanoVGContext::fillRect(int x, int y, int width, int height) {
  nvgBeginPath(vg);
  nvgRect(vg, x, y, width, height);
  fill();
}

void NanoVGContext::strokeRect(int x, int y, int width, int height) {
  nvgBeginPath(vg);
  nvgRect(vg, x, y, width, height);
  nvgStroke(vg);
}

void NanoVGContext::clearRect(int x, int y, int width, int height) {
  nvgBeginPath(vg);
  nvgRect(vg, x, y, width, height);
  nvgFillColor(vg, nvgRGBA(0,0,0,0));
  nvgFill(vg);
}

void NanoVGContext::fill() {
  //TODO: merge NVGcolor and rgb_t into the one struct type;
  NVGcolor color;
  rgba rgbcolor;  
  int r,g,b;

  EM_ASM({
    var _ctx = exports.get();
    _ctx.set_fillStyle(_ctx.fillStyle);
  });

  rgba_parse(&rgbcolor, fillStyle);
  color.r = rgbcolor.r / 255.0f;
  color.g = rgbcolor.g / 255.0f;
  color.b = rgbcolor.b / 255.0f;
  color.a = 0.0f;

  nvgFillColor(vg, color);
  nvgFill(vg);
}

void NanoVGContext::rect(int x, int y, int w, int h){
  nvgRect(vg, x, y, w, h);
}

void NanoVGContext::rotate(float angle) {
  nvgRotate(vg, angle);
}

void NanoVGContext::scale(float x, float y) {
  nvgScale(vg, x, y);
}

void NanoVGContext::scissor(int x, int y, int w, int h){
  nvgScissor(vg, x, y, w, h);
}

void NanoVGContext::set_globalAlpha(float a) {
  globalAlpha = a;
  nvgGlobalAlpha(vg, a);
}

void NanoVGContext::setTransform(float a, float b, float c, float d, float e, float f) {
  nvgResetTransform(vg);
  nvgTransform(vg, a, b, c, d, e, f);
}

void NanoVGContext::arc(float x, float y, float radius, float startAngle, float endAngle, bool antiClockwise) {
  nvgArc(vg, x, y, radius, startAngle, endAngle, antiClockwise ? NVG_CCW : NVG_CW);
}

void NanoVGContext::arcTo(float x1, float y1, float x2, float y2, float radius) {
  nvgArcTo(vg, x1, y1, x2, y2, radius);
}

void NanoVGContext::bezierCurveTo(float cpx1, float cpy1, float cpx2, float cpy2, float x, float y) {
  nvgBezierTo(vg, cpx1, cpy1, cpx2, cpy2, x, y);
}

void NanoVGContext::quadraticCurveTo(float cpx, float cpy, float x, float y) {
  nvgQuadTo(vg, cpx, cpy, x, y);
}




// So we are wrapping the calls to font textAlign and fillText to do this to a blank texture
/*
void NanoVGContext::set_font(char* text) {

}

void NanoVGContext::set_textAlign(char* text) {

}
*/


// For now we change src/ui/backend/canvas/TextFlow.js ctx.measureText(" ").width to ctx.measureText(" ")
// How do we pull width and height out of measureText as an object ?
// eg. ctx.measureText(" ").width
float NanoVGContext::measureText(char* text) {
  // TODO: reuse the same canvas rather then creating one each time
  int measure;
  EM_ASM_INT({
  var buffer = document.createElement('canvas');
  // What do we set the canvas size as ?
  // I think we should set it similar to the area of the opengl window
  // Chrome seems to complain if it is not multiple of 2
  buffer.width = GLctx.canvas.wodth;
  buffer.height = GLctx.canvas.height;
  var myctx = buffer.getContext("2d");
  var mytext = myctx.measureText(" ");
  return mytext.width; // 16;
  }, measure);

  return measure;
}

// do not recreate the texture for every frame
// fill text is called on every frame which is something we want to cache and not recall


// TODO: FIX THE OFFSETS OF WHERE TEXT IS BEING RENDERED
void NanoVGContext::fillText(char* text, int x, int y) {
/*
  // this is the point where we actually render the texture that we created from the canvas2d text

  // create the hiddencanvas
  EM_ASM({
    var cubeImage = document.createElement('canvas');
    cubeImage.width = GLctx.canvas.width;
    cubeImage.height = GLctx.canvas.height;
    var textContext = cubeImage.getContext('2d');
    // draw a background with an alpha of 0
    textContext.beginPath();
    textContext.rect(0, 0, textContext.canvas.width, textContext.canvas.height);
    textContext.fillStyle = 'rgba(0,0,0,0)';
    textContext.fill();
    var _ctx = exports.get();
    textContext.fillStyle = _ctx.fillStyle;
    textContext.strokeStyle = _ctx.strokeStyle;
    textContext.font = _ctx.font;
    textContext.textAlign = _ctx.textAlign;
    textContext.fillText(_ctx.get_text(), _ctx.fontx, _ctx.fonty);
    textContext.restore();
    _ctx.currentText=cubeImage;
  });
  // we store this for each new text created and reuse the texture when the content is the same.
  // TODO : We only do this if the texture is not cached
  createTextureFromText(x,y);

  //drawImage(int mTexture, sx, sy, sWidth, sHeight, dx, dy, int dWidth, int dHeight, int tWidth, int tHeight, float alpha);

  // TODO: Calculate the correct text positions
  drawImage(drawTextMapID, 0, 0, width, height, 0, 0, width, height, width, height, 1.0f);
  return;
*/

  // create the hiddencanvas
  EM_ASM({
    var cubeImage = document.createElement('canvas');
    cubeImage.width = 512;
    cubeImage.height = 512;
    var textContext = cubeImage.getContext('2d');
    // draw a background with an alpha of 0
    textContext.beginPath();
    textContext.rect(0, 0, textContext.canvas.width, textContext.canvas.height);
    textContext.fillStyle = 'rgba(0,0,0,0)';
    textContext.fill();
    var _ctx = exports.get();
    textContext.fillStyle = _ctx.fillStyle;
    textContext.strokeStyle = _ctx.strokeStyle;
    textContext.font = _ctx.font;
    textContext.textAlign = _ctx.textAlign;
    textContext.fillText(_ctx.get_text(), textContext.canvas.width/2, textContext.canvas.height/2);
    textContext.restore();
    _ctx.currentText=cubeImage;
  });
    // we store this for each new text created and reuse the texture when the content is the same.
  /*
    char buf[256] = {'\0'};
    float scale = device_get_text_scale();
    snprintf(buf, sizeof(buf), "@TEXT%s|%i|%i|%i|%i|%i|%i|%i|%f|%s",
             font_name,(int) (scale * size),
             (int) (255 * color->r),
             (int) (255 * color->g),
             (int) (255 * color->b),
             (int) (255 * color->a),
             (int)(scale * max_width),
             text_style,
             scale * stroke_width,
             text);
  */

  // TODO : We only do this if the texture is not cached
  createTextureFromText(x,y);

  // TODO: Calculate the correct text positions
  drawImage(drawTextMapID, 0, 0, 512, 512, -128, -128-64, 512, 512, 512, 512, 1.0f);

  return;

}

void NanoVGContext::strokeText(char* text, int x, int y) {

  // this is the point where we actually render the texture that we created from the canvas2d text

  // create the hiddencanvas
  EM_ASM({
    var cubeImage = document.createElement('canvas');
    cubeImage.width = 512;
    cubeImage.height = 512;
    var textContext = cubeImage.getContext('2d');
    // draw a background with an alpha of 0
    textContext.beginPath();
    textContext.rect(0, 0, textContext.canvas.width, textContext.canvas.height);
    textContext.fillStyle = 'rgba(0,0,0,0)';
    textContext.fill();
    var _ctx = exports.get();
    textContext.fillStyle = _ctx.fillStyle;
    textContext.strokeStyle = _ctx.strokeStyle;
    textContext.font = _ctx.font;
    textContext.textAlign = _ctx.textAlign;
    textContext.strokeText(_ctx.get_text(), textContext.canvas.width/2, textContext.canvas.height/2);
    textContext.restore();
    _ctx.currentText=cubeImage;
  });
    // we store this for each new text created and reuse the texture when the content is the same.
  /*
    char buf[256] = {'\0'};
    float scale = device_get_text_scale();
    snprintf(buf, sizeof(buf), "@TEXT%s|%i|%i|%i|%i|%i|%i|%i|%f|%s",
             font_name,(int) (scale * size),
             (int) (255 * color->r),
             (int) (255 * color->g),
             (int) (255 * color->b),
             (int) (255 * color->a),
             (int)(scale * max_width),
             text_style,
             scale * stroke_width,
             text);
  */

  // TODO : We only do this if the texture is not cached
  createTextureFromText(x,y);

  // TODO: Calculate the correct text positions
  drawImage(drawTextMapID, 0, 0, 512, 512, -128, -128-64, 512, 512, 512, 512, 1.0f);

  return;
}
