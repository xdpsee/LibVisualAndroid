#include "config.h"
#include "lv_event.h"
#include "lv_common.h"

extern "C" {

VisEventQueue* visual_event_queue_new ()
{
    return new LV::EventQueue;
}

void visual_event_queue_free (VisEventQueue *self)
{
    delete self;
}

void visual_event_queue_add  (VisEventQueue *self, VisEvent *event)
{
    visual_return_if_fail (self != NULL);

    self->add (*event);
    visual_event_free (event);
}

int visual_event_queue_poll (VisEventQueue *self, VisEvent *event)
{
    visual_return_val_if_fail (self != NULL, FALSE);
    visual_return_val_if_fail (self != NULL, FALSE);

    return self->poll (*event);
}

VisEvent* visual_event_new_keyboard (VisKey keysym, int keymod, VisKeyState state)
{
    // FIXME name to VISUAL_KEYB_DOWN and KEYB_UP
    LV::Event* event = new LV::Event;

    if (state == VISUAL_KEY_DOWN)
        event->type = VISUAL_EVENT_KEYDOWN;
    else
        event->type = VISUAL_EVENT_KEYUP;

    event->event.keyboard.keysym.sym = keysym;
    event->event.keyboard.keysym.mod = keymod;

    return event;
}

VisEvent* visual_event_new_mousemotion (int dx, int dy)
{
    LV::Event* event = new LV::Event;

    event->type = VISUAL_EVENT_MOUSEMOTION;

    //event->event.mousemotion.state = eventqueue->mousestate;
    event->event.mousemotion.x = dx;
    event->event.mousemotion.y = dy;
    
    return event;
}

VisEvent* visual_event_new_mousebutton (int button, VisMouseState state, int x, int y)
{
    LV::Event* event = new LV::Event;

    if (state == VISUAL_MOUSE_DOWN)
        event->type = VISUAL_EVENT_MOUSEBUTTONDOWN;
    else
        event->type = VISUAL_EVENT_MOUSEBUTTONUP;

    event->event.mousebutton.button = button;
    event->event.mousebutton.state = state;

    event->event.mousebutton.x = x;
    event->event.mousebutton.y = y;

    return event;
}

VisEvent* visual_event_new_resize (int width, int height)
{
    LV::Event* event = new LV::Event;

    event->type = VISUAL_EVENT_RESIZE;

    event->event.resize.width = width;
    event->event.resize.height = height;

    return event;
}

VisEvent* visual_event_new_newsong (VisSongInfo *songinfo)
{
    LV::Event* event = new LV::Event;

    event->type = VISUAL_EVENT_NEWSONG;

    /* FIXME refcounting */
    event->event.newsong.songinfo = songinfo;

    return event;
}

VisEvent* visual_event_new_param (void *param)
{
    LV::Event* event = new LV::Event;

    event->type = VISUAL_EVENT_PARAM;

    /* FIXME ref count the param */
    event->event.param.param = param;

    return event;
}

VisEvent* visual_event_new_quit ()
{
    LV::Event* event = new LV::Event;

    event->type = VISUAL_EVENT_QUIT;

    return event;
}

VisEvent* visual_event_new_visibility (int is_visible)
{
    LV::Event* event = new LV::Event;

    event->type = VISUAL_EVENT_VISIBILITY;
    event->event.visibility.is_visible = is_visible;

    return event;
}

VisEvent* visual_event_new_generic (int eid, int param_int, void *param_ptr)
{
    LV::Event* event = new LV::Event;

    event->type = VISUAL_EVENT_GENERIC;

    event->event.generic.event_id = eid;
    event->event.generic.data_int = param_int;
    event->event.generic.data_ptr = param_ptr;

    return event;
}

void visual_event_copy (VisEvent *dest, VisEvent *src)
{
    visual_return_if_fail (dest != NULL);
    visual_return_if_fail (src != NULL);

    *dest = *src;
}

void visual_event_free (VisEvent *event)
{
    delete event;
}

} // extern C

