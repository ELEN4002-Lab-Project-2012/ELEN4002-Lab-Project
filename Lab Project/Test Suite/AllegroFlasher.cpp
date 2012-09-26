#include "AllegroFlasher.h"

AllegroFlasher::AllegroFlasher(const int w, const int h):
	//FPS(2*freq),
    width(w),
    height(h),
    //frames(2*freq*time),  // Total number of frames to run the test for.
	event_queue(NULL),
	timer(NULL),
    display(NULL),
	mouse_xpos(0),
	mouse_ypos(0),
    done(false),
    PatternIndex(2), CycleIndex(1), CycleCounter(0), ColorIndex(1)
{
	InitializationStatus initialisation_status = AllegroInit();
	if(initialisation_status != OKAY)
	{
		throw initialisation_status;
	}
}

void AllegroFlasher::initFlasher(double freq, double time, int cycle, int pattern, bool startWithBlank)
{
    if(startWithBlank)
        useBlankStart = true;
    else
        useBlankStart = false;

    FPS.clear(); frames.clear();
    CycleIndex = cycle;         // Full Cycle [0] or Half Cycle [1]
    PatternIndex = pattern;     // Full Screen [0], Horizontal Pillars [1], Checkered Board [2]
    FPS.push_back(2*freq);
    frames.push_back(2*freq*time);
    //cout << "Allegro Frequency (Hz): " << FPS/2 << endl;
}

void AllegroFlasher::initFlasher(vector<double> freqs, double time, int cycle, int pattern, bool startWithBlank)
{
    if(startWithBlank)
        useBlankStart = true;
    else
        useBlankStart = false;

    FPS.clear(); frames.clear();
    CycleIndex = cycle;         // Full Cycle [0] or Half Cycle [1]
    PatternIndex = pattern;     // Full Screen [0], Horizontal Pillars [1], Checkered Board [2]
    
    for(int i = 0; i != freqs.size(); i++)
    {
        FPS.push_back(2*freqs.at(i));
        frames.push_back(2*freqs.at(i)*time);
    }
    //cout << "Allegro Frequency (Hz): " << FPS/2 << endl;
    //frames = 2*freq*time;
}

void AllegroFlasher::run()
{
    const int zero = 0;

    timer = al_create_timer(1.0 / FPS.at(zero));
    display = al_create_display(width, height);
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);		        // Start the timer

    int counter = 0;
    while(!done)                        // Main loop
    {
        EVENT Event = EventType();      // Wait for an event to occur

        if (Event == ESCAPE) {			// Quit the program
            counter = frames.at(zero); 
            break;
        }
        else if (Event == UPDATE) {     // A timer event 
            if(useBlankStart && counter < frames.at(zero)/3)
                displayBlankScreen();   // Simply display a black screen
            else
                updateDisplay();
            counter++;
        }

        if(counter == frames.at(zero))
            done = true;
    }
    al_destroy_timer(timer);
	al_destroy_display(display);
    done = false;                       // Set again for the next time
}

AllegroFlasher::~AllegroFlasher()
{
	al_destroy_event_queue(event_queue);
	//al_destroy_timer(timer);
	//al_destroy_display(display);						//destroy our display object
}

InitializationStatus AllegroFlasher::AllegroInit()
{
	//const int FPS = 60;

	// initialise Allegro itself
	if(!al_init()) return ALLEGRO_INITIALISATION_ERROR;

	// initialise primitives addon (for drawing shapes)
	if(!al_init_primitives_addon()) return PRIMITIVES_ADDON_ERROR;

	// install the keyboard
	if(!al_install_keyboard()) return KEYBOARD_ERROR;

	//install the mouse
	if(!al_install_mouse()) return MOUSE_ERROR;

	// initialise font and ttf addon (for drawing text)
	al_init_font_addon();

	if (!al_init_ttf_addon()) return TTF_ADDON_ERROR;

	//initialise audio
	if(!al_install_audio()) return AUDIO_ERROR;

	//initialise audio codecs
	if(!al_init_acodec_addon()) return CODEC_ERROR;

	//reserve samples
	if (!al_reserve_samples(5)) return RESERVE_SAMPLES_ERROR;

	event_queue = al_create_event_queue();
    
    //display = al_create_display(width, height);
    //if(!display)
    //    return DISPLAY_ERROR;

	//timer = al_create_timer(1.0 / FPS);	    // Set the period of the time

	// Register keyboard input, mouse input, display input, and timer as event source on event_queue
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
    
	//al_register_event_source(event_queue, al_get_timer_event_source(timer));
	
    return OKAY;
}

void displayErrorMessage(const string& warning)
{
	al_show_native_message_box(NULL, NULL, NULL, warning.c_str(), NULL, ALLEGRO_MESSAGEBOX_ERROR);
}


void AllegroFlasher::reportSetupError(InitializationStatus status)
{
	switch (status)
	{
	case ALLEGRO_INITIALISATION_ERROR:
		displayErrorMessage("Failed to initialize Allegro!");
		break;
	case PRIMITIVES_ADDON_ERROR:
		displayErrorMessage("Failed to initialize primitives add-on!");
		break;
	case TTF_ADDON_ERROR:
		displayErrorMessage("Failed to initialize TTF add-on!");
		break;
	case KEYBOARD_ERROR:
		displayErrorMessage("Failed to install keyboard driver!");
		break;
	case MOUSE_ERROR:
		displayErrorMessage("Failed to install mouse driver!");
		break;
	case AUDIO_ERROR:
		displayErrorMessage("Failed to install audio driver!");
		break;
	case CODEC_ERROR:
		displayErrorMessage("Failed to install audio codecs!");
		break;
	case RESERVE_SAMPLES_ERROR:
		displayErrorMessage("Failed to reserve sampples!");
		break;
	case FONT_LOADING_ERROR:
		displayErrorMessage("A font could not be loaded! Make sure that you copy the 'data' directory with the font file to the directory containing your project file.");
		break;
	default:
		break;
		//should never reach here
	}
}

EVENT AllegroFlasher::EventType()
{
	EVENT event_type = UPDATE;
	ALLEGRO_EVENT ev;							// Create a new event
	al_wait_for_event(event_queue, &ev);		// Wait for an event (timer overflow, keyboard, display)

	if(ev.type == ALLEGRO_EVENT_KEY_DOWN)		// If event type is a key press (down)
	{
		switch(ev.keyboard.keycode)				// Switch to the appropriate key code
		{
			case ALLEGRO_KEY_UP:				// If the specific key is pressed
				event_type = UP;				// Set the status of the key (only changed once we release the key)
				break;
			case ALLEGRO_KEY_DOWN:
				event_type = DOWN;
				break;
			case ALLEGRO_KEY_RIGHT:
				event_type = RIGHT;
				break;
			case ALLEGRO_KEY_LEFT:
				event_type = LEFT;
				break;
			case ALLEGRO_KEY_ESCAPE:
				al_uninstall_audio();
				event_type = ESCAPE;
				break;
			case ALLEGRO_KEY_SPACE:
				event_type = SPACE;
				break;
			case ALLEGRO_KEY_ENTER:
				event_type = ENTER;
				break;
			case ALLEGRO_KEY_L:
				event_type = LAY_MAZE;
				break;
			case ALLEGRO_KEY_R:
				event_type = RELOAD;
				break;
			default:
				event_type = UPDATE;
				break;
		}
	}

	else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)		// Mouse click the close button
	{
		event_type = ESCAPE;
	}

	else if(ev.type == ALLEGRO_EVENT_TIMER)
		event_type = UPDATE;

	if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
	{
		if(ev.mouse.button & 1)
			event_type = LEFT_CLICK;
		else if(ev.mouse.button & 2)
			event_type = RIGHT_CLICK;
	}

	else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES)
	{
		event_type = MOVE_MOUSE;
		mouse_xpos = ev.mouse.x;
		mouse_ypos = ev.mouse.y;
	}

	return event_type;
}

bool AllegroFlasher::eventQueueIsEmpty()
{
	if (al_is_event_queue_empty(event_queue))
		return true;
	else
		return false;
}

void AllegroFlasher::displayBlankScreen()
{
    al_clear_to_color(al_map_rgb(255,255,255));
}

void AllegroFlasher::updateDisplay()
{
    if (PatternIndex == 0)
	{
		if (ColorIndex > 0)
		{
			al_draw_filled_rectangle(0, 0, width, height, al_map_rgb(255,255,255));
			ColorIndex =0 ;
		}
		else
		{
			ColorIndex = 1;
			al_draw_filled_rectangle(0, 0, width, height, al_map_rgb(0,0,0));
		}
	}
	else if ((PatternIndex == 1) && (CycleIndex == 0))
	{
		double barWidth = width/8 ; 
		double pos = 0 ;
		barWidth = floor(barWidth);
		if (ColorIndex > 0)
		{
			for (int a = 0; a < 4 ; a++)
			{
				pos = (barWidth )*(a*2) ;
				pos = floor(pos);
				al_draw_filled_rectangle(pos, 0, (pos+barWidth), height, al_map_rgb(255,255,255));
			}			
			ColorIndex =0 ;
		}
		else
		{
			ColorIndex = 1;
			for (int a = 0; a < 4 ; a++)
			{
				pos = (barWidth )*((a*2)+1) ;
				pos = floor(pos);
						
				al_draw_filled_rectangle(pos, 0, pos+barWidth, height, al_map_rgb(255,255,255));
			}
		}
	}

	else if ((PatternIndex == 1) && (CycleIndex == 1))
	{
		double barWidth = width/8 ; 
		double pos = 0 ;
		barWidth = floor(barWidth);

		if (CycleCounter == 0)
		{
			CycleCounter = 1 ;
			if (ColorIndex > 0)
			{
				for (int a = 0; a < 4 ; a++)
				{
					pos = (barWidth )*(a*2) ;
					pos = floor(pos);
					al_draw_filled_rectangle(pos, 0, (pos+barWidth), height, al_map_rgb(255,255,255));
				}	
				ColorIndex =0 ;
			}
			else
			{
				ColorIndex = 1;
				for (int a = 0; a < 4 ; a++)
				{
					pos = (barWidth )*((a*2)+1) ;
					pos = floor(pos);
						
					al_draw_filled_rectangle(pos, 0, pos+barWidth, height, al_map_rgb(255,255,255));
				}
			}
		}

		else
		{
			al_draw_filled_rectangle(0, 0, width, height, al_map_rgb(0,0,0));
			CycleCounter = 0 ;
		}
	}
	else if ((PatternIndex == 2) && (CycleIndex == 0))
	{
		double sqrWidth = width/8 ; 
		double sqrHeight = height / 8;
				
		sqrWidth = floor(sqrWidth);
		sqrHeight = floor(sqrHeight);
		int xpos, ypos;
			
		if (ColorIndex > 0)
		{
			for (int a = 0; a < 4 ; a++)
			{
				xpos = sqrWidth * (a*2);
				for (int b = 0; b < 8 ; b++)
				{
				    ypos = sqrHeight * (b);
					if (b%2 == 0)
					{							
						al_draw_filled_rectangle(xpos, ypos, xpos+sqrWidth, ypos+sqrHeight, al_map_rgb(255,255,255));
					}
					else
					{							
						al_draw_filled_rectangle(xpos+sqrWidth, ypos, xpos+sqrWidth+sqrWidth, ypos+sqrHeight, al_map_rgb(255,255,255));
					}
				}
			}					
			ColorIndex =0 ;
		}
		else
		{
			ColorIndex = 1;
			for (int a = 0; a < 4 ; a++)
			{
				xpos = sqrWidth * ((a*2)+1);
				for (int b = 0; b < 8 ; b++)
				{
					ypos = sqrHeight * ((b));
					if (b%2 == 0)
					{								
						al_draw_filled_rectangle(xpos, ypos, xpos+sqrWidth, ypos+sqrHeight, al_map_rgb(255,255,255));
					}
					else
					{								
						al_draw_filled_rectangle(xpos+sqrWidth, ypos, xpos+sqrWidth+sqrWidth, ypos+sqrHeight, al_map_rgb(255,255,255));
					}

				}
			}
		}
	}
	else if ((PatternIndex == 2) && (CycleIndex == 1))
	{
		double sqrWidth = width/8 ; 
		double sqrHeight = height / 8;
				
		sqrWidth = floor(sqrWidth);
		sqrHeight = floor(sqrHeight);
		int xpos, ypos;
		if (CycleCounter == 0)
		{
			CycleCounter = 1 ;
			
			if (ColorIndex > 0)
			{
				for (int a = 0; a < 4 ; a++)
				{
					xpos = sqrWidth * (a*2);
					for (int b = 0; b < 4 ; b++)
					{
						ypos = sqrHeight * (b*2);
						al_draw_filled_rectangle(xpos, ypos, xpos+sqrWidth, ypos+sqrHeight, al_map_rgb(255,255,255));
					}
				}
					
					
				ColorIndex =0 ;
			}
			else
			{
				ColorIndex = 1;
				for (int a = 0; a < 4 ; a++)
				{
					xpos = sqrWidth * ((a*2)+1);
					for (int b = 0; b < 4 ; b++)
					{
						ypos = sqrHeight * ((b*2)+1);
						al_draw_filled_rectangle(xpos, ypos, xpos+sqrWidth, ypos+sqrHeight, al_map_rgb(255,255,255));
					
					}
				}
			}
		}
		else
		{
			al_draw_filled_rectangle(0, 0, width, height, al_map_rgb(0,0,0));
			CycleCounter = 0 ;
		}
	}

	al_flip_display();
	al_clear_to_color(al_map_rgb(0,0,0));
}
