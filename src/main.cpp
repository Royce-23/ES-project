/*
 * ES LCD menu - Arduino Uno R3 / ATmega328P 16 MHz.
 * LCD: RS=D6, E=D7, DB4..DB7=D8..D11, RW=GND.
 * Buttons: UP=D2, DOWN=D3, OK=D4, BACK=D5; each connects to GND.
 * D13 drives one demonstration LED. Relay/motor states are shown on LCD.
 * Optional inputs: LM35 OUT=A0, humidity potentiometer wiper=A1.
 */
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <stdint.h>
#include <stdio.h>

constexpr uint8_t PIN_UP = 3, PIN_DOWN = 2, PIN_OK = 5, PIN_BACK = 4;
constexpr uint8_t PIN_LED = 13, PIN_TEMP = A0, PIN_HUMIDITY = A1;
constexpr uint32_t DEBOUNCE_MS = 30, LIVE_REFRESH_MS = 400;

LiquidCrystal lcd(6, 7, 8, 9, 10, 11);

enum class Key : uint8_t { None, Up, Down, Ok, Back };
enum class View : uint8_t { Menu, Message, Live, Edit };
enum class LivePage : uint8_t { None, Temperature, Humidity, Time };

// Con tro ham: tro toi ham void, khong co tham so.
using Action = void (*)();

// Mot node menu; prev/next tao list doi vong, parent/child tao cay menu.
struct MenuItem {
    const char *label;
    Action action;
    MenuItem *prev, *next, *parent, *child;
    explicit MenuItem(const char *text, Action callback = nullptr)
        : label(text), action(callback), prev(nullptr), next(nullptr),
          parent(nullptr), child(nullptr) {}
};

void showTemperature(); void setupTemperature();
void showHumidity(); void setupHumidity();
void showTime(); void setupTime();
void relay1On(); void relay1Off(); void relay2On(); void relay2Off();
void relay3On(); void relay3Off(); void relay4On(); void relay4Off();
void motor1On(); void motor1Off(); void motor2On(); void motor2Off();
void ledOnAction(); void ledOffAction(); void allOutputsOff(); void showAbout();

// Menu Main.
MenuItem root("Main");
MenuItem sensors("Sensors"), actuators("Actuators"), systemSetup("Setup");

// Menu Sensors.
MenuItem temperature("Temperature"), humidity("Humidity"), timeMenu("Time");
MenuItem tempDisplay("Display", showTemperature);
MenuItem tempSetup("Setup", setupTemperature);
MenuItem humDisplay("Display", showHumidity);
MenuItem humSetup("Setup", setupHumidity);
MenuItem timeDisplay("Display", showTime);
MenuItem timeSetup("Setup", setupTime);

// Menu Actuators.
MenuItem relays("Relays"), motors("Motors"), leds("LEDs");
MenuItem relay1("Relay 1"), relay2("Relay 2"), relay3("Relay 3"), relay4("Relay 4");
MenuItem r1On("ON", relay1On), r1Off("OFF", relay1Off);
MenuItem r2On("ON", relay2On), r2Off("OFF", relay2Off);
MenuItem r3On("ON", relay3On), r3Off("OFF", relay3Off);
MenuItem r4On("ON", relay4On), r4Off("OFF", relay4Off);
MenuItem motor1("Motor 1"), motor2("Motor 2");
MenuItem m1On("ON", motor1On), m1Off("OFF", motor1Off);
MenuItem m2On("ON", motor2On), m2Off("OFF", motor2Off);
MenuItem led1("LED"), ledOnItem("ON", ledOnAction), ledOffItem("OFF", ledOffAction);

// Menu Setup.
MenuItem setupAllOff("All outputs OFF", allOutputsOff);
MenuItem setupAbout("About", showAbout);

MenuItem *selected = nullptr;
View view = View::Menu;
LivePage livePage = LivePage::None;
bool relayState[4] = {}, motorState[2] = {}, ledState = false, dirty = true;
uint8_t temperatureLimit = 35, humidityLimit = 70, timeOffsetHours = 0;
uint8_t *editTarget = nullptr, editDraft = 0, editMin = 0, editMax = 100;
const char *editTitle = "Setup", *editUnit = "";
char messageLine1[17] = "", messageLine2[17] = "";
uint32_t lastLiveRefresh = 0;

void appendChild(MenuItem &parent, MenuItem &node) {
    node.parent = &parent;
    if (parent.child == nullptr) {
        parent.child = &node;
        node.prev = node.next = &node;
        return;
    }
    MenuItem *first = parent.child;
    MenuItem *last = first->prev;
    node.prev = last;
    node.next = first;
    last->next = &node;
    first->prev = &node;
}

void buildMenu() {
    appendChild(root, sensors); appendChild(root, actuators); appendChild(root, systemSetup);
    appendChild(sensors, temperature); appendChild(sensors, humidity); appendChild(sensors, timeMenu);
    appendChild(temperature, tempDisplay); appendChild(temperature, tempSetup);
    appendChild(humidity, humDisplay); appendChild(humidity, humSetup);
    appendChild(timeMenu, timeDisplay); appendChild(timeMenu, timeSetup);

    appendChild(actuators, relays); appendChild(actuators, motors); appendChild(actuators, leds);
    appendChild(relays, relay1); appendChild(relays, relay2);
    appendChild(relays, relay3); appendChild(relays, relay4);
    appendChild(relay1, r1On); appendChild(relay1, r1Off);
    appendChild(relay2, r2On); appendChild(relay2, r2Off);
    appendChild(relay3, r3On); appendChild(relay3, r3Off);
    appendChild(relay4, r4On); appendChild(relay4, r4Off);
    appendChild(motors, motor1); appendChild(motors, motor2);
    appendChild(motor1, m1On); appendChild(motor1, m1Off);
    appendChild(motor2, m2On); appendChild(motor2, m2Off);
    appendChild(leds, led1); appendChild(led1, ledOnItem); appendChild(led1, ledOffItem);

    appendChild(systemSetup, setupAllOff); appendChild(systemSetup, setupAbout);
    selected = root.child;
}

struct DebouncedButton {
    uint8_t pin; Key key; uint8_t lastRaw, stable; uint32_t changedAt;
    DebouncedButton(uint8_t gpio, Key event)
        : pin(gpio), key(event), lastRaw(HIGH), stable(HIGH), changedAt(0) {}
    void begin() {
        pinMode(pin, INPUT_PULLUP);
        lastRaw = stable = digitalRead(pin);
        changedAt = millis();
    }
    bool pressed(uint32_t now) {
        const uint8_t raw = digitalRead(pin);
        if (raw != lastRaw) { lastRaw = raw; changedAt = now; }
        if (static_cast<uint32_t>(now - changedAt) >= DEBOUNCE_MS && raw != stable) {
            stable = raw;
            return stable == LOW;
        }
        return false;
    }
};

DebouncedButton buttons[] = {
    {PIN_DOWN, Key::Down}, {PIN_UP, Key::Up},
    {PIN_OK, Key::Ok}, {PIN_BACK, Key::Back}
};

Key readKey() {
    Key result = Key::None;
    const uint32_t now = millis();
    for (auto &button : buttons) if (button.pressed(now)) result = button.key;
    return result;
}

void showMessage(const char *line1, const char *line2) {
    snprintf(messageLine1, sizeof(messageLine1), "%.16s", line1);
    snprintf(messageLine2, sizeof(messageLine2), "%.16s", line2);
    view = View::Message;
    dirty = true;
}

void openLive(LivePage page) {
    livePage = page;
    view = View::Live;
    lastLiveRefresh = 0;
    dirty = true;
}

void beginEdit(uint8_t &value, uint8_t minValue, uint8_t maxValue,
               const char *title, const char *unit) {
    editTarget = &value; editDraft = value; editMin = minValue; editMax = maxValue;
    editTitle = title; editUnit = unit; view = View::Edit; dirty = true;
}

void showTemperature() { openLive(LivePage::Temperature); }
void setupTemperature() { beginEdit(temperatureLimit, 0, 99, "Temp limit", "C"); }
void showHumidity() { openLive(LivePage::Humidity); }
void setupHumidity() { beginEdit(humidityLimit, 0, 100, "Humidity limit", "%"); }
void showTime() { openLive(LivePage::Time); }
void setupTime() { beginEdit(timeOffsetHours, 0, 23, "Time offset", "h"); }

void setRelay(uint8_t index, bool on) {
    relayState[index] = on;
    char line[17];
    snprintf(line, sizeof(line), "Relay %u: %s", index + 1, on ? "ON" : "OFF");
    showMessage(line, "BACK/OK: return");
}
void relay1On(){setRelay(0,true);} void relay1Off(){setRelay(0,false);}
void relay2On(){setRelay(1,true);} void relay2Off(){setRelay(1,false);}
void relay3On(){setRelay(2,true);} void relay3Off(){setRelay(2,false);}
void relay4On(){setRelay(3,true);} void relay4Off(){setRelay(3,false);}

void setMotor(uint8_t index, bool on) {
    motorState[index] = on;
    char line[17];
    snprintf(line, sizeof(line), "Motor %u: %s", index + 1, on ? "ON" : "OFF");
    showMessage(line, "BACK/OK: return");
}
void motor1On(){setMotor(0,true);} void motor1Off(){setMotor(0,false);}
void motor2On(){setMotor(1,true);} void motor2Off(){setMotor(1,false);}

void setLed(bool on) {
    ledState = on;
    digitalWrite(PIN_LED, on ? HIGH : LOW);
    showMessage(on ? "LED: ON" : "LED: OFF", "BACK/OK: return");
}
void ledOnAction(){setLed(true);} void ledOffAction(){setLed(false);}

void allOutputsOff() {
    for (bool &state : relayState) state = false;
    for (bool &state : motorState) state = false;
    ledState = false;
    digitalWrite(PIN_LED, LOW);
    showMessage("All outputs OFF", "BACK/OK: return");
}
void showAbout(){showMessage("ES LCD MENU", "List + FuncPtr");}

void handleMenuKey(Key key) {
    if (selected == nullptr) return;
    switch (key) {
        case Key::Up: selected = selected->prev; break;
        case Key::Down: selected = selected->next; break;
        case Key::Ok:
            if (selected->child) selected = selected->child;
            else if (selected->action) selected->action(); // goi qua function pointer
            break;
        case Key::Back:
            if (selected->parent && selected->parent != &root) selected = selected->parent;
            break;
        case Key::None: return;
    }
    dirty = true;
}

void handleKey(Key key) {
    if (key == Key::None) return;
    if (view == View::Menu) { handleMenuKey(key); return; }
    if (view == View::Edit) {
        if (key == Key::Up && editDraft < editMax) ++editDraft;
        else if (key == Key::Down && editDraft > editMin) --editDraft;
        else if (key == Key::Ok) {
            if (editTarget) *editTarget = editDraft;
            view = View::Menu;
        } else if (key == Key::Back) view = View::Menu;
        dirty = true;
        return;
    }
    if (key == Key::Ok || key == Key::Back) {
        livePage = LivePage::None;
        view = View::Menu;
        dirty = true;
    }
}

void printLine(uint8_t row, const char *text) {
    lcd.setCursor(0, row);
    uint8_t col = 0;
    while (col < 16 && *text) { lcd.write(static_cast<uint8_t>(*text++)); ++col; }
    while (col < 16) { lcd.write(static_cast<uint8_t>(' ')); ++col; }
}

void renderLive() {
    char line[17];
    if (livePage == LivePage::Temperature) {
        const unsigned value = static_cast<unsigned>(analogRead(PIN_TEMP)) * 500U / 1023U;
        snprintf(line, sizeof(line), "Temp: %u C", value); printLine(0, line);
        snprintf(line, sizeof(line), "Limit: %u C", temperatureLimit); printLine(1, line);
    } else if (livePage == LivePage::Humidity) {
        const unsigned value = static_cast<unsigned>(analogRead(PIN_HUMIDITY)) * 100U / 1023U;
        snprintf(line, sizeof(line), "Hum: %u%%", value); printLine(0, line);
        snprintf(line, sizeof(line), "Limit: %u%%", humidityLimit); printLine(1, line);
    } else {
        const uint32_t total = millis() / 1000UL;
        const uint8_t s = total % 60UL, m = (total / 60UL) % 60UL;
        const uint8_t h = ((total / 3600UL) + timeOffsetHours) % 24UL;
        snprintf(line, sizeof(line), "Time %02u:%02u:%02u", h, m, s); printLine(0, line);
        printLine(1, "BACK/OK: return");
    }
}

void render() {
    if (!dirty) return;
    dirty = false;
    char line[17];
    if (view == View::Menu) {
        if (!selected) { printLine(0, "Empty menu"); printLine(1, ""); return; }
        printLine(0, selected->parent ? selected->parent->label : root.label);
        snprintf(line, sizeof(line), ">%-14.14s%c", selected->label, selected->child ? '>' : ' ');
        printLine(1, line);
    } else if (view == View::Message) {
        printLine(0, messageLine1); printLine(1, messageLine2);
    } else if (view == View::Live) {
        renderLive();
    } else {
        snprintf(line, sizeof(line), "%s: %u%s", editTitle, editDraft, editUnit);
        printLine(0, line); printLine(1, "OK:save BACK:no");
    }
}

void setup() {
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);
    for (auto &button : buttons) button.begin();
    lcd.begin(16, 2);
    buildMenu();
    render();
}

void loop() {
    handleKey(readKey());
    if (view == View::Live &&
        static_cast<uint32_t>(millis() - lastLiveRefresh) >= LIVE_REFRESH_MS) {
        lastLiveRefresh = millis();
        dirty = true;
    }
    render();
}
