#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include "ModListPopup.hpp"

using namespace geode::prelude;

#include <Geode/modify/CCLayer.hpp>
class $modify(ModsLayerExt, CCLayer) {

	void tryCustomSetup(float) {
		if (!typeinfo_cast<CCLayer*>(this)) return;

		bool geodeTheme = Loader::get()->getLoadedMod("geode.loader")->getSettingValue<bool>("enable-geode-theme");

		if (this->getID() == "ModsLayer") {
			if (CCMenu* actionsMenu = typeinfo_cast<CCMenu*>(this->getChildByID("actions-menu"))) {

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
		}
	}

	$override bool init() {

		this->scheduleOnce(
			schedule_selector(ModsLayerExt::tryCustomSetup), 0.f
		);

		return CCLayer::init();
	}

};

$execute {
	for(Mod* mod : Loader::get()->getAllMods()){
		auto value = Mod::get()->getSavedValue<bool>(mod->getID(), true);
		mod->setLoggingEnabled(value);
	}
}