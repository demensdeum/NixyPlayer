#include <NixyPlayerContext.h>
#include <map>
#include <string>
#include <tiny-js/TinyJS.h>
#include <tiny-js/TinyJS_Functions.h>
#include <tiny-js/TinyJS_MathFunctions.h>
#include <utils.h>

using std::map;
using std::string;

void tinyJSBindings_Include(CScriptVar *v, void *icontext) {
    NixyPlayerContext *context = static_cast<NixyPlayerContext *>(icontext);
    string path = v->getParameter("text")->getString();
    string includeScriptString = fileContent(path.c_str(), context->verbose);
    CTinyJS *tinyJS = static_cast<CTinyJS *>(context->tinyJS);
    tinyJS->execute(includeScriptString.c_str());
};

void tinyJSBindings_Print(CScriptVar *v, void *) {
    cout << v->getParameter("text")->getString() << endl;
}

extern "C" map<string, JSCallback> registerNixyPlayerExtensions() {
    map<string, JSCallback> mapped;
    mapped["function print(text)"] = tinyJSBindings_Print;
    mapped["function include(text)"] = tinyJSBindings_Include;
    return mapped;
}