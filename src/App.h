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

#include <memory>

#include <wx/app.h>
#include <wx/timer.h>
#include <wx/cmdline.h>
#include <wx/fileconf.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>


class App : public wxApp
{
public:
    App();
    ~App();

    bool OnInit() override;
    int OnExit() override;
    void OnInitCmdLine(wxCmdLineParser& parser) override;
    bool OnCmdLineParsed(wxCmdLineParser& parser) override;

    void OnTestModeTimer(wxTimerEvent& event);
    wxString GetProxyHost() const { return proxy_host_; }
    bool SetProxy(const wxString& host);
    wxFileConfig* GetConfig() { return config_; }
    wxString GetLanguage();
    wxString GetRootDir() const { return root_dir_; }
    bool IsPkMode() const { return pk_mode_; }
    wxString GetPkAi1() const { return pk_ai1_; }
    wxString GetPkAi2() const { return pk_ai2_; }

private:
    wxString language_;
    wxString root_dir_;
    wxString config_path_;
    wxString proxy_host_;
    wxString js_path_;
    wxFileConfig* config_;
    bool pk_mode_;
    wxString pk_ai1_;
    wxString pk_ai2_;
};
