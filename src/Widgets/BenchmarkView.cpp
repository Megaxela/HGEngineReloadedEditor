// Editor
#include <Editor/Application.hpp>
#include <Widgets/BenchmarkView.hpp>
#include <Tools/ImGuiWidgets.hpp>
#include <Tools/ImGuiIdentificators.hpp>

// C++ STL
#include <sstream>

// HG::Core
#include <HG/Core/Benchmark.hpp>
#include <cmath>

// ALogger
#include <CurrentLogger.hpp>

HG::Editor::Widgets::BenchmarkView::BenchmarkView() :
    m_pixelsPerSecond(360000)
{

}

HG::Editor::Widgets::BenchmarkView::~BenchmarkView()
{

}

void HG::Editor::Widgets::BenchmarkView::onDraw()
{
    ImGui::IDGuard idGuard(HG::ID::BenchmarkView::Window);

    auto flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_HorizontalScrollbar;

    if (ImGui::Begin(HG::Names::BenchmarkView::Window, &m_opened, flags))
    {
        drawToolbar();

        drawBenchmark(application()->benchmark());
    }

    ImGui::End();
}

void HG::Editor::Widgets::BenchmarkView::drawToolbar()
{
    ImGui::BeginMenuBar();

    if (!application()->benchmark()->isRunning())
    {
        // todo: replace with HG::ID value
        if (ImGui::MenuItem("Run"))
        {
            application()->benchmark()->start();
        }
    }
    else
    {
        application()->benchmark()->finish();
        // todo: replace with HG::ID value
        if (ImGui::MenuItem("Stop"))
        {
            application()->benchmark()->finish();
        }
    }

    // todo: replace with HG::ID value
    if (ImGui::MenuItem("Clear"))
    {
        application()->benchmark()->clear();
    }

    ImGui::EndMenuBar();
}

void HG::Editor::Widgets::BenchmarkView::drawBenchmark(HG::Core::Benchmark *benchmark)
{

    // Benchmark total
    auto benchStart = benchmark->startTime();
    auto benchFinish = benchmark->finishTime();

    // Total microseconds time
    auto benchDuration = benchFinish - benchStart;

    // Amount of seconds
    auto seconds = std::chrono::duration_cast<
            std::chrono::microseconds
    >(benchDuration).count() / double(std::chrono::duration_cast<
            std::chrono::microseconds
    >(std::chrono::seconds(1)).count());

    // Total width
    auto totalWidth = static_cast<int>(std::ceil(seconds * m_pixelsPerSecond));

    // Threads
    std::vector<std::thread::id> ids;
    benchmark->getThreadsId(ids);

    {
        auto benchDurationMcs = std::chrono::duration_cast<std::chrono::microseconds>(benchDuration).count();

        ImGui::Text(
                "Total time: %02d:%02d.%03d.%03d",
                ((benchDurationMcs % 1000000000) / 1000000 / 60) % 60,
                 (benchDurationMcs % 1000000000) / 1000000,
                 (benchDurationMcs % 1000000   ) / 1000,
                 (benchDurationMcs % 1000      )
        );
    }

    ImGui::Columns(2);

    std::vector<bool> opened;

    // todo: remove this check and enable ongoing display
    if (benchmark->isRunning())
    {
        return;
    }

    // Drawing tree widgets
    for (const auto& id : ids)
    {
        // todo: FIX THIS SHITCODE FFS
        std::stringstream ss;
        ss << id;

        opened.push_back(
            ImGui::TreeNode(ss.str().c_str(), "Thread: %s", ss.str().c_str())
        );

        if (opened.back())
        {
            ImGui::TreePop();
        }
    }

    ImGui::NextColumn();

    ImGui::BeginChild(HG::ID::BenchmarkView::Child, {0, 0}, true, ImGuiWindowFlags_HorizontalScrollbar);

    auto pixelsPerSecond = ImGui::GetWindowContentRegionWidth() / seconds;

    int buttonHeight = 0;

    decltype(opened)::size_type i = 0;
    decltype(opened)::size_type j = 0;
    for (const auto& id : ids)
    {
        auto jobsStack = benchmark->getClosedJobs(id);

        auto currentOpened = opened[i++];

        // Draw base line
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0, 0});

        std::stack<std::size_t> depthStack;

        while (!jobsStack.empty())
        {
            const auto job = std::move(jobsStack.top());
            jobsStack.pop();

            // Skip anything if tree widget was not open
            auto skipLayer = !currentOpened && depthStack.size() > 0;

            // Calculate depth shit

            if (!depthStack.empty())
            {
                --depthStack.top();

                if (depthStack.top() == 0)
                {
                    depthStack.pop();
                }
            }

            auto currentStack = depthStack.size();

            if (job.subjobs > 0)
            {
                depthStack.push(job.subjobs);
            }

            if (skipLayer)
            {
                continue;
            }

            // Calculating desired position
            auto desiredPosition = (std::chrono::duration_cast<
                    std::chrono::microseconds
            >(job.startTime - benchStart).count() / float(std::chrono::duration_cast<
                    std::chrono::microseconds
            >(std::chrono::seconds(1)).count())) * pixelsPerSecond;

            // Calculating job's width, based on
            // duration
            auto width = (std::chrono::duration_cast<
                    std::chrono::microseconds
            >(job.finishTime - job.startTime).count() / double(std::chrono::duration_cast<
                    std::chrono::microseconds
            >(std::chrono::seconds(1)).count())) * pixelsPerSecond;

            if (width < 2)
            {
                continue;
            }

            // Moving cursor
            ImGui::SetCursorPos({float(desiredPosition), float(currentStack * buttonHeight)});

            ImGui::IDGuard buttonGuard(++j);

            float hue = i*0.05f;
            ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue, 0.6f, 0.6f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue, 0.7f, 0.7f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue, 0.8f, 0.8f));
            ImGui::Button(job.name.c_str(), ImVec2(static_cast<float>(width), 0.0f));



            if (buttonHeight == 0)
            {
                buttonHeight = ImGui::GetItemRectSize().y;
            }

            ImGui::PopStyleColor(3);
        }

        ImGui::PopStyleVar(1);
    }

    ImGui::EndChild();
    ImGui::Columns();

}

void HG::Editor::Widgets::BenchmarkView::drawMetricBar(std::chrono::steady_clock::time_point begin,
                                                       std::chrono::steady_clock::time_point finish)
{

}
