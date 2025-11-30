#include "imgui.h"
#include <iostream>

#include <uTypedef.h>

class TsubMenu : public TmenuHandle{
	public:
		sdds_var(Tint32,value0)
		sdds_var(Tfloat32,value1)
};

class TuserStruct : public TmenuHandle{
	public:
		sdds_var(Tint32,counter)
		sdds_var(Tfloat32,sliderVal)
		sdds_var(TsubMenu,sub)
		sdds_var(Tstring,str,0,"ajsdlkfjasölkdjfölsyjdf")
		TuserStruct(){
			timer.start(10);
			on(timer){
				timer.start(1000);
				counter++;
			};
		}
	private:
		Ttimer timer;

} userStruct;

#if USE_UDP_COMM 
#include <uUdpCommHandler.h>
TudpCommHandler udch(userStruct);
#endif

void sddsTreeTable(TmenuHandle* node){
	if (!node) return;

	for (auto it = node->iterator(); it.hasCurrent(); it.jumpToNext())
	{
		auto d = it.current();
		const char* name = d->name();

		ImGui::TableNextRow();
		ImGui::TableNextColumn();

		if (d->isStruct())
		{
			ImGui::PushID(d);

			bool open = ImGui::TreeNodeEx(
				name,
				ImGuiTreeNodeFlags_SpanFullWidth
			);

			ImGui::TableNextColumn();
			//ImGui::TextUnformatted("<Struct>");

			if (open)
			{
				sddsTreeTable(static_cast<Tstruct*>(d)->value());
				ImGui::TreePop();
			}

			ImGui::PopID();
		}
		else
		{
			ImGui::PushID(d);

			ImGui::BulletText("%s", name);

			ImGui::TableNextColumn();

			switch (d->type()){
			case sdds::Ttype::INT32:
			{
				auto v = static_cast<Tint32*>(d);
				if (ImGui::InputInt("##int", &v->Fvalue))
				{
					d->signalEvents();
				}
				break;
			}

			case sdds::Ttype::FLOAT32:
			{
				auto f = static_cast<Tfloat32*>(d);
				if (ImGui::InputFloat("##float", &f->Fvalue, 0.0f, 0.0f, "%.3f"))
				{
					f->signalEvents();
				}
				break;
			}

			case sdds::Ttype::STRING:
			{
				std::string s = d->to_string();
				char buffer[256];
				strncpy(buffer, s.c_str(), sizeof(buffer));
				buffer[255] = 0;

				if (ImGui::InputText("##str", buffer, sizeof(buffer)))
				{
					d->setValue(buffer);
				}
				break;
			}

			default:
				ImGui::TextUnformatted(d->to_string().c_str());
				break;
			}

			ImGui::PopID();
		}
	}
}



bool show_another_window = false;
bool show_demo_window = true;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void loop(){
	TtaskHandler::handleEvents();
	//logStruct(&userStruct);
	{
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		static float f = 0.0f;
		static int counter = 0;

        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);
		ImGui::Text("sdds_counter = %d", userStruct.counter.Fvalue);


		if (ImGui::SliderFloat("sliderVal", &userStruct.sliderVal.Fvalue, 0.0f, 100.0f)){
			userStruct.sliderVal.signalEvents();
		}

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

	if (ImGui::Begin("SDDS-Browser"))
	{
		if (ImGui::BeginTable("treeTable", 2,
			ImGuiTableFlags_Resizable |
			ImGuiTableFlags_BordersInnerV |
			ImGuiTableFlags_RowBg |
			ImGuiTableFlags_ScrollY))
		{
			ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableHeadersRow();

			sddsTreeTable(&userStruct);

			ImGui::EndTable();
		}

		ImGui::End();
	}

}