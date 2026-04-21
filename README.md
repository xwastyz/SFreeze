# SFreeze
SFreeze is a simple Gtk4 App Written in C that freezes Sober temporarily.
Why is this useful? On Windows as example you can hold the X button of roblox which puts it in a "Standby" state Alot of games will recognise this and see the client as "Frozen" Which will in many cases result in a lagback.
This can be useful if you play a game like Fling things and people on roblox as example, If someone flings you very far above the water you can freeze the client and trick the game into thinking You are flying to fast Causing it to lag you back to a safe spot.
This tool is mostly intended for Window managers However Desktop enviroments work too.

Tool in action:
https://github.com/user-attachments/assets/b354a588-94e8-4236-adb1-106cec61a680
Keybinds also work but youll have to manually set them in your Window managers config or in Keyboard shortcuts in your DE.

What the tool does is just a very simple PID lookup of sober If it finds them all it will just Use "SIGSTOP" To freeze the instance and "SIGCONT" To continue the instance which is also very simple to achive in the terminal but im sure you wouldnt have fun typing "SIGSTOP" And "SIGCONT" Everytime in the terminal to due exactly what this tool does but simpler.

How to implement the keybind [WARNING: You need "netcat" installed on your system for this to work]: 
On a DE [Desktop Enviroment, Cinnamon, KDE Plasma etc] Go to your settings app and search for keyboard then go to keybinds/shortcuts and add a custom command, as the name use anything and the command is: sh -c "echo freeze | nc -u -w1 127.0.0.1 5555"
Change the keybind to anything you want!

On a WM [Window Manager] Add a keybind to your main config file in niri as example paste this at the end of the config.kdl file: binds {
    F1 { spawn "sh" "-c" "echo freeze | nc -u -w0 127.0.0.1 5555"; } 
}

Change the keybind to anything you want!
Thats it have fun.
<img width="1920" height="1200" alt="image" src="https://github.com/user-attachments/assets/ea4caa69-5f87-4ff0-969c-6e665b088677" />
<img width="1318" height="786" alt="image" src="https://github.com/user-attachments/assets/6561e681-561f-4618-89ef-e130692e6a90" />
