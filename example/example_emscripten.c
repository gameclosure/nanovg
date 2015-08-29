// emcc --preload-file ../example -s USE_GLFW=3 -s FULL_ES2=1 -I../src -o example.html ../example/example_emscripten.c ../src/nanovg.c 
// ../example/demo.c ../example/perf.c -DNANOVG_GLEW -DEMSCRIPTEN -DDEMO -O3 
// -O0 - no opti
// -O1 - asm.js opti
// -02 - llvm bytecode opti 

#include <stdio.h>
#include <stdlib.h>
#include <emscripten/emscripten.h>
#define GLFW_INCLUDE_ES2 
#include <GLFW/glfw3.h>
#include "nanovg.h"
#define NANOVG_GLES2_IMPLEMENTATION
#include "nanovg_gl.h"
#include "nanovg_gl_utils.h"
#ifdef DEMO
#include "demo.h"
#include "perf.h"
#endif

GLFWwindow * window;
NVGcontext *vg;

#ifdef DEMO
DemoData data;
PerfGraph fps;
#endif

double prevt;
int width = 1000;
int height = 600; 

void loop(){

  double mx, my, t, dt;
  int winWidth, winHeight;
  int fbWidth, fbHeight;
  float pxRatio;

#ifdef DEMO
  t = glfwGetTime();
  dt = t - prevt;
  prevt = t;

  updateGraph(&fps, dt);
#endif

  glfwGetCursorPos(window, &mx, &my);
  glfwGetWindowSize(window, &winWidth, &winHeight);
  glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
  // Calculate pixel ration for hi-dpi devices.
  pxRatio = (float)fbWidth / (float)winWidth;

  // Update and render
  glViewport(0, 0, fbWidth, fbHeight);

  glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

  nvgBeginFrame(vg, width, height, pxRatio);
#ifdef DEMO
  renderDemo(vg, mx,my, winWidth,winHeight, t, 0, &data);
  renderGraph(vg, 5,5, &fps);
#endif
  nvgEndFrame(vg);

  glfwSwapBuffers(window);
  glfwPollEvents();
}
 
int main(){  
  glfwInit();
#ifdef DEMO
  initGraph(&fps, GRAPH_RENDER_FPS, "Frame Time");
#endif

  window = glfwCreateWindow(width, height, "NanoVG", NULL, NULL);
  glfwMakeContextCurrent(window);

  vg = nvgCreateGLES2(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
#ifdef DEMO
  loadDemoData(vg, &data);
#endif

  glfwSetTime(0);
  prevt = glfwGetTime();

  emscripten_set_main_loop(loop, 0, 1);

  glfwSwapInterval(1);
  
  nvgDeleteGLES2(vg);
  glfwTerminate();
  glfwDestroyWindow(window);

  return 0;
}
