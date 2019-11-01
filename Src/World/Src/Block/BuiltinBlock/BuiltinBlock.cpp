#include <agz/utility/image.h>

#include <VRPG/World/Block/BasicDescription/DefaultBlockDescription.h>
#include <VRPG/World/Block/BasicDescription/DiffuseBoxDescription.h>
#include <VRPG/World/Block/BasicEffect/DefaultBlockEffect.h>
#include <VRPG/World/Block/BasicEffect/DiffuseBlockEffect.h>
#include <VRPG/World/Block/BuiltinBlock/BuiltinBlock.h>

VRPG_WORLD_BEGIN

namespace
{
	agz::math::tensor_t<Vec4b, 2> LoadTextureFrom(const std::string &filename)
	{
		return agz::img::load_rgba_from_file(filename).map(
        [](const agz::math::color4b &c)
		{
            return Vec4b(c.r, c.g, c.b, c.a);
		});
	}
}

void BuiltinBlockTypeManager::RegisterBuiltinBlockTypes()
{
	auto &descMgr = BlockDescriptionManager::GetInstance();
	auto &effectMgr = BlockEffectManager::GetInstance();
	
    {
		auto defaultEffect = std::make_shared<DefaultBlockEffect>();
		effectMgr.RegisterBlockEffect(defaultEffect);
		
		auto defaultDesc = std::make_shared<DefaultBlockDescription>(defaultEffect);
		descMgr.RegisterBlockDescription(defaultDesc);
		info_[int(BuiltinBlockType::Default)].desc = defaultDesc;
	}

    DiffuseBlockEffectGenerator diffuseEffectGenerator(64, 64);
	auto diffuseBlockEffect = std::make_shared<DiffuseBlockEffect>();
	effectMgr.RegisterBlockEffect(diffuseBlockEffect);
	
	auto prepareDiffuseTextureSpace = [&](int textureCount)
	{	
		if(!diffuseEffectGenerator.HasEnoughSpaceFor(textureCount))
		{
			diffuseEffectGenerator.InitializeEffect(*diffuseBlockEffect);
			diffuseBlockEffect = std::make_shared<DiffuseBlockEffect>();
			effectMgr.RegisterBlockEffect(diffuseBlockEffect);
		}
	};
	
	{
		prepareDiffuseTextureSpace(1);
		int textureIndex = diffuseEffectGenerator.AddTexture(LoadTextureFrom("Asset/World/Texture/BuiltinBlock/Stone.png").raw_data());
		int textureIndices[] = { textureIndex, textureIndex, textureIndex, textureIndex, textureIndex, textureIndex };
		
		auto stoneDesc = std::make_shared<DiffuseBoxDescription>(
			"stone", diffuseBlockEffect, textureIndices, BLOCK_BRIGHTNESS_MIN);
		descMgr.RegisterBlockDescription(stoneDesc);
		info_[int(BuiltinBlockType::Stone)].desc = stoneDesc;
	}
	
	{
		prepareDiffuseTextureSpace(1);
		int textureIndex = diffuseEffectGenerator.AddTexture(LoadTextureFrom("Asset/World/Texture/BuiltinBlock/Soil.png").raw_data());
		int textureIndices[] = { textureIndex, textureIndex, textureIndex, textureIndex, textureIndex, textureIndex };
		
		auto soilDesc = std::make_shared<DiffuseBoxDescription>(
			"soil", diffuseBlockEffect, textureIndices, BLOCK_BRIGHTNESS_MIN);
		descMgr.RegisterBlockDescription(soilDesc);
		info_[int(BuiltinBlockType::Soil)].desc = soilDesc;
	}
	
	{
		prepareDiffuseTextureSpace(3);
		int topIndex    = diffuseEffectGenerator.AddTexture(LoadTextureFrom("Asset/World/Texture/BuiltinBlock/LawnTop.png").raw_data());
		int sideIndex   = diffuseEffectGenerator.AddTexture(LoadTextureFrom("Asset/World/Texture/BuiltinBlock/LawnSide.png").raw_data());
		int bottomIndex = diffuseEffectGenerator.AddTexture(LoadTextureFrom("Asset/World/Texture/BuiltinBlock/LawnBottom.png").raw_data());
		
		int textureIndices[6];
		textureIndices[PositiveX] = sideIndex;
		textureIndices[PositiveY] = topIndex;
		textureIndices[PositiveZ] = sideIndex;
		textureIndices[NegativeX] = sideIndex;
		textureIndices[NegativeY] = bottomIndex;
		textureIndices[NegativeZ] = sideIndex;
		
		auto lawnDesc = std::make_shared<DiffuseBoxDescription>(
			"lawn", diffuseBlockEffect, textureIndices, BLOCK_BRIGHTNESS_MIN);
		descMgr.RegisterBlockDescription(lawnDesc);
		info_[int(BuiltinBlockType::Lawn)].desc = lawnDesc;
	}

    if(!diffuseEffectGenerator.IsEmpty())
        diffuseEffectGenerator.InitializeEffect(*diffuseBlockEffect);
}

VRPG_WORLD_END