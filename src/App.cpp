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

#include "App.h"

#include <iostream>

#include "LanguageStrings.h"
#include "MainWindow.h"
#include "Utils.h"

namespace {
constexpr int kTestModeAutoCloseTimeout_s = 3;
}

App::App()
    : pk_mode_{false}
    , config_{nullptr}
    , pk_ai1_{wxEmptyString}
    , pk_ai2_{wxEmptyString}
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

bool App::OnInit()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    bool ret = false;

    if (wxApp::OnInit())
    {
        wxImage::AddHandler(new wxPNGHandler);
        wxLog* logger = new wxLogStderr(fopen("aao.log", "w"));
        wxLog::SetActiveTarget(logger);

        wxString exePath = wxStandardPaths::Get().GetExecutablePath();
        wxFileName exeFileName(exePath);
        root_dir_ = exeFileName.GetPath();
        config_path_ = root_dir_ + wxFileName::GetPathSeparator() + "aaoconfig.ini";
        // read config
        config_ = new wxFileConfig("AIATONCE", "", config_path_);
        proxy_host_ = config_->Read("/Settings/proxy", "");
        language_ = config_->Read("/Settings/language", "cn");
        pk_mode_ = (config_->ReadLong("/PK/pkmode", 0)==1?true:false);
        pk_ai1_ = config_->Read("/PK/ai1", "");
        pk_ai2_ = config_->Read("/PK/ai2", "");

        LanguageStrings::SetLanguage(language_);
        // acording to wxWidjets spec window is suicide object, and we mast not use delete operator
        MainWindow* m = new MainWindow(nullptr, wxID_ANY, LanguageStrings::Get("title"));
        m->Show();
        m->Raise();

        ret = true;
    }

    return ret;
}

int App::OnExit()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    return wxApp::OnExit();
}

App::~App()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void App::OnInitCmdLine(wxCmdLineParser& parser)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    std::string pk_desc =
        Sprintf("PK mode: /pk AI1,AI2");

    const wxCmdLineEntryDesc cmds[] = {
        {wxCMD_LINE_SWITCH, "h", "help", "Display this help text", wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP},
        {wxCMD_LINE_SWITCH, "pk", "pk", pk_desc.c_str(), wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL},

        wxCMD_LINE_DESC_END};

    parser.SetDesc(cmds);
}

bool App::OnCmdLineParsed(wxCmdLineParser& parser)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;

    pk_mode_ = parser.Found("pk");

    wxString pkParam;
    if (parser.Found("pk", &pkParam))
    {
        wxArrayString tokens = wxSplit(pkParam, ',');
        if (tokens.size() == 2)
        {
            pk_ai1_ = tokens[0];
            pk_ai2_ = tokens[1];
        }
    }

    return true;
}

bool App::SetProxy(const wxString& host)
{
    bool ret = false;
    proxy_host_ = host;
    //wxFileConfig* config = new wxFileConfig("AIATONCE", "", config_path_);
    if (config_ != nullptr)
    {
        config_->Write("/Settings/proxy", host);
        ret = config_->Flush();
    }
    //delete config;

    return ret;
}

wxString App::GetLanguage()
{
    language_ = config_->Read("/Settings/language", "cn");
    return language_;
}