#include <tiny-js/TinyJS.h>
#include <tiny-js/TinyJS_Functions.h>
#include <tiny-js/TinyJS_MathFunctions.h>

#include <iostream>
#include <fstream>
#include <print>

using std::println;
using std::string;

string fileContent(const char *path) {
        std::ifstream file(path);

        if (!file.is_open()) {
            println("Failed to open the file: {0}", path);
            exit(1);
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

void tinyJSBindingsToFlameSteelEngineGameToolkit_Include(CScriptVar *v, void *pointerToTinyJS) {
    auto path = v->getParameter("text")->getString();
    auto includeScriptString = fileContent(path.c_str());
    CTinyJS *tinyJS = static_cast<CTinyJS *>(pointerToTinyJS);
    tinyJS->execute(includeScriptString.c_str());
};

void tinyJSBindings_Print(CScriptVar *v, void *) {
    println("{0}", v->getParameter("text")->getString());
}

int main(int argc, char **argv) {
    println("Welcome to NixyPlayer!");
    for (int i = 0; i < argc; ++i) {
        println("argv[{0}] = {1}", i, string(argv[i]));
    }    
    if (argc < 2) {
        println("No start JavaScript file provided as first argument. Error 1");
        exit(1);
    }
    else {
        string startFileContent = fileContent(argv[1]);

        auto tinyJS = new CTinyJS();

        tinyJS->addNative("function print(text)", &tinyJSBindings_Print, 0);
        tinyJS->addNative("function include(text)", &tinyJSBindingsToFlameSteelEngineGameToolkit_Include, tinyJS);
        registerFunctions(tinyJS);
        registerMathFunctions(tinyJS);

        try {
            tinyJS->execute(startFileContent);
        }
        catch (CScriptException *error) {
            println("---[SCRIPT ERROR START]---");
            println("Tiny-JS error: {0}", error->text);
            println("---[SCRIPT ERROR END]---");
            println("Error 2");
            exit(1);
        }

    }
    return 0;
}