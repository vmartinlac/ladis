
#ifdef __cplusplus
extern "C"
{
#endif

enum LADIS_EventType
{
    LADIS_EVENT_NONE,
    LADIS_EVENT_MOUSE_MOVED,
    LADIS_EVENT_MOUSE_BUTTON_DOWN,
    LADIS_EVENT_MOUSE_BUTTON_UP,
    LADIS_EVENT_KEY_UP,
    LADIS_EVENT_KEY_DOWN,
    LADIS_EVENT_QUIT
};

struct LADIS_Event
{
    int type;
    int dx;
    int dy;
    int button;
    int key;
};

void LADIS_start();
void LADIS_stop();
void LADIS_update_image(int w, int h, uint8_t* data);
void LADIS_read_event(struct LADIS_Event* ev);

#ifdef __cplusplus
}
#endif

