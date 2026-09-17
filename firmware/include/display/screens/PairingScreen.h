#pragma once

class PairingScreen : public Screen {
public:
    void setPairingCode(const String& code);
    void draw(U8G2& display) override;

private:
    String pairingCode;
};