#include "../cmd.h"
#include "../commandmap.h"
#include "../value.h"
#include "../virtualmachine.h"
#include "../arraydata.h"
#include "../configdata.h"
#include "../marker.h"
#include "../objectdata.h"
#include "../innerobj.h"
#include <string>

using namespace sqf;
namespace
{
	std::shared_ptr<value> allmapmarkers_(virtualmachine* vm)
	{
		auto arr = std::make_shared<arraydata>();
		for (auto& marker : vm->get_markers())
		{
			arr->push_back(std::make_shared<value>(marker.first));
		}
		return std::make_shared<value>(arr, ARRAY);
	}
	std::shared_ptr<value> getmarkertype_string(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto name = right->as_string();
		auto marker = vm->get_marker(name);
		if (!marker)
		{
			vm->wrn() << "Provided marker name does not exist." << std::endl;
			return std::make_shared<value>("");
		}
		return marker->get_type_sqf();
	}
	std::shared_ptr<value> getmarkersize_string(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto name = right->as_string();
		auto marker = vm->get_marker(name);
		if (!marker)
		{
			vm->wrn() << "Provided marker name does not exist." << std::endl;
			auto arr = std::make_shared<arraydata>(2);
			arr->push_back(std::make_shared<value>(0));
			arr->push_back(std::make_shared<value>(0));
			return std::make_shared<value>(arr, ARRAY);
		}
		return marker->get_size_sqf();
	}
	std::shared_ptr<value> getmarkercolor_string(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto name = right->as_string();
		auto marker = vm->get_marker(name);
		if (!marker)
		{
			vm->wrn() << "Provided marker name does not exist." << std::endl;
			return std::make_shared<value>("");
		}
		return marker->get_color_sqf();
	}
	std::shared_ptr<value> getmarkerpos_string(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto name = right->as_string();
		auto marker = vm->get_marker(name);
		if (!marker)
		{
			vm->wrn() << "Provided marker name does not exist." << std::endl;
			auto arr = std::make_shared<arraydata>(3);
			arr->push_back(std::make_shared<value>(0));
			arr->push_back(std::make_shared<value>(0));
			arr->push_back(std::make_shared<value>(0));
			return std::make_shared<value>(arr, ARRAY);
		}
		return marker->get_pos_sqf();
	}
	std::shared_ptr<value> markerbrush_string(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto name = right->as_string();
		auto marker = vm->get_marker(name);
		if (!marker)
		{
			vm->wrn() << "Provided marker name does not exist." << std::endl;
			return std::make_shared<value>("");
		}
		return marker->get_brush_sqf();
	}
	std::shared_ptr<value> markertext_string(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto name = right->as_string();
		auto marker = vm->get_marker(name);
		if (!marker)
		{
			vm->wrn() << "Provided marker name does not exist." << std::endl;
			return std::make_shared<value>("");
		}
		return marker->get_text_sqf();
	}
	std::shared_ptr<value> markerdir_string(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto name = right->as_string();
		auto marker = vm->get_marker(name);
		if (!marker)
		{
			vm->wrn() << "Provided marker name does not exist." << std::endl;
			return std::make_shared<value>(0);
		}
		return marker->get_direction_sqf();
	}
	std::shared_ptr<value> markershape_string(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto name = right->as_string();
		auto marker = vm->get_marker(name);
		if (!marker)
		{
			vm->wrn() << "Provided marker name does not exist." << std::endl;
			return std::make_shared<value>("");
		}
		return marker->get_shape_sqf();
	}
	std::shared_ptr<value> markeralpha_string(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto name = right->as_string();
		auto marker = vm->get_marker(name);
		if (!marker)
		{
			vm->wrn() << "Provided marker name does not exist." << std::endl;
			return std::make_shared<value>(0);
		}
		return marker->get_alpha_sqf();
	}
	std::shared_ptr<value> createmarker_array(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto arr = right->data<arraydata>();
		std::array<float, 3> pos;
		std::string name;
		if (arr->check_type(vm, std::array<type, 2> { STRING, OBJECT}))
		{
			name = arr->at(0)->as_string();
			auto objdata = arr->at(1)->data<sqf::objectdata>();
			if (objdata->is_null())
			{
				vm->wrn() << "Provided object is null." << std::endl;
				return std::shared_ptr<value>();
			}
			auto obj = objdata->obj();
			auto tmp = obj->pos();
			pos = std::array<float, 3> { (float)tmp[0], (float)tmp[1], (float)tmp[2]};
		}
		else if (arr->check_type(vm, std::array<type, 2> { STRING, ARRAY }))
		{
			name = arr->at(0)->as_string();
			auto tmpArr = arr->at(1)->data<arraydata>();
			pos = std::array<float, 3>
			{
				tmpArr->at(0)->as_float(),
				tmpArr->at(1)->as_float(),
				tmpArr->size() > 2 ? tmpArr->at(2)->as_float() : 0
			};
			if (!arr->check_type(vm, SCALAR, 2, 3))
			{
				return std::shared_ptr<value>();
			}
		}
		else
		{
			return std::shared_ptr<value>();
		}
		if (vm->get_marker(name))
		{
			vm->wrn() << "Provided marker name is already existing." << std::endl;
			return std::make_shared<value>("");
		}
		auto m = marker();
		m.set_pos(pos);
		vm->set_marker(name, m);
		return std::make_shared<value>(name);
	}
	std::shared_ptr<value> deletemarker_string(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto name = right->as_string();
		if (vm->get_marker(name))
		{
			vm->remove_marker(name);
		}
		else
		{
			vm->wrn() << "Attempt to delete a non-existing marker has been made." << std::endl;
		}
		return std::make_shared<value>();
	}
	std::shared_ptr<value> setmarkershape_string_string(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto mname = left->as_string();
		auto m = vm->get_marker(mname);
		if (!m)
		{
			vm->wrn() << "Provided marker name does not exist." << std::endl;
			return std::make_shared<value>();
		}
		auto shapename = right->as_string();
		if (str_cmpi(shapename.c_str(), -1, "ICON", -1) == 0)
		{
			m->set_shape(sqf::marker::shape::Icon);
		}
		else if (str_cmpi(shapename.c_str(), -1, "RECTANGLE", -1) == 0)
		{
			m->set_shape(sqf::marker::shape::Rectangle);
		}
		else if (str_cmpi(shapename.c_str(), -1, "ELLIPSE", -1) == 0)
		{
			m->set_shape(sqf::marker::shape::Ellipse);
		}
		else if (str_cmpi(shapename.c_str(), -1, "POLYLINE", -1) == 0)
		{
			m->set_shape(sqf::marker::shape::Polyline);
		}
		else
		{
			vm->err() << "Invalid marker shape was provided." << std::endl;
		}
		return std::make_shared<value>();
	}
	std::shared_ptr<value> setmarkerbrush_string_string(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto mname = left->as_string();
		auto m = vm->get_marker(mname);
		if (!m)
		{
			vm->wrn() << "Provided marker name does not exist." << std::endl;
			return std::make_shared<value>();
		}
		auto brushname = right->as_string();
		if (vm->perform_classname_checks())
		{
			auto configBin = sqf::configdata::configFile()->data<sqf::configdata>();
			auto cfgVehicles = configBin->navigate("CfgMarkerBrushes");
			auto brushConfig = cfgVehicles->data<sqf::configdata>()->navigate(brushname);
			if (brushConfig->data<configdata>()->is_null())
			{
				vm->wrn() << "The config entry '" << brushname << "' could not be located in `ConfigBin >> CfgMarkerBrushes`." << std::endl;
				return std::make_shared<value>();
			}
		}
		m->set_brush(brushname);
		return std::make_shared<value>();
	}
	std::shared_ptr<value> setmarkerpos_string_array(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto mname = left->as_string();
		auto m = vm->get_marker(mname);
		if (!m)
		{
			vm->wrn() << "Provided marker name does not exist." << std::endl;
			return std::make_shared<value>();
		}
		auto arr = right->data<arraydata>();
		if (!arr->check_type(vm, SCALAR, 2, 3))
		{
			return std::shared_ptr<value>();
		}
		m->set_pos(std::array<float, 3>{ arr->at(0)->as_float(), arr->at(1)->as_float(), arr->size() > 2 ? arr->at(2)->as_float() : 0 });
		return std::make_shared<value>();
	}
	std::shared_ptr<value> setmarkertype_string_string(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto mname = left->as_string();
		auto m = vm->get_marker(mname);
		if (!m)
		{
			vm->wrn() << "Provided marker name does not exist." << std::endl;
			return std::make_shared<value>();
		}
		auto tname = right->as_string();
		if (vm->perform_classname_checks())
		{
			auto configBin = sqf::configdata::configFile()->data<sqf::configdata>();
			auto cfgVehicles = configBin->navigate("CfgMarkers");
			auto typeConfig = cfgVehicles->data<sqf::configdata>()->navigate(tname);
			if (typeConfig->data<configdata>()->is_null())
			{
				vm->wrn() << "The config entry '" << tname << "' could not be located in `ConfigBin >> CfgMarkers`." << std::endl;
				return std::make_shared<value>();
			}
		}
		m->set_type(tname);
		return std::make_shared<value>();
	}
	std::shared_ptr<value> setmarkertext_string_string(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto mname = left->as_string();
		auto m = vm->get_marker(mname);
		if (!m)
		{
			vm->wrn() << "Provided marker name does not exist." << std::endl;
			return std::make_shared<value>();
		}
		auto text = right->as_string();
		m->set_text(text);
		return std::make_shared<value>();
	}
	std::shared_ptr<value> setmarkerdir_string_scalar(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto mname = left->as_string();
		auto m = vm->get_marker(mname);
		if (!m)
		{
			vm->wrn() << "Provided marker name does not exist." << std::endl;
			return std::make_shared<value>();
		}
		auto dir = right->as_float();
		m->set_direction(dir);
		return std::make_shared<value>();
	}
	std::shared_ptr<value> setmarkercolor_string_string(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto mname = left->as_string();
		auto m = vm->get_marker(mname);
		if (!m)
		{
			vm->wrn() << "Provided marker name does not exist." << std::endl;
			return std::make_shared<value>();
		}
		auto colorname = right->as_string();
		if (vm->perform_classname_checks())
		{
			auto configBin = sqf::configdata::configFile()->data<sqf::configdata>();
			auto cfgVehicles = configBin->navigate("CfgMarkerColors");
			auto colorConfig = cfgVehicles->data<sqf::configdata>()->navigate(colorname);
			if (colorConfig->data<configdata>()->is_null())
			{
				vm->wrn() << "The config entry '" << colorname << "' could not be located in `ConfigBin >> CfgMarkerColors`." << std::endl;
				return std::make_shared<value>();
			}
		}
		m->set_color(colorname);
		return std::make_shared<value>();
	}
	std::shared_ptr<value> setmarkersize_string_array(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto mname = left->as_string();
		auto m = vm->get_marker(mname);
		if (!m)
		{
			vm->wrn() << "Provided marker name does not exist." << std::endl;
			return std::make_shared<value>();
		}
		auto arr = right->data<arraydata>();
		if (!arr->check_type(vm, SCALAR, 2))
		{
			return std::shared_ptr<value>();
		}
		m->set_size(std::array<float, 2>{ arr->at(0)->as_float(), arr->at(1)->as_float() });
		return std::make_shared<value>();
	}
	std::shared_ptr<value> setmarkeralpha_string_scalar(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto mname = left->as_string();
		auto m = vm->get_marker(mname);
		if (!m)
		{
			vm->wrn() << "Provided marker name does not exist." << std::endl;
			return std::make_shared<value>();
		}
		auto alpha = right->as_float();
		m->set_alpha(alpha);
		return std::make_shared<value>();
	}
}

void sqf::commandmap::initmarkercmds()
{
	add(nular("allMapMarkers", "Return all markers.", allmapmarkers_));
	add(unary("getMarkerType", type::STRING, "Returns the type of a given marker. See CfgMarkers.", getmarkertype_string));
	add(unary("markerType", type::STRING, "Returns the type of a given marker. See CfgMarkers.", getmarkertype_string));
	add(unary("getMarkerSize", type::STRING, "Returns the size of the given marker.", getmarkersize_string));
	add(unary("markerSize", type::STRING, "Returns the size of the given marker.", getmarkersize_string));
	add(unary("getMarkerColor", type::STRING, "Returns marker color for given marker. See CfgMarkerColors.", getmarkercolor_string));
	add(unary("markerColor", type::STRING, "Returns marker color for given marker. See CfgMarkerColors.", getmarkercolor_string));
	add(unary("getMarkerPos", type::STRING, "Returns the position of a given marker in format [x, y, 0].", getmarkerpos_string));
	add(unary("markerPos", type::STRING, "Returns the position of a given marker in format [x, y, 0].", getmarkerpos_string));
	add(unary("markerBrush", type::STRING, "Returns the fill texture for the marker. See CfgMarkerBrushes.", markerbrush_string));
	add(unary("markerText", type::STRING, "Get marker text.", markertext_string));
	add(unary("markerDir", type::STRING, "Get marker direction.", markerdir_string));
	add(unary("markerShape", type::STRING, "Gets the marker shape. Shape can be 'ICON', 'RECTANGLE', 'ELLIPSE' or 'POLYLINE'.", markershape_string));
	add(unary("markerAlpha", type::STRING, "Gets the marker alpha.", markeralpha_string));
	add(unary("createMarker", type::ARRAY, "Creates a new marker at the given position. The marker name has to be unique.", createmarker_array));
	add(unary("createMarkerLocal", type::ARRAY, "Creates a new local marker at the given position. The marker name has to be unique.", createmarker_array));
	add(unary("deleteMarker", type::STRING, "Destroys the given marker.", deletemarker_string));
	add(unary("deletemarkerlocal", type::STRING, "Destroys the given marker.", deletemarker_string));
	add(binary(4, "setMarkerShape", type::STRING, type::STRING, "Selects the shape (type) of the marker. Shape can be 'ICON', 'RECTANGLE', 'ELLIPSE' or 'POLYLINE'.", setmarkershape_string_string));
	add(binary(4, "setMarkerShapeLocal", type::STRING, type::STRING, "Selects the shape (type) of the local marker. Shape can be 'ICON', 'RECTANGLE', 'ELLIPSE' or 'POLYLINE'.", setmarkershape_string_string));
	add(binary(4, "setMarkerBrush", type::STRING, type::STRING, "Selects the fill texture for the marker. Brush is the name of the subclass in CfgMarkerBrushes.", setmarkerbrush_string_string));
	add(binary(4, "setMarkerBrushLocal", type::STRING, type::STRING, "Selects the fill texture for the marker. Brush is the name of the subclass in CfgMarkerBrushes.", setmarkerbrush_string_string));
	add(binary(4, "setMarkerPos", type::STRING, type::ARRAY, "Moves the marker.", setmarkerpos_string_array));
	add(binary(4, "setMarkerPosLocal", type::STRING, type::ARRAY, "Moves the local marker.", setmarkerpos_string_array));
	add(binary(4, "setMarkerType", type::STRING, type::STRING, "Set marker type. Type is the name of the subclass in CfgMarkers.", setmarkertype_string_string));
	add(binary(4, "setMarkerTypeLocal", type::STRING, type::STRING, "Set marker type. Type is the name of the subclass in CfgMarkers.", setmarkertype_string_string));
	add(binary(4, "setMarkerText", type::STRING, type::STRING, "Sets the text label of an existing marker.", setmarkertext_string_string));
	add(binary(4, "setMarkerTextLocal", type::STRING, type::STRING, "Sets the text label of an existing local marker.", setmarkertext_string_string));
	add(binary(4, "setMarkerDir", type::STRING, type::SCALAR, "Sets the orientation of the marker. Angle is in degrees.", setmarkerdir_string_scalar));
	add(binary(4, "setMarkerDirLocal", type::STRING, type::SCALAR, "Sets the orientation of the local marker. Angle is in degrees.", setmarkerdir_string_scalar));
	add(binary(4, "setMarkerColor", type::STRING, type::STRING, "Sets the markers color. Color is the name of the subclass in CfgMarkers.", setmarkercolor_string_string));
	add(binary(4, "setMarkerColorLocal", type::STRING, type::STRING, "Sets the local markers color. Color is the name of the subclass in CfgMarkers.", setmarkercolor_string_string));
	add(binary(4, "setMarkerSize", type::STRING, type::ARRAY, "Sets markers size. Size is in format [a-axis, b-axis].", setmarkersize_string_array));
	add(binary(4, "setMarkerSizeLocal", type::STRING, type::ARRAY, "Sets local markers size. Size is in format [a-axis, b-axis].", setmarkersize_string_array));
	add(binary(4, "setMarkerAlpha", type::STRING, type::SCALAR, "Sets the markers alpha.", setmarkeralpha_string_scalar));
	add(binary(4, "setMarkerAlphaLocal", type::STRING, type::SCALAR, "Sets the local markers alpha.", setmarkeralpha_string_scalar));

}