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

#include "MainWindow.h"

#include <iostream>

MainWindow::MainWindow(
    wxWindow* parent,
    wxWindowID id,
    const wxString& title,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxString& name)
    : wxFrame(parent, id, title, pos, size, style, name)
{
    wxIcon icon(wxT("IDI_ICON1"), wxBITMAP_TYPE_ICO_RESOURCE);
    this->SetIcon(icon);
    app_ = dynamic_cast<App*>(wxApp::GetInstance());
    if (app_ == nullptr)
    {
        wxLogError("App is not available.");
        return;
    }
    Maximize();

    // Create menu bar
    wxMenuBar* menuBar = new wxMenuBar();

    // Create Settings menu
    wxMenu* settingsMenu = new wxMenu();
    wxMenu* lanMenu = new wxMenu;
    settingsMenu->AppendSubMenu(lanMenu, LanguageStrings::Get("language"), "Choose language");
    chnMenuItem_ = lanMenu->AppendCheckItem(wxID_ANY, "中文");
    engMenuItem_ = lanMenu->AppendCheckItem(wxID_ANY, "English");
    if (app_->GetLanguage() == "cn")
    {
        chnMenuItem_->Check(true);
        engMenuItem_->Check(false);
    }
    else
    {
        engMenuItem_->Check(true);
        chnMenuItem_->Check(false);
    }
    wxMenuItem* proxyItem =
        settingsMenu->Append(wxID_ANY, LanguageStrings::Get("proxy_settings"));
    menuBar->Append(settingsMenu, LanguageStrings::Get("settings"));

    wxMenu* aiMenu = new wxMenu();
    wxMenuItem* chooseaiItem = aiMenu->Append(wxID_ANY, LanguageStrings::Get("choose_ai"));
    menuBar->Append(aiMenu, LanguageStrings::Get("ai_management"));

    wxMenu* controlMenu = new wxMenu();
    wxMenuItem* refreshItem = controlMenu->Append(wxID_ANY, LanguageStrings::Get("refresh"), "Refresh");
    wxMenuItem* gobackItem =
        controlMenu->Append(wxID_ANY, LanguageStrings::Get("go_back"), "Go back");
    menuBar->Append(controlMenu, LanguageStrings::Get("control"));

    wxMenu* pkMenu = new wxMenu();
    wxMenuItem* choosepkItem =
        pkMenu->Append(wxID_ANY, LanguageStrings::Get("choose_pk_ai"), "Choose PK AI");
    wxMenuItem* quitpkItem =
        pkMenu->Append(wxID_ANY, LanguageStrings::Get("quit_pk_mode"), "Quit PK Mode");
    menuBar->Append(pkMenu, LanguageStrings::Get("pk_mode"));

    wxMenu* helpMenu = new wxMenu();
    wxMenuItem* aboutItem = helpMenu->Append(wxID_ANY, LanguageStrings::Get("about"));
    menuBar->Append(helpMenu, LanguageStrings::Get("help"));
    // Set the menu bar
    SetMenuBar(menuBar);

    // Bind menu event
    Bind(wxEVT_MENU, &MainWindow::OnProxySettings, this, proxyItem->GetId());
    Bind(wxEVT_MENU, &MainWindow::OnChooseAi, this, chooseaiItem->GetId());
    Bind(wxEVT_MENU, &MainWindow::OnRefresh, this, refreshItem->GetId());
    Bind(wxEVT_MENU, &MainWindow::OnGoBack, this, gobackItem->GetId());
    Bind(wxEVT_MENU, &MainWindow::OnChoosePk, this, choosepkItem->GetId());
    Bind(wxEVT_MENU, &MainWindow::OnQuitPk, this, quitpkItem->GetId());
    Bind(wxEVT_MENU, &MainWindow::OnChooseLanguage, this, chnMenuItem_->GetId());
    Bind(wxEVT_MENU, &MainWindow::OnChooseLanguage, this, engMenuItem_->GetId());
    Bind(
        wxEVT_MENU,
        [this](wxCommandEvent& event) {
            wxMessageBox(
                "AI_At_Once\n\nVersion: 1.0.0\n\nAuthor: "
                "diroverflow\n\nhttps://github.com/diroverflow/AIAtOnce",
                "About",
                wxOK | wxICON_INFORMATION);
        },
        aboutItem->GetId());

    wxPanel* topPanel = new wxPanel(this);
    wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);

    inputField_ = new wxTextCtrl(topPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);

    wxButton* button = new wxButton(topPanel, wxID_ANY, LanguageStrings::Get("ask"));

    topSizer->Add(inputField_, 1, wxEXPAND | wxALL, 5);
    topSizer->Add(button, 0, wxALL, 5);

    topPanel->SetSizer(topSizer);
    topSizer->SetSizeHints(topPanel);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    if (app_->IsPkMode())
    {
        wxLogMessage("PK mode");

        splitter_ = new wxSplitterWindow(
            this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D | wxSP_LIVE_UPDATE);
        wxPanel* leftPanel = new wxPanel(splitter_);
        wxPanel* rightPanel = new wxPanel(splitter_);
        wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);
        wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);
        LoadPkAi(leftPanel, leftSizer, app_->GetPkAi1());
        LoadPkAi(rightPanel, rightSizer, app_->GetPkAi2());
        splitter_->SplitVertically(leftPanel, rightPanel);
        splitter_->SetSashGravity(0.5);
        mainSizer->Add(topPanel, 0, wxEXPAND);
        mainSizer->Add(splitter_, 1, wxEXPAND);
    }
    else
    {
        notebook_ = new wxAuiNotebook(
            this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_DEFAULT_STYLE);
            //wxAUI_NB_TOP | wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS);

        if (!wxWebView::IsBackendAvailable(wxWebViewBackendEdge))
        {
            wxLogError("Edge backend is not available.");
        }

        LoadAi();

        mainSizer->Add(topPanel, 0, wxEXPAND);
        mainSizer->Add(notebook_, 1, wxEXPAND);

        // Bind the notebook page close event
        notebook_->Bind(wxEVT_AUINOTEBOOK_PAGE_CLOSE, &MainWindow::OnNotebookPageClose, this);
    }
    this->SetSizer(mainSizer);

    this->SetAutoLayout(true);
    mainSizer->SetSizeHints(this);

    Bind(wxEVT_BUTTON, &MainWindow::OnSendMessage, this, button->GetId());
}

MainWindow::~MainWindow()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

bool MainWindow::LoadPkAi(wxPanel* panel, wxBoxSizer* sizer, const wxString& aiName)
{
    wxFileConfig* configFile = app_->GetConfig();
    if (configFile == nullptr)
    {
        wxLogError("configFile is not available.");
        return false;
    }
    configFile->SetPath("/AI");
    wxString groupName;
    long groupIndex = 0;
    while (configFile->GetNextGroup(groupName, groupIndex))
    {
        wxLogMessage("Group: %s", groupName);
        configFile->SetPath("/AI/" + groupName);
        wxString configAiName = configFile->Read("name", wxEmptyString);
        if (aiName == configAiName)
        {
            AiProfile* profile = new AiProfile();
            int useProxy = configFile->ReadLong("useproxy", 0);
            profile->rootdir = app_->GetRootDir();
            profile->name = configAiName;
            profile->url = configFile->Read("url", wxEmptyString);
            profile->jsfile = configFile->Read("jsfile", wxEmptyString);
            profile->iconfile = configFile->Read("iconfile", wxEmptyString);
            profile->enable = true;
            wxString keyboard_cmd = configFile->Read("usekeyboard", wxEmptyString);
            if (keyboard_cmd != wxEmptyString)
            {
                profile->usekeyboard = true;
                wxStringTokenizer tokenizer(keyboard_cmd, ",");
                while (tokenizer.HasMoreTokens())
                {
                    profile->keyboard_cmd.push_back(tokenizer.GetNextToken());
                }
            }
            else
            {
                profile->usekeyboard = false;
            }
            profile->page = panel;
            profile->notebook = nullptr;

            if (useProxy == 1)
            {
                profile->proxy = app_->GetProxyHost();
            }
            else
            {
                profile->proxy = wxEmptyString;
            }

            AiObj* aiObj = new AiObj(profile);
            aiObjs_.push_back(aiObj);

            return true;
        }
        configFile->SetPath("/AI");
    }
    
    return false;
}

bool MainWindow::LoadAi()
{
    if (notebook_ == nullptr)
    {
        wxLogError("notebook_ is not available.");
        return false;
    }
    wxFileConfig* configFile = app_->GetConfig();
    if (configFile == nullptr)
    {
        wxLogError("configFile is not available.");
        return false;
    }
    configFile->SetPath("/AI");
    wxString groupName;
    long groupIndex = 0;
    while (configFile->GetNextGroup(groupName, groupIndex))
    {
        wxLogMessage("Group: %s", groupName);

        configFile->SetPath("/AI/" + groupName);
        // only load enabled AI
        int enableAI = configFile->ReadLong("enable", 0);
        if (enableAI == 0)
        {
            configFile->SetPath("/AI");
            continue;
        }
        int defaultDisplay = configFile->ReadLong("default", 0);
        int useProxy = configFile->ReadLong("useproxy", 0);

        AiProfile* profile = new AiProfile();
        profile->rootdir = app_->GetRootDir();
        profile->name = configFile->Read("name", wxEmptyString);
        profile->url = configFile->Read("url", wxEmptyString);
        profile->jsfile = configFile->Read("jsfile", wxEmptyString);
        profile->iconfile = app_->GetRootDir() + wxFileName::GetPathSeparator() +
                            configFile->Read("icon", "icon/default.png");
        profile->enable = (enableAI == 1);
        wxString keyboard_cmd = configFile->Read("usekeyboard", wxEmptyString);
        if (keyboard_cmd != wxEmptyString)
        {
            profile->usekeyboard = true;
            wxStringTokenizer tokenizer(keyboard_cmd, ",");
            while (tokenizer.HasMoreTokens())
            {
                profile->keyboard_cmd.push_back(tokenizer.GetNextToken());
            }
        }
        else
        {
            profile->usekeyboard = false;
        }
        profile->page = nullptr;
        profile->notebook = notebook_;

        if (useProxy == 1)
        {
            profile->proxy = app_->GetProxyHost();
        }
        else
        {
            profile->proxy = wxEmptyString;
        }

        AiObj* aiObj = new AiObj(profile);
        aiObjs_.push_back(aiObj);
        aiObj->AddToNotebook(defaultDisplay == 1);

        configFile->SetPath("/AI");
    }
    return true;
}

void MainWindow::OnChooseLanguage(wxCommandEvent& event)
{
    wxMenuBar* menuBar = GetMenuBar();
    wxMenuItem* item = menuBar->FindItem(event.GetId());
    if (item == nullptr)
    {
        wxLogError("item is not available.");
        return;
    }
    wxString selectedLanguage = item->GetItemLabel();
    
    wxFileConfig* configFile = app_->GetConfig();
    if (configFile == nullptr)
    {
        wxLogError("configFile is not available.");
        return;
    }

    if (selectedLanguage == "中文")
    {
        if (app_->GetLanguage() == "cn")
        {
            return;
        }
        configFile->Write("/Settings/language", "cn");
        LanguageStrings::SetLanguage("cn");
        engMenuItem_->Check(false);
    }
    else if (selectedLanguage == "English")
    {
        if (app_->GetLanguage() == "en")
        {
            return;
        }
        configFile->Write("/Settings/language", "en");
        LanguageStrings::SetLanguage("en");
        chnMenuItem_->Check(false);
    }
    configFile->Flush();

    // Update all menu labels
    UpdateMenuLabels();
}

void MainWindow::UpdateMenuLabels()
{
    wxMenuBar* menuBar = GetMenuBar();
    if (!menuBar) return;
    this->SetTitle(LanguageStrings::Get("title"));
    // Update all menu labels
    menuBar->SetMenuLabel(0, LanguageStrings::Get("settings"));
    menuBar->SetMenuLabel(1, LanguageStrings::Get("ai_management"));
    menuBar->SetMenuLabel(2, LanguageStrings::Get("control"));
    menuBar->SetMenuLabel(3, LanguageStrings::Get("pk_mode"));
    menuBar->SetMenuLabel(4, LanguageStrings::Get("help"));
    wxMenu* settingsMenu = menuBar->GetMenu(0);
    wxMenuItem* languageItem = settingsMenu->FindItemByPosition(0);
    languageItem->SetItemLabel(LanguageStrings::Get("language"));
    wxMenuItem* proxyItem = settingsMenu->FindItemByPosition(1);
    proxyItem->SetItemLabel(LanguageStrings::Get("proxy_settings"));
    wxMenu* aiMenu = menuBar->GetMenu(1);
    wxMenuItem* chooseaiItem = aiMenu->FindItemByPosition(0);
    chooseaiItem->SetItemLabel(LanguageStrings::Get("choose_ai"));
    wxMenu* controlMenu = menuBar->GetMenu(2);
    wxMenuItem* refreshItem = controlMenu->FindItemByPosition(0);
    refreshItem->SetItemLabel(LanguageStrings::Get("refresh"));
    wxMenuItem* gobackItem = controlMenu->FindItemByPosition(1);
    gobackItem->SetItemLabel(LanguageStrings::Get("go_back"));
    wxMenu* pkMenu = menuBar->GetMenu(3);
    wxMenuItem* choosepkItem = pkMenu->FindItemByPosition(0);
    choosepkItem->SetItemLabel(LanguageStrings::Get("choose_pk_ai"));
    wxMenuItem* quitpkItem = pkMenu->FindItemByPosition(1);
    quitpkItem->SetItemLabel(LanguageStrings::Get("quit_pk_mode"));
    wxMenu* helpMenu = menuBar->GetMenu(4);
    wxMenuItem* aboutItem = helpMenu->FindItemByPosition(0);
    aboutItem->SetItemLabel(LanguageStrings::Get("about"));
}

// Add the following method to handle the notebook page close event
void MainWindow::OnNotebookPageClose(wxAuiNotebookEvent& event)
{
    int pageIndex = event.GetSelection();
    wxString pageTitle = notebook_->GetPageText(pageIndex);

    wxMessageDialog dialog(
        nullptr,
        wxString::Format(LanguageStrings::Get("confirm_close_tab"), pageTitle),
        LanguageStrings::Get("confirm"),
        wxYES_NO | wxICON_QUESTION
        );
    dialog.SetYesNoLabels("Yes", "No");
    int response = dialog.ShowModal();
    if (response == wxID_YES)
    {
        for (auto aiObj : aiObjs_)
        {
            if (aiObj->GetName() == pageTitle)
            {
                aiObjs_.erase(std::remove(aiObjs_.begin(), aiObjs_.end(), aiObj), aiObjs_.end());
                break;
            }
        }
        event.Skip(); // Allow the page to close
    }
    else
    {
        event.Veto(); // Prevent the page from closing
    }
}

void MainWindow::OnSendMessage(wxCommandEvent& event)
    {
    wxString text = inputField_->GetValue();
    wxLogMessage("Input text: %s", text);
    if (wxTheClipboard->Open()) {
        wxTheClipboard->SetData(new wxTextDataObject(text));
        wxTheClipboard->Close();
    }
    for(auto aiObj : aiObjs_)
    {
        aiObj->SendMsg(text);
    }
    inputField_->Clear();
}

void MainWindow::OnProxySettings(wxCommandEvent& event)
{
    wxString app_host = app_->GetProxyHost();

    wxDialog* dialog = new wxDialog(
        this, wxID_ANY, LanguageStrings::Get("proxy_settings"), wxDefaultPosition, wxDefaultSize);
    wxBoxSizer* dialogSizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* hostSizer = new wxBoxSizer(wxHORIZONTAL);
    wxTextCtrl* hostCtrl = new wxTextCtrl(dialog, wxID_ANY, app_host);
    hostSizer->Add(hostCtrl, 1, wxALL, 5);

    wxBoxSizer* txtSizer = new wxBoxSizer(wxHORIZONTAL);
    txtSizer->Add(new wxStaticText(dialog, wxID_ANY, LanguageStrings::Get("proxy_example")), 0, wxALL, 5);

    wxBoxSizer* aiSizer = new wxBoxSizer(wxHORIZONTAL);
    wxCheckListBox* aiList = new wxCheckListBox(dialog, wxID_ANY);
    wxFileConfig* configFile = app_->GetConfig();
    if (configFile != nullptr)
    {
        configFile->SetPath("/AI");
        wxString groupName;
        long groupIndex = 0;
        while (configFile->GetNextGroup(groupName, groupIndex))
        {
            configFile->SetPath("/AI/" + groupName);
            wxString aiName = configFile->Read("name", wxEmptyString);
            aiList->Append(aiName);
            int useProxy = configFile->ReadLong("useproxy", 0);
            aiList->Check(aiList->GetCount() - 1, useProxy == 1);
            configFile->SetPath("/AI");
        }
    }
    aiSizer->Add(aiList, 1, wxALL, 5);

    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* okButton = new wxButton(dialog, wxID_OK, "OK");

    App* myApp = app_;
    dialog->Bind(wxEVT_CLOSE_WINDOW, [dialog](wxCloseEvent& event) { dialog->Destroy(); });
    dialog->Bind(wxEVT_BUTTON, [myApp, hostCtrl, aiList](wxCommandEvent& evt) {
        // Save proxy settings
        wxString host = hostCtrl->GetValue();
        // Save proxy settings to App
        if(myApp != nullptr) {
            myApp->SetProxy(host);
        }
        // Set chosen AI to use proxy
        wxFileConfig* configFile = myApp->GetConfig();
        if (configFile != nullptr)
        {
            configFile->SetPath("/AI");
            wxString groupName;
            long groupIndex = 0;
            while (configFile->GetNextGroup(groupName, groupIndex))
            {
                configFile->SetPath("/AI/" + groupName);
                for (int i = 0; i < aiList->GetCount(); i++)
                {
                    wxString aiName = configFile->Read("name", wxEmptyString);
                    if (aiName == aiList->GetString(i))
                    {
                        if (aiList->IsChecked(i))
                        {
                            if (configFile->Write("useproxy", 1) == false)
                            {
                                wxLogError("Write useproxy failed.");
                            }
                        }
                        else
                        {
                            if (configFile->Write("useproxy", 0) == false)
                            {
                                wxLogError("Write useproxy failed.");
                            }
                        }
                        configFile->Flush();
                        break;
                    }
                }
                configFile->SetPath("/AI");
            }
        }
        wxMessageBox(LanguageStrings::Get("restart_apply"));
        evt.Skip(); // Allow dialog to close
    }, wxID_OK);
    buttonSizer->Add(okButton, 0, wxALL, 5);

    dialogSizer->Add(hostSizer, 0, wxEXPAND | wxALL, 5);
    dialogSizer->Add(txtSizer, 0, wxEXPAND | wxALL, 5);
    dialogSizer->Add(aiSizer, 0, wxEXPAND | wxALL, 5);
    dialogSizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxALL, 5);

    dialog->SetSizer(dialogSizer);
    dialogSizer->Fit(dialog);
    dialog->Centre(wxBOTH);
    dialog->ShowModal();
    dialog->Destroy();
}

void MainWindow::OnRefresh(wxCommandEvent& event)
{
    wxWindowList pages;
    if (app_->IsPkMode())
    {
        pages.push_back(splitter_->GetWindow1());
        pages.push_back(splitter_->GetWindow2());
    }
    else
    {
        pages.push_back(notebook_->GetCurrentPage());
    }
    for (auto page : pages)
    {
        wxPanel* customPage = dynamic_cast<wxPanel*>(page);
        if (customPage)
        {
            const wxWindowList& children = customPage->GetChildren();
            for (wxWindow* child : children)
            {
                if (auto webView = dynamic_cast<wxWebViewEdge*>(child))
                {
                    webView->Reload();
                }
            }
        }
    }
}

void MainWindow::OnGoBack(wxCommandEvent& event)
{
    wxWindowList pages;
    if (app_->IsPkMode())
    {
        pages.push_back(splitter_->GetWindow1());
        pages.push_back(splitter_->GetWindow2());
    }
    else
    {
        pages.push_back(notebook_->GetCurrentPage());
    }
    for (auto page : pages)
    {
        wxPanel* customPage = dynamic_cast<wxPanel*>(page);
        if (customPage)
        {
            const wxWindowList& children = customPage->GetChildren();
            for (wxWindow* child : children)
            {
                if (auto webView = dynamic_cast<wxWebViewEdge*>(child))
                {
                    if (webView->CanGoBack())
                        webView->GoBack();
                }
            }
        }
    }
}

void MainWindow::OnChooseAi(wxCommandEvent& event)
{
    wxDialog* dialog = new wxDialog(
        this, wxID_ANY, LanguageStrings::Get("choose_ai"), wxDefaultPosition, wxDefaultSize);
    wxBoxSizer* dialogSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* aiSizer = new wxBoxSizer(wxHORIZONTAL);
    wxCheckListBox* aiList = new wxCheckListBox(dialog, wxID_ANY);
    wxFileConfig* configFile = app_->GetConfig();
    if (configFile != nullptr)
    {
        configFile->SetPath("/AI");
        wxString groupName;
        long groupIndex = 0;
        while (configFile->GetNextGroup(groupName, groupIndex))
        {
            configFile->SetPath("/AI/" + groupName);
            int aiEabled = configFile->ReadLong("enable", 0);
            wxString aiName = configFile->Read("name", wxEmptyString);
            aiList->Append(aiName);
            aiList->Check(aiList->GetCount() - 1, aiEabled == 1);
            configFile->SetPath("/AI");
        }
    }
    aiSizer->Add(aiList, 1, wxALL, 5);
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* okButton = new wxButton(dialog, wxID_OK, "OK");
    dialog->Bind(wxEVT_CLOSE_WINDOW, [dialog](wxCloseEvent& event) { dialog->Destroy(); });
    dialog->Bind(
        wxEVT_BUTTON,
        [this, aiList](wxCommandEvent& evt) {
            wxFileConfig* configFile = app_->GetConfig();
            if (configFile != nullptr)
            {
                configFile->SetPath("/AI");
                wxString groupName;
                long groupIndex = 0;
                while (configFile->GetNextGroup(groupName, groupIndex))
                {
                    wxLogMessage("Group: %s", groupName);
                    configFile->SetPath("/AI/" + groupName);
                    for (int i = 0; i < aiList->GetCount(); i++)
                    {
                        wxString aiName = configFile->Read("name", wxEmptyString);
                        if (aiName == aiList->GetString(i))
                        {
                            if (configFile->Write("enable", aiList->IsChecked(i) ? 1 : 0) == false)
                            {
                                wxLogError("Write enable failed.");
                            }
                            configFile->Flush();
                            break;
                        }
                    }
                    configFile->SetPath("/AI");
                }
            }
            //this->LoadAi();
            ReStartSelf();
            evt.Skip(); // Allow dialog to close
        },
        wxID_OK);
    buttonSizer->Add(okButton, 0, wxALL, 5);
    dialogSizer->Add(aiSizer, 0, wxEXPAND | wxALL, 5);
    dialogSizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxALL, 5);
    dialog->SetSizer(dialogSizer);
    dialogSizer->Fit(dialog);
    dialog->Centre(wxBOTH);
    dialog->ShowModal();
    dialog->Destroy();
}

void MainWindow::ReStartSelf()
{
    TCHAR szPath[MAX_PATH];
    if (!GetModuleFileName(NULL, szPath, MAX_PATH))
    {
        return;
    }

    STARTUPINFO si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb = sizeof(si);

    if (!CreateProcess(szPath, GetCommandLine(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        return;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    ::ExitProcess(0);
}

void MainWindow::OnChoosePk(wxCommandEvent& event)
{
    wxDialog* dialog = new wxDialog(
        this, wxID_ANY, LanguageStrings::Get("choose_pk_ai"), wxDefaultPosition, wxDefaultSize);
    wxBoxSizer* dialogSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* aiSizer = new wxBoxSizer(wxHORIZONTAL);
    wxCheckListBox* aiList = new wxCheckListBox(dialog, wxID_ANY);
    wxFileConfig* configFile = app_->GetConfig();
    if (configFile != nullptr)
    {
        configFile->SetPath("/AI");
        wxString groupName;
        long groupIndex = 0;
        while (configFile->GetNextGroup(groupName, groupIndex))
        {
            configFile->SetPath("/AI/" + groupName);
            wxString aiName = configFile->Read("name", wxEmptyString);
            aiList->Append(aiName);
            configFile->SetPath("/AI");
        }
    }
    aiSizer->Add(aiList, 1, wxALL, 5);
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* okButton = new wxButton(dialog, wxID_OK, "OK");
    dialog->Bind(wxEVT_CLOSE_WINDOW, [dialog](wxCloseEvent& event) { dialog->Destroy(); });
    dialog->Bind(wxEVT_BUTTON, [this, aiList](wxCommandEvent& evt) {
        wxVector<wxString> selectedAi;
        for (int i = 0; i < aiList->GetCount(); i++)
        {
            if (aiList->IsChecked(i))
            {
                selectedAi.push_back(aiList->GetString(i));
            }
        }
        if (selectedAi.size() != 2)
        {
            wxMessageBox(LanguageStrings::Get("max_two_ai"),
                LanguageStrings::Get("notice"),
                wxOK | wxICON_INFORMATION);
            return;
        }
        wxFileConfig* configFile = this->app_->GetConfig();
        if (configFile != nullptr)
        {
            configFile->Write("/PK/pkmode", 1);
            configFile->Write("/PK/ai1", selectedAi[0]);
            configFile->Write("/PK/ai2", selectedAi[1]);
            configFile->Flush();
            ReStartSelf();
        }
        evt.Skip(); // Allow dialog to close
        });
    aiList->Bind(wxEVT_CHECKLISTBOX, [aiList](wxCommandEvent& evt) {
        int checkedCount = 0;
        for (size_t i = 0; i < aiList->GetCount(); ++i)
        {
            if (aiList->IsChecked(i))
            {
                ++checkedCount;
            }
        }
        if (checkedCount > 2)
        {
            wxMessageBox(
                LanguageStrings::Get("max_two_ai"),
                LanguageStrings::Get("notice"),
                wxOK | wxICON_INFORMATION);
            aiList->Check(evt.GetInt(), false);
        }
    });
    buttonSizer->Add(okButton, 0, wxALL, 5);
    dialogSizer->Add(aiSizer, 0, wxEXPAND | wxALL, 5);
    dialogSizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxALL, 5);
    dialog->SetSizer(dialogSizer);
    dialogSizer->Fit(dialog);
    dialog->Centre(wxBOTH);
    dialog->ShowModal();
    dialog->Destroy();
}

void MainWindow::OnQuitPk(wxCommandEvent& event)
{
    if (app_->IsPkMode() == false)
    {
        wxMessageBox(
            LanguageStrings::Get("not_pk"), LanguageStrings::Get("notice"), wxOK | wxICON_INFORMATION);
        return;
    }
    wxFileConfig* configFile = app_->GetConfig();
    if (configFile != nullptr)
    {
        configFile->Write("/PK/pkmode", 0);
        configFile->Flush();
        ReStartSelf();
    }
}