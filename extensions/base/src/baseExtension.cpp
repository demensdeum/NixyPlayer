#include <NixyPlayerContext.h>
#include <map>
#include <string>
#include <tiny-js/TinyJS.h>
#include <tiny-js/TinyJS_Functions.h>
#include <tiny-js/TinyJS_MathFunctions.h>
#include <utils.h>

using std::map;
using std::string;

#if defined(_WIN32) || defined(_WIN64)
#define LIB_EXPORT __declspec(dllexport)
#else
#define LIB_EXPORT
#endif

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define INCLUDE_EXTENSION_FUNCTION tinyJSBindings_Include
#define PRINT_EXTENSION_FUNCTION tinyJSBindings_Print

extern "C" LIB_EXPORT void INCLUDE_EXTENSION_FUNCTION(CScriptVar *v, void *icontext) {
    NixyPlayerContext *context = static_cast<NixyPlayerContext *>(icontext);
    string path = v->getParameter("text")->getString();
    string includeScriptString = fileContent(path.c_str(), context->verbose);
    CTinyJS *tinyJS = static_cast<CTinyJS *>(context->tinyJS);
    tinyJS->execute(includeScriptString.c_str());
};

extern "C" LIB_EXPORT void PRINT_EXTENSION_FUNCTION(CScriptVar *v, void *) {
    cout << v->getParameter("text")->getString() << endl;
}

extern "C" LIB_EXPORT map<string, string> registerNixyPlayerExtensions() {
    map<string, string> mapped;
    mapped["function print(text)"] = TOSTRING(PRINT_EXTENSION_FUNCTION);
    mapped["function console.log(text)"] = TOSTRING(PRINT_EXTENSION_FUNCTION);
    mapped["function include(text)"] = TOSTRING(INCLUDE_EXTENSION_FUNCTION);
    return mapped;
}