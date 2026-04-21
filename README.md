# SFreeze
SFreeze is a simple Gtk4 app that freezes Sober temporarily.
Why is this useful? On Windows as example you can hold the X button of roblox which puts it in a "Standby" state Alot of games will recognise this and see the client as "Frozen" Which will in many cases result in a lagback.
This can be useful if you play a game like Fling things and people on roblox as example, If someone flings you very far above the water you can freeze the client and trick the game into thinking You are flying to fast Causing it to lag you back to a safe spot.
This tool is mostly intended for Window managers, Desktop enviroments work too but im not sure if the native window buttons Can do the same Thing as the native Roblox client on windows.

Tool in action:
https://github.com/user-attachments/assets/b354a588-94e8-4236-adb1-106cec61a680
Keybinds also work but youll have to manually set them in your Window managers config or in Keyboard shortcuts in your DE.

What the tool does is just a very simple PID lookup of sober If it finds them all it will just Use "SIGSTOP" To freeze the instance and "SIGCONT" To continue the instance which is also very simple to achive in the terminal but im sure you wouldnt have fun typing "SIGSTOP" And "SIGCONT" Everytime in the terminal to due exactly what this tool does but simpler.
