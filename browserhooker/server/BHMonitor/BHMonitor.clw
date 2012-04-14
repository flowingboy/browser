; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CBHMonitorDlg
LastTemplate=CEdit
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "BHMonitor.h"

ClassCount=6
Class1=CBHMonitorApp
Class2=CBHMonitorDlg
Class3=CAboutDlg

ResourceCount=7
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_BHMONITOR_DIALOG
Resource4=IDD_SEETING_DIALOG
Resource5=IDD_BHMONITOR_DIALOG (English (U.S.))
Class4=CSettingDlg
Resource6=IDD_ABOUTBOX (English (U.S.))
Class5=CHKRuleSettingDlg
Class6=CConsoleLog
Resource7=IDD_HKRULE_SETTING

[CLS:CBHMonitorApp]
Type=0
HeaderFile=BHMonitor.h
ImplementationFile=BHMonitor.cpp
Filter=N

[CLS:CBHMonitorDlg]
Type=0
HeaderFile=BHMonitorDlg.h
ImplementationFile=BHMonitorDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CBHMonitorDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=BHMonitorDlg.h
ImplementationFile=BHMonitorDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg


[DLG:IDD_BHMONITOR_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CBHMonitorDlg

[DLG:IDD_BHMONITOR_DIALOG (English (U.S.))]
Type=1
Class=CBHMonitorDlg
ControlCount=10
Control1=IDC_BT_HKRULE_SETTING,button,1342242816
Control2=IDC_BT_COMMUNICATION_SETTING,button,1342242816
Control3=IDC_BT_START_SERVER,button,1342242816
Control4=IDC_BT_STOP_SERVER,button,1342242816
Control5=IDC_BT_EXPORT_CLIENT,button,1342242816
Control6=IDOK,button,1342242817
Control7=IDC_STATIC,button,1342177287
Control8=IDC_LIST_CTRL_CLIENTS,SysListView32,1350633473
Control9=IDC_CONSOLE_LOGS,RICHEDIT,1352731844
Control10=IDC_STATIC,button,1342177287

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SEETING_DIALOG]
Type=1
Class=CSettingDlg
ControlCount=19
Control1=IDC_BT_SAVE_SEETING,button,1342242816
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,button,1342177287
Control10=IDC_CK_SEND_ORDERED,button,1342242819
Control11=IDC_CK_RECV_ORDERED,button,1342251011
Control12=IDC_STATIC,static,1342308352
Control13=IDC_PORT,edit,1350631552
Control14=IDC_MAX_CONNECTION,edit,1350631552
Control15=IDC_NUM_IO_WORKERS,edit,1350631552
Control16=IDC_NUM_LG_WORKERS,edit,1350631552
Control17=IDC_MAX_UNSAFE_BUFFER,edit,1350631552
Control18=IDC_MAX_UNUSED_CLIENTS,edit,1350631552
Control19=IDC_NUM_PENDING_RLOOP,edit,1350631552

[CLS:CSettingDlg]
Type=0
HeaderFile=SettingDlg.h
ImplementationFile=SettingDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_PORT

[DLG:IDD_HKRULE_SETTING]
Type=1
Class=CHKRuleSettingDlg
ControlCount=4
Control1=IDC_LIST_HOOKING_RULES,SysListView32,1350631937
Control2=IDC_BT_SAVE_HKRULES,button,1342242816
Control3=IDC_BT_ADD_HKRULES,button,1342242816
Control4=IDC_BT_DEL_HKRULES,button,1342242816

[CLS:CHKRuleSettingDlg]
Type=0
HeaderFile=HKRuleSettingDlg.h
ImplementationFile=HKRuleSettingDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CHKRuleSettingDlg

[CLS:CConsoleLog]
Type=0
HeaderFile=ConsoleLog.h
ImplementationFile=ConsoleLog.cpp
BaseClass=CEdit
Filter=W

