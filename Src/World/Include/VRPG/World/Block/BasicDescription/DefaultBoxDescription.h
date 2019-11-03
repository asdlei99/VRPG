#pragma once

#include <VRPG/World/Block/BlockDescription.h>

VRPG_WORLD_BEGIN

class DefaultBlockEffect;

class DefaultBlockDescription : public BlockDescription
{
    std::shared_ptr<const DefaultBlockEffect> effect_;

public:

    explicit DefaultBlockDescription(std::shared_ptr<const DefaultBlockEffect> effect) noexcept;

    const char *GetName() const override { return "default"; }

    FaceVisibilityProperty GetFaceVisibilityProperty(Direction direction) const noexcept override;

    bool IsVisible() const noexcept override;

    bool IsLightSource() const noexcept override;

    BlockBrightness LightAttenuation() const noexcept override;

    BlockBrightness InitialBrightness() const noexcept override;

    void AddBlockModel(
        PartialSectionModelBuilderSet &modelBuilders,
        const Vec3i &blockPosition,
        const BlockDescription *neighborBlocks[3][3][3],
        const BlockBrightness neighborBrightness[3][3][3],
        const BlockOrientation neighborOrientations[3][3][3]) const override;
};

VRPG_WORLD_END