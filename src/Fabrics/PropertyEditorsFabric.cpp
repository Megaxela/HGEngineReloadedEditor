#include <Fabrics/PropertyEditorsFabric.hpp>

HG::Editor::Fabrics::PropertyEditorsFabric::PropertyEditorsFabric() :
    m_data()
{

}

std::shared_ptr<HG::Editor::AbstractPropertyProcessor> HG::Editor::Fabrics::PropertyEditorsFabric::create(std::size_t typeHash)
{
    auto iterator = m_data.find(typeHash);

    if (iterator == m_data.end())
    {
        return nullptr;
    }

    auto& pair = iterator->second;

    if (pair.first != nullptr)
    {
        return pair.first;
    }

    pair.first = pair.second();

    return pair.first;
}

void HG::Editor::Fabrics::PropertyEditorsFabric::clear()
{
    m_data.clear();
}
