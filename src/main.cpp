#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include "ModListPopup.hpp"

using namespace geode::prelude;

#include <Geode/modify/CCLayer.hpp>

class ModsLayer : public CCNode {};

class $modify(ModsLayerExt, CCLayer) {	
    bool init() {
    	if (!CCLayer::init()) return false;
		if (typeinfo_cast<ModsLayer*>(this)) {
			queueInMainThread([this] {
				bool geodeTheme = Loader::get()->getLoadedMod("geode.loader")->getSettingValue<bool>("enable-geode-theme");
				if (CCMenu* actionsMenu = typeinfo_cast<CCMenu*>(getChildByID("actions-menu"))) {

					CCSprite* spr = CircleButtonSprite::createWithSprite(
						"terminal.png"_spr, .85f,
						(geodeTheme ? CircleBaseColor::DarkPurple : CircleBaseColor::Green)
					);
					spr->setScale(0.8f);

					CCMenuItemSpriteExtra* logsBtn = CCMenuItemExt::createSpriteExtra(
						spr, [](CCNode*) {
							ModListPopup::create()->show();
						}
					);
					logsBtn->setID("mods-logs-button");
					actionsMenu->addChild(logsBtn);
					actionsMenu->updateLayout();
				}
			});
		}
		return true;
	}
};

$on_mod(Loaded) {
	Mod* unlog = Mod::get();
	auto& saveContainer = unlog->getSaveContainer();
	for(Mod* mod : Loader::get()->getAllMods()){
		auto id = mod->getID();
		if (saveContainer.contains(id) && !saveContainer[id].asBool().unwrapOr(true)) {
			saveContainer[id] = 4;
		}

		mod->setLogLevel(Severity::cast(std::clamp(unlog->getSavedValue<int>(id, 0), 0, 4)));
	}
}