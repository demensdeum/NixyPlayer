#include <NixyPlayerContext.h>
#include <dylib/dylib.hpp>
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

    NixyPlayerContext context{tinyJS, verbose};

    std::stringstream ss(extensions);  
    string extension;

    vector<dylib *> libs;

    while (ss >> extension) {
        dylib* lib = new dylib(extension, dylib::no_filename_decorations);
        lib->verbose = verbose;
        libs.push_back(lib);
        auto jsExtensionsMapPointer = lib->get_function<map<string, string>*()>("registerNixyPlayerExtensions");
        auto jsExtensionsMap = jsExtensionsMapPointer();

        for (auto jsExtension : *jsExtensionsMap) {
            if (verbose) {
                cout << "jsExtension: " << jsExtension.first << endl;
            }
            const char *jsExtensionFunctionName = jsExtension.second.c_str();
                if (lib->has_symbol(jsExtensionFunctionName)) {
                    if (verbose) {
                        cout << "Library: " << extension << " has " << jsExtensionFunctionName << endl;
                    }
                }
                else {
                    cout << "Library: " << extension << " does not have " << jsExtensionFunctionName << ". Error 5" << endl;
                    exit(5);
                }
            JSCallback jsExtensionFunctionPointer = reinterpret_cast<JSCallback>(lib->get_function<void>(jsExtensionFunctionName));
            tinyJS->addNative(jsExtension.first,
                              jsExtensionFunctionPointer,
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

    for (auto lib : libs) {
        delete(lib);
    }

    return 0;
}