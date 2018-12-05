
typedef struct Notification {
    uint32_t timer_counter;
    uint32_t seconds_passed;
    uint16_t scancode;
    bool timerNotif;
    bool keyboardNotif;
    bool mouseNotif;
    struct packet mouse_packet;
} Notification;

typedef enum Device {
    Timer,
    Keyboard,
    Mouse
} Device;

Notification GetNotification();
int subscribe_device(Device device);
int unsubscribe_device(Device device);
