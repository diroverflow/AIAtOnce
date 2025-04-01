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

#include "App.h"
#include "AiObj.h"
#include "LanguageStrings.h"

#include <wx/wx.h>
#include <wx/tokenzr.h>
#include <wx/aui/aui.h>
#include <wx/webview.h>
#include <wx/clipbrd.h>
#include <wx/checklst.h>
#include <wx/splitter.h>
#include <wx/msgdlg.h>

class MainWindow : public wxFrame
{
public:
    MainWindow(
        wxWindow* parent,
        wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE,
        const wxString& name = wxFrameNameStr);
    ~MainWindow();

private:
    wxTextCtrl* inputField_;
    wxAuiNotebook* notebook_;
    wxSplitterWindow* splitter_;
    App* app_;
    std::vector<AiObj*> aiObjs_;
    wxMenuItem* chnMenuItem_;
    wxMenuItem* engMenuItem_;

private:
    App* GetApp() { return app_; }
    void UpdateMenuLabels();
    bool LoadAi();
    void ReStartSelf();
    bool LoadPkAi(wxPanel* panel, wxBoxSizer* sizer, const wxString& aiName);
    void OnSendMessage(wxCommandEvent& event);
    void OnProxySettings(wxCommandEvent& event);
    void OnChooseAi(wxCommandEvent& event);
    void OnRefresh(wxCommandEvent& event);
    void OnGoBack(wxCommandEvent& event);
    void OnChoosePk(wxCommandEvent& event);
    void OnQuitPk(wxCommandEvent& event);
    void OnNotebookPageClose(wxAuiNotebookEvent& event);
    void OnChooseLanguage(wxCommandEvent& event);
};
