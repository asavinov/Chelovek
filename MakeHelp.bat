@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by CHELOVEK.HPJ. >"hlp\Chelovek.hm"
echo. >>"hlp\Chelovek.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\Chelovek.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\Chelovek.hm"
echo. >>"hlp\Chelovek.hm"
echo // Prompts (IDP_*) >>"hlp\Chelovek.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\Chelovek.hm"
echo. >>"hlp\Chelovek.hm"
echo // Resources (IDR_*) >>"hlp\Chelovek.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\Chelovek.hm"
echo. >>"hlp\Chelovek.hm"
echo // Dialogs (IDD_*) >>"hlp\Chelovek.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\Chelovek.hm"
echo. >>"hlp\Chelovek.hm"
echo // Frame Controls (IDW_*) >>"hlp\Chelovek.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\Chelovek.hm"
REM -- Make help for Project CHELOVEK


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\Chelovek.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\Chelovek.hlp" goto :Error
if not exist "hlp\Chelovek.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\Chelovek.hlp" Debug
if exist Debug\nul copy "hlp\Chelovek.cnt" Debug
if exist Release\nul copy "hlp\Chelovek.hlp" Release
if exist Release\nul copy "hlp\Chelovek.cnt" Release
echo.
goto :done

:Error
echo hlp\Chelovek.hpj(1) : error: Problem encountered creating help file

:done
echo.
