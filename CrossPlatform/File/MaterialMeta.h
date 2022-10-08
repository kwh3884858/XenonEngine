#pragma once
#include "CrossPlatform/File/IFileMeta.h"
namespace XenonEngine
{
	class ObjectLoader;
}
namespace CrossPlatform
{
	class Material;
	class MaterialMeta :public IFileMeta
	{
	public:
		friend class XenonEngine::ObjectLoader;

		MaterialMeta(const FileHeader& header) :IFileMeta(header) { }
		virtual ~MaterialMeta() override = default;

		// Load into memory
		virtual void Load() override;

		// Clear from memory
		virtual void Clear() override;

		// Save to hard drive as a data file
		virtual void Save() override;

		// Delete data file from hard drive
		virtual void Delete() override;

		const Material* const GetMaterial()const { return m_material; }

	private:
		Material* m_material;
	};
}