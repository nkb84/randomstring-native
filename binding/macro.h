#ifndef _MACRO_H_
#define _MACRO_H_

#define DEFAULT_CHARACTER_SET "0123456789abcdefghijklmnopqrstuvxyz"
#define OPTION_LENGTH   "length"
#define OPTION_COUNT    "count"
#define OPTION_CHARACTER_SET   "charset"
#define OPTION_METHOD   "method"
#define OPTION_EXCLUDES  "excludes"

#define METHOD_RANDOM   "random"
#define METHOD_CRYPTO   "crypto"
#define METHOD_MT19937  "mt19937"

#define RESULT_OK               0
#define RESULT_WRONG_ARGUMENT   -1
#define RESULT_CANNOT_GENERATE  -2

#define OPTIONAL_MEMBER_STRING(var, name, obj)          \
    if (obj.Has(name) && obj.Get(name).IsString()) {    \
        var = obj.Get(name).As<Napi::String>().Utf8Value(); \
    }

#define OPTIONAL_MEMBER_NUMBER(var, name, obj)          \
    if (obj.Has(name) && obj.Get(name).IsNumber()) {    \
        var = obj.Get(name).As<Napi::Number>().Int64Value(); \
    }

#define OPTIONAL_MEMBER_ARRAY(var, name, op, obj)          \
    if (obj.Has(name) && obj.Get(name).IsArray()) {    \
        const Napi::Array _tmp_##name = obj.Get(name).As<Napi::Array>();  \
        for (uint32_t _tmp_i_##name = 0; _tmp_i_##name < _tmp_##name.Length(); _tmp_i_##name++) { \
            var.op(_tmp_##name[_tmp_i_##name].As<Napi::String>().Utf8Value());  \
        } \
    }

#endif