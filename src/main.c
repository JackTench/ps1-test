#include <stdio.h>
#include <sys/types.h>

// PS1 specific headers.
#include <psxetc.h>
#include <psxgte.h>
#include <psxgpu.h>

// Define variables, structs etc.
#define OTLEN 8			// Length of ordering table. Easy to change later.
DISPENV disp[2];		// Environment pair for display.
DRAWENV draw[2];		// Encironment pair for draw.
int db;					// Buffer counter.
int ot[2][OTLEN];		// Ordering table. Handles overlaps.
char pribuff[2][32768];	// Primitive buffer. 32KB in size.
char *nextpri;			// Next primitive pointer.

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

	// Ensure db starts at 0.
	db = 0;

	// Set initial primitive pointer address.
	nextpri = pribuff[0];

	// Load PS1 debug font.
	FntLoad(960, 0);
	// Create text stream.
	FntOpen(0, 8, 320, 224, 0, 100);

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

	// Draw ordering table.
	DrawOTag(ot[db]+OTLEN-1);

	// Reset next prim pointer.
	nextpri = pribuff[db];

}

// Main function.
int main()
{

	// Primitive pointer.
	TILE *tile;

	// Init console.
	init();

	// Main loop.
	while (1)
	{

		// Clear ordering table.
		ClearOTagR(ot[db], OTLEN);

		// Cast next prim.
		tile = (TILE*)nextpri;

		// Draw square.
		setTile(tile);
		// Set start pos.
		setXY0(tile, 32, 32);
		// Set size.
		setWH(tile, 8, 16);
		// Set color.
		setRGB0(tile, 250, 179, 255);
		// Add to ordering table.
		addPrim(ot[db], tile);
		
		// Print hello world.
		FntPrint(-1, "JACK TENCH PS1 DEVELOPMENT TEST");
		FntFlush(-1);

		// Advance pointer.
		nextpri += sizeof(TILE);

		// Update display.
		display();

	}

	return 0;

}