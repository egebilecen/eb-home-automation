@echo off

:: DIRECTORY DEFINATIONS
SET BASE_DIR=D:\egebi\Yazilim\Projeler\eb_home_automation\home_controller\
::  ^^^^^^^^   EDIT
SET ALIAS_NAME=eb_home_controller
:: ^^^^^^^^^^^ EDIT
SET KEYSTORE=%BASE_DIR%%ALIAS_NAME%.keystore
SET APK_PATH=%BASE_DIR%platforms\android\app\build\outputs\apk\release\app-release-unsigned.apk

:: BATCH MAIN
echo [?] Building cordova project.

cmd /C cordova build --release android >NUL 2>&1

if NOT %ERRORLEVEL% == 0 (
    echo [!] An error occured while building cordova project.
    goto :end
)

echo [+] Cordova project successfuly built.

echo [?] Signing the jar.
cmd /C jarsigner -verbose -sigalg SHA1withRSA -digestalg SHA1 -keystore %KEYSTORE% %APK_PATH% %ALIAS_NAME% >NUL 2>&1

if NOT %ERRORLEVEL% == 0 (
    echo.
    echo [!] An error occured while signing the jar.
    goto :end
)

echo.
echo [+] Jar successfuly signed.

cmd /C del /f %ALIAS_NAME%.apk >NUL 2>&1

if NOT %ERRORLEVEL% == 0 (
    echo [!] An error occured while deleting unsigned apk.
    goto :end
)

echo [?] Aligning the zip.
cmd /C zipalign -v 4 %APK_PATH% %ALIAS_NAME%.apk >NUL 2>&1

if NOT %ERRORLEVEL% == 0 (
    echo [!] An error occured while aligning the zip.
    goto :end
)

echo [+] Zip successfuly aligned.

echo [?] APK successfuly built.
goto :end

:end
PAUSE
