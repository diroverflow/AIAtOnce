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

#include "AiObj.h"

#include <WebView2.h>
#include <iostream>
 
AiObj::AiObj(AiProfile *profile)
{
    root_dir_ = profile->rootdir;
    name_ = profile->name;
    url_ = profile->url;
    jsfile_ = profile->jsfile;
    useKeyboard_ = profile->usekeyboard;
    notebook_ = profile->notebook;
    keyboard_cmd_ = profile->keyboard_cmd;
    enable_ = profile->enable;
    
    // Load AI icon
    wxBitmap bitmap;
    if (bitmap.LoadFile(profile->iconfile, wxBITMAP_TYPE_PNG))
    {
        if (bitmap.GetSize() != wxSize(16, 16))
        {
            wxImage image = bitmap.ConvertToImage();
            image.Rescale(16, 16, wxIMAGE_QUALITY_HIGH);
            bitmap = wxBitmap(image);
        }
        icon_ = new wxBitmap(bitmap);
    }

    if (profile->page)
    {
        page_ = profile->page;
    }
    else
    {
        page_ = new wxPanel(notebook_);
    }
    jsContent_ = GetJsFileContent();
    
    wxWebViewConfiguration config = wxWebView::NewConfiguration();
    config.SetDataPath(config.GetDataPath() + name_);
    webview_ = new wxWebViewEdge(config);
    if(webview_ == nullptr)
    {
        wxLogError("webview_ is not supported on this platform.");
        return;
    }
    wxLogMessage("  name %s proxy: %s", name_, profile->proxy);
    if (!profile->proxy.IsEmpty())
    {
        wxLogMessage("  enable proxy!!!");
        webview_->SetProxy(profile->proxy);  // the 3.3 version of wxWidgets has implemented this function
        /*ICoreWebView2EnvironmentOptions* webViewOptions =
            (ICoreWebView2EnvironmentOptions*)config.GetNativeConfiguration();
        webViewOptions->put_AdditionalBrowserArguments(
            wxString::Format("--proxy-server=\"%s\"", profile->proxy).wc_str());*/
    }
    webview_->Create(
        page_, wxID_ANY, url_, wxDefaultPosition, wxDefaultSize, 0, wxASCII_STR(wxWebViewNameStr));

    webview_->Bind(wxEVT_WEBVIEW_LOADED, &AiObj::OnWebViewLoaded, this);

    // enable debug
    webview_->EnableAccessToDevTools(true);

    wxBoxSizer* pageSizer = new wxBoxSizer(wxVERTICAL);
    pageSizer->Add(webview_, 1, wxEXPAND);
    page_->SetSizer(pageSizer);

    // add script message handler
    webview_->AddScriptMessageHandler("wx_msg");
}

AiObj::~AiObj()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    if (webview_)
    {
        webview_->RemoveScriptMessageHandler("wx_msg");
        delete webview_;
    }
    if(page_) delete page_;
}

void AiObj::AddToNotebook(bool active)
{
    dynamic_cast<wxAuiNotebook*>(notebook_)->AddPage(page_, name_, active, *icon_);
}

void AiObj::DeleteFromNotebook()
{
    dynamic_cast<wxAuiNotebook*>(notebook_)->DeletePage(
        dynamic_cast<wxAuiNotebook*>(notebook_)->GetPageIndex(page_));
}

void AiObj::OnWebViewLoaded(wxWebViewEvent& event) {
    wxLogMessage("loaded %s %s", event.GetURL(), webview_->IsAccessToDevToolsEnabled()? "true" : "false");
}

wxString AiObj::GetJsFileContent()
{
    if(jsfile_.IsEmpty())
    {
        return wxEmptyString;
    }
    wxString content;
    wxFile file(root_dir_ + wxFileName::GetPathSeparator() + jsfile_);
    if (file.IsOpened())
    {
        wxFileOffset length = file.Length();
        char* buffer = new char[length];
        file.Read(buffer, length);
        content = wxString::FromUTF8(buffer, length);
        delete[] buffer;
    }
    return content;
}

void AiObj::FocusInput()
{
    OutputDebugString(L"FocusInput");
    webview_->SetFocus();
    //if (!jsContent_.IsEmpty())
    //{
    //    webview_->RunScript(
    //        //"console.log('FocusInput1');document.querySelector('textarea[dir=\"auto\"]').focus();console.log('FocusInput2');");
    //        wxString::Format(jsContent_, "bMsg=false,bFocus=true,bSent=false", ""));
    //}
}

void AiObj::SendMsg(const wxString& message)
{
    if (webview_ == nullptr)
    {
        wxLogError("webview_ is not vailable.");
        return;
    }
    OutputDebugString(L"==========SendMsg===========");
    OutputDebugString(this->name_);
    HANDLE hEvent = ::CreateEventW(NULL, FALSE, FALSE, NULL);
    if(!jsfile_.IsEmpty())
    {
        if(!jsContent_.IsEmpty())
        {
            // focus on the webview first
            webview_->SetFocus();
            bool sent_success = false;
            wxLogMessage("Run script %s", jsfile_);
            // prepare to receive message
            auto const handler = [this, hEvent, &sent_success](wxWebViewEvent& evt) {
                wxString cmd = evt.GetString();
                wxLogMessage(
                    "Script message received; value = %s, handler = %s", cmd, evt.GetMessageHandler());
                // when js has focused, it will send a message to us: window.wx_msg.postMessage('focused');
                if (cmd == "focused")
                {
                    OutputDebugString(L"receive focused msg from js");
                    wxSleep(1);
                    webview_->SetFocus();
                    if (this->useKeyboard_)
                    {
                        // then we can emulate keyboard press
                        this->RunKeyboardCmd();
                    }
                    //::SetEvent(hEvent);
                }
                else if (cmd == "sent")
                {
                    OutputDebugString(L"receive sent msg from js");
                    OutputDebugString(this->name_);
                    wxLogMessage("sent");
                    sent_success = true;
                    ::SetEvent(hEvent);
                }
            };
            
            webview_->Bind(wxEVT_WEBVIEW_SCRIPT_MESSAGE_RECEIVED, handler);
            wxString script =
                wxString::Format(jsContent_, "bMsg=true,bFocus=true,bSent=true", message);
            webview_->RunScript(script);
            if (::WaitForSingleObject(hEvent, 3000) == WAIT_TIMEOUT)
            {
                OutputDebugString(L"WaitForSingleObject timeout");
                int counter = 2;
                while (!sent_success)
                {
                    if (counter-- <= 0)
                    {
                        break;
                    }
                    wxSleep(1);
                    if (this->useKeyboard_)
                    {
                        this->RunKeyboardCmd();
                    }
                }                
            }            
            //webview_->Unbind(wxEVT_WEBVIEW_SCRIPT_MESSAGE_RECEIVED, handler);
        }
    }
    CloseHandle(hEvent);
}

bool AiObj::RunKeyboardCmd()
{
    OutputDebugString(L"RunKeyboardCmd");
    if(keyboard_cmd_.empty())
    {
        return false;
    }
    wxUIActionSimulator sim;
    for(wxString cmd : keyboard_cmd_)
    {
        FocusInput();
        if(cmd == "CTRL+A")
        {
            OutputDebugString(L"CTRL+A");
            wxLogMessage("CTRL+A");
            //sim.Char('A', wxMOD_CONTROL);
            sim.KeyDown(WXK_CONTROL);
            wxMilliSleep(100);
            sim.Char('A');
            wxMilliSleep(100);
            sim.KeyUp(WXK_CONTROL);
        }
        else if(cmd == "CTRL+V")
        {
            OutputDebugString(L"CTRL+V");
            wxLogMessage("CTRL+V");
            //sim.Char('V', wxMOD_CONTROL);
            sim.KeyDown(WXK_CONTROL);
            wxMilliSleep(100);
            sim.Char('V');
            wxMilliSleep(100);
            sim.KeyUp(WXK_CONTROL);
        }
        else if(cmd == "CTRL+ENTER")
        {
            OutputDebugString(L"CTRL+ENTER");
            wxLogMessage("CTRL+ENTER");
            //sim.Char(WXK_RETURN, wxMOD_CONTROL);
            sim.KeyDown(WXK_CONTROL);
            wxMilliSleep(100);
            sim.Char(WXK_RETURN);
            wxMilliSleep(100);
            sim.KeyUp(WXK_CONTROL);
        }
        else if(cmd == "ENTER")
        {
            OutputDebugString(L"ENTER");
            wxLogMessage("ENTER");
            sim.Char(WXK_RETURN);
        }
        else if (cmd == "SPACE")
        {
            OutputDebugString(L"SPACE");
            wxLogMessage("SPACE");
            sim.Char(WXK_SPACE);
        }
        else
        {
            wxLogError("Unknown command %s", cmd);
        }
        wxMilliSleep(100);
    }
    return true;
}