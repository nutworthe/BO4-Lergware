#include "overlay.h"

ImColor esp_rgb()
{

	{
		if (flHue > 255.0f)
			flHue = 0.0f;
	}

	flHue += (rgb_speed / ImGui::GetIO().Framerate);

	ImColor EspPnDColorsrgb = ImColor::HSV(flHue, 1.f, 1.f, 1.f);
	return EspPnDColorsrgb;
}

ImColor rgb_bg()
{

	{
		if (flHue > 255.0f)
			flHue = 0.0f;
	}

	ImColor EspPnDColorsrgb_bg = ImColor::HSV(flHue, 1.f, 1.f, 0.3f);
	return EspPnDColorsrgb_bg;
}

namespace overlay
{
	void draw()
	{
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);
		vec2_t res = { desktop.right, desktop.bottom };

		ImColor esp_rgb_c = esp_rgb();
		ImColor rgb_bg_c = rgb_bg();

		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoBackground;
		window_flags |= ImGuiWindowFlags_NoTitleBar;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
		window_flags |= ImGuiWindowFlags_NoInputs;
		window_flags |= ImGuiWindowFlags_NoDecoration;
		ImGui::SetNextWindowSize(ImVec2(res.x, res.y));
		ImGui::Begin("overlay", NULL, window_flags);
		ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

		for (auto& p : players)
		{
			float height = p.bone_array.head.y - p.bone_array.origin.y;
			float width = height / 2;

			ImColor main_color;

			if (rgb_enemy && p.team == false) {
				main_color = esp_rgb_c;
			}
			else if (p.team && rgb_friendly && !enemies_only) {
				main_color = esp_rgb_c;
			}
			else if (p.team) {
				main_color = ImColor(friendly_colors);
			}
			else if (!p.team && !p.targeted) {
				main_color = ImColor(enemy_colors);
			}
			else if (!p.team && p.targeted)
			{
				main_color = ImColor(255, 215, 0);
			}

			ImColor healthBar_c;
			if (p.health > 125) {
				healthBar_c = ImColor(0, 255, 0);
			}
			else if (p.health > 90) {
				healthBar_c = ImColor(255, 164, 0);
			}
			else if (p.health > 50) {
				healthBar_c = ImColor(252, 76, 2);
			}
			else if (p.health > 25) {
				healthBar_c = ImColor(226, 82, 47);
			}
			else {
				healthBar_c = ImColor(255, 0, 0);
			}

			if (!visuals_active) continue;
			if (enemies_only && p.team) continue;

			if (unfilled_boxes)
			{
				draw_list->AddRect(ImVec2(p.bone_array.origin.x - (width / 1.35) - 1, p.bone_array.origin.y - 1), ImVec2(p.bone_array.origin.x + (width / 1.35) - 1, p.bone_array.head.y - 1), ImColor(0, 0, 0));

				draw_list->AddRect(ImVec2(p.bone_array.origin.x - (width / 1.35) + 1, p.bone_array.origin.y + 1), ImVec2(p.bone_array.origin.x + (width / 1.35) + 1, p.bone_array.head.y + 1), ImColor(0, 0, 0));

				draw_list->AddRect(ImVec2(p.bone_array.origin.x - (width / 1.35), p.bone_array.origin.y), ImVec2(p.bone_array.origin.x + (width / 1.35), p.bone_array.head.y), main_color);
			}

			if (filled_boxes)
			{
				draw_list->AddRectFilled(ImVec2(p.bone_array.origin.x - (width / 1.35), p.bone_array.origin.y), ImVec2(p.bone_array.origin.x + (width / 1.35), p.bone_array.head.y), main_color - ImColor(0, 0, 0, 180));
			}

			if (corner_esp) {
				draw_list->AddLine(ImVec2(p.bone_array.origin.x - (width / 1.35), p.bone_array.origin.y), ImVec2(p.bone_array.origin.x - (width / 4), p.bone_array.origin.y), main_color, boldness); // br to middle

				draw_list->AddLine(ImVec2(p.bone_array.origin.x - (width / 1.35), p.bone_array.origin.y), ImVec2(p.bone_array.origin.x - (width / 1.35), p.bone_array.origin.y + (width / 2)), main_color, boldness); // br to tr middle

				draw_list->AddLine(ImVec2(p.bone_array.origin.x + (width / 1.35), p.bone_array.origin.y), ImVec2(p.bone_array.origin.x + (width / 4), p.bone_array.origin.y), main_color, boldness); // bl to middle

				draw_list->AddLine(ImVec2(p.bone_array.origin.x + (width / 1.35), p.bone_array.origin.y), ImVec2(p.bone_array.origin.x + (width / 1.35), p.bone_array.origin.y + (width / 2)), main_color, boldness); // bl to tl middle

				draw_list->AddLine(ImVec2(p.bone_array.origin.x - (width / 1.35), p.bone_array.head.y), ImVec2(p.bone_array.origin.x - (width / 4), p.bone_array.head.y), main_color, boldness); // tr to middle

				draw_list->AddLine(ImVec2(p.bone_array.origin.x - (width / 1.35), p.bone_array.head.y), ImVec2(p.bone_array.origin.x - (width / 1.35), p.bone_array.head.y - (width / 2)), main_color, boldness); // tr to br middle

				draw_list->AddLine(ImVec2(p.bone_array.origin.x + (width / 1.35), p.bone_array.head.y), ImVec2(p.bone_array.origin.x + (width / 4), p.bone_array.head.y), main_color, boldness); // tl to middle

				draw_list->AddLine(ImVec2(p.bone_array.origin.x + (width / 1.35), p.bone_array.head.y), ImVec2(p.bone_array.origin.x + (width / 1.35), p.bone_array.head.y - (width / 2)), main_color, boldness); // tl to bl middle
			}

			if (bone_esp)
			{
				//bones
				draw_list->AddLine(ImVec2(p.bone_array.j_neckpos_screen.x, p.bone_array.j_neckpos_screen.y), ImVec2(p.bone_array.j_spineupperpos_screen.x, p.bone_array.j_spineupperpos_screen.y), main_color, boldness);
				draw_list->AddLine(ImVec2(p.bone_array.j_spineupperpos_screen.x, p.bone_array.j_spineupperpos_screen.y), ImVec2(p.bone_array.j_spinelowerpos_screen.x, p.bone_array.j_spinelowerpos_screen.y), main_color, boldness);

				//right top
				draw_list->AddLine(ImVec2(p.bone_array.j_neckpos_screen.x, p.bone_array.j_neckpos_screen.y), ImVec2(p.bone_array.j_shoulder_ripos_screen.x, p.bone_array.j_shoulder_ripos_screen.y), main_color, boldness);
				draw_list->AddLine(ImVec2(p.bone_array.j_shoulder_ripos_screen.x, p.bone_array.j_shoulder_ripos_screen.y), ImVec2(p.bone_array.j_elbow_ripos_screen.x, p.bone_array.j_elbow_ripos_screen.y), main_color, boldness);
				draw_list->AddLine(ImVec2(p.bone_array.j_elbow_ripos_screen.x, p.bone_array.j_elbow_ripos_screen.y), ImVec2(p.bone_array.j_wrist_ripos_screen.x, p.bone_array.j_wrist_ripos_screen.y), main_color, boldness);

				//right bottom
				draw_list->AddLine(ImVec2(p.bone_array.j_spinelowerpos_screen.x, p.bone_array.j_spinelowerpos_screen.y), ImVec2(p.bone_array.j_knee_ripos_screen.x, p.bone_array.j_knee_ripos_screen.y), main_color, boldness);
				draw_list->AddLine(ImVec2(p.bone_array.j_knee_ripos_screen.x, p.bone_array.j_knee_ripos_screen.y), ImVec2(p.bone_array.j_ankle_ripos_screen.x, p.bone_array.j_ankle_ripos_screen.y), main_color, boldness);


				//left top
				draw_list->AddLine(ImVec2(p.bone_array.j_neckpos_screen.x, p.bone_array.j_neckpos_screen.y), ImVec2(p.bone_array.j_shoulder_lepos_screen.x, p.bone_array.j_shoulder_lepos_screen.y), main_color, boldness);
				draw_list->AddLine(ImVec2(p.bone_array.j_shoulder_lepos_screen.x, p.bone_array.j_shoulder_lepos_screen.y), ImVec2(p.bone_array.j_elbow_lepos_screen.x, p.bone_array.j_elbow_lepos_screen.y), main_color, boldness);
				draw_list->AddLine(ImVec2(p.bone_array.j_elbow_lepos_screen.x, p.bone_array.j_elbow_lepos_screen.y), ImVec2(p.bone_array.j_wrist_lepos_screen.x, p.bone_array.j_wrist_lepos_screen.y), main_color, boldness);

				//left bottom
				draw_list->AddLine(ImVec2(p.bone_array.j_spinelowerpos_screen.x, p.bone_array.j_spinelowerpos_screen.y), ImVec2(p.bone_array.j_knee_lepos_screen.x, p.bone_array.j_knee_lepos_screen.y), main_color, boldness);
				draw_list->AddLine(ImVec2(p.bone_array.j_knee_lepos_screen.x, p.bone_array.j_knee_lepos_screen.y), ImVec2(p.bone_array.j_ankle_lepos_screen.x, p.bone_array.j_ankle_lepos_screen.y), main_color, boldness);
			}

			if (name_esp)
				draw_list->AddText(ImVec2(p.bone_array.origin.x - (ImGui::CalcTextSize(p.player_name).x / 2), p.bone_array.origin.y), main_color, p.player_name);

			if (healthbars)
			{
				float healthHeight = (height) * (p.health / 150.f);

				draw_list->AddLine(ImVec2(p.bone_array.origin.x - (width / 1.35) + 3, p.bone_array.origin.y), ImVec2(p.bone_array.origin.x - (width / 1.35) + 3, p.bone_array.head.y), ImColor(0, 0, 0, 255), boldness);

				draw_list->AddLine(ImVec2(p.bone_array.origin.x - (width / 1.35) + 3, p.bone_array.origin.y), ImVec2(p.bone_array.origin.x - (width / 1.35) + 3, p.bone_array.head.y), ImColor(30, 30, 30, 255), boldness);

				draw_list->AddLine(ImVec2(p.bone_array.origin.x - (width / 1.35) + 3, p.bone_array.origin.y), ImVec2(p.bone_array.origin.x - (width / 1.35) + 3, p.bone_array.head.y - height + healthHeight), healthBar_c, boldness);
			}

			if (snaplines)
			{
				draw_list->AddLine(ImVec2(res.x / 2, res.y - 1), ImVec2(p.bone_array.origin.x, p.bone_array.origin.y), main_color);
			}
		}

		for (auto& tracer : tracers)
		{
			if (tracer.opacity <= 0 || !BulletTracers) continue;

			draw_list->AddLine(ImVec2(tracer.start_screen.x, tracer.start_screen.y), ImVec2(tracer.end_screen.x, tracer.end_screen.y), ImColor(esp_rgb_c.Value.x, esp_rgb_c.Value.y, esp_rgb_c.Value.z, tracer.opacity));

			if (tracer_type == 0)
			{
				draw_list->AddLine(ImVec2(tracer.end_screen.x - tracer.radius, tracer.end_screen.y - tracer.radius), ImVec2(tracer.end_screen.x + tracer.radius, tracer.end_screen.y + tracer.radius), ImColor(esp_rgb_c.Value.x, esp_rgb_c.Value.y, esp_rgb_c.Value.z, tracer.opacity));
				draw_list->AddLine(ImVec2(tracer.end_screen.x - tracer.radius, tracer.end_screen.y + tracer.radius), ImVec2(tracer.end_screen.x + tracer.radius, tracer.end_screen.y - tracer.radius), ImColor(esp_rgb_c.Value.x, esp_rgb_c.Value.y, esp_rgb_c.Value.z, tracer.opacity));
			}
			else if (tracer_type == 1)
				draw_list->AddCircle(ImVec2(tracer.end_screen.x, tracer.end_screen.y), tracer.radius, ImColor(esp_rgb_c.Value.x, esp_rgb_c.Value.y, esp_rgb_c.Value.z, tracer.opacity), 360);

			tracer.opacity -= 3.5;
		}

		ImGui::End();
	}
}