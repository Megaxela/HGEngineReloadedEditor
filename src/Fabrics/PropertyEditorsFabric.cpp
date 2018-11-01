// C++ STL
#include <typeinfo>

// Editor
#include <Fabrics/PropertyEditorsFabric.hpp>
#include <DefaultPropertyProcessors/FloatProcessor.hpp>
#include <DefaultPropertyProcessors/DoubleProcessor.hpp>
#include <DefaultPropertyProcessors/NumberProcessor.hpp>
#include <DefaultPropertyProcessors/ProjectionProcessor.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Camera.hpp>

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

void HG::Editor::Fabrics::PropertyEditorsFabric::registrateDefault()
{
    registrate<HG::Editor::PropertyProcessors::FloatProcessor>(
        typeid(float).hash_code()
    );
    registrate<HG::Editor::PropertyProcessors::DoubleProcessor>(
        typeid(double).hash_code()
    );
    registrate<HG::Editor::PropertyProcessors::NumberProcessor<uint8_t>>(
            typeid(uint8_t).hash_code()
    );
    registrate<HG::Editor::PropertyProcessors::NumberProcessor<int8_t>>(
            typeid(int8_t).hash_code()
    );
    registrate<HG::Editor::PropertyProcessors::NumberProcessor<uint16_t>>(
            typeid(uint16_t).hash_code()
    );
    registrate<HG::Editor::PropertyProcessors::NumberProcessor<int16_t>>(
            typeid(int16_t).hash_code()
    );
    registrate<HG::Editor::PropertyProcessors::NumberProcessor<uint32_t>>(
            typeid(uint32_t).hash_code()
    );
    registrate<HG::Editor::PropertyProcessors::NumberProcessor<int32_t>>(
            typeid(int32_t).hash_code()
    );
    registrate<HG::Editor::PropertyProcessors::NumberProcessor<uint64_t>>(
            typeid(uint64_t).hash_code()
    );
    registrate<HG::Editor::PropertyProcessors::NumberProcessor<int64_t>>(
            typeid(int64_t).hash_code()
    );
    registrate<HG::Editor::PropertyProcessors::ProjectionProcessor>(
        typeid(HG::Editor::PropertyProcessors::ProjectionProcessor).hash_code()
    );
}
