#include "LanguageStrings.h"

wxString LanguageStrings::currentLanguage = "en";

std::map<wxString, wxString> LanguageStrings::cnStrings = {
    {"title", "AI大乱斗"},
    {"settings", "设置"},
    {"language", "Language"},
    {"proxy_settings", "代理服务器"},
    {"ai_management", "AI管理"},
    {"control", "控制"},
    {"pk_mode", "PK模式"},
    {"choose_ai", "选择使用AI"},
    {"refresh", "刷新"},
    {"go_back", "后退"},
    {"choose_pk_ai", "选择PK AI"},
    {"quit_pk_mode", "退出PK模式"},
    {"ask", "提问"},
    {"confirm_close_tab", "确定要关闭标签页 \"%s\" 吗？"},
    {"confirm", "确认关闭"},
    {"proxy_example", "eg: socks://127.0.0.1:8080\r\n选择使用代理的AI："},
    {"select_two_ai", "请选择两个AI"},
    {"notice", "提示"},
    {"max_two_ai", "最多只能选择两个AI"},
    {"not_pk", "现在不是PK模式"},
    {"restart_apply", "更改代理设置需要重启本程序后生效"},
    {"about", "关于"},
    {"help", "帮助"}
};

std::map<wxString, wxString> LanguageStrings::enStrings = {
    {"title", "AI At Once"},
    {"settings", "Settings"},
    {"language", "Language"},
    {"proxy_settings", "Proxy Settings"},
    {"ai_management", "AI Management"},
    {"control", "Control"},
    {"pk_mode", "PK Mode"},
    {"choose_ai", "Choose AI"},
    {"refresh", "Refresh"},
    {"go_back", "Go Back"},
    {"choose_pk_ai", "Choose PK AI"},
    {"quit_pk_mode", "Quit PK Mode"},
    {"ask", "Ask"},
    {"confirm_close_tab", "Are you sure you want to close tab \"%s\"?"},
    {"confirm", "Confirm"},
    {"proxy_example", "eg: socks://127.0.0.1:8080\r\nSelect AIs to use proxy:"},
    {"select_two_ai", "Please select two AIs"},
    {"notice", "Notice"},
    {"max_two_ai", "You can select maximum two AIs"},
    {"not_pk", "It's not PK mode now"},
    {"restart_apply", "You need to restart the program to apply the proxy settings"},
    {"about", "About"},
    {"help", "Help"}};