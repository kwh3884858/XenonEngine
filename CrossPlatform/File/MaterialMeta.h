#pragma once
#include "CrossPlatform/File/IFileMeta.h"
#include "CrossPlatform/Material/Material.h"
namespace XenonEngine
{
	class ObjectImporter;
}
namespace CrossPlatform
{
	//class Material;
	class MaterialMeta :public IFileMeta
	{
	public:
		friend class XenonEngine::ObjectImporter;

		MaterialMeta(const FileHeader& header) :IFileMeta(header) { }
		virtual ~MaterialMeta() override = default;

		// Load into memory
		virtual Material* Instantiate() override;

		// Clear from memory
		virtual void Clear() override;

		// Save to hard drive as a data file
		virtual void Save(const Material* material = nullptr) override;

		// Delete data file from hard drive
		virtual void Delete() override;

		//const Material* const GetMaterial()const { return m_material; }

	private:
		//Material* m_material = nullptr;
	};
}