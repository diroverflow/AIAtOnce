#pragma once
#include <map>
#include <wx/string.h>

class LanguageStrings {
public:
    static wxString Get(const wxString& key) {
        if (currentLanguage == "cn") {
            return cnStrings[key];
        }
        return enStrings[key];
    }

    static void SetLanguage(const wxString& lang) {
        currentLanguage = lang;
    }

private:
    static wxString currentLanguage;
    static std::map<wxString, wxString> cnStrings;
    static std::map<wxString, wxString> enStrings;
};