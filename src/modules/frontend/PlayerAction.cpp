/**
 * @file
 */

#include "PlayerAction.h"
#include "core/command/Command.h"
#include "core/Trace.h"
#include "frontend/ClientEntity.h"

namespace frontend {

bool PlayerAction::init() {
	return true;
}

void PlayerAction::update(double nowSeconds, const ClientEntityPtr& entity) {
	core_trace_scoped(UpdatePlayerAction);
	// TODO: if not gliding or diving
	if (_triggerAction.pressed()) {
		_triggerAction.execute(nowSeconds, 0.1, [&entity, this] () {
			entity->addAnimation(network::Animation::TOOL, 0.1);
			++_triggerActionCounter;
		});
	}
}

void PlayerAction::construct() {
	core::Command::registerActionButton("triggeraction", _triggerAction);
}

void PlayerAction::shutdown() {
	core::Command::unregisterActionButton("triggeraction");
	_triggerAction.handleUp(core::ACTION_BUTTON_ALL_KEYS, 0.0);
}

}
