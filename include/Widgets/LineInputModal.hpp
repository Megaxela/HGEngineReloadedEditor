#pragma once

// C++ STL
#include <string>
#include <functional>

// Editor
#include <Widgets/AbstractWidget.hpp>

namespace HG::Editor::Widgets
{
    /**
     * @brief Class, that describes simple modal
     * window for renaming etc.
     */
    class LineInputModal : public AbstractWidget
    {
    public:

        constexpr static std::size_t MaxLength = 256;

        using SuccessCallback = std::function<void(std::string)>;
        using CancelCallback = std::function<void()>;

        /**
         * @brief Constructor.
         */
        LineInputModal();

        /**
         * @brief Destructor.
         */
        ~LineInputModal() override;

        /**
         * @brief Method for setting callback, that will
         * be called at next execution after pressing OK.
         * @param callback Callback.
         */
        void setOnSuccessCallback(SuccessCallback callback);

        /**
         * @brief Method for setting callback, that will
         * be called at next execution after pressing Cancel.
         * @param callback Callback.
         */
        void setOnCancelCallback(CancelCallback callback);

        /**
         * @brief Method, that shows line input modal with
         * provided body text.
         * @param title Modal title.
         * @param text Modal body text.
         */
        void execute(const std::string& value, std::string text=std::string());

    protected:

        /**
         * @brief Method for drawing widget.
         */
        void onDraw() override;

    private:
        void cleanAndClose();

        char m_buffer[MaxLength];

        std::string m_text;

        SuccessCallback m_successCallback;
        CancelCallback m_cancelCallback;

        bool m_currentOpenState;
    };
}


