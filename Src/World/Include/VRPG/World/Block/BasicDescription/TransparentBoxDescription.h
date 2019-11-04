#pragma once

#include <VRPG/World/Block/BasicEffect/TransparentBlockEffect.h>
#include <VRPG/World/Block/BlockDescription.h>

VRPG_WORLD_BEGIN

class TransparentBoxDescription : public BlockDescription
{
public:

    TransparentBoxDescription(
        std::string name,
        std::shared_ptr<TransparentBlockEffect> effect, int textureIndexInEffect[],
        BlockBrightness attenuation);

    const char *GetName() const override;

    FaceVisibilityProperty GetFaceVisibilityProperty(Direction direction) const noexcept override;

    bool IsVisible() const noexcept override;

    bool IsFullOpaque() const noexcept override;

    bool IsLightSource() const noexcept override;

    BlockBrightness LightAttenuation() const noexcept override;

    BlockBrightness InitialBrightness() const noexcept override;

    void AddBlockModel(
        PartialSectionModelBuilderSet &modelBuilders,
        const Vec3i &blockPosition,
        const BlockDescription *neighborBlocks[3][3][3],
        const BlockBrightness neighborBrightness[3][3][3],
        const BlockOrientation neighborOrientations[3][3][3]) const override;

private:

    std::string name_;

    std::shared_ptr<const TransparentBlockEffect> effect_;
    int textureIndexInEffect_[6];

    BlockBrightness attenuation_;
};

VRPG_WORLD_END
