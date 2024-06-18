#include "Entity.hpp"

/// <summary>
/// Initialise l'entité à une position (x,y) avec une taille (width,height).
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="height"></param>
/// <param name="width"></param>
Entity::Entity(float x, float y, int hei, int wid) : x(x), y(y), height(hei), width(wid) {}

/// <summary>
/// Initialise l'entité à une position (x,y) avec une taille (width,height),
/// ces paramètres étant récupérés à partir d'un node xml.
/// </summary>
/// <param name="node"></param>
Entity::Entity(const pugi::xml_node& node) : x(node.attribute("x").as_float()), 
											y(node.attribute("y").as_float()), 
											height(node.attribute("h").as_int()), 
											width(node.attribute("w").as_int()) {}