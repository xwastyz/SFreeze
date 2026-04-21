Howto compile main.c yourself.

Put the main.c file in your home directory and compile it with;
"gcc `pkg-config --cflags gtk4` -o SFreeze main.c `pkg-config --libs gtk4`" In your terminal
