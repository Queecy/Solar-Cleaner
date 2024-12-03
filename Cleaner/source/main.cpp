#include "includes.h"
#include "cleaner.h"

HWND main_hwnd = nullptr;
LPDIRECT3DDEVICE9 g_pd3dDevice = nullptr;
D3DPRESENT_PARAMETERS g_d3dpp;
LPDIRECT3D9 g_pD3D = nullptr;

namespace ui 
{
    void init(LPDIRECT3DDEVICE9 device) 
    {

    }

    void render()
    {
        ImGui::SetNextWindowSize(ImVec2(550, 400));
        static bool show_window = true;

        ImGui::Begin("[SOLAR] Cleaner by Queecy & Adrian376 & Kevin", &show_window, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
        ImGui::Text("[SOLAR] Cleaner created by Queecy & Adrian376 & Kevin");

        ImVec2 closeButtonPos(530, 5);
        ImGui::SetCursorPos(closeButtonPos);
        if (ImGui::Button("X"))
        {
            PostQuitMessage(0);
        }

        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();

        static bool checkbox_brave = false;
        static bool checkbox_mozilla = false;
        static bool checkbox_operagx = false;
        static bool checkbox_chrome = false;

        static bool checkbox_fivem = false;
        static bool checkbox_valorant = false;
        static bool checkbox_fortnite = false;
        static bool checkbox_ac = false;

        static bool checkbox_temp = false;
        static bool checkbox_logs = false;
        static bool checkbox_update = false;
        static bool checkbox_events = false;
        static bool checkbox_recent = false;
        static bool checkbox_memory = false;
        static bool checkbox_registry = false;
        static bool checkbox_history = false;
        static bool checkbox_prefetch = false;
        static bool checkbox_regseeker = false;
        static bool checkbox_crashdumps = false;
        static bool checkbox_downloadfile = false;
        static bool checkbox_lastactivity = false;

        static bool checbox_dnscache = false;
        static bool checbox_steamacc = false;
        static bool checbox_driveop = false;
        static bool checbox_fivemop = false;
        static bool checbox_systemrestorepoint = false;

        ImGui::Text("Browse"); 
        ImGui::Checkbox("Brave", &checkbox_brave);
        ImGui::SameLine();
        ImGui::Checkbox("Mozilla", &checkbox_mozilla);
        ImGui::SameLine();
        ImGui::Checkbox("Opera GX", &checkbox_operagx);
        ImGui::SameLine();
        ImGui::Checkbox("Google Chrome", &checkbox_chrome);
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::Text("Traces");
        ImGui::Checkbox("Fivem", &checkbox_fivem);
        ImGui::SameLine();
        ImGui::Checkbox("Valorant", &checkbox_valorant);
        ImGui::SameLine();
        ImGui::Checkbox("Fortnite", &checkbox_fortnite);
        ImGui::SameLine();
        ImGui::Checkbox("Anty-Cheat", &checkbox_ac);
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::Text("Windows");
        ImGui::Checkbox("Temp", &checkbox_temp);
        ImGui::SameLine();
        ImGui::Checkbox("Logs", &checkbox_logs);
        ImGui::SameLine();
        ImGui::Checkbox("Update", &checkbox_update);
        ImGui::SameLine();
        ImGui::Checkbox("Events", &checkbox_events);
        ImGui::SameLine();
        ImGui::Checkbox("Recent", &checkbox_recent);
        ImGui::SameLine();
        ImGui::Checkbox("Memory", &checkbox_memory);
        ImGui::SameLine();
        ImGui::Checkbox("Registry", &checkbox_registry);
        ImGui::Checkbox("History", &checkbox_history);
        ImGui::SameLine();
        ImGui::Checkbox("Prefetch", &checkbox_prefetch);
        ImGui::SameLine();
        ImGui::Checkbox("Regseeker", &checkbox_regseeker);
        ImGui::SameLine();
        ImGui::Checkbox("Crash Dumps", &checkbox_crashdumps);
        ImGui::SameLine();
        ImGui::Checkbox("Download File", &checkbox_downloadfile);
        ImGui::Checkbox("Last Activity", &checkbox_lastactivity);
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::Text("Other");
        ImGui::Checkbox("DNS cache", &checbox_dnscache);
        ImGui::SameLine();
        ImGui::Checkbox("Steam acc", &checbox_steamacc);
        ImGui::SameLine();
        ImGui::Checkbox("Drive optimizer", &checbox_driveop);
        ImGui::SameLine();
        ImGui::Checkbox("Fivem optimalize", &checbox_fivemop);
        ImGui::Checkbox("System restore point", &checbox_systemrestorepoint);
        ImGui::SameLine();

        ImGui::Spacing();
        ImGui::Separator();

        if (ImGui::Button("Clean")) 
        {
            if (checkbox_brave)
            {
                ClearBrave();
            }
            if (checkbox_mozilla) 
            {
                ClearMozilla();
            }
            if (checkbox_operagx) 
            {
                ClearOperaGX();
            }
            if (checkbox_chrome)
            {
                ClearChrome();
            }
            if (checkbox_fivem)
            {
                ClearFiveM();
            }
            if (checkbox_valorant) 
            {
                ClearValorant();
            }
            if (checkbox_fortnite)
            {
                ClearFortnite();
            }
            if (checkbox_ac)
            {
                ClearAC();
            }
            if (checkbox_temp)
            {
                ClearTemp();
            }
            if (checkbox_logs)
            {
                ClearLogs();
            }
            if (checkbox_update)
            {
                ClearUpdate();
            }
            if (checkbox_events)
            {
                ClearEvents();
            }
            if (checkbox_recent) 
            {
                ClearRecent();
            }
            if (checkbox_memory) 
            {
                ClearMemory();
            }
            if (checkbox_registry)
            {
                ClearRegistry();
            }
            if (checkbox_history) 
            {
                ClearHistory();
            }
            if (checkbox_prefetch) 
            {
                ClearPrefetch();
            }
            if (checkbox_regseeker)
            {
                ClearRegSeeker();
            }
            if (checkbox_crashdumps) 
            {
                ClearCrashDumps();
            }
            if (checkbox_downloadfile)
            {
                ClearDownloadFile();
            }
            if (checkbox_lastactivity) 
            {
                ClearLastActivity();
            }
            if (checbox_dnscache)
            {
                ClearDnsCache();
            }
            if (checbox_steamacc)
            {
                ClearSteamAccount();
            }
            if (checbox_driveop)
            {
                ClearDriveOP();
            }
            if (checbox_fivemop)
            {
                ClearFivemOP();
            }
            if (checbox_systemrestorepoint)
            {
                ClearSystemRestorePoints();
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Clean all"))
        {
            ClearBrave();
            ClearMozilla();
            ClearOperaGX();
            ClearChrome();
            ClearFiveM();
            ClearValorant();
            ClearFortnite();
            ClearAC();
            ClearTemp();
            ClearLogs();
            ClearUpdate();
            ClearEvents();
            ClearRecent();
            ClearMemory();
            ClearRegistry();
            ClearHistory();
            ClearPrefetch();
            ClearRegSeeker();
            ClearCrashDumps();
            ClearDownloadFile();
            ClearLastActivity();
            ClearDnsCache();
            ClearSteamAccount();
            ClearDriveOP();
            ClearFivemOP();
            ClearSystemRestorePoints();
        }

        ImGui::SameLine();
        if (ImGui::Button("Open console logs"))
        {
            AllocConsole();
            FILE* pCout;
            freopen_s(&pCout, "CONOUT$", "w", stdout);
            printf("Solar console logs [OPEN]\n");
            ShowWindow(GetConsoleWindow(), SW_SHOW);
        }

        ImGui::End();
        if (!show_window) 
        {
            PostQuitMessage(0);  
        }
    }
}

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;
    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg) {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"Loader base", NULL };
    RegisterClassEx(&wc);
    main_hwnd = CreateWindow(wc.lpszClassName, L"Loader base", WS_POPUP, 0, 0, 5, 5, NULL, NULL, wc.hInstance, NULL);


    if (!CreateDeviceD3D(main_hwnd))
    {
        CleanupDeviceD3D();
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ShowWindow(main_hwnd, SW_HIDE);
    UpdateWindow(main_hwnd);

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplWin32_Init(main_hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    bool isActive = true;
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) 
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        static bool initialized = false;
        if (!initialized) 
        {
            ui::init(g_pd3dDevice);
            initialized = true;
        }
        ui::render();

        ImGui::EndFrame();

        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0) 
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) 
        {
            ResetDevice();
        }
        if (!isActive) 
        {
            msg.message = WM_QUIT;
        }
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    DestroyWindow(main_hwnd);
    UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}