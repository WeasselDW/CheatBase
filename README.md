# External CheatBase
### Windows only! (Only tested on windows 11)
External CheatBase is a basis of which a external cheat can be made, it utilizes the ImGui library(directX11) for the graphics (esp) and for the Menu (GUI). A Object Oriented Programing style is applied to the Graphics and the Memory.

## Installation

Download/Clone this project, then in Memory.cpp change the Cheatname variable to the exe name of the game.

Memory.cpp
```cpp
LPCWSTR GameName = L"game.exe";
```

change the cheat's window name at Graphics.cpp at the little arrow.
Graphics.cpp
```cpp
		window = CreateWindowExW(
			WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
			wc.lpszClassName,
	>       	L"CANGECREATNAME",
			WS_POPUP,
			0, 0, screenWidth, screeHeight,
			nullptr,
			nullptr,
			wc.hInstance,
			nullptr
		);
```

Be sure to use the right platform (x86 for 32 bit games and x64 for 64 bit games)
## Usage
Graphics.cpp
```cpp
	// draw code goes here |
	//  		      \|/

		// Menu code here
		if (MenuOpen) {
			ImGui::Begin("Test window");
			ImGui::Text("Hello");
			ImGui::Checkbox("Toggle", &some_variable);
			ImGui::Button("Press");
			ImGui::End();
		}
		// ESP code here
		ImGui::GetBackgroundDrawList()->AddCircleFilled({ 1000, 1000 }, 100.f, ImColor(1.f, 0.f, 0.f));
	// end of drawing
```
Make sure to use percentages of screen width and height to avoid problems with different resolutions.
## Questions?
add me on discord Wessel#9460
## License

[MIT](https://choosealicense.com/licenses/mit/)
