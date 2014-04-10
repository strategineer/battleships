#include "UnitInformation.h"
#include "Boat.h"


UnitInformation::UnitInformation()
{
  name.set_prefix("Name: ");
  speed.set_prefix("Speed: ");
  is_armored.set_prefix("Armoured? ");
}


UnitInformation::~UnitInformation()
{}

void UnitInformation::popup_at(int x, int y)
{
  name.set_position(x, y);
  speed.set_position(x, y + 30);
  is_armored.set_position(x, y + 60);
}
void UnitInformation::set_selected_unit(const Unit * ut)
{
  selected_unit = ut;
  if ( selected_unit == nullptr ) { return; }
  Boat* bt;
  switch ( selected_unit->get_unit_type() )
  {
  case Unit::UnitClass::BASE:
    name.set_value("Base");
    speed.set_value("0");
    break;
  case Unit::UnitClass::BOAT:
    bt = (Boat*)selected_unit;
    std::string boat_name;
    switch ( bt->get_boat_type() )
    {
    case Boat::BoatType::CRUISER:
      boat_name = "Cruiser";
      break;
    case Boat::BoatType::DESTROYER:
      boat_name = "Destroyer";
      break;
    case Boat::BoatType::KAMIKAZE_BOAT:
      boat_name = "Kamikaze";
      break;
    case Boat::BoatType::MINE_LAYER:
      boat_name = "Mine Layer";
      break;
    case Boat::BoatType::RADAR_BOAT:
      boat_name = "Radar";
      break;
    case Boat::BoatType::REPAIR_BOAT:
      boat_name = "Repair";
      break;
    case Boat::BoatType::TORPEDO_BOAT:
      boat_name = "Torpedo";
      break;
    default:
      boat_name = "Boat";
      break;
    }
    name.set_value(boat_name);
    speed.set_value(std::to_string(bt->get_speed()));
    break;
  }
  is_armored.set_value(selected_unit->get_maximum_health() < 2 ? "No" : "Yes");
}
void UnitInformation::reset_selected_unit()
{
  selected_unit = nullptr;
}

void UnitInformation::do_render()
{
  name.render();
  speed.render();
  is_armored.render();
}
