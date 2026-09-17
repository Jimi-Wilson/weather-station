#include <Arduino.h>
#include "display/Screen.h"
#include "display/screens/PairingScreen.h"

namespace {
constexpr uint8_t SCREEN_WIDTH = 128;

void drawLinkMark(U8G2& display) {
    // Interlocking links identify pairing without implying a Wi-Fi connection.
    display.drawRFrame(108, 2, 10, 7, 2);
    display.drawRFrame(113, 5, 10, 7, 2);
}

String spacedCode(const String& code) {
    if (code.length() == 0) {
        return "- - - - - -";
    }

    // Spacing makes short, random claim codes much easier to transcribe.
    String result;
    result.reserve((code.length() * 2) - 1);
    for (size_t index = 0; index < code.length(); ++index) {
        if (index > 0) {
            result += ' ';
        }
        result += code[index];
    }
    return result;
}
}  // namespace

void PairingScreen::draw(U8G2& display) {
    display.setDrawColor(1);
    display.setFontMode(1);
    display.setFontPosBaseline();

    // Quiet utility header: the code card below remains the focal point.
    display.setFont(u8g2_font_6x10_tf);
    display.drawStr(3, 9, "PAIR DEVICE");
    drawLinkMark(display);
    display.drawHLine(3, 13, 122);

    display.setFont(u8g2_font_5x7_tf);
    display.drawStr(11, 23, "ENTER CODE IN THE APP");

    // Inverted ticket-style code field for maximum OLED contrast.
    display.drawRBox(3, 28, 122, 25, 3);

    const String code = spacedCode(pairingCode);
    display.setDrawColor(0);
    display.setFont(u8g2_font_9x18B_tf);
    int16_t codeWidth = display.getStrWidth(code.c_str());

    // Keep unexpectedly long server codes inside the ticket.
    if (codeWidth > 114) {
        display.setFont(u8g2_font_7x14B_tf);
        codeWidth = display.getStrWidth(code.c_str());
    }

    const int16_t codeX = (SCREEN_WIDTH - codeWidth) / 2;
    display.drawStr(codeX > 6 ? codeX : 6, 47, code.c_str());

    display.setDrawColor(1);
    display.setFont(u8g2_font_5x7_tf);
    if ((millis() / 500) % 2 == 0) {
        display.drawDisc(5, 60, 2);
    }
    display.drawStr(11, 63, "WAITING FOR CONNECTION");
}

void PairingScreen::setPairingCode(const String& code) {
    pairingCode = code;
}
