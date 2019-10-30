#include <node.h>
#include <v8.h>
#include <cstring>
#include "UVCLinux.h"

typedef void *PVOID;
BOOL bIsOurDevice;
PVOID pDevice;

v8::Local<v8::Object> getInputSignal(const v8::FunctionCallbackInfo<v8::Value> &args, v8::Isolate *isolate)
{
  char *devicePath = "/dev/hidraw0";
  if (args.Length() > 0 && args[0]->IsString())
  {
    v8::String::Utf8Value inputDevPath(args[0]->ToString());
    char *c_inputDevPath = *inputDevPath;
    if (strlen(c_inputDevPath) > 0)
    {
      char c_inputDevPathNew[256] = "";
      strcpy(c_inputDevPathNew, c_inputDevPath);
      devicePath = c_inputDevPathNew;
    }
  }

  int openDevice = UVC_OPEN_DEVICE_EX(devicePath, &bIsOurDevice, &pDevice);

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
    result->Set(v8::String::NewFromUtf8(isolate, "hdcp"), v8::Boolean::New(isolate, bIsHDCP));
    result->Set(v8::String::NewFromUtf8(isolate, "hdmi"), v8::Boolean::New(isolate, bIsHDMI));
  }

  return result;
}

void GetInputSignal(const v8::FunctionCallbackInfo<v8::Value> &args)
{
  v8::Isolate *isolate = args.GetIsolate();
  v8::HandleScope scope(isolate);
  args.GetReturnValue().Set(getInputSignal(args, isolate));
}

void init(v8::Local<v8::Object> exports)
{
  NODE_SET_METHOD(exports, "getInputSignal", GetInputSignal);
}

NODE_MODULE(binding, init)
