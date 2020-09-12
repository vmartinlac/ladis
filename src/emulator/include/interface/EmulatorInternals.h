
#ifdef __cplusplus
extern "C"
{
#endif

enum EmulatorEventType
{
    EMULATOR_EVENT_NONE,
    EMULATOR_EVENT_MOUSE_MOVED,
    EMULATOR_EVENT_MOUSE_BUTTON_DOWN,
    EMULATOR_EVENT_MOUSE_BUTTON_UP,
    EMULATOR_EVENT_KEY_UP,
    EMULATOR_EVENT_KEY_DOWN,
    EMULATOR_EVENT_QUIT
};

struct EmulatorEvent
{
    int type;
    int dx;
    int dy;
    int button;
    int key;
};

void emulator_update_image(int w, int h, uint8_t* data);
void emulator_read_event(struct EmulatorEvent* ev);

#ifdef __cplusplus
}
#endif

