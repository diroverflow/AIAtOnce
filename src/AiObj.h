/*
 * Copyright (C) 2025 https://github.com/diroverflow
 *
 * This is free software. You can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 3 as published by the Free Software Foundation.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY.
 */
#pragma once 

#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <wx/panel.h>
#include <wx/file.h>
#include <wx/webview.h>
#include <wx/msw/webview_edge.h>
#include <webview2EnvironmentOptions.h>
#include <wx/uiaction.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>

typedef struct
{
    wxString rootdir;
    wxAuiNotebook* notebook;
    wxPanel* page;
    wxString name;
    wxString url;
    wxString jsfile;
    wxString proxy;
    wxString iconfile;
    bool enable;
    bool usekeyboard;
    wxVector<wxString> keyboard_cmd;
} AiProfile;

class AiObj
{
public:
   AiObj(AiProfile *profile);
   ~AiObj();

   void AddToNotebook(bool active = false);
   void DeleteFromNotebook();
   void SendMsg(const wxString& message);
   wxString GetName() { return name_; }
   bool IsEnabled() { return enable_; }
   void SetEnable(bool enable) { enable_ = enable; }
   wxWebView *GetWebView() { return webview_; }

private:
    wxString name_;
    wxString url_;
    wxString jsfile_;
    wxString jsContent_;
    wxWebViewEdge* webview_;
    wxPanel* page_;
    wxAuiNotebook* notebook_;
    wxBitmap* icon_;
    bool isReady_ = false;
    bool useKeyboard_ = false;
    bool enable_ = false;
    wxVector<wxString> keyboard_cmd_;
    wxString root_dir_;

private:
    void OnWebViewLoaded(wxWebViewEvent& event);
    wxString GetJsFileContent();
    bool RunKeyboardCmd();
    void FocusInput();
};
