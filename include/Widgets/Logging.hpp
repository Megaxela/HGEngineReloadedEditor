#pragma once

// Editor
#include <Widgets/AbstractWidget.hpp>

// ALogger
#include <Loggers/AbstractLogger.hpp>

// RingBuffer
#include <ringbuffer.hpp>

namespace HG::Editor::Widgets
{
    /**
     * @brief Class, that describes logging widget.
     */
    class Logging : public AbstractWidget
    {
    public:

        /**
         * @brief Constructor.
         */
        Logging();

        /**
         * @brief Method for settings logs listener.
         * @param logsListener Smart pointer to logs listener.
         */
        void setLogsListener(Logger::LogsListenerPtr logsListener);

        /**
         * @brief Method for getting logs listener for widget.
         */
        Logger::LogsListenerPtr logsListener() const;

    protected:
        void onDraw() override;

        void onUpdate() override;

    private:
        Logger::LogsListenerPtr m_logsListener;

        ringbuffer<AbstractLogger::Message, 1000> m_messagesBuffer;
    };
}
