#include "ModItem.hpp"
#include <Geode/utils/ColorProvider.hpp>
#include <Geode/ui/GeodeUI.hpp>

constexpr std::array frames = {
    "GJ_completesIcon_001.png",
    "GJ_infoIcon_001.png",
    "geode.loader/info-warning.png",
    "geode.loader/info-alert.png",
    "GJ_deleteIcon_001.png"
};

bool ModItem::init(Mod* mod, CCSize const& size) {
    if (!CCNode::init()) {
        return false;
    }

    m_mod = mod;

    this->setContentSize(size);
    this->setAnchorPoint({ 0.5f, 0.5f });

    ccColor4B bgColor = ColorProvider::get()->color("geode.loader/mod-developer-item-bg");

    m_bg = CCScale9Sprite::create("square02b_001.png");
    m_bg->setColor(to3B(bgColor));
    m_bg->setOpacity(bgColor.a);
    m_bg->setScale(.3f);
    m_bg->setContentSize(CCSize {
        size.width,
        size.height
    } / m_bg->getScale());
    this->addChildAtPosition(
        m_bg,
        Anchor::Center
    );

    auto logoBtn = CCMenuItemExt::createSpriteExtra(
        geode::createModLogo(mod), [mod](CCNode*) {
            geode::openInfoPopup(mod);
        }
    );

    logoBtn->m_baseScale = (0.38f);
    logoBtn->setScale(logoBtn->m_baseScale);
    logoBtn->m_animationEnabled = false;
    logoBtn->m_colorEnabled = true;
    logoBtn->setAnchorPoint({ 0.0f, 0.5f });

    this->addChildAtPosition(
        CCMenu::createWithItem(logoBtn),
        Anchor::Left,
        { 4, 0 }
    );

    auto nameLabel = CCLabelBMFont::create(
        mod->getName().c_str(),
        "bigFont.fnt"
    );

    auto logLevel = mod->getLogLevel();

    auto toggleSprite = CCSprite::createWithSpriteFrameName(frames[logLevel]);
    toggleSprite->setScale(0.9f);

    m_toggle = CCMenuItemSpriteExtra::create(toggleSprite, this, menu_selector(ModItem::onToggle));
    m_toggle->setTag(logLevel);

    nameLabel->setScale(0.4f);
    nameLabel->setAnchorPoint({ 0.0f, 0.5f });

    constexpr float padding_left = 60.000f;

    float calc = size.width - padding_left;
    if (nameLabel->getScaledContentWidth() >= calc) {
        nameLabel->setScale(calc / nameLabel->getContentWidth());
    }

    this->addChildAtPosition(
        nameLabel,
        Anchor::Left,
        { 26, 4 }
    );

    auto idLabel = CCLabelBMFont::create(
        mod->getID().c_str(),
        "bigFont.fnt"
    );

    idLabel->setScale(0.2f);
    idLabel->setAnchorPoint({0.0f, 0.5f});
    idLabel->setOpacity(128);

    this->addChildAtPosition(
        idLabel,
        Anchor::Left,
        { 26, -6 }
    );

    auto menu = CCMenu::create();
    menu->setAnchorPoint({1.0f, 0.5f});
    menu->setContentSize({ padding_left, size.height});
    menu->addChildAtPosition(
        m_toggle,
        Anchor::Right,
        { 0, 0 }
    );

    auto layout = RowLayout::create();
    layout->setDefaultScaleLimits(0.5f, 0.7f);
    layout->setAxisAlignment(AxisAlignment::End);
    layout->setAxisReverse(true);
    menu->setLayout(layout);

    this->addChildAtPosition(
        menu,
        Anchor::Right,
        { -3, 0 }
    );

    return true;
}

void ModItem::onToggle(CCObject* sender){
    auto tag = sender->getTag();
    tag = tag == 4 ? 0 : tag + 1;
    sender->setTag(tag);

    static_cast<CCSprite*>(m_toggle->getNormalImage())->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName(frames[tag]));
    m_toggle->updateSprite();

    m_mod->setLogLevel(Severity::cast(tag));
    Mod::get()->setSavedValue<int>(m_mod->getID(), tag);
}

ModItem* ModItem::create(Mod* mod, CCSize const& size) {
    auto ret = new ModItem();
    if (!ret || !ret->init(mod, size)) {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
    ret->autorelease();
    return ret;
}