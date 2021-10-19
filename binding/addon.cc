#include "random.h"

namespace {

Napi::Object RegisterModule(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    exports["generate"] = Napi::Function::New(env, generate);
    return exports;
}

}

NODE_API_MODULE(randstring_native, RegisterModule)