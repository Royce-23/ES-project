#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdint.h>
uint8_t mockPins[32], mockModes[32];
int mockAnalog[32];
uint32_t mockTime = 0;
#include "../src/main.cpp"

void verifyRing(MenuItem &parent, unsigned count) {
    MenuItem *node = parent.child;
    assert(node);
    for (unsigned i = 0; i < count; ++i) {
        assert(node->parent == &parent);
        assert(node->next->prev == node && node->prev->next == node);
        node = node->next;
    }
    assert(node == parent.child);
}

void click(uint8_t pin) {
    mockPins[pin] = LOW; loop(); mockTime += 31; loop();
    mockPins[pin] = HIGH; loop(); mockTime += 31; loop();
}

int main() {
    std::fill(mockPins, mockPins + 32, HIGH);
    std::fill(mockAnalog, mockAnalog + 32, 512);
    setup();
    assert(selected == &sensors);
    assert(lcd.rows[0] == "Main            ");
    assert(lcd.rows[1] == ">Sensors       >");
    verifyRing(root, 3); verifyRing(sensors, 3); verifyRing(temperature, 2);
    verifyRing(actuators, 3); verifyRing(relays, 4); verifyRing(relay1, 2);
    std::cout << "PASS 1: menu tree and circular linked lists\n";

    handleKey(Key::Ok); handleKey(Key::Ok); handleKey(Key::Ok);
    assert(selected == &tempDisplay && view == View::Live);
    handleKey(Key::Back); handleKey(Key::Down); handleKey(Key::Ok);
    assert(view == View::Edit && editDraft == 35);
    handleKey(Key::Up); handleKey(Key::Ok);
    assert(temperatureLimit == 36 && view == View::Menu);
    std::cout << "PASS 2: Display and Setup callbacks\n";

    selected = &actuators; view = View::Menu;
    handleKey(Key::Ok); handleKey(Key::Ok); handleKey(Key::Ok); handleKey(Key::Ok);
    assert(relayState[0] && view == View::Message);
    handleKey(Key::Back); handleKey(Key::Down); handleKey(Key::Ok);
    assert(!relayState[0]);
    std::cout << "PASS 3: relay function pointers\n";

    selected = &ledOnItem; view = View::Menu; handleKey(Key::Ok);
    assert(ledState && mockPins[PIN_LED] == HIGH);
    selected = &setupAllOff; view = View::Menu; handleKey(Key::Ok);
    assert(!ledState && mockPins[PIN_LED] == LOW);
    std::cout << "PASS 4: LED and all-off actions\n";

    selected = &sensors; view = View::Menu;
    click(PIN_DOWN); assert(selected == &actuators);
    click(PIN_UP); assert(selected == &sensors);
    click(PIN_OK); assert(selected == &temperature);
    click(PIN_BACK); assert(selected == &sensors);
    std::cout << "PASS 5: debounced button path\nALL TESTS PASSED\n";
}
