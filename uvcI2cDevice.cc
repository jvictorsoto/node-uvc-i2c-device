#include <node.h>
#include <v8.h>
#include "UVCLinux.h"

typedef void *PVOID;
BOOL bIsOurDevice;
PVOID pDevice;

v8::Local<v8::Object> getInputSignal(v8::Isolate *isolate)
{
  int openDevice = UVC_OPEN_DEVICE_EX("0x1164057d", &bIsOurDevice, &pDevice);

  ULONG nWidth, nHeight, nFPS;
  BOOL bIs_1000_1001, bIsInterleaved, bIsSignalLocked, bIsHDCP, bIsHDMI;

  UVC_GET_VIDEO_FORMAT_POLLING_READ(pDevice, &nWidth, &nHeight, &nFPS, &bIs_1000_1001, &bIsInterleaved, &bIsSignalLocked, &bIsHDCP, &bIsHDMI);
  UVC_CLOSE_DEVICE(pDevice);

  v8::Local<v8::Object> result = v8::Object::New(isolate);
  result->Set(v8::String::NewFromUtf8(isolate, "detected"), v8::Boolean::New(isolate, bIsSignalLocked));

  if (bIsSignalLocked)
  {
    result->Set(v8::String::NewFromUtf8(isolate, "resX"), v8::Integer::New(isolate, nWidth));
    result->Set(v8::String::NewFromUtf8(isolate, "resY"), v8::Integer::New(isolate, nHeight));
    result->Set(v8::String::NewFromUtf8(isolate, "fps"), v8::Integer::New(isolate, nFPS));
    result->Set(v8::String::NewFromUtf8(isolate, "interleave"), v8::Boolean::New(isolate, bIsInterleaved));
  }

  return result;
}

void GetInputSignal(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  v8::Isolate *isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);
  args.GetReturnValue().Set(getInputSignal(isolate));
}

void init(v8::Local<v8::Object> exports)
{
  NODE_SET_METHOD(exports, "getInputSignal", GetInputSignal);
}

NODE_MODULE(binding, init)
