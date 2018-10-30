// Editor
#include <Scenes/EditorScene.hpp>
#include <EditBehaviours/GraphicsInterface.hpp>

// HG::Core
#include <HG/Core/GameObjectBuilder.hpp>
#include <HG/Core/Application.hpp>
#include <HG/Core/ResourceManager.hpp>

// HG::Rendering::Base
#include <HG/Rendering/Base/Camera.hpp>
#include <HG/Rendering/Base/Material.hpp>
#include <HG/Rendering/Base/Renderer.hpp>
#include <HG/Rendering/Base/Behaviours/Mesh.hpp>
#include <HG/Rendering/Base/MaterialCollection.hpp>

// HG::Utils
#include <HG/Utils/Loaders/AssimpLoader.hpp>
#include <HG/Utils/Model.hpp>

// GLM
#include <glm/gtx/quaternion.hpp>
#include <HG/Utils/Color.hpp>

class PlainMaterial : public HG::Rendering::Base::Material
{
public:

    static constexpr const char* rawShader = R"(
#ifdef VertexShader
layout (location = 0) in vec3 inPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(inPosition, 1.0f);
}
#endif

#ifdef FragmentShader
uniform vec3 color;

out vec4 FragColor;

void main()
{
    FragColor = vec4(color, 1.0f);
}
#endif
)";

    void setColor(const HG::Utils::Color& color)
    {
        set("color", color.toRGBVector());
    }
};

HG::Editor::EditorScene::EditorScene(HG::Core::Scene *sceneToEdit) :
    m_scene(sceneToEdit)
{

}

void HG::Editor::EditorScene::start()
{
    // Creating editing camera
    auto editingCameraGameObject = HG::Core::GameObjectBuilder()
        .setName(".EditorCamera")
        // Setting object to hidden mode
        // to hide it from user code.
        .setHidden(true)
        .setGlobalPosition({0.0f, 0.0f, 2.5f})
        // Adding camera behaviour
        .addBehaviour(new HG::Rendering::Base::Camera)
        // Adding behaviours for GUI rendering
        .addBehaviour(new HG::Editor::Behaviours::GraphicsInterface)
        .deploy();

    // If there is scene to edit -
    // adding camera to it and setting it like active
    // scene
    if (m_scene != nullptr)
    {
        m_scene->addGameObject(editingCameraGameObject);

        application()->setScene(m_scene);
    }
    // Otherwise - setup this scene as empty for editing.
    else
    {
        addGameObject(editingCameraGameObject);
    }

    // Loading model
    auto cubeModel = application()->resourceManager()
        ->load<HG::Utils::AssimpLoader>("../engine/examples/RenderToTexture/Assets/Models/cube.obj")
        .guaranteeGet();

    // Creating material
    auto material = registerResource(
            application()
                ->renderer()
                ->materialCollection()
                ->getMaterial<PlainMaterial>()
    );

    material->setColor(HG::Utils::Color::fromRGB(20, 0, 0));

    auto parentGO = HG::Core::GameObjectBuilder()
        .setName("Parent1")
        .addBehaviour(
            new HG::Rendering::Base::Behaviours::Mesh(
                  cubeModel->children()[0]->meshes()[0],
                  material
            )
        ).deploy();

    auto parentGO2 = HG::Core::GameObjectBuilder()
        .setName("Parent2")
        .deploy();

    auto child1 = HG::Core::GameObjectBuilder()
        .setName("Child1")
        .setParent(parentGO)
        .deploy();

    auto child2 = HG::Core::GameObjectBuilder()
        .setName("Child2")
        .setParent(parentGO2)
        .deploy();

    addGameObject(parentGO);
    addGameObject(parentGO2);
    addGameObject(child1);
    addGameObject(child2);
}
