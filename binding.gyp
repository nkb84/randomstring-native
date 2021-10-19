{
    "targets": [
        {
            "target_name": "randomstring-native",
            "cflags!": [ "-fno-exceptions" ],
            "cflags_cc!": [ "-fno-exceptions" ],
            "xcode_settings": {
                "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
                "CLANG_CXX_LIBRARY": "libc++",
                "MACOSX_DEPLOYMENT_TARGET": "10.7",
            },
            "msvs_settings": {
                "VCCLCompilerTool": { "ExceptionHandling": 1 },
            },
            "sources": [
                "binding/addon.cc",
                "binding/random.cc"
            ],
            "includes": [

            ],
            "libraries": [
                "-lcrypto"
            ],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")",
                "include"
            ],
            "defines": [ "NAPI_VERSION=<(napi_build_version)", "NAPI_DISABLE_CPP_EXCEPTIONS=1" ]
        }
    ]
}