/**
 * @file
 */

#pragma once

#include "voxel/Mesh.h"
#include "core/IComponent.h"
#include "core/StringUtil.h"
#include "core/collection/StringMap.h"
#include <memory>

namespace voxelformat {

/**
 * @brief Cache @c voxel::Mesh instances by their name
 */
class MeshCache : public core::IComponent {
protected:
	core::StringMap<voxel::Mesh*> _meshes;
	int _initCalls = 0;

	voxel::Mesh& cacheEntry(const char *fullPath);
	bool loadMesh(const char* fullPath, voxel::Mesh& mesh);
public:
	~MeshCache();
	const voxel::Mesh* getMesh(const char *fullPath);
	bool removeMesh(const char *fullPath);
	bool init() override;
	void shutdown() override;
};

using MeshCachePtr = std::shared_ptr<MeshCache>;

}
