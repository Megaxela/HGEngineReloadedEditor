// Editor
#include <Scenes/EditorScene.hpp>
#include <EditBehaviours/GraphicsInterface.hpp>

// HG::Core
#include <GameObjectBuilder.hpp>
#include <Application.hpp>

// HG::Rendering::Base
#include <Camera.hpp>

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
        .setGlobalPosition({0, 0, 0})
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

    auto parentGO = HG::Core::GameObjectBuilder()
        .setName("Parent1")
        .deploy();

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
