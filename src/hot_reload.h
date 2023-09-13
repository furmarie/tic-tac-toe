#pragma once

#include "ttt_base.h"

#ifdef HOT_RELOAD
    #include <dlfcn.h>
    static const char* libplug_file_name = "libplug.so";
    static void* libplug = nullptr;
    bool reload_libplug(GameBase* ptr) {
        if(libplug != nullptr) {
            printf("here1\n");
            dlclose(libplug);
        }
        printf("here\n");

        libplug = dlopen(libplug_file_name, RTLD_NOW);
        if(libplug == nullptr) {
            TraceLog(LOG_ERROR, "HOTRELOAD: could not load %s: %s", libplug_file_name, dlerror());
            return false;
        }

        const char* symbol = "factory";
        void* factory = dlsym(libplug, symbol);

        printf("here2\n");
        if(factory == nullptr) {
            TraceLog(LOG_ERROR, "HOTRELOAD: could not find %s symbol in %s: %s",  libplug_file_name, dlerror());
            return false;
        }

        Base_creator_t creator = reinterpret_cast<Base_creator_t>(factory);
        
        // typedef GameBase* (*fptr)();
        // fptr func = reinterpret_cast<fptr>(reinterpret_cast<void*>(factory));

        ptr = creator();
        printf("HOT RELOADED!!\n");
        if(ptr == nullptr) {
            return false;
        }
        return true;
    }
#else
    #define reload_libplug(...) true
#endif