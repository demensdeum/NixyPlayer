#include <tiny-js/TinyJS.h>
#include <tiny-js/TinyJS_Functions.h>
#include <tiny-js/TinyJS_MathFunctions.h>

#include <iostream>
#include <fstream>
#include "constants.h"

using std::println;
using std::string;
using std::cout;
using std::endl;

struct NixyPlayerContext {
    CTinyJS *tinyJS;
    bool verbose;
};


string fileContent(const char *path, bool verbose) {
        std::ifstream file(path);

        if (!file.is_open()) {
            if (verbose) {
                cout << "Failed to open the file: " << path << endl;
            }
            exit(3);
        }

        string startFileContent;
        string line;

        while (std::getline(file, line)) {
            startFileContent += line;
            startFileContent += '\n';
        }

        file.close();

        return startFileContent;
}

void tinyJSBindingsToFlameSteelEngineGameToolkit_Include(CScriptVar *v, void *icontext) {
    NixyPlayerContext *context = static_cast<NixyPlayerContext *>(icontext);
    string path = v->getParameter("text")->getString();
    string includeScriptString = fileContent(path.c_str(), context->verbose);
    CTinyJS *tinyJS = static_cast<CTinyJS *>(context->tinyJS);
    tinyJS->execute(includeScriptString.c_str());
};

void tinyJSBindings_Print(CScriptVar *v, void *) {
    cout << v->getParameter("text")->getString() << endl;
}

int main(int argc, char **argv) {
    bool verbose = false;

    for (int i = 0; i < argc; ++i) {
        if (string(argv[i]) == "--verbose" || string(argv[i]) == "-v") {
            verbose = true;
        }
    }

    if (verbose) {
        cout << "Welcome to NixyPlayer (" << NixyPlayerVersion << ")" << endl;
        cout << "Verbose mode enabled" << endl;
        for (int i = 0; i < argc; ++i) {
            cout << "argv " << i << " - " << argv[i] << endl;
        }
    }
    if (argc < 2) {
        if (verbose) {
            cout << "No start JavaScript file provided as first argument. Error 1" << endl;
        }
        exit(1);
    }
    else {
        string startFileContent = fileContent(argv[1], verbose);

        CTinyJS *tinyJS = new CTinyJS();

        tinyJS->addNative("function print(text)", &tinyJSBindings_Print, 0);
        tinyJS->addNative("function include(text)", &tinyJSBindingsToFlameSteelEngineGameToolkit_Include, tinyJS);
        registerFunctions(tinyJS);
        registerMathFunctions(tinyJS);

        try {
            tinyJS->execute(startFileContent);
        }
        catch (CScriptException *error) {
            if (verbose) {
                cout << "---[SCRIPT ERROR START]---" << endl;
                cout << "Tiny-JS error: " << error->text << endl;
                cout << "---[SCRIPT ERROR END]---" << endl;
                cout << "Error 2" << endl;
            }
            exit(2);
        }
    }
    return 0;
}