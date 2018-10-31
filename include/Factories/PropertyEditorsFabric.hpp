#pragma once

// C++ STL
#include <memory>
#include <unordered_map>
#include <functional>

namespace HG::Editor
{
    class AbstractPropertyProcessor;

    /**
     * @brief Class, that describes fabric for
     */
    class PropertyEditorsFabric
    {
    public:

        using PropertyProcessorPtr = std::shared_ptr<HG::Editor::AbstractPropertyProcessor>;

        /**
         * @brief Constructor.
         */
        PropertyEditorsFabric();

        /**
         * @brief Property processor constructor
         * @param typeHash Type hash id.
         * @return Shared pointer with property processor. Or nullptr if
         * can't create processor.
         */
        PropertyProcessorPtr create(std::size_t typeHash);

        /**
         * @brief Method, that performs registration of some
         * type processor into this fabric.
         * @tparam Processor Processor type.
         * @param typeHash Target type hash.
         */
        template<typename Processor>
        void registrate(std::size_t typeHash)
        {
            m_data[typeHash] = {
                nullptr,
                []() -> PropertyProcessorPtr
                {
                    return std::make_shared<Processor>();
                }
            };
        }

        /**
         * @brief Method, that performs clearing of
         * fabric data.
         */
        void clear();

    private:

        std::unordered_map<
            std::size_t,
            std::pair<
                std::shared_ptr<HG::Editor::AbstractPropertyProcessor>,
                std::function<PropertyProcessorPtr()>
            >
        > m_data;

    };
}
