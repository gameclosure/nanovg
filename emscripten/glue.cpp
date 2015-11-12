
#include <emscripten.h>
#include "NanoVGContext.h"

extern "C" {

// Not using size_t for array indices as the values used by the javascript code are signed.
void array_bounds_check(const int array_size, const int array_idx) {
  if (array_idx < 0 || array_idx >= array_size) {
    EM_ASM_INT({
      throw 'Array index ' + $0 + ' out of bounds: [0,' + $1 + ')';
    }, array_idx, array_size);
  }
}

// NanoVGContext

NanoVGContext* EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_NanoVGContext_2(int arg0, int arg1) {
  return new NanoVGContext(arg0, arg1);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_createTextureFromImage_2(NanoVGContext* self, int arg0, int arg1) {
  self->createTextureFromImage(arg0, arg1);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_glBeginLoop_0(NanoVGContext* self) {
  self->glBeginLoop();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_glEndLoop_0(NanoVGContext* self) {
  self->glEndLoop();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_save_0(NanoVGContext* self) {
  self->save();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_restore_0(NanoVGContext* self) {
  self->restore();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_clear_0(NanoVGContext* self) {
  self->clear();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_translate_2(NanoVGContext* self, int arg0, int arg1) {
  self->translate(arg0, arg1);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_rect_4(NanoVGContext* self, int arg0, int arg1, int arg2, int arg3) {
  self->rect(arg0, arg1, arg2, arg3);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_rotate_1(NanoVGContext* self, float arg0) {
  self->rotate(arg0);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_scale_2(NanoVGContext* self, float arg0, float arg1) {
  self->scale(arg0, arg1);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_scissor_4(NanoVGContext* self, int arg0, int arg1, int arg2, int arg3) {
  self->scissor(arg0, arg1, arg2, arg3);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_drawImage_12(NanoVGContext* self, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11) {
  self->drawImage(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_beginPath_0(NanoVGContext* self) {
  self->beginPath();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_closePath_0(NanoVGContext* self) {
  self->closePath();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_stroke_0(NanoVGContext* self) {
  self->stroke();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_moveTo_2(NanoVGContext* self, int arg0, int arg1) {
  self->moveTo(arg0, arg1);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_lineTo_2(NanoVGContext* self, int arg0, int arg1) {
  self->lineTo(arg0, arg1);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_fillRect_4(NanoVGContext* self, int arg0, int arg1, int arg2, int arg3) {
  self->fillRect(arg0, arg1, arg2, arg3);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_strokeRect_4(NanoVGContext* self, int arg0, int arg1, int arg2, int arg3) {
  self->strokeRect(arg0, arg1, arg2, arg3);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_clearRect_4(NanoVGContext* self, int arg0, int arg1, int arg2, int arg3) {
  self->clearRect(arg0, arg1, arg2, arg3);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_fill_0(NanoVGContext* self) {
  self->fill();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_fillText_3(NanoVGContext* self, char* arg0, int arg1, int arg2) {
  self->fillText(arg0, arg1, arg2);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_strokeText_3(NanoVGContext* self, char* arg0, int arg1, int arg2) {
  self->strokeText(arg0, arg1, arg2);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_arc_6(NanoVGContext* self, float arg0, float arg1, float arg2, float arg3, float arg4, bool arg5) {
  self->arc(arg0, arg1, arg2, arg3, arg4, arg5);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_arcTo_5(NanoVGContext* self, float arg0, float arg1, float arg2, float arg3, float arg4) {
  self->arcTo(arg0, arg1, arg2, arg3, arg4);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_bezierCurveTo_6(NanoVGContext* self, float arg0, float arg1, float arg2, float arg3, float arg4, float arg5) {
  self->bezierCurveTo(arg0, arg1, arg2, arg3, arg4, arg5);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_quadraticCurveTo_4(NanoVGContext* self, float arg0, float arg1, float arg2, float arg3) {
  self->quadraticCurveTo(arg0, arg1, arg2, arg3);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_set_globalAlpha_1(NanoVGContext* self, float arg0) {
  self->set_globalAlpha(arg0);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_setTransform_6(NanoVGContext* self, float arg0, float arg1, float arg2, float arg3, float arg4, float arg5) {
  self->setTransform(arg0, arg1, arg2, arg3, arg4, arg5);
}

float EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_measureText_1(NanoVGContext* self, char* arg0) {
  return self->measureText(arg0);
}

char* EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_get_fillStyle_0(NanoVGContext* self) {
  return self->fillStyle;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_set_fillStyle_1(NanoVGContext* self, char* arg0) {
  self->fillStyle = arg0;
}

char* EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_get_strokeStyle_0(NanoVGContext* self) {
  return self->strokeStyle;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_set_strokeStyle_1(NanoVGContext* self, char* arg0) {
  self->strokeStyle = arg0;
}

char* EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_get_textAlign_0(NanoVGContext* self) {
  return self->textAlign;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_set_textAlign_1(NanoVGContext* self, char* arg0) {
  self->textAlign = arg0;
}

char* EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_get_font_0(NanoVGContext* self) {
  return self->font;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_set_font_1(NanoVGContext* self, char* arg0) {
  self->font = arg0;
}

char* EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_get_text_0(NanoVGContext* self) {
  return self->text;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_set_text_1(NanoVGContext* self, char* arg0) {
  self->text = arg0;
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_get_drawImageMapID_0(NanoVGContext* self) {
  return self->drawImageMapID;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_set_drawImageMapID_1(NanoVGContext* self, int arg0) {
  self->drawImageMapID = arg0;
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_get_drawTextMapID_0(NanoVGContext* self) {
  return self->drawTextMapID;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext_set_drawTextMapID_1(NanoVGContext* self, int arg0) {
  self->drawTextMapID = arg0;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_NanoVGContext___destroy___0(NanoVGContext* self) {
  delete self;
}

// VoidPtr

void EMSCRIPTEN_KEEPALIVE emscripten_bind_VoidPtr___destroy___0(void** self) {
  delete self;
}

}

