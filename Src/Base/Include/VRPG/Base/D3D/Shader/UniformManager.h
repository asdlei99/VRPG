#pragma once

#include <tuple>

#include <VRPG/Base/D3D/Shader/ConstantBufferSlot.h>
#include <VRPG/Base/D3D/Shader/ShaderResourceSlot.h>
#include <VRPG/Base/D3D/Shader/SamplerSlot.h>
#include <VRPG/Base/D3D/Shader/Stage.h>

VRPG_BASE_D3D_BEGIN

namespace Impl
{

    template<ShaderStage STAGE>
    class StageUniformManager
    {
        ConstantBufferSlotManager<STAGE> constantBufferSlotMgr_;
        ShaderResourceSlotManager<STAGE> shaderResourceSlotMgr_;
        SamplerSlotManager<STAGE>        samplerSlotMgr_;

    public:

        explicit StageUniformManager(const Stage<STAGE> &stage)
            : constantBufferSlotMgr_(stage.GetConstantBufferTable()),
            shaderResourceSlotMgr_(stage.GetShaderResourceTable()),
            samplerSlotMgr_(stage.GetSamplerTable())
        {

        }

        StageUniformManager(const StageUniformManager &) = default;
        StageUniformManager &operator=(const StageUniformManager &) = default;

        StageUniformManager(StageUniformManager &&)            noexcept = default;
        StageUniformManager &operator=(StageUniformManager &&) noexcept = default;

        ~StageUniformManager() = default;

        ConstantBufferSlotManager<STAGE> *GetConstantBufferSlotManager() noexcept
        {
            return &constantBufferSlotMgr_;
        }

        ShaderResourceSlotManager<STAGE> *GetShaderResourceSlotManager() noexcept
        {
            return &shaderResourceSlotMgr_;
        }

        SamplerSlotManager<STAGE> *GetSamplerSlotManager() noexcept
        {
            return &samplerSlotMgr_;
        }

        const ConstantBufferSlotManager<STAGE> *GetConstantBufferSlotManager()  const noexcept
        {
            return &constantBufferSlotMgr_;
        }

        const ShaderResourceSlotManager<STAGE> *GetShaderResourceSlotManager()  const noexcept
        {
            return &shaderResourceSlotMgr_;
        }

        const SamplerSlotManager<STAGE> *GetSamplerSlotManager()  const noexcept
        {
            return &samplerSlotMgr_;
        }

        ConstantBufferSlot<STAGE> *GetConstantBufferSlot(const std::string &name)
        {
            return constantBufferSlotMgr_.Get(name);
        }

        ShaderResourceSlot<STAGE> *GetShaderResourceSlot(const std::string &name)
        {
            return shaderResourceSlotMgr_.Get(name);
        }

        SamplerSlot<STAGE> *GetSamplerSlot(const std::string &name)
        {
            return samplerSlotMgr_.Get(name);
        }

        const ConstantBufferSlot<STAGE> *GetConstantBufferSlot(const std::string &name) const
        {
            return constantBufferSlotMgr_.Get(name);
        }

        const ShaderResourceSlot<STAGE> *GetShaderResourceSlot(const std::string &name) const
        {
            return shaderResourceSlotMgr_.Get(name);
        }

        const SamplerSlot<STAGE> *GetSamplerSlot(const std::string &name) const
        {
            return samplerSlotMgr_.Get(name);
        }

        void Bind() const
        {
            constantBufferSlotMgr_.Bind();
            shaderResourceSlotMgr_.Bind();
            samplerSlotMgr_.Bind();
        }

        void Unbind() const
        {
            constantBufferSlotMgr_.Unbind();
            shaderResourceSlotMgr_.Unbind();
            samplerSlotMgr_.Unbind();
        }
    };

} // namespace Impl

template<ShaderStage...STAGES>
class UniformManager
{
    std::tuple<Impl::StageUniformManager<STAGES>...> stageUniformMgrs_;

    template<ShaderStage STAGE>
    Impl::StageUniformManager<STAGE> *GetStageUniforms()
    {
        return &std::get<Impl::StageUniformManager<STAGE>>(stageUniformMgrs_);
    }

    template<ShaderStage STAGE>
    const Impl::StageUniformManager<STAGE> *GetStageUniforms() const
    {
        return &std::get<Impl::StageUniformManager<STAGE>>(stageUniformMgrs_);
    }

public:

    explicit UniformManager(const Stage<STAGES>&...stages)
        : stageUniformMgrs_(stages...)
    {
        
    }

    UniformManager(const UniformManager &)            = default;
    UniformManager &operator=(const UniformManager &) = default;

    UniformManager(UniformManager &&)            noexcept = default;
    UniformManager &operator=(UniformManager &&) noexcept = default;

    ~UniformManager() = default;

    template<ShaderStage STAGE>
    ConstantBufferSlotManager<STAGE> *GetConstantBufferSlotManager()
    {
        return GetStageUniforms<STAGE>()->GetConstantBufferSlotManager();
    }

    template<ShaderStage STAGE>
    ShaderResourceSlotManager<STAGE> *GetShaderResourceSlotManager()
    {
        return GetStageUniforms<STAGE>()->GetShaderResourceSlotManager();
    }

    template<ShaderStage STAGE>
    SamplerSlotManager<STAGE> *GetSamplerSlotManager()
    {
        return GetStageUniforms<STAGE>()->GetSamplerSlotManager();
    }

    template<ShaderStage STAGE>
    const ConstantBufferSlotManager<STAGE> *GetConstantBufferSlotManager() const
    {
        return GetStageUniforms<STAGE>()->GetConstantBufferSlotManager();
    }

    template<ShaderStage STAGE>
    const ShaderResourceSlotManager<STAGE> *GetShaderResourceSlotManager() const
    {
        return GetStageUniforms<STAGE>()->GetShaderResourceSlotManager();
    }

    template<ShaderStage STAGE>
    const SamplerSlotManager<STAGE> *GetSamplerSlotManager() const
    {
        return GetStageUniforms<STAGE>()->GetSamplerSlotManager();
    }

    template<ShaderStage STAGE>
    ConstantBufferSlot<STAGE> *GetConstantBufferSlot(const std::string &name)
    {
        return GetStageUniforms<STAGE>()->GetConstantBufferSlot(name);
    }

    template<ShaderStage STAGE>
    ShaderResourceSlot<STAGE> *GetShaderResourceSlot(const std::string &name)
    {
        return GetStageUniforms<STAGE>()->GetShaderResourceSlot(name);
    }

    template<ShaderStage STAGE>
    SamplerSlot<STAGE> *GetSamplerSlot(const std::string &name)
    {
        return GetStageUniforms<STAGE>()->GetSamplerSlot(name);
    }

    template<ShaderStage STAGE>
    const ConstantBufferSlot<STAGE> *GetConstantBufferSlot(const std::string &name) const
    {
        return GetStageUniforms<STAGE>()->GetConstantBufferSlot(name);
    }

    template<ShaderStage STAGE>
    const ShaderResourceSlot<STAGE> *GetShaderResourceSlot(const std::string &name) const
    {
        return GetStageUniforms<STAGE>()->GetShaderResourceSlot(name);
    }

    template<ShaderStage STAGE>
    const SamplerSlot<STAGE> *GetSamplerSlot(const std::string &name) const
    {
        return GetStageUniforms<STAGE>()->GetSamplerSlot(name);
    }

    void Bind() const
    {
        std::apply(
            [](const auto &...mgr) { ((mgr.Bind()), ...); },
            stageUniformMgrs_);
    }

    void Unbind() const
    {
        std::apply(
            [](const auto &...mgr) { ((mgr.Unbind()), ...); },
            stageUniformMgrs_);
    }
};

VRPG_BASE_D3D_END
