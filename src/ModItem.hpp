#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ModItem : public CCNode {
protected:
    CCScale9Sprite* m_bg;
    CCMenuItemSpriteExtra* m_toggle;
    Mod* m_mod;
public:
    static ModItem* create(Mod* mod, CCSize const& size);
private:
    bool init(Mod* mod, CCSize const& size);
    void onToggle(CCObject* sender);
};
