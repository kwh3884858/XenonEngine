#pragma once
#include "Algorithms/TypeString.h"
namespace CrossPlatform
{
    enum FileType : int
    {
        FileTypeNone,
        FileTypeFolder,
		FileTypeMesh3D,
		FileTypePolygon,
		FileTypeMaterial,
		FileTypeObjMaterial,
        FileTypeWorld,
		FileTypeImage
    };

    const Algorithm::TypeString<FileType> FileTypeString[] = {
		{"None", FileType::FileTypeNone},
		{"Folder", FileType::FileTypeFolder},
		{"Mesh3D", FileType::FileTypeMesh3D},
		{"Polygon", FileType::FileTypePolygon},
		{"Material", FileType::FileTypeMaterial},
		{"ObjMaterial", FileType::FileTypeObjMaterial},
		{"World", FileType::FileTypeWorld},
		{"Image", FileType::FileTypeImage},
    };

	const Algorithm::TypeString<iFI> FileTypeString[] = {
	{"Folder", FolderMeta},
	{"Mesh3D", Mesh3DMeta},
	{"Polygon", Polygon3DMeta},
	{"Material", MaterialMeta},
	{"World", GameObjectWorldMeta},
	{"Image", ImageMeta},
	};
}