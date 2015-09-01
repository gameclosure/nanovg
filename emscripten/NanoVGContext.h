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
  void clearRect(int x, int y, int width, int height);

  void closePath();

  //NOTE: we do not support pathrule for fill
  void fill();


  //STUB: do max width
  void fillText(char* text, int x, int y);
  void strokeText(char* text, int x, int y);
/*
    //Functions
    CanvasRenderingContext2D.addHitRegion()
    CanvasRenderingContext2D.arc()
    CanvasRenderingContext2D.arcTo()
    CanvasRenderingContext2D.asyncDrawXULElement()

    CanvasRenderingContext2D.bezierCurveTo()
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
    CanvasRenderingContext2D.quadraticCurveTo()

    CanvasRenderingContext2D.removeHitRegion()
    CanvasRenderingContext2D.resetTransform()


    CanvasRenderingContext2D.scrollPathIntoView()
    CanvasRenderingContext2D.setLineDash()
    CanvasRenderingContext2D.setTransform()

    CanvasRenderingContext2D.strokeRect()

    CanvasRenderingContext2D.transform()

*/

  char *fillStyle;
  char *font;
  char *textAlign;

  void set_globalAlpha(float a);

  float globalAlpha;

/*
    //Variables
    CanvasRenderingContext2D.canvas
    CanvasRenderingContext2D.currentTransform
    CanvasRenderingContext2D.direction

    CanvasRenderingContext2D.filter
    CanvasRenderingContext2D.font
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
    CanvasRenderingContext2D.strokeStyle

    CanvasRenderingContext2D.textBaseline
*/

//private:
int width;
int height;
// we need an string to set the texture name so js can access it.
int drawImageMapID; 
int displayingImage;
// this is to ensure glBegin is not called again before glEnd
bool savestate;
};