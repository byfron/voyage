//
// Created by rubio on 27/03/17.
//

#ifndef VOYAGE_VISIBILITYMANAGER_HPP
#define VOYAGE_VISIBILITYMANAGER_HPP

#include <pumpkin.hpp>

class VisibilityManager {
public:

    void setPlayerPosition(const Vec2f & player_pos);

    std::vector<Vec2f> getPolygon() {}

protected:

    Vec2f m_player_pos;
};


#endif //VOYAGE_VISIBILITYMANAGER_HPP
