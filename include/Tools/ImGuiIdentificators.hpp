#pragma once

#define STR_ID(ID_NS, NAME, VALUE)\
namespace ID \
{ \
    namespace ID_NS \
    { \
        constexpr const char* NAME = #ID_NS "::" #NAME "_" VALUE ;\
    } \
}

#define STR_NAME_ID(ID_NS, NAME, VALUE) \
namespace Names \
{ \
    namespace ID_NS \
    { \
        constexpr const char* NAME = VALUE; \
    } \
} \
STR_ID(ID_NS, NAME, VALUE)

namespace HG
{
    // Names/Menu
    STR_NAME_ID(Inspector,        Window,          "Inspector");
    STR_NAME_ID(Inspector,        EnabledCheckbox, "");

    STR_NAME_ID(GameObject,       Window,       "GameObject");
    STR_ID     (GameObject,       ContextMenu,  "GameObject");
    STR_ID     (GameObject,       Header,       "GameObject");
    STR_ID     (GameObject,       TreeItem,     "TreeItem");

    STR_NAME_ID(GameObject::Menu, Copy,         "Copy");
    STR_NAME_ID(GameObject::Menu, Paste,        "Paste");
    STR_NAME_ID(GameObject::Menu, Duplicate,    "Duplicate");
    STR_NAME_ID(GameObject::Menu, Rename,       "Rename");
    STR_NAME_ID(GameObject::Menu, Delete,       "Delete");
    STR_NAME_ID(GameObject::Menu, CreateEmpty,  "CreateEmpty");
    STR_NAME_ID(GameObject::Menu, Menu3DObject, "3D Object");
    STR_NAME_ID(GameObject::Menu, Cube,         "Cube");
    STR_NAME_ID(GameObject::Menu, Menu2DObject, "2D Object");
    STR_NAME_ID(GameObject::Menu, Sprite,       "Sprite");
    STR_NAME_ID(GameObject::Menu, MenuLight,    "Light");
    STR_NAME_ID(GameObject::Menu, PointLight,   "PointLight");
    STR_NAME_ID(GameObject::Menu, MenuAudio,    "MenuAudio");
    STR_NAME_ID(GameObject::Menu, MenuUI,       "UI");
    STR_NAME_ID(GameObject::Menu, Camera,       "Camera");

    STR_NAME_ID(Logging,          Window,       "Logging");

    STR_NAME_ID(OpenPath,         Window,       "Explorer");
    STR_NAME_ID(OpenPath,         Child,        "");
    STR_ID     (OpenPath::Button, Path,         "PathButton");
    STR_NAME_ID(OpenPath::Button, Cancel,       "Cancel");
    STR_NAME_ID(OpenPath::Button, Ok,           "Ok");
    STR_NAME_ID(OpenPath::Item,   Back,         "..");
    STR_NAME_ID(OpenPath::Input,  Path,         "");
    STR_NAME_ID(OpenPath::Combo,  Extension,    "");

    STR_NAME_ID(InformationModal, Window,       "Info");

    STR_NAME_ID(Scene,            Window,       "Scene");

    // DND
    STR_ID     (DragDrop,         GameObject,   "GameObject");
}