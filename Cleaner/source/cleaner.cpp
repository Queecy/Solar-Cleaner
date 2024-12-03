#include "includes.h"
#include "cleaner.h"
namespace fs = std::filesystem;

// ============================================================================================================
bool DeleteDirectory(const std::wstring& dirPath) {
    SHFILEOPSTRUCT fileOp = { 0 };
    fileOp.wFunc = FO_DELETE;
    fileOp.pFrom = dirPath.c_str();
    fileOp.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_ALLOWUNDO;
    return SHFileOperation(&fileOp) == 0;
}

bool DeleteRegistryKey(const std::wstring& fullPath)
{
    size_t splitPos = fullPath.find(L'\\');
    if (splitPos == std::wstring::npos) return false;

    std::wstring hive = fullPath.substr(0, splitPos);
    std::wstring subKey = fullPath.substr(splitPos + 1); 

    HKEY hRootKey = (hive == L"HKEY_LOCAL_MACHINE") ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;
    LONG result = RegDeleteTreeW(hRootKey, subKey.c_str());

    return result == ERROR_SUCCESS;
}

void KillProcess(const std::wstring& processName)
{
    DWORD processID = 0;
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnapshot == INVALID_HANDLE_VALUE)
        return;

    if (Process32First(hSnapshot, &pe32))
    {
        do
        {
            if (processName == pe32.szExeFile)
            {
                processID = pe32.th32ProcessID;
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processID);
                if (hProcess)
                {
                    TerminateProcess(hProcess, 0);
                    CloseHandle(hProcess);
                }
            }
        } while (Process32Next(hSnapshot, &pe32));
    }
    CloseHandle(hSnapshot);
}

bool DeleteDirectoryContents(const std::wstring& directoryPath) {
    try 
    {
        for (const auto& entry : fs::directory_iterator(directoryPath)) 
        {
            if (fs::is_directory(entry)) 
            {
                DeleteDirectoryContents(entry.path().wstring());
            }
            else
            {
                fs::remove(entry.path());
            }
        }
        return true;
    }
    catch (const std::exception& e)
    {
        return false;
    }
}

class RecentDocsHelpers {
public:
    static void ClearAll() {
        SHAddToRecentDocs(SHARD_PIDL, nullptr);
    }
};

// ============================================================================================================
void ClearBrave()
{
    wchar_t appDataLocal[MAX_PATH];
    SHGetSpecialFolderPathW(NULL, appDataLocal, CSIDL_LOCAL_APPDATA, FALSE);

    std::wstring braveProfileDir = std::wstring(appDataLocal) + L"\\BraveSoftware\\Brave-Browser\\User Data";
    std::vector<std::wstring> braveDirs =
    {
        braveProfileDir + L"\\Cache",
        braveProfileDir + L"\\Code Cache",
        braveProfileDir + L"\\GPUCache",
        braveProfileDir + L"\\IndexedDB",
        braveProfileDir + L"\\Local Storage",
        braveProfileDir + L"\\Cookies",
        braveProfileDir + L"\\History"
    };

    for (const auto& dir : braveDirs)
    {
        DeleteDirectoryContents(dir);
    }

    MessageBox(NULL, L"Brave browser data cleared successfully.", L"Success [Brave]", MB_OK | MB_ICONINFORMATION);
}

void ClearMozilla()
{
    wchar_t appDataRoaming[MAX_PATH];
    SHGetSpecialFolderPathW(NULL, appDataRoaming, CSIDL_APPDATA, FALSE);

    std::wstring firefoxProfileDir = std::wstring(appDataRoaming) + L"\\Mozilla\\Firefox\\Profiles";
    for (const auto& entry : fs::directory_iterator(firefoxProfileDir))
    {
        if (fs::is_directory(entry))
        {
            std::vector<std::wstring> firefoxDirs = {
                entry.path().wstring() + L"\\cache2",
                entry.path().wstring() + L"\\cookies.sqlite",
                entry.path().wstring() + L"\\history.sqlite",
                entry.path().wstring() + L"\\localstorage",
                entry.path().wstring() + L"\\recovery.jsonlz4"
            };

            for (const auto& dir : firefoxDirs)
            {
                DeleteDirectoryContents(dir);
            }
        }
    }

    MessageBox(NULL, L"Mozilla Firefox data cleared successfully.", L"Success [Firefox]", MB_OK | MB_ICONINFORMATION);
}

void ClearOperaGX()
{
    wchar_t appDataLocal[MAX_PATH];
    SHGetSpecialFolderPathW(NULL, appDataLocal, CSIDL_LOCAL_APPDATA, FALSE);

    std::wstring operaGXProfileDir = std::wstring(appDataLocal) + L"\\Opera Software\\Opera GX Stable";
    std::vector<std::wstring> operaGXDirs = {
        operaGXProfileDir + L"\\Cache",
        operaGXProfileDir + L"\\Code Cache",
        operaGXProfileDir + L"\\GPUCache",
        operaGXProfileDir + L"\\IndexedDB",
        operaGXProfileDir + L"\\Local Storage",
        operaGXProfileDir + L"\\Cookies",
        operaGXProfileDir + L"\\History"
    };

    for (const auto& dir : operaGXDirs)
    {
        DeleteDirectoryContents(dir);
    }

    MessageBox(NULL, L"Opera GX browser data cleared successfully.", L"Success [Opera GX]", MB_OK | MB_ICONINFORMATION);
}

void ClearChrome()
{
    wchar_t appDataLocal[MAX_PATH];
    SHGetSpecialFolderPathW(NULL, appDataLocal, CSIDL_LOCAL_APPDATA, FALSE);

    std::wstring chromeProfileDir = std::wstring(appDataLocal) + L"\\Google\\Chrome\\User Data";
    std::vector<std::wstring> chromeDirs =
    {
        chromeProfileDir + L"\\Cache",
        chromeProfileDir + L"\\Code Cache",
        chromeProfileDir + L"\\GPUCache",
        chromeProfileDir + L"\\IndexedDB",
        chromeProfileDir + L"\\Local Storage",
        chromeProfileDir + L"\\Cookies",
        chromeProfileDir + L"\\History"
    };

    for (const auto& dir : chromeDirs)
    {
        DeleteDirectoryContents(dir);
    }

    MessageBox(NULL, L"Google Chrome data cleared successfully.", L"Success [Chrome]", MB_OK | MB_ICONINFORMATION);
}

void ClearFiveM() 
{
    try
    {
        std::vector<std::wstring> processes =
        {
            L"FiveM.exe", L"FiveM_b2802_DumpServer.exe", L"FiveM_b2802_GTAProcess.exe",
            L"FiveM_ChromeBrowser.exe", L"FiveM_ROSLauncher.exe", L"FiveM_FiveM_ROSService.exe",
            L"Steam.exe", L"steam.exe", L"EpicGamesLauncher.exe", L"EpicGamesLauncher.exe",
            L"EpicWebHelper.exe", L"EpicGamesLauncher.exe", L"smartscreen.exe", L"dnf.exe",
            L"DNF.exe", L"CrossProxy.exe", L"tensafe_1.exe", L"TenSafe_1.exe", L"tensafe_2.exe",
            L"tencentdl.exe", L"TenioDL.exe", L"uishell.exe", L"BackgroundDownloader.exe",
            L"conime.exe", L"QQDL.EXE", L"qqlogin.exe", L"dnfchina.exe", L"dnfchinatest.exe",
            L"txplatform.exe", L"TXPlatform.exe", L"Launcher.exe", L"LauncherPatcher.exe",
            L"SocialClubHelper.exe", L"RockstarErrorHandler.exe", L"RockstarService.exe"
        };

        for (const auto& process : processes)
        {
            KillProcess(process);
        }

        MessageBox(NULL, L"Success cleared fivem", L"Success [FIVEM]", MB_OK | MB_ICONINFORMATION);
    }
    catch (const std::exception& ex)
    {
        MessageBox(NULL, L"Fivem not successful.", L"Error [FIVEM]", MB_OK | MB_ICONERROR);
    }
}

void ClearValorant() 
{
    try
    {
        std::vector<std::wstring> processes = 
        {
            L"EasyAntiCheat_Setup.exe", L"EasyAntiCheat.exe", L"BEService_x64.exe", L"smartscreen.exe",
            L"dnf.exe", L"DNF.exe", L"CrossProxy.exe", L"tensafe_1.exe", L"TenSafe_1.exe", L"tensafe_2.exe",
            L"tencentdl.exe", L"TenioDL.exe", L"uishell.exe", L"BackgroundDownloader.exe", L"conime.exe",
            L"QQDL.EXE", L"qqlogin.exe", L"dnfchina.exe", L"dnfchinatest.exe", L"txplatform.exe",
            L"TXPlatform.exe", L"Launcher.exe", L"LauncherPatcher.exe", L"OriginWebHelperService.exe",
            L"Origin.exe", L"OriginClientService.exe", L"OriginER.exe", L"OriginThinSetupInternal.exe",
            L"OriginLegacyCLI.exe", L"Agent.exe", L"Client.exe", L"BattleEye.exe", L"BEService.exe", L"BEServices.exe"
        };

        for (const auto& process : processes)
        {
            KillProcess(process);
        }

        MessageBox(NULL, L"Success cleared valorant", L"Success [VALORANT]", MB_OK | MB_ICONINFORMATION);
    }
    catch (const std::exception& ex)
    {
        MessageBox(NULL, L"Valorant not successful.", L"Error [VALORANT]", MB_OK | MB_ICONERROR);
    }

}

void ClearFortnite()
{
    try
    {
        std::vector<std::wstring> processes = 
        {
            L"epicgameslauncher.exe", L"FortniteClient-Win64-Shipping_EAC.exe", L"FortniteClient-Win64-Shipping.exe",
            L"FortniteClient-Win64-Shipping_BE.exe", L"FortniteLauncher.exe", L"EpicGamesLauncher.exe",
            L"EasyAntiCheat_Setup.exe", L"EpicWebHelper.exe", L"EasyAntiCheat.exe",
            L"BEService_x64.exe", L"dnf.exe", L"DNF.exe", L"BattleEye.exe",
            L"BEService.exe", L"BEServices.exe"
        };

        for (const auto& process : processes)
        {
            KillProcess(process);
        }

        MessageBox(NULL, L"Success cleared fortnite", L"Success [FORTNITE]", MB_OK | MB_ICONINFORMATION);
    }
    catch (const std::exception& ex)
    {
        MessageBox(NULL, L"Fortnite not successful.", L"Error [FORTNITE]", MB_OK | MB_ICONERROR);
    }
}

void ClearAC()
{
    try 
    {
        std::vector<std::wstring> dirsToDelete =
        {
            L"\\documents\\Call of Duty Modern Warfare",
            L"\\Battle.net",
            L"\\Blizzard Entertainment",
            L"\\Battle.net",
            L"\\CommonApplicationData\\Battle.net",
            L"\\CommonApplicationData\\Blizzard Entertainment",
            L"\\Documents\\Call of Duty Black Ops Cold War\\report",
            L"\\Documents\\Call of Duty Black Ops Cold War",
            L"\\Battle.net\\BrowserCache",
            L"\\Battle.net\\BrowserCache\\GPUCache",
            L"\\Battle.net\\BrowserCache\\GPUCache\\data_0.dcache",
            L"\\Battle.net\\BrowserCache\\GPUCache\\data_1.dcache",
            L"\\Battle.net\\BrowserCache\\GPUCache\\data_2.dcache",
            L"\\Battle.net\\BrowserCache\\GPUCache\\data_3.dcache",
            L"\\Battle.net\\BrowserCache\\GPUCache\\f_000001.dcache",
            L"\\Battle.net\\BrowserCache\\GPUCache\\index.dcache",
            L"\\Battle.net\\BrowserCache\\Cache\\index",
            L"\\Battle.net\\BrowserCache\\GPUCache\\data_0",
            L"\\Battle.net\\BrowserCache\\GPUCache\\data_1",
            L"\\Battle.net\\BrowserCache\\GPUCache\\data_2",
            L"\\Battle.net\\BrowserCache\\GPUCache\\data_3",
            L"\\Battle.net\\BrowserCache\\GPUCache\\f_000001",
            L"\\Battle.net\\BrowserCache\\GPUCache\\index",
            L"\\Battle.net\\BrowserCache\\Cache\\index.dcache",
            L"\\Battle.net\\BrowserCache\\Cache\\data_0.dcache",
            L"\\Battle.net\\BrowserCache\\Cache\\data_1.dcache",
            L"\\Battle.net\\BrowserCache\\Cache\\data_2.dcache",
            L"\\Battle.net\\BrowserCache\\Cache\\data_3.dcache",
            L"\\Battle.net\\BrowserCache\\Cache\\data_0",
            L"\\Battle.net\\BrowserCache\\Cache\\data_1",
            L"\\Battle.net\\BrowserCache\\Cache\\data_2",
            L"\\Battle.net\\BrowserCache\\Cache\\data_3",
            L"\\Battle.net\\Cache",
            L"\\Battle.net\\Logs",
            L"\\Battle.net\\WidevineCdm",
            L"\\Battle.net\\CachedData",
            L"\\Blizzard Entertainment",
            L"\\Roaming\\Battle.net",
            L"\\ProgramFiles\\Battle.net",
            L"\\ProgramFilesX86\\Blizzard Entertainment"
        };

        for (const auto& dir : dirsToDelete) 
        {
            std::wstring fullPath = L"\\documents" + dir; 
            DeleteDirectory(fullPath);
        }

        std::vector<std::wstring> regKeysToDelete = 
        {
            L"SOFTWARE\\WOW6432Node\\EasyAntiCheat",
            L"SYSTEM\\ControlSet001\\Services\\EasyAntiCheat",
            L"SYSTEM\\ControlSet001\\Services\\EasyAntiCheat\\Security",
            L"SOFTWARE\\WOW6432Node\\EasyAntiCheat",
            L"SYSTEM\\ControlSet001\\Services\\EasyAntiCheat",
            L"SOFTWARE\\WOW6432Node\\EasyAntiCheat"
        };

        for (const auto& key : regKeysToDelete) 
        {
            DeleteRegistryKey(key);
        }

        MessageBox(NULL, L"Success cleared traces ac", L"Success [TRACES AC]", MB_OK | MB_ICONINFORMATION);
    }
    catch (const std::exception& ex) 
    {
        MessageBox(NULL, L"TRACES AC not successful.", L"Error [TRACES AC]", MB_OK | MB_ICONERROR);
    }
}

void ClearTemp()
{
    try 
    {
        std::string tempDir = "C:\\Windows\\Temp";

        for (const auto& entry : std::filesystem::directory_iterator(tempDir)) {
            try 
            {
                std::filesystem::remove(entry.path());
            }
            catch (const std::exception& ex)
            {
                MessageBoxA(nullptr, ("Error: " + std::string(ex.what())).c_str(), "Error [TEMP]", MB_OK | MB_ICONERROR);
            }
        }

        MessageBoxA(nullptr, "Successfully cleared windows [TEMP]", "Success [TEMP]", MB_OK | MB_ICONINFORMATION);

    }
    catch (const std::exception& ex)
    {
        MessageBoxA(nullptr, ("Error: " + std::string(ex.what())).c_str(), "Error [TEMP]", MB_OK | MB_ICONERROR);
    }
}

void ClearLogs() 
{
    try
    {
        std::wstring command = L"wevtutil el";

        STARTUPINFO si = { 0 };
        PROCESS_INFORMATION pi = { 0 };
        si.cb = sizeof(si);

        if (!CreateProcess(
            nullptr,
            &command[0],
            nullptr,
            nullptr,
            FALSE, 
            CREATE_NO_WINDOW, 
            nullptr,
            nullptr,
            &si,
            &pi)
            ) 
        {
            throw std::runtime_error("Failed to create process. Error code: " + std::to_string(GetLastError()));
        }

        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        MessageBoxW(nullptr, L"Successfully cleared windows [LOGS]", L"Success [LOGS]", MB_OK | MB_ICONINFORMATION);

    }
    catch (const std::exception& ex) {
        std::wstring errorMessage = L"Error: " + std::wstring(ex.what(), ex.what() + strlen(ex.what()));
        MessageBoxW(nullptr, errorMessage.c_str(), L"Error [LOGS]", MB_OK | MB_ICONERROR);
    }
}

void ClearUpdate() 
{
    try
    {
        std::string updateCachePath = "C:\\Windows\\SoftwareDistribution\\Download";

        if (fs::exists(updateCachePath) && fs::is_directory(updateCachePath))
        {
            for (const auto& file : fs::directory_iterator(updateCachePath))
            {
                if (fs::is_regular_file(file))
                {
                    try
                    {
                        fs::remove(file);
                    }
                    catch (const std::exception& ex)
                    {
                        MessageBoxA(NULL, ("Delete error: " + file.path().string() + ", " + ex.what()).c_str(), "Error [UPDATE]", MB_OK | MB_ICONERROR);
                    }
                }
            }

            for (const auto& dir : fs::directory_iterator(updateCachePath))
            {
                if (fs::is_directory(dir))
                {
                    try
                    {
                        fs::remove_all(dir);
                    }
                    catch (const std::exception& ex)
                    {
                        MessageBoxA(NULL, ("Folder error: " + dir.path().string() + ", " + ex.what()).c_str(), "Error [UPDATE]", MB_OK | MB_ICONERROR);
                    }
                }
            }
        }

        MessageBoxA(NULL, "Successfully cleared windows [UPDATE]", "Success [UPDATE]", MB_OK | MB_ICONINFORMATION);
    }
    catch (const std::exception& ex)
    {
        MessageBoxA(NULL, ("Error: " + std::string(ex.what())).c_str(), "Error [UPDATE]", MB_OK | MB_ICONERROR);
    }
}

void ClearEvents() 
{
    const wchar_t* logTypes[] = { L"Application", L"System", L"Security" };
    for (int i = 0; i < 3; ++i)
    {
        HANDLE hEventLog = OpenEventLog(NULL, logTypes[i]);

        if (hEventLog != NULL)
        {
            if (ClearEventLog(hEventLog, NULL))
            {
                std::wcout << L"Successfully cleared event log: " << logTypes[i] << std::endl;
            }
            else
            {
                std::wcout << L"Failed to clear event log: " << logTypes[i] << std::endl;
            }

            CloseEventLog(hEventLog);
        }
        else
        {
            std::wcout << L"Failed to open event log: " << logTypes[i] << std::endl;
        }
    }

    MessageBox(NULL, L"Successfully cleared windows [EVENTS]", L"Success [EVENTS]", MB_OK | MB_ICONINFORMATION);
}

void ClearRecent() 
{
    RecentDocsHelpers::ClearAll();
    MessageBoxA(nullptr, "Successfully cleared windows [RECENT]", "Success [RECEMT]", MB_OK | MB_ICONINFORMATION);
}

void ClearMemory() 
{
    __pragma(warning(suppress: 6031)) _heapmin();
    MessageBoxA(NULL, "Successfully cleared windows [MEMORY]", "Success [MEMORY]", MB_OK | MB_ICONINFORMATION);
}

void ClearRegistry()
{
    DeleteRegistryKey(L"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\UserAssist\\{CEBFF5CD-ACE2-4F4F-9178-9926F41749EA}");
    DeleteRegistryKey(L"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows NT\\CurrentVersion\\AppCompatFlags\\Compatibility Assistant\\Store");
    DeleteRegistryKey(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RecentDocs");
    DeleteRegistryKey(L"Software\\Classes\\Local Settings\\Software\\Microsoft\\Windows\\Shell\\BagMRU");
    DeleteRegistryKey(L"Software\\Classes\\Local Settings\\Software\\Microsoft\\Windows\\Shell\\MuiCache");
    DeleteRegistryKey(L"Software\\Microsoft\\Windows\\Shell");
    DeleteRegistryKey(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ComDlg32");
    DeleteRegistryKey(L"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
    DeleteRegistryKey(L"SOFTWARE\\WinRAR\\ArcHistory");
    DeleteRegistryKey(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FeatureUsage\\AppSwitched");
    DeleteRegistryKey(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FeatureUsage\\ShowJumpView");
    DeleteRegistryKey(L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ComDlg32\\OpenSavePidlMRU");
    DeleteRegistryKey(L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ComDlg32\\LastVisitedPidlMRU");
    DeleteRegistryKey(L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ComDlg32\\CIDSizeMRU");
    DeleteRegistryKey(L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\TypedPaths");
    DeleteRegistryKey(L"SOFTWARE\\Clients\\StartMenuInternet");

    MessageBoxA(NULL, "Successfully cleared windows [REGISTRY]", "Success [REGISTRY]", MB_OK | MB_ICONINFORMATION);
}

void ClearHistory()
{
    auto runCommand = [](const std::wstring& command) 
        {
        STARTUPINFO si = { 0 };
        PROCESS_INFORMATION pi = { 0 };

        si.cb = sizeof(STARTUPINFO);
        if (!CreateProcess(NULL, const_cast<LPWSTR>(command.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) 
        {
            std::wcerr << L"CreateProcess failed (" << GetLastError() << L")" << std::endl;
        }
        else 
        {
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
        };

    runCommand(L"rundll32.exe InetCpl.cpl,ClearMyTracksByProcess 1");
    runCommand(L"rundll32.exe InetCpl.cpl,ClearMyTracksByProcess 1");
    runCommand(L"rundll32.exe InetCpl.cpl,ClearMyTracksByProcess 16");

    MessageBox(NULL, L"Successfully cleared windows [HISTORY]", L"Success [HISTORY]", MB_OK | MB_ICONINFORMATION);
}

void ClearPrefetch()
{
    const std::wstring prefetchPath = L"C:\\Windows\\Prefetch";

    try 
    {
        if (!fs::exists(prefetchPath) || !fs::is_directory(prefetchPath))
        {
            throw std::runtime_error("Directory does not exist.");
        }

        for (const auto& entry : fs::directory_iterator(prefetchPath))
        {
            if (entry.is_regular_file()) 
            {
                fs::remove(entry.path());
            }
        }

        MessageBox(NULL, L"Successfully cleared [PREFETCH].", L"Success [PREFETCH]", MB_OK | MB_ICONINFORMATION);
    }
    catch (const std::exception& ex) 
    {
        MessageBox(NULL, L"Error occurred during clearing [PREFETCH].", L"Error [PREFETCH]", MB_OK | MB_ICONERROR);
    }
}

void ClearRegSeeker() 
{
    try
    {
        std::vector<std::wstring> regKeysToDelete =
        {
            L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\UserAssist\\{CEBFF5CD-ACE2-4F4F-9178-9926F41749EA}",
            L"Software\\Microsoft\\Windows NT\\CurrentVersion\\AppCompatFlags\\Compatibility Assistant\\Store",
            L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RecentDocs",
            L"Software\\Classes\\Local Settings\\Software\\Microsoft\\Windows\\Shell\\BagMRU",
            L"Software\\Classes\\Local Settings\\Software\\Microsoft\\Windows\\Shell\\MuiCache",
            L"Software\\Microsoft\\Windows\\Shell",
            L"SOFTWARE\\WinRAR\\ArcHistory",
            L"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall",
            L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FeatureUsage\\AppSwitched",
            L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FeatureUsage\\ShowJumpView",
            L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ComDlg32\\OpenSavePidlMRU",
            L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ComDlg32\\LastVisitedPidlMRU",
            L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ComDlg32\\CIDSizeMRU",
            L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\TypedPaths"
        };

        for (const auto& regKey : regKeysToDelete)
        {
            if (!DeleteRegistryKey(regKey))
            {
                std::wstring errorMessage = L"Failed to delete registry key: " + regKey;
                MessageBox(NULL, errorMessage.c_str(), L"Error [REGSEEKER]", MB_OK | MB_ICONERROR);
            }
        }

        MessageBox(NULL, L"Successfully cleared registry [REGSEEKER].", L"Success [REGSEEKER]", MB_OK | MB_ICONINFORMATION);
    }
    catch (const std::exception& ex)
    {
        std::wstring errorMessage = L"Error during registry cleanup: " + std::wstring(ex.what(), ex.what() + strlen(ex.what()));
        MessageBox(NULL, errorMessage.c_str(), L"Error [REGSEEKER]", MB_OK | MB_ICONERROR);
    }
}

void ClearCrashDumps()
{
    char* localAppData;
    size_t len = 0;
    if (_dupenv_s(&localAppData, &len, "LOCALAPPDATA") != 0 || localAppData == nullptr)
    {
        throw std::runtime_error("Unable to retrieve LOCALAPPDATA.");
    }
    std::string folderPath = localAppData;
    folderPath += "\\CrashDumps";
    free(localAppData);

    try
    {
        if (!fs::exists(folderPath) || !fs::is_directory(folderPath))
        {
            throw std::runtime_error("Directory does not exist.");
        }

        for (const auto& entry : fs::directory_iterator(folderPath))
        {
            if (entry.is_regular_file() && entry.path().extension() == ".dmp")
            {
                fs::remove(entry.path());
            }
        }

        MessageBox(NULL, L"Success cleared crash dumps", L"Succes [CRASH DUMPS]", MB_OK | MB_ICONINFORMATION);
    }
    catch (const std::exception& ex)
    {
        MessageBox(NULL, L"Error occurred during clearing crash dumps", L"Error [CRASH DUMPS] ", MB_OK | MB_ICONERROR);
    }
}

void ClearDownloadFile()
{
    char* userProfile = nullptr;
    size_t len = 0;

    if (_dupenv_s(&userProfile, &len, "USERPROFILE") != 0 || userProfile == nullptr) 
    {
        return;
    }

    std::string downloadsPath = std::string(userProfile) + "\\Downloads";
    free(userProfile);

    try
    {
        if (fs::exists(downloadsPath) && fs::is_directory(downloadsPath))
        {
            for (const auto& entry : fs::directory_iterator(downloadsPath))
            {
                if (entry.is_regular_file())
                {
                    try
                    {
                        fs::remove(entry.path());
                    }
                    catch (const std::exception& ex)
                    {
                        
                    }
                }
            }

            MessageBox(NULL, L"Success cleared download file", L"Success [DOWNLOAD FILE]", MB_OK | MB_ICONINFORMATION);
        }
    }
    catch (const std::exception& ex)
    {
        MessageBoxA(NULL, ("Error: " + std::string(ex.what())).c_str(), "Error [DOWNLOAD FILE]", MB_OK | MB_ICONERROR);
    }
}

void ClearLastActivity() 
{
    try 
    {
        DeleteRegistryKey(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RecentDocs");
        DeleteRegistryKey(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ComDlg32");
        DeleteRegistryKey(L"Software\\Classes\\Local Settings\\Software\\Microsoft\\Windows\\Shell\\BagMRU");
        DeleteRegistryKey(L"Software\\Classes\\Local Settings\\Software\\Microsoft\\Windows\\Shell\\MuiCache");
        DeleteRegistryKey(L"Software\\Microsoft\\Windows\\Shell");
        DeleteRegistryKey(L"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
        DeleteRegistryKey(L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ComDlg32\\OpenSavePidlMRU");

        for (const auto& entry : fs::directory_iterator(L"C:\\Windows\\Prefetch"))
        {
            try
            {
                fs::remove(entry.path());
            }
            catch (const std::exception&) 
            {

            }
        }

        for (const auto& entry : fs::directory_iterator(L"C:\\Windows\\Temp")) 
        {
            try
            {
                fs::remove(entry.path());
            }
            catch (const std::exception&) 
            {

            }
        }

        HANDLE hEventLog = OpenEventLogW(NULL, L"System");
        if (hEventLog != NULL)
        {
            ClearEventLogW(hEventLog, NULL);
            CloseEventLog(hEventLog);
        }
        hEventLog = OpenEventLogW(NULL, L"Application");
        if (hEventLog != NULL)
        {
            ClearEventLogW(hEventLog, NULL);
            CloseEventLog(hEventLog);
        }

        MessageBox(NULL, L"Success cleared last activity", L"Success [LAST ACTIVITY]", MB_OK | MB_ICONINFORMATION);
    }
    catch (const std::exception&) 
    {
        MessageBox(NULL, L"Error occurred during clearing last activity", L"Error [LAST ACTIVITY]", MB_OK | MB_ICONERROR);
    }
}

void ClearDnsCache()
{
#ifdef _WIN32
    int result = system("ipconfig /flushdns");
    if (result == 0) 
    {
        MessageBox(NULL, L"Success cleared DNS cachce", L"Success [DNS]", MB_OK | MB_ICONINFORMATION);
    }
    else 
    {
        MessageBox(NULL, L"DNS cleanup not successful.", L"Error [DNS]", MB_OK | MB_ICONERROR);
    }
#endif
}

void ClearSteamAccount() 
{
    std::filesystem::path steamConfigPath = "C:\\Program Files (x86)\\Steam\\config";
    try 
    {
        for (const auto& entry : std::filesystem::directory_iterator(steamConfigPath)) 
        {
            try 
            {
                std::filesystem::remove(entry.path());
            }
            catch (const std::exception&)
            {
                
            }
        }

        MessageBox(NULL, L"Success cleared steam account", L"Success [STEAM ACC]", MB_OK | MB_ICONINFORMATION);
    }
    catch (const std::filesystem::filesystem_error& e)
    {
        MessageBox(NULL, L"An error occurred while clearing Steam account files.", L"Error [STEAM ACC]", MB_OK | MB_ICONERROR);
    }
}

void ClearDriveOP()
{
    // system("defrag C: /O");
    system("dfrgui");
}

void ClearFivemOP()
{
    wchar_t tempPath[MAX_PATH];
    GetTempPathW(MAX_PATH, tempPath); 
    std::wstring tempFilename = std::wstring(tempPath) + L"temp.bat";

    std::ofstream writer(tempFilename);
    if (writer.is_open())
    {
        writer << "@echo off\n";
        writer << "powercfg -s 8c5e7fda-e8bf-4a96-9a85-a6e23a8c635c\n";
        writer << "taskkill /f /im GTAVLauncher.exe\n";
        writer << "wmic process where name='FiveM_b2189_GTAProcess.exe' CALL setpriority 128\n";
        writer << "taskkill /f /im wmpnetwk.exe\n";
        writer << "taskkill /f /im OneDrive.exe\n";
        writer << "taskkill /f /im speedfan.exe\n";
        writer << "taskkill /f /im TeamViewer_Service.exe\n";
        writer << "taskkill /f /im opera.exe\n";
        writer << "taskkill /f /im java.exe\n";
        writer.close();
    }
    else
    {
        MessageBox(NULL, L"Bat file was not created", L"Error [FIVEM OP]", MB_OK | MB_ICONERROR);
        return;
    }

    STARTUPINFOW si = { sizeof(STARTUPINFOW) };
    PROCESS_INFORMATION pi;
    if (CreateProcessW(tempFilename.c_str(), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        DeleteFileW(tempFilename.c_str());

        MessageBox(NULL, L"Success fivem optimalized", L"Success [FIVEM OP]", MB_OK | MB_ICONINFORMATION);
    }
    else 
    {
        MessageBox(NULL, L"An error occurred while fivem op account files.", L"Error [FIVEM OP]", MB_OK | MB_ICONERROR);
    }
}

void ClearSystemRestorePoints() 
{
    try
    {
        const std::wstring command = L"vssadmin delete shadows /all /quiet";
        STARTUPINFO si = { sizeof(STARTUPINFO) };
        PROCESS_INFORMATION pi;

        if (CreateProcess(NULL, const_cast<LPWSTR>(command.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
        {
            WaitForSingleObject(pi.hProcess, INFINITE);
            DWORD exitCode;
            if (GetExitCodeProcess(pi.hProcess, &exitCode) && exitCode == 0)
            {
                MessageBox(NULL, L"Successfully cleared system restore point", L"Success [SYSTEM RESTORE POINT]", MB_OK | MB_ICONINFORMATION);
            }
            else
            {
                MessageBox(NULL, L"Error occurred while clearing system restore point", L"Error [SYSTEM RESTORE POINT]", MB_OK | MB_ICONERROR);
            }

            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
        else
        {
            MessageBox(NULL, L"Failed to execute the command.", L"Error [SYSTEM RESTORE POINT]", MB_OK | MB_ICONERROR);
        }
    }
    catch (const std::exception& ex)
    {
        std::wstring errorMessage = L"Error: " + std::wstring(ex.what(), ex.what() + strlen(ex.what()));
        MessageBox(NULL, errorMessage.c_str(), L"Error [SYSTEM RESTORE POINT]", MB_OK | MB_ICONERROR);
    }
}