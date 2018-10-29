#pragma once

// HG::Core
#include <HG/Core/Scene.hpp>

namespace HG::Editor
{
    /**
     * @brief Default scene. If any other scene is opened -
     * this scene is manages setting up of new scene
     * with editing systems.
     */
    class EditorScene : public HG::Core::Scene
    {
    public:
        /**
         * @brief Constructor.
         * @param sceneToEdit Scene to setup for editing.
         */
        explicit EditorScene(HG::Core::Scene* sceneToEdit=nullptr);

        /**
         * @brief Method for setting up.
         */
        void start() override;

    private:

        HG::Core::Scene* m_scene;
    };
}


