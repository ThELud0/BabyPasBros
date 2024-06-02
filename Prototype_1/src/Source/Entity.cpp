#include "Entity.hpp"

Entity::Entity(float x, float y, int hei, int wid) : x(x), y(y), height(hei), width(wid) {}

Entity::Entity(const pugi::xml_node& node) : x(node.attribute("x").as_float()), 
											y(node.attribute("y").as_float()), 
											height(node.attribute("h").as_int()), 
											width(node.attribute("w").as_int()) {}