/*
    SDL - Simple DirectMedia Layer
    Copyright (C) 1997-2012 Sam Lantinga

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Sam Lantinga
    slouken@libsdl.org
*/
#include "SDL_config.h"

/* Being a null driver, there's no event stream. We just define stubs for
   most of the API. */

#include "SDL.h"
#include "../../events/SDL_sysevents.h"
#include "../../events/SDL_events_c.h"

#include "SDL_nullvideo.h"
#include "SDL_nullevents_c.h"

#include "EmulatorInternals.h"

void DUMMY_PumpEvents(_THIS)
{
	struct EmulatorEvent event;

    while( emulator_read_event(&event), event.type != EMULATOR_EVENT_NONE )
    {
        if( event.type == EMULATOR_EVENT_MOUSE_MOVED )
        {
            SDL_PrivateMouseMotion(0, 1, event.dx, event.dy);
        }
        else if( event.type == EMULATOR_EVENT_MOUSE_BUTTON_DOWN )
        {
            SDL_PrivateMouseButton(SDL_PRESSED, event.button, 0, 0);
        }
        else if( event.type == EMULATOR_EVENT_MOUSE_BUTTON_UP )
        {
            SDL_PrivateMouseButton(SDL_RELEASED, event.button, 0, 0);
        }
        else if( event.type == EMULATOR_EVENT_KEY_UP )
        {
            SDL_keysym keysym;

            keysym.mod = 0;
            keysym.sym = event.key;
            keysym.scancode = 0;

            SDL_PrivateKeyboard(SDL_RELEASED, &keysym);
        }
        else if( event.type == EMULATOR_EVENT_KEY_DOWN )
        {
            SDL_keysym keysym;

            keysym.mod = 0;
            keysym.sym = event.key;
            keysym.scancode = 0;

            SDL_PrivateKeyboard(SDL_PRESSED, &keysym);
        }
        else if( event.type == EMULATOR_EVENT_QUIT )
        {
            SDL_PrivateQuit();
        }
    }
}

void DUMMY_InitOSKeymap(_THIS)
{
	/* do nothing. */
}

/* end of SDL_nullevents.c ... */

