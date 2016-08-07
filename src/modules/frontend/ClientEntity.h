/**
 * @file
 */

#pragma once

#include "ClientEntityId.h"
#include "ServerMessages_generated.h"
#include "Shared_generated.h"
#include "util/PosLerp.h"
#include "attrib/Attributes.h"
#include <vector>
#include <functional>
#include "video/Mesh.h"
#include <memory>

namespace frontend {

class ClientEntity {
private:
	util::PosLerp _posLerp;
	ClientEntityId _id;
	network::messages::EntityType _type;
	float _orientation;
	video::MeshPtr _mesh;
	attrib::Attributes _attrib;
public:
	ClientEntity(ClientEntityId id, network::messages::EntityType type, const glm::vec3& pos, float orientation, const video::MeshPtr& mesh);
	~ClientEntity();

	void lerpPosition(const glm::vec3& position, float orientation);
	void update(long dt);

	void attribUpdate(const network::messages::server::AttribEntry& attribEntry);

	inline network::messages::EntityType type() const {
		return _type;
	}

	inline ClientEntityId id() const {
		return _id;
	}

	inline float orientation() const {
		return _orientation;
	}

	inline const glm::vec3& position() const {
		return _posLerp.position();
	}

	inline float scale() const {
		return 1.0f;
	}

	inline const video::MeshPtr& mesh() const {
		return _mesh;
	}

	inline bool operator==(const ClientEntity& other) const {
		return _id == other._id;
	}
};

typedef std::shared_ptr<ClientEntity> ClientEntityPtr;

}

namespace std {
template<> struct hash<frontend::ClientEntity> {
	size_t operator()(const frontend::ClientEntity &c) const {
		return hash<int>()(c.id());
	}
};
}
