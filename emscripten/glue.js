
// Bindings utilities

function WrapperObject() {
}
WrapperObject.prototype = Object.create(WrapperObject.prototype);
WrapperObject.prototype.constructor = WrapperObject;
WrapperObject.prototype.__class__ = WrapperObject;
WrapperObject.__cache__ = {};
Module['WrapperObject'] = WrapperObject;

function getCache(__class__) {
  return (__class__ || WrapperObject).__cache__;
}
Module['getCache'] = getCache;

function wrapPointer(ptr, __class__) {
  var cache = getCache(__class__);
  var ret = cache[ptr];
  if (ret) return ret;
  ret = Object.create((__class__ || WrapperObject).prototype);
  ret.ptr = ptr;
  return cache[ptr] = ret;
}
Module['wrapPointer'] = wrapPointer;

function castObject(obj, __class__) {
  return wrapPointer(obj.ptr, __class__);
}
Module['castObject'] = castObject;

Module['NULL'] = wrapPointer(0);

function destroy(obj) {
  if (!obj['__destroy__']) throw 'Error: Cannot destroy object. (Did you create it yourself?)';
  obj['__destroy__']();
  // Remove from cache, so the object can be GC'd and refs added onto it released
  delete getCache(obj.__class__)[obj.ptr];
}
Module['destroy'] = destroy;

function compare(obj1, obj2) {
  return obj1.ptr === obj2.ptr;
}
Module['compare'] = compare;

function getPointer(obj) {
  return obj.ptr;
}
Module['getPointer'] = getPointer;

function getClass(obj) {
  return obj.__class__;
}
Module['getClass'] = getClass;

// Converts a value into a C-style string, storing it in temporary space

var ensureStringCache = {
  buffer: 0,  // the main buffer of temporary storage
  size: 0,   // the size of buffer
  pos: 0,    // the next free offset in buffer
  temps: [], // extra allocations
  needed: 0, // the total size we need next time

  prepare: function() {
    if (this.needed) {
      // clear the temps
      for (var i = 0; i < this.temps.length; i++) {
        Module['_free'](this.temps[i]);
      }
      this.temps.length = 0;
      // prepare to allocate a bigger buffer
      Module['_free'](this.buffer);
      this.buffer = 0;
      this.size += this.needed;
      // clean up
      this.needed = 0;
    }
    if (!this.buffer) { // happens first time, or when we need to grow
      this.size += 100; // heuristic, avoid many small grow events
      this.buffer = Module['_malloc'](this.size);
      assert(this.buffer);
    }
    this.pos = 0;
  },
  alloc: function(value) {
    assert(this.buffer);
    var array = intArrayFromString(value);
    var len = array.length;
    var ret;
    if (this.pos + len >= this.size) {
      // we failed to allocate in the buffer, this time around :(
      assert(len > 0); // null terminator, at least
      this.needed += len;
      ret = Module['_malloc'](len);
      this.temps.push(ret);
    } else {
      // we can allocate in the buffer
      ret = this.buffer + this.pos;
      this.pos += len;
    }
    writeArrayToMemory(array, ret);
    return ret;
  },
};

function ensureString(value) {
  if (typeof value === 'string') return ensureStringCache.alloc(value);
  return value;
}


// NanoVGContext
function NanoVGContext(arg0, arg1) {
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  if (arg1 && typeof arg1 === 'object') arg1 = arg1.ptr;
  this.ptr = _emscripten_bind_NanoVGContext_NanoVGContext_2(arg0, arg1);
  getCache(NanoVGContext)[this.ptr] = this;
};;
NanoVGContext.prototype = Object.create(WrapperObject.prototype);
NanoVGContext.prototype.constructor = NanoVGContext;
NanoVGContext.prototype.__class__ = NanoVGContext;
NanoVGContext.__cache__ = {};
Module['NanoVGContext'] = NanoVGContext;

NanoVGContext.prototype['createTextureFromImage'] = NanoVGContext.prototype.createTextureFromImage = function(arg0, arg1) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  if (arg1 && typeof arg1 === 'object') arg1 = arg1.ptr;
  _emscripten_bind_NanoVGContext_createTextureFromImage_2(self, arg0, arg1);
};;

NanoVGContext.prototype['glBeginLoop'] = NanoVGContext.prototype.glBeginLoop = function() {
  var self = this.ptr;
  _emscripten_bind_NanoVGContext_glBeginLoop_0(self);
};;

NanoVGContext.prototype['glEndLoop'] = NanoVGContext.prototype.glEndLoop = function() {
  var self = this.ptr;
  _emscripten_bind_NanoVGContext_glEndLoop_0(self);
};;

NanoVGContext.prototype['save'] = NanoVGContext.prototype.save = function() {
  var self = this.ptr;
  _emscripten_bind_NanoVGContext_save_0(self);
};;

NanoVGContext.prototype['restore'] = NanoVGContext.prototype.restore = function() {
  var self = this.ptr;
  _emscripten_bind_NanoVGContext_restore_0(self);
};;

NanoVGContext.prototype['translate'] = NanoVGContext.prototype.translate = function(arg0, arg1) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  if (arg1 && typeof arg1 === 'object') arg1 = arg1.ptr;
  _emscripten_bind_NanoVGContext_translate_2(self, arg0, arg1);
};;

NanoVGContext.prototype['rect'] = NanoVGContext.prototype.rect = function(arg0, arg1, arg2, arg3) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  if (arg1 && typeof arg1 === 'object') arg1 = arg1.ptr;
  if (arg2 && typeof arg2 === 'object') arg2 = arg2.ptr;
  if (arg3 && typeof arg3 === 'object') arg3 = arg3.ptr;
  _emscripten_bind_NanoVGContext_rect_4(self, arg0, arg1, arg2, arg3);
};;

NanoVGContext.prototype['rotate'] = NanoVGContext.prototype.rotate = function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_NanoVGContext_rotate_1(self, arg0);
};;

NanoVGContext.prototype['scale'] = NanoVGContext.prototype.scale = function(arg0, arg1) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  if (arg1 && typeof arg1 === 'object') arg1 = arg1.ptr;
  _emscripten_bind_NanoVGContext_scale_2(self, arg0, arg1);
};;

NanoVGContext.prototype['scissor'] = NanoVGContext.prototype.scissor = function(arg0, arg1, arg2, arg3) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  if (arg1 && typeof arg1 === 'object') arg1 = arg1.ptr;
  if (arg2 && typeof arg2 === 'object') arg2 = arg2.ptr;
  if (arg3 && typeof arg3 === 'object') arg3 = arg3.ptr;
  _emscripten_bind_NanoVGContext_scissor_4(self, arg0, arg1, arg2, arg3);
};;

NanoVGContext.prototype['drawImage'] = NanoVGContext.prototype.drawImage = function(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  if (arg1 && typeof arg1 === 'object') arg1 = arg1.ptr;
  if (arg2 && typeof arg2 === 'object') arg2 = arg2.ptr;
  if (arg3 && typeof arg3 === 'object') arg3 = arg3.ptr;
  if (arg4 && typeof arg4 === 'object') arg4 = arg4.ptr;
  if (arg5 && typeof arg5 === 'object') arg5 = arg5.ptr;
  if (arg6 && typeof arg6 === 'object') arg6 = arg6.ptr;
  if (arg7 && typeof arg7 === 'object') arg7 = arg7.ptr;
  if (arg8 && typeof arg8 === 'object') arg8 = arg8.ptr;
  if (arg9 && typeof arg9 === 'object') arg9 = arg9.ptr;
  if (arg10 && typeof arg10 === 'object') arg10 = arg10.ptr;
  if (arg11 && typeof arg11 === 'object') arg11 = arg11.ptr;
  _emscripten_bind_NanoVGContext_drawImage_12(self, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);
};;

NanoVGContext.prototype['beginPath'] = NanoVGContext.prototype.beginPath = function() {
  var self = this.ptr;
  _emscripten_bind_NanoVGContext_beginPath_0(self);
};;

NanoVGContext.prototype['closePath'] = NanoVGContext.prototype.closePath = function() {
  var self = this.ptr;
  _emscripten_bind_NanoVGContext_closePath_0(self);
};;

NanoVGContext.prototype['stroke'] = NanoVGContext.prototype.stroke = function() {
  var self = this.ptr;
  _emscripten_bind_NanoVGContext_stroke_0(self);
};;

NanoVGContext.prototype['moveTo'] = NanoVGContext.prototype.moveTo = function(arg0, arg1) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  if (arg1 && typeof arg1 === 'object') arg1 = arg1.ptr;
  _emscripten_bind_NanoVGContext_moveTo_2(self, arg0, arg1);
};;

NanoVGContext.prototype['lineTo'] = NanoVGContext.prototype.lineTo = function(arg0, arg1) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  if (arg1 && typeof arg1 === 'object') arg1 = arg1.ptr;
  _emscripten_bind_NanoVGContext_lineTo_2(self, arg0, arg1);
};;

NanoVGContext.prototype['fillRect'] = NanoVGContext.prototype.fillRect = function(arg0, arg1, arg2, arg3) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  if (arg1 && typeof arg1 === 'object') arg1 = arg1.ptr;
  if (arg2 && typeof arg2 === 'object') arg2 = arg2.ptr;
  if (arg3 && typeof arg3 === 'object') arg3 = arg3.ptr;
  _emscripten_bind_NanoVGContext_fillRect_4(self, arg0, arg1, arg2, arg3);
};;

NanoVGContext.prototype['clearRect'] = NanoVGContext.prototype.clearRect = function(arg0, arg1, arg2, arg3) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  if (arg1 && typeof arg1 === 'object') arg1 = arg1.ptr;
  if (arg2 && typeof arg2 === 'object') arg2 = arg2.ptr;
  if (arg3 && typeof arg3 === 'object') arg3 = arg3.ptr;
  _emscripten_bind_NanoVGContext_clearRect_4(self, arg0, arg1, arg2, arg3);
};;

NanoVGContext.prototype['fill'] = NanoVGContext.prototype.fill = function() {
  var self = this.ptr;
  _emscripten_bind_NanoVGContext_fill_0(self);
};;

NanoVGContext.prototype['fillText'] = NanoVGContext.prototype.fillText = function(arg0, arg1, arg2) {
  var self = this.ptr;
  ensureStringCache.prepare();
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  else arg0 = ensureString(arg0);
  if (arg1 && typeof arg1 === 'object') arg1 = arg1.ptr;
  if (arg2 && typeof arg2 === 'object') arg2 = arg2.ptr;
  _emscripten_bind_NanoVGContext_fillText_3(self, arg0, arg1, arg2);
};;

NanoVGContext.prototype['strokeText'] = NanoVGContext.prototype.strokeText = function(arg0, arg1, arg2) {
  var self = this.ptr;
  ensureStringCache.prepare();
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  else arg0 = ensureString(arg0);
  if (arg1 && typeof arg1 === 'object') arg1 = arg1.ptr;
  if (arg2 && typeof arg2 === 'object') arg2 = arg2.ptr;
  _emscripten_bind_NanoVGContext_strokeText_3(self, arg0, arg1, arg2);
};;

NanoVGContext.prototype['set_globalAlpha'] = NanoVGContext.prototype.set_globalAlpha = function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_NanoVGContext_set_globalAlpha_1(self, arg0);
};;

  NanoVGContext.prototype['get_fillStyle'] = NanoVGContext.prototype.get_fillStyle = function() {
  var self = this.ptr;
  return Pointer_stringify(_emscripten_bind_NanoVGContext_get_fillStyle_0(self));
};
    NanoVGContext.prototype['set_fillStyle'] = NanoVGContext.prototype.set_fillStyle = function(arg0) {
  var self = this.ptr;
  ensureStringCache.prepare();
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  else arg0 = ensureString(arg0);
  _emscripten_bind_NanoVGContext_set_fillStyle_1(self, arg0);
};
  NanoVGContext.prototype['get_textAlign'] = NanoVGContext.prototype.get_textAlign = function() {
  var self = this.ptr;
  return Pointer_stringify(_emscripten_bind_NanoVGContext_get_textAlign_0(self));
};
    NanoVGContext.prototype['set_textAlign'] = NanoVGContext.prototype.set_textAlign = function(arg0) {
  var self = this.ptr;
  ensureStringCache.prepare();
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  else arg0 = ensureString(arg0);
  _emscripten_bind_NanoVGContext_set_textAlign_1(self, arg0);
};
  NanoVGContext.prototype['get_font'] = NanoVGContext.prototype.get_font = function() {
  var self = this.ptr;
  return Pointer_stringify(_emscripten_bind_NanoVGContext_get_font_0(self));
};
    NanoVGContext.prototype['set_font'] = NanoVGContext.prototype.set_font = function(arg0) {
  var self = this.ptr;
  ensureStringCache.prepare();
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  else arg0 = ensureString(arg0);
  _emscripten_bind_NanoVGContext_set_font_1(self, arg0);
};
  NanoVGContext.prototype['get_drawImageMapID'] = NanoVGContext.prototype.get_drawImageMapID = function() {
  var self = this.ptr;
  return _emscripten_bind_NanoVGContext_get_drawImageMapID_0(self);
};
    NanoVGContext.prototype['set_drawImageMapID'] = NanoVGContext.prototype.set_drawImageMapID = function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_NanoVGContext_set_drawImageMapID_1(self, arg0);
};
  NanoVGContext.prototype['__destroy__'] = NanoVGContext.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_NanoVGContext___destroy___0(self);
};
// VoidPtr
function VoidPtr() { throw "cannot construct a VoidPtr, no constructor in IDL" }
VoidPtr.prototype = Object.create(WrapperObject.prototype);
VoidPtr.prototype.constructor = VoidPtr;
VoidPtr.prototype.__class__ = VoidPtr;
VoidPtr.__cache__ = {};
Module['VoidPtr'] = VoidPtr;

  VoidPtr.prototype['__destroy__'] = VoidPtr.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_VoidPtr___destroy___0(self);
};
(function() {
  function setupEnums() {
    
  }
  if (Module['calledRun']) setupEnums();
  else addOnPreMain(setupEnums);
})();
})();
var __instance = null;
exports = {
  get: function(rWidth, rHeight) {
    if (!__instance) {
      __instance = new GLOBAL.Module.NanoVGContext(rWidth, rHeight);
      __instance.canvas = Module.canvas;
    }
    return __instance;
  }
}
