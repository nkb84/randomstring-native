#include "random.h"
#include "macro.h"

#include <random>
#include <openssl/rand.h>

#include <math.h>
#include <set>

static int doGenerateUIDsByRand(
                    std::set<std::string>& uidSet,
                    std::string& characterSet,
                    int uniqueIdLength, size_t numberOfID,
                    std::set<std::string> existUids) {
    srand (time(NULL));
    std::vector<char> id(uniqueIdLength);

    do {
        for (int i = 0; i < uniqueIdLength; i++) {
            id[i] = characterSet[rand() % characterSet.length()];
        }
        if (existUids.find(&id[0]) == existUids.end()) {
            uidSet.insert(&id[0]);
        }
    } while (uidSet.size() != numberOfID);

    return 0;
}

static int doGenerateUIDsByMT19937(
                    std::set<std::string>& uidSet,
                    std::string& characterSet,
                    int uniqueIdLength, size_t numberOfID,
                    std::set<std::string> existUids) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::string str = characterSet;
    std::string id;

    do {
        std::shuffle(str.begin(), str.end(), generator);
        id = str.substr(0, uniqueIdLength);
        if (existUids.find(id) == existUids.end()) {
            uidSet.insert(id);
        }        
    } while (uidSet.size() != numberOfID);

    return 0;
}

static int doGenerateUIDsByOpenSSL(
                    std::set<std::string>& uidSet,
                    std::string& characterSet,
                    int uniqueIdLength, size_t numberOfID,
                    std::set<std::string> existUids) {
    int i = 0;
    int randomLength = 0;
    std::vector<char> uids;
    std::vector<char> id(uniqueIdLength);
    unsigned char* bytes;
    do {
        if (i >= randomLength) {
            i = 0;
            // Generate more 20% each time
            randomLength = floor(uniqueIdLength*(numberOfID - uidSet.size())*1.2);
            uids.resize(randomLength);

            if (RAND_bytes((unsigned char *)&uids[0], randomLength) != 1) {
                // Generation false
                return -2;
            }
        }

        bytes = (unsigned char*)&uids[i+=uniqueIdLength];
        for (int j = 0; j < uniqueIdLength; j++) {
            id[j] = characterSet[bytes[j] % characterSet.length()];
        }
        if (existUids.find(&id[0]) == existUids.end()) {
            uidSet.insert(&id[0]);
        }
    } while (uidSet.size() != numberOfID);

    return 0;
}

Napi::Value generate(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    if (info.Length() != 0 && (info.Length() != 1 || !info[0].IsObject())) {
        Napi::TypeError::New(env, "Wrong argument").ThrowAsJavaScriptException();
        return env.Null();
    }

    // Default value
    int64_t numberOfID = 1;
    int64_t uniqueIdLength = 10;
    std::string method = METHOD_CRYPTO;
    std::string characterSet = DEFAULT_CHARACTER_SET;
    std::set<std::string> existUidSet;

    if (info.Length() == 1) {
        Napi::Object obj = info[0].As<Napi::Object>();
        OPTIONAL_MEMBER_STRING(characterSet,    OPTION_CHARACTER_SET,   obj)
        OPTIONAL_MEMBER_STRING(method,          OPTION_METHOD,          obj)
        OPTIONAL_MEMBER_NUMBER(uniqueIdLength,  OPTION_LENGTH,          obj)
        OPTIONAL_MEMBER_NUMBER(numberOfID,      OPTION_COUNT,           obj)
        OPTIONAL_MEMBER_ARRAY (existUidSet,     OPTION_EXCLUDES, insert,obj)
    }

    // Validate parameter
    if (!characterSet.length()
        || !uniqueIdLength
        || !numberOfID
        || uniqueIdLength < 1
        || numberOfID < 1
        ) {
        Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
        return env.Null();
    }

    // Generate random string
    int ret;
    std::set<std::string> uidSet;
    if (method == METHOD_RANDOM) {
        ret = doGenerateUIDsByRand(uidSet, characterSet, uniqueIdLength, numberOfID, existUidSet);
    } else if (method == METHOD_CRYPTO) {
        ret = doGenerateUIDsByOpenSSL(uidSet, characterSet, uniqueIdLength, numberOfID, existUidSet);
    } else if (method == METHOD_MT19937) {
        ret = doGenerateUIDsByMT19937(uidSet, characterSet, uniqueIdLength, numberOfID, existUidSet);
    } else {
        Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (ret != 0) {
        Napi::TypeError::New(env, "Cannot generate the random number").ThrowAsJavaScriptException();
        return env.Null();
    }

    // Create js array object
    uint32_t idx = 0;
    Napi::Array array = Napi::Array::New(env, numberOfID);
    for (auto& uid : uidSet) {
        if (idx >= numberOfID) {
            break;
        }
        array[idx++] = Napi::String::New(env, uid);
    }

    return array;
}