﻿#pragma once

#include <VRPG/Game/World/BlockUpdater.h>

VRPG_GAME_BEGIN

class LiquidUpdater : public BlockUpdater
{
    Vec3i blockPos_;

    /**
     * @brief 处理某个方块与其周围的液体发生的反应
     *
     * 返回true当且仅当发生了反应
     */
    bool ReactWithNeighborhood(
        const BlockInstance &block, BlockUpdaterManager &updateManager, ChunkManager &chunkManager, StdClock::time_point now);

    /**
     * @brief 处理某个方块周围流到该方块处的结果
     */
    void FlowFromNeighborhood(
        const BlockInstance &block, BlockUpdaterManager &updateManager, ChunkManager &chunkManager, StdClock::time_point now);

public:

    LiquidUpdater(StdClock::time_point expectedUpdatingTime, const Vec3i &blockPos) noexcept;

    void Execute(BlockUpdaterManager &updateManager, ChunkManager &chunkManager, StdClock::time_point now) override;
};

VRPG_GAME_END
