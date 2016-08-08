// Add this to your imconfig.h

#include <SFML/System/Vector2.hpp>
#include <3rdParty/imgui-sfml/export.hpp>

#define IM_VEC2_CLASS_EXTRA                                                 \
        ImVec2(const sf::Vector2f& v) { x = v.x; y = v.y; }                 \
        operator sf::Vector2f() const { return sf::Vector2f(x, y); }

#define IMGUI_API IMGUI_SFML_EXPORT