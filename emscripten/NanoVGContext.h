#define GLFW_INCLUDE_ES2 
#include <GLFW/glfw3.h>
#include "nanovg.h"

class NanoVGContext {
public:
  NanoVGContext(int rWidth, int rHeight);
  ~NanoVGContext();

  void glBeginLoop();
  void glEndLoop();


  void save();
  void restore();

  // create texture is so that we can return a texture a GL Texture id to javascript
  int createTextureFromImage(int sWidth, int sHeight);
  int createTextureFromText(int sWidth, int sHeight);

  //clear is not part of the spec but we need it
  void clear();

 
  void translate(int x, int y);
  void rect(int x, int y, int w, int h);
  void rotate(float angle);
  void scale(float x, float y);

  //scissor is not part of the spec but we use it for clipping
  void scissor(int x, int y, int w, int h);

  //TODO: FIXME
  void drawImage(int mTexture, int sx, int sy, int sWidth, int sHeight, int dx, int dy, int dWidth, int dHeight, int tWidth, int tHeight, float alpha);


  void beginPath();
  void stroke();
  void moveTo(int x, int y);
  void lineTo(int x, int y);
  void fillRect(int x, int y, int width, int height);
  void strokeRect(int x, int y, int width, int height);
  void clearRect(int x, int y, int width, int height);

  void closePath();

  //NOTE: we do not support pathrule for fill
  void fill();

  void arc(float x, float y, float radius, float startAngle, float endAngle, bool antiClockwise);
  void arcTo(float x1, float y1, float x2, float y2, float radius);

  void bezierCurveTo(float cpx1, float cpy1, float cpx2, float cpy2, float x, float y);
  void quadraticCurveTo(float cpx, float cpy, float x, float y);

  //STUB: do max width
  void fillText(char* text, int x, int y);
  void strokeText(char* text, int x, int y);

/*
    //Functions
    CanvasRenderingContext2D.addHitRegion()
    CanvasRenderingContext2D.arc()
    CanvasRenderingContext2D.arcTo()
    CanvasRenderingContext2D.asyncDrawXULElement()

    CanvasRenderingContext2D.clearHitRegions()

    CanvasRenderingContext2D.createImageData()
    CanvasRenderingContext2D.createLinearGradient()
    CanvasRenderingContext2D.createPattern()
    CanvasRenderingContext2D.createRadialGradient()
    CanvasRenderingContext2D.drawFocusIfNeeded()

    CanvasRenderingContext2D.drawWindow()
    CanvasRenderingContext2D.ellipse()
    CanvasRenderingContext2D.fill()


    CanvasRenderingContext2D.getImageData()
    CanvasRenderingContext2D.getLineDash()
    CanvasRenderingContext2D.isPointInPath()
    CanvasRenderingContext2D.isPointInStroke()

    CanvasRenderingContext2D.measureText()
    CanvasRenderingContext2D.moveTo()
    CanvasRenderingContext2D.putImageData()

    CanvasRenderingContext2D.removeHitRegion()
    CanvasRenderingContext2D.resetTransform()


    CanvasRenderingContext2D.scrollPathIntoView()
    CanvasRenderingContext2D.setLineDash()
    CanvasRenderingContext2D.setTransform()

    CanvasRenderingContext2D.strokeRect()

    CanvasRenderingContext2D.transform()

*/

  void set_globalAlpha(float a);

  void setTransform(float a, float b, float c, float d, float e, float f);

  float measureText(char* text);

  float globalAlpha;

/*
    //Variables
    CanvasRenderingContext2D.canvas
    CanvasRenderingContext2D.currentTransform
    CanvasRenderingContext2D.direction

    CanvasRenderingContext2D.filter
    CanvasRenderingContext2D.globalAlpha
    CanvasRenderingContext2D.globalCompositeOperation
    CanvasRenderingContext2D.imageSmoothingEnabled
    CanvasRenderingContext2D.lineCap
    CanvasRenderingContext2D.lineDashOffset
    CanvasRenderingContext2D.lineJoin
    CanvasRenderingContext2D.lineWidth
    CanvasRenderingContext2D.miterLimit
    CanvasRenderingContext2D.shadowBlur
    CanvasRenderingContext2D.shadowColor
    CanvasRenderingContext2D.shadowOffsetX
    CanvasRenderingContext2D.shadowOffsetY
    
*/

  // CanvasRenderingContext2D.fillStyle
  char *fillStyle;
  // CanvasRenderingContext2D.strokeStyle
  char *strokeStyle;
  // CanvasRenderingContext2D.font
  char *font;
  // CanvasRenderingContext2D.textAlign
  char *textAlign;
  // CanvasRenderingContext2D.textBaseline
  char *textBaseline;
  // We need actual text to print
  char *text;

//private:
int width;
int height;
// we need an string to set the texture name so js can access it.
int drawImageMapID; 
int drawTextMapID;

int displayingImage;
// this is to ensure glBegin is not called again before glEnd
bool savestate;
};