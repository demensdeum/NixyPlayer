#include <NixyPlayerContext.h>
#include <dlfcn.h>
#include <tiny-js/TinyJS.h>
#include <tiny-js/TinyJS_Functions.h>
#include <tiny-js/TinyJS_MathFunctions.h>
#include <utils.h>

#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <ranges>
#include <sstream>

#include "constants.h"

using std::cout;
using std::endl;
using std::map;
using std::println;
using std::string;
using std::vector;

int main(int argc, char **argv) {
    bool verbose = false;

    for (int i = 0; i < argc; ++i) {
        if (string(argv[i]) == "--verbose" || string(argv[i]) == "-v") {
            verbose = true;
        }
    }

    if (verbose) {
        cout << "Welcome to Nixy Player (" << NixyPlayerVersion << ")" << endl;
        cout << "Verbose mode enabled" << endl;
        for (int i = 0; i < argc; ++i) {
            cout << "argv " << i << " - " << argv[i] << endl;
        }
    }
    if (argc < 2) {
        cout << "No start JavaScript file provided as first argument. Error 1"
             << endl;
        exit(1);
    }

    string startFileContent = fileContent(argv[1], verbose);

    CTinyJS *tinyJS = new CTinyJS();

    registerFunctions(tinyJS);
    registerMathFunctions(tinyJS);

    auto extensions = fileContent("extensions.nixyp", verbose);

    NixyPlayerContext context{tinyJS = tinyJS, verbose = verbose};

    std::stringstream ss(extensions);  
    string extension;
    while (ss >> extension) {
        auto extensionHandle = dlopen(extension.c_str(), RTLD_LAZY);

        if (!extensionHandle) {
            const char *errorString = dlerror();
            cout << errorString << endl;
            cout << "Error 4" << endl;
            exit(4);
        }

        typedef map<string, void *> (*JSExtensionFunctionPointer)();

        auto functionPointer = JSExtensionFunctionPointer();
        functionPointer = (JSExtensionFunctionPointer)dlsym(
            extensionHandle, "registerNixyPlayerExtensions");
        auto dlsymError = dlerror();

        if (dlsymError) {
            const char *errorString = dlerror();
            cout << errorString << endl;
            cout << "Error 5" << endl;
            exit(5);
        }

        auto jsExtensionsMap = functionPointer();

        for (auto jsExtension : jsExtensionsMap) {
            if (verbose) {
                cout << "jsExtension: " << jsExtension.first << endl;
            }
            tinyJS->addNative(jsExtension.first,
                              reinterpret_cast<JSCallback>(jsExtension.second),
                              &context);
        }
    }

    try {
        tinyJS->execute(startFileContent);
    } catch (CScriptException *error) {
        cout << "---[SCRIPT ERROR START]---" << endl;
        cout << "Tiny-JS error: " << error->text << endl;
        cout << "---[SCRIPT ERROR END]---" << endl;
        cout << "Error 2" << endl;
        exit(2);
    }
    return 0;
}