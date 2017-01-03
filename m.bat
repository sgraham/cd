@echo off
call cl /nologo /Zi crash.cc
call cl /D_HAS_EXCEPTIONS=0 /wd4530 /nologo /Zi /W4 /WX /Ithird_party/include/lldb/API /Ithird_party/include main.cc /link third_party\lib\liblldb.lib /out:cd.exe
.\cd crash
