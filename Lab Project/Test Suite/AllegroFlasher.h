#ifndef ALLEGROFLASHER_H
#define ALLEGROFLASHER_H

#include <iostream>
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>				// Our primitive header file
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <iostream>
#include <vector>
#include <string>
#include "Runnable.h"
using std::cin;
using std::cout;
using std::vector;

//! Class Allegro.h

/*!
 * \enum EVENT
 */

enum EVENT
{ UP, /*!< Enumeration value UP */
DOWN, /*!< Enumeration value DOWN */
LEFT, /*!< Enumeration value LEFT */
RIGHT, /*!< Enumeration value RIGHT */
ENTER, /*!< Enumeration value ENTER */
ESCAPE, /*!< Enumeration value ESCAPE */
SPACE, /*!< Enumeration value SPACE */
UPDATE,	/*!< Enumeration value UPDATE */
RELOAD, /*!< Enumeration value RELOAD */
LEFT_CLICK, /*!< Enumeration value LEFT_CLICK */
RIGHT_CLICK, /*!< Enumeration value RIGHT_CLICK */
MOVE_MOUSE, /*!< Enumeration value MOVE_MOUSE */
LAY_MAZE,/*!< Enumeration value LAY_MAZE */
};

using std::cerr;
using std::endl;
using std::string;

/*!
 * \enum InitializationStatus
 */
enum InitializationStatus
{
	OKAY, /*!< Enumeration value OKAY */
	ALLEGRO_INITIALISATION_ERROR,/*!< Enumeration value ALLEGRO_INITIALISATION_ERROR */
	PRIMITIVES_ADDON_ERROR,/*!< Enumeration value PRIMITIVES_ADDON_ERROR */
	TTF_ADDON_ERROR, /*!< Enumeration value TTF_ADDON_ERROR */
	KEYBOARD_ERROR, /*!< Enumeration value KEYBOARD_ERROR */
	MOUSE_ERROR, /*!< Enumeration value MOUSE_ERROR */
	FONT_LOADING_ERROR, /*!< Enumeration value FONT_LOADING_ERROR */
	AUDIO_ERROR, /*!< Enumeration value AUDIO_ERROR */
	CODEC_ERROR, /*!< Enumeration value CODEC_ERROR */
	RESERVE_SAMPLES_ERROR, /*!< Enumeration value RESERVE_SAMPLES_ERROR */
    DISPLAY_ERROR
};


class AllegroFlasher: public Runnable
{
public:
	AllegroFlasher(const int w, const int h);
    virtual void run();
	InitializationStatus AllegroInit();
	void DestroyAllegro();
	bool eventQueueIsEmpty();
	EVENT EventType();
	void reportSetupError(InitializationStatus status);
	double getMouseXPos(){return mouse_xpos;};
	double getMouseYPos(){return mouse_ypos;};
    virtual ~AllegroFlasher();
    void initFlasher(double freq, double time, int cycle, int pattern, bool startWithBlank);
    void initFlasher(vector<double> freq, double time, int cycle, int pattern, bool startWithBlank);

private:
    void updateDisplay();
    void displayBlankScreen();
    int PatternIndex, CycleIndex, CycleCounter, ColorIndex;
    bool useBlankStart;
    bool done;
    int width;
	int height;
    vector<double> FPS;
    vector<int> frames;

	ALLEGRO_EVENT_QUEUE *event_queue;
	ALLEGRO_TIMER *timer;
    ALLEGRO_DISPLAY *display;
	double mouse_xpos;
	double mouse_ypos;
};

#endif