#pragma once

#include "Ads.h"

class AdsText : public Ads {
protected:
    bool Load(const char * jsonFile);
public:
    AdsText();
    ~AdsText();
    bool update();
    void render();
}; 