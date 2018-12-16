#pragma once

// Editor
#include <Widgets/AbstractWidget.hpp>
#include <chrono>

namespace HG::Core
{
    class Benchmark;
}

namespace HG::Editor::Widgets
{
    /**
     * @brief Class, that describes BenchmarkView widget.
     */
    class BenchmarkView : public AbstractWidget
    {
    public:

        /**
         * @brief Constructor.
         */
        BenchmarkView();

        /**
         * @brief Destructor.
         */
        ~BenchmarkView() override;

    protected:

        /**
         * @brief Method, that describes method
         * for drawing.
         */
        void onDraw() override;

    private:

        /**
         * @brief Method for drawing benchmark toolbar.
         */
        void drawToolbar();

        /**
         * @brief Method for drawing specific benchmark.
         * @param benchmark
         */
        void drawBenchmark(HG::Core::Benchmark* benchmark);

        /**
         * @brief Method for drawing metric bar with
         * seconds, milliseconds and microseconds.
         * @param begin Begin time.
         * @param finish Finish time.
         */
        void drawMetricBar(std::chrono::steady_clock::time_point begin,
                           std::chrono::steady_clock::time_point finish);

        int m_pixelsPerSecond;
        float m_scale;
    };
}

