#include "styles.h"

int N = 650;
int lineMaxDist = 1000;
ImColor lineCol = { 45, 45, 45, 255 };
float lineThickness = 2.0f;

void setupPoints(std::vector<std::pair<ImVec2, ImVec2>>& n) {
    ImVec2 screenSize(ImGui::GetIO().DisplaySize);
    for (auto& p : n)
        p.second = p.first = ImVec2(rand() % (int)screenSize.x, rand() % (int)screenSize.y);
}
float length(ImVec2 x) { return x.x * x.x + x.y * x.y; }
void FX(ImDrawList* d, ImVec2 b)
{
    static std::vector<std::pair<ImVec2, ImVec2>> points(N);
    static auto once = (setupPoints(points), true);
    float Dist;
    for (auto& p : points) {
        Dist = sqrt(length(p.first - p.second));
        if (Dist > 0) p.first += (p.second - p.first) / Dist;
        if (Dist < 4) p.second = ImVec2(rand() % (int)b.x, rand() % (int)b.y);
    }
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            Dist = length(points[i].first - points[j].first);
            if (Dist < lineMaxDist) d->AddLine(points[i].first, points[j].first, lineCol, lineThickness);
        }
    }
}

ImColor menu_rgb()
{

    {
        if (flHue > 255.0f)
            flHue = 0.0f;
    }

    flHue += (rgb_speed / ImGui::GetIO().Framerate);

    ImColor EspPnDColorsrgb_menu = ImColor::HSV(flHue, 1.f, 1.f, 0.7f);
    return EspPnDColorsrgb_menu;
}

void runstyle()
{

    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    vec2_t res = { desktop.right, desktop.bottom };

    auto& style = ImGui::GetStyle();

    ImVec4* colors = ImGui::GetStyle().Colors;

    ImGui::SetNextWindowSize(ImVec2(518, 490), ImGuiCond_Once);


    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.14f, 0.14f, 0.14f, 0.92f);
    colors[ImGuiCol_Border] = ImVec4(0.14f, 0.14f, 0.14f, 0.29f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.14f, 0.14f, 0.14f, 0.54f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImColor(255, 174, 0, 255);
    colors[ImGuiCol_TitleBgActive] = ImColor(255, 174, 0, 255);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImColor(255, 174, 0, 255);;
    colors[ImGuiCol_CheckMark] = ImColor(255, 174, 0, 255);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_SliderGrabActive] = ImColor(255, 174, 0, 255);
    colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.14f, 0.14f, 0.14f, 0.54f);
    colors[ImGuiCol_ButtonActive] = ImColor(255, 174, 0, 255);
    colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
    colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabActive] = ImColor(255, 174, 0, 255);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

    style.WindowPadding = ImVec2(6, 4);
    style.WindowRounding = 10.f;
    style.FramePadding = ImVec2(5, 2);
    style.FrameRounding = 4.0f;
    style.ItemSpacing = ImVec2(7, 1);
    style.ItemInnerSpacing = ImVec2(1, 1);
    style.TouchExtraPadding = ImVec2(0, 0);
    style.IndentSpacing = 6.0f;
    style.ScrollbarSize = 12.0f;
    style.ScrollbarRounding = 16.0f;
    style.GrabMinSize = 20.0f;
    style.GrabRounding = 2.0f;
    style.WindowTitleAlign.x = 0.50f;
    style.FrameBorderSize = 0.0f;
    style.WindowBorderSize = 1.0f;
    style.ItemInnerSpacing = ImVec2(5, 0);

}

void alternatestyle()
{

    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    vec2_t res = { desktop.right, desktop.bottom };

    auto& style = ImGui::GetStyle();

    ImGui::SetNextWindowSize(ImVec2(518, 490), ImGuiCond_Once);

    style.Colors[ImGuiCol_Separator] = ImColor(20, 20, 20, 200);
    style.Colors[ImGuiCol_SeparatorActive] = ImColor(20, 20, 20, 200);
    style.Colors[ImGuiCol_SeparatorHovered] = ImColor(20, 20, 20, 200);
    style.Colors[ImGuiCol_PlotLines] = ImColor(210, 210, 210, 255);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImColor(255, 174, 0, 255);
    style.Colors[ImGuiCol_PlotHistogram] = ImColor(210, 210, 210, 255);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImColor(255, 174, 0, 255);
    style.Colors[ImGuiCol_Text] = ImColor(245, 245, 245, 255);
    style.Colors[ImGuiCol_Border] = ImColor(18, 18, 18, 255);
    style.Colors[ImGuiCol_WindowBg] = ImColor(30, 30, 30, 255);
    style.Colors[ImGuiCol_TitleBg] = ImColor(255, 174, 0, 200);
    style.Colors[ImGuiCol_TitleBgActive] = ImColor(255, 174, 0, 200);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(255, 174, 0, 200);
    style.Colors[ImGuiCol_Button] = ImColor(20, 20, 20, 200);			// button color
    style.Colors[ImGuiCol_ButtonActive] = ImColor(255, 174, 0, 200);	// button color when pressed
    style.Colors[ImGuiCol_ButtonHovered] = ImColor(100, 100, 100, 100);
    style.Colors[ImGuiCol_CheckMark] = ImColor(255, 174, 0, 200);
    style.Colors[ImGuiCol_FrameBg] = ImColor(18, 18, 18, 255);
    style.Colors[ImGuiCol_FrameBgActive] = ImColor(18, 18, 18, 255);
    style.Colors[ImGuiCol_FrameBgHovered] = ImColor(18, 18, 18, 255);
    style.Colors[ImGuiCol_Header] = ImColor(255, 174, 0, 200);			// for collapsing headers , etc
    style.Colors[ImGuiCol_HeaderActive] = ImColor(24, 24, 24, 255);
    style.Colors[ImGuiCol_HeaderHovered] = ImColor(255, 174, 0, 200);
    style.Colors[ImGuiCol_ResizeGrip] = ImColor(24, 24, 24, 255);		// the resize grip thing bottom right
    style.Colors[ImGuiCol_ResizeGripActive] = ImColor(24, 24, 24, 255);	// when you hold it / active
    style.Colors[ImGuiCol_ResizeGripHovered] = ImColor(24, 24, 24, 255);
    style.Colors[ImGuiCol_SliderGrab] = ImColor(255, 174, 0, 200);
    style.Colors[ImGuiCol_SliderGrabActive] = ImColor(255, 174, 0, 200);
    style.Colors[ImGuiCol_Tab] = ImColor(255, 174, 0, 200);
    style.Colors[ImGuiCol_MenuBarBg] = ImColor(255, 174, 0, 200);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_TabActive] = ImColor(5, 5, 5, 255);
    style.Colors[ImGuiCol_TabUnfocused] = ImColor(255, 174, 0, 200);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImColor(5, 5, 5, 255);
    style.Colors[ImGuiCol_TextSelectedBg] = ImColor(255, 174, 0, 200);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImColor(255, 174, 0, 255);;


    style.WindowPadding = ImVec2(6, 4);
    style.WindowRounding = 10.f;
    style.FramePadding = ImVec2(5, 2);
    style.FrameRounding = 4.0f;
    style.ItemSpacing = ImVec2(7, 1);
    style.ItemInnerSpacing = ImVec2(1, 1);
    style.TouchExtraPadding = ImVec2(0, 0);
    style.IndentSpacing = 6.0f;
    style.ScrollbarSize = 12.0f;
    style.ScrollbarRounding = 16.0f;
    style.GrabMinSize = 20.0f;
    style.GrabRounding = 2.0f;
    style.WindowTitleAlign.x = 0.50f;
    style.FrameBorderSize = 0.0f;
    style.WindowBorderSize = 1.0f;
    style.ItemInnerSpacing = ImVec2(5, 0);

}

void blackstyle()
{

    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    vec2_t res = { desktop.right, desktop.bottom };

    auto& style = ImGui::GetStyle();

    ImVec4* colors = ImGui::GetStyle().Colors;

    ImGui::SetNextWindowSize(ImVec2(518, 490), ImGuiCond_Once);

    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.14f, 0.14f, 0.14f, 0.92f);
    colors[ImGuiCol_Border] = ImVec4(0.14f, 0.14f, 0.14f, 0.29f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.14f, 0.14f, 0.14f, 0.54f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImColor(255, 174, 0, 255);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_CheckMark] = ImColor(255, 174, 0, 255);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.14f, 0.14f, 0.14f, 0.54f);
    colors[ImGuiCol_ButtonActive] = ImColor(255, 174, 0, 255);
    colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
    colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

    style.WindowPadding = ImVec2(6, 4);
    style.WindowRounding = 10.f;
    style.FramePadding = ImVec2(5, 2);
    style.FrameRounding = 4.0f;
    style.ItemSpacing = ImVec2(7, 1);
    style.ItemInnerSpacing = ImVec2(1, 1);
    style.TouchExtraPadding = ImVec2(0, 0);
    style.IndentSpacing = 6.0f;
    style.ScrollbarSize = 12.0f;
    style.ScrollbarRounding = 16.0f;
    style.GrabMinSize = 20.0f;
    style.GrabRounding = 2.0f;
    style.WindowTitleAlign.x = 0.50f;
    style.FrameBorderSize = 0.0f;
    style.WindowBorderSize = 1.0f;
    style.ItemInnerSpacing = ImVec2(5, 0);

}

void rgbstyle()
{
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    vec2_t res = { desktop.right, desktop.bottom };

    ImColor menu_rgb_c = menu_rgb();

    auto& style = ImGui::GetStyle();

    ImGui::SetNextWindowSize(ImVec2(518, 490), ImGuiCond_Once);

    style.Colors[ImGuiCol_Separator] = ImColor(20, 20, 20, 200);
    style.Colors[ImGuiCol_SeparatorActive] = ImColor(20, 20, 20, 200);
    style.Colors[ImGuiCol_SeparatorHovered] = ImColor(20, 20, 20, 200);
    style.Colors[ImGuiCol_PlotLines] = ImColor(210, 210, 210, 255);
    style.Colors[ImGuiCol_PlotLinesHovered] = menu_rgb_c;
    style.Colors[ImGuiCol_PlotHistogram] = ImColor(210, 210, 210, 255);
    style.Colors[ImGuiCol_PlotHistogramHovered] = menu_rgb_c;
    style.Colors[ImGuiCol_Text] = ImColor(245, 245, 245, 255);
    style.Colors[ImGuiCol_Border] = ImColor(18, 18, 18, 255);
    style.Colors[ImGuiCol_WindowBg] = ImColor(30, 30, 30, 255);
    style.Colors[ImGuiCol_TitleBg] = menu_rgb_c;
    style.Colors[ImGuiCol_TitleBgActive] = menu_rgb_c;
    style.Colors[ImGuiCol_TitleBgCollapsed] = menu_rgb_c;
    style.Colors[ImGuiCol_Button] = ImColor(20, 20, 20, 200);			// button color
    style.Colors[ImGuiCol_ButtonActive] = menu_rgb_c;	// button color when pressed
    style.Colors[ImGuiCol_ButtonHovered] = ImColor(100, 100, 100, 100);
    style.Colors[ImGuiCol_CheckMark] = menu_rgb_c;
    style.Colors[ImGuiCol_FrameBg] = ImColor(18, 18, 18, 255);
    style.Colors[ImGuiCol_FrameBgActive] = ImColor(18, 18, 18, 255);
    style.Colors[ImGuiCol_FrameBgHovered] = ImColor(18, 18, 18, 255);
    style.Colors[ImGuiCol_Header] = menu_rgb_c;			// for collapsing headers , etc
    style.Colors[ImGuiCol_HeaderActive] = ImColor(24, 24, 24, 255);
    style.Colors[ImGuiCol_HeaderHovered] = menu_rgb_c;
    style.Colors[ImGuiCol_ResizeGrip] = ImColor(24, 24, 24, 255);		// the resize grip thing bottom right
    style.Colors[ImGuiCol_ResizeGripActive] = menu_rgb_c;	// when you hold it / active
    style.Colors[ImGuiCol_ResizeGripHovered] = ImColor(24, 24, 24, 255);
    style.Colors[ImGuiCol_SliderGrab] = menu_rgb_c;
    style.Colors[ImGuiCol_SliderGrabActive] = menu_rgb_c;
    style.Colors[ImGuiCol_Tab] = menu_rgb_c;
    style.Colors[ImGuiCol_MenuBarBg] = menu_rgb_c;
    style.Colors[ImGuiCol_TabHovered] = ImColor(5, 5, 5, 255);
    style.Colors[ImGuiCol_TabActive] = ImColor(5, 5, 5, 255);
    style.Colors[ImGuiCol_TabUnfocused] = menu_rgb_c;
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImColor(5, 5, 5, 255);
    style.Colors[ImGuiCol_TextSelectedBg] = menu_rgb_c;

    style.WindowPadding = ImVec2(6, 4);
    style.WindowRounding = 10.f;
    style.FramePadding = ImVec2(5, 2);
    style.FrameRounding = 4.0f;
    style.ItemSpacing = ImVec2(7, 1);
    style.ItemInnerSpacing = ImVec2(1, 1);
    style.TouchExtraPadding = ImVec2(0, 0);
    style.IndentSpacing = 6.0f;
    style.ScrollbarSize = 12.0f;
    style.ScrollbarRounding = 16.0f;
    style.GrabMinSize = 20.0f;
    style.GrabRounding = 2.0f;
    style.WindowTitleAlign.x = 0.50f;
    style.FrameBorderSize = 0.0f;
    style.WindowBorderSize = 1.0f;
    style.ItemInnerSpacing = ImVec2(5, 0);
}
