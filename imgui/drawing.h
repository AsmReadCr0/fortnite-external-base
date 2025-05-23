#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>

class drawing_
{
public:
	void outlined_text(ImDrawList* drawlist, int x, int y, ImColor Color, const char* text)
	{
		ImVec2 offsets[] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

		for (const auto& offset : offsets)
		{
			drawlist->AddText(ImVec2(x + offset.x, y + offset.y), ImColor(0, 0, 0, 255), text);
		}

		drawlist->AddText(ImVec2(x, y), Color, text);
	}

	void box_2d(ImDrawList* drawlist, float X, float Y, float W, float H, ImColor col, int thickness, bool filled)
	{
		if (filled)
		{
			drawlist->AddRectFilled(ImVec2(X, Y), ImVec2(X + W, Y + H), ImColor(255, 255, 255, 85), 0, 0);
		}

		drawlist->AddRect(ImVec2(X, Y), ImVec2(X + W, Y + H), col, 0, 0, thickness);
	}

    auto image_box(int X, int Y, int W, int H, const ImU32& color, int thickness, ImTextureID texture) -> void
    {
        float lineW = static_cast<float>(W) / 3;
        float lineH = static_cast<float>(H) / 3;

        float uvX = 0.0f;
        float uvY = 0.0f;
        float uvW = 1.0f;
        float uvH = 1.0f;

        ImGui::GetBackgroundDrawList()->AddImage(texture, ImVec2(X, Y), ImVec2(X + lineW, Y + lineH), ImVec2(uvX, uvY), ImVec2(uvX + uvW / 3, uvY + uvH / 3));
        ImGui::GetBackgroundDrawList()->AddImage(texture, ImVec2(X + W - lineW, Y), ImVec2(X + W, Y + lineH), ImVec2(uvX + 2 * uvW / 3, uvY), ImVec2(uvX + uvW, uvY + uvH / 3));
        ImGui::GetBackgroundDrawList()->AddImage(texture, ImVec2(X, Y + H - lineH), ImVec2(X + lineW, Y + H), ImVec2(uvX, uvY + 2 * uvH / 3), ImVec2(uvX + uvW / 3, uvY + uvH));
        ImGui::GetBackgroundDrawList()->AddImage(texture, ImVec2(X + W - lineW, Y + H - lineH), ImVec2(X + W, Y + H), ImVec2(uvX + 2 * uvW / 3, uvY + 2 * uvH / 3), ImVec2(uvX + uvW, uvY + uvH));
        ImGui::GetBackgroundDrawList()->AddImage(texture, ImVec2(X, Y + lineH), ImVec2(X + lineW, Y + H - lineH), ImVec2(uvX, uvY + uvH / 3), ImVec2(uvX + uvW / 3, uvY + 2 * uvH / 3));
        ImGui::GetBackgroundDrawList()->AddImage(texture, ImVec2(X + W - lineW, Y + lineH), ImVec2(X + W, Y + H - lineH), ImVec2(uvX + 2 * uvW / 3, uvY + uvH / 3), ImVec2(uvX + uvW, uvY + 2 * uvH / 3));
        ImGui::GetBackgroundDrawList()->AddImage(texture, ImVec2(X + lineW, Y), ImVec2(X + W - lineW, Y + lineH), ImVec2(uvX + uvW / 3, uvY), ImVec2(uvX + 2 * uvW / 3, uvY + uvH / 3));
        ImGui::GetBackgroundDrawList()->AddImage(texture, ImVec2(X + lineW, Y + H - lineH), ImVec2(X + W - lineW, Y + H), ImVec2(uvX + uvW / 3, uvY + 2 * uvH / 3), ImVec2(uvX + 2 * uvW / 3, uvY + uvH));
        ImGui::GetBackgroundDrawList()->AddImage(texture, ImVec2(X + lineW, Y + lineH), ImVec2(X + W - lineW, Y + H - lineH), ImVec2(uvX + uvW / 3, uvY + uvH / 3), ImVec2(uvX + 2 * uvW / 3, uvY + 2 * uvH / 3));
    }
};
inline drawing_ drawing;