#include <stdio.h>
#include <sys/types.h>

// PS1 specific headers.
#include <psxetc.h>
#include <psxgte.h>
#include <psxgpu.h>

// Define variables, structs etc.
DISPENV disp[2];	// Environment pair for display.
DRAWENV draw[2];	// Encironment pair for draw.
int db;				// Buffer counter.

// Function to init everything required.
void init()
{
	
	// Reset GPU and enable interrupts.
	ResetGraph(0);

	// Configure disp pair for NTSC video.
	// 320x240 resolution.
	SetDefDispEnv(&disp[0], 0, 0, 320, 240);
	SetDefDispEnv(&disp[1], 0, 240, 320, 240);
	// Configure draw pair.
	SetDefDrawEnv(&draw[0], 0, 240, 320, 240);
	SetDefDrawEnv(&draw[1], 0, 0, 320, 240);

	// Set the clear colour of DRAWENV and enable background clearing.
	// A hall of mirrors effect will occur without this.
	setRGB0(&draw[0], 63, 0, 127);
	setRGB0(&draw[1], 63, 0, 127);
	draw[0].isbg = 1;
	draw[1].isbg = 1;

	// Apply environments.
	PutDispEnv(&disp[0]);
	PutDrawEnv(&draw[0]);

}

// Display function.
void display()
{

	// VSync
	// Wait for GPU to finish drawing and V-Blank.
	DrawSync(0);
	VSync(0);

	// Flip buffer counter.
	db = !db;

	// Apply environments.
	PutDispEnv(&disp[db]);
	PutDrawEnv(&draw[db]);

	// Enable display.
	// Without this, you get a black screen.
	SetDispMask(1);

}

// Main function.
int main()
{

	// Init console.
	init();

	// Main loop.
	while (1)
	{
		// App code goes here.
		display();
	}

	return 0;

}