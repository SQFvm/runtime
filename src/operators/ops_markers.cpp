#include "ops_markers.h"
#include "../runtime/value.h"
#include "../runtime/logging.h"
#include "../runtime/runtime.h"
#include "../runtime/sqfop.h"
#include "../runtime/util.h"

#include "../runtime/d_string.h"
#include "../runtime/d_scalar.h"
#include "../runtime/d_boolean.h"
#include "../runtime/d_array.h"

#include "d_object.h"

namespace err = logmessage::runtime;
using namespace sqf::runtime;
using namespace sqf::types;

namespace
{
	value allmapmarkers_(runtime& runtime)
	{
		std::vector<value> marker_names;

		for (auto& marker : runtime.storage<sqf::operators::markers_storage>())
		{
			marker_names.push_back(marker.first);
		}
		return marker_names;
	}
	value getmarkertype_string(runtime& runtime, value::cref right)
	{
		auto name = right.data<d_string, std::string>();
		if (!runtime.storage<sqf::operators::markers_storage>().exists(name))
		{
			runtime.__logmsg(err::MarkerNotExisting((*runtime.active_context().current_frame().current())->diag_info(), name));
			runtime.__logmsg(err::ReturningEmptyString((*runtime.active_context().current_frame().current())->diag_info()));
			return "";
		}
		auto marker = runtime.storage<sqf::operators::markers_storage>().at(name);
		return marker.get_type_sqf();
	}
	value getmarkersize_string(runtime& runtime, value::cref right)
	{
		auto name = right.data<d_string, std::string>();
		if (!runtime.storage<sqf::operators::markers_storage>().exists(name))
		{
			runtime.__logmsg(err::MarkerNotExisting((*runtime.active_context().current_frame().current())->diag_info(), name));
			runtime.__logmsg(err::ReturningDefaultArray((*runtime.active_context().current_frame().current())->diag_info(), 2));
			return std::array<value, 2> { 0, 0 };
		}
		auto marker = runtime.storage<sqf::operators::markers_storage>().at(name);
		return marker.get_size_sqf();
	}
	value getmarkercolor_string(runtime& runtime, value::cref right)
	{
		auto name = right.data<d_string, std::string>();
		if (!runtime.storage<sqf::operators::markers_storage>().exists(name))
		{
			runtime.__logmsg(err::MarkerNotExisting((*runtime.active_context().current_frame().current())->diag_info(), name));
			runtime.__logmsg(err::ReturningEmptyString((*runtime.active_context().current_frame().current())->diag_info()));
			return "";
		}
		auto marker = runtime.storage<sqf::operators::markers_storage>().at(name);
		return marker.get_color_sqf();
	}
	value getmarkerpos_string(runtime& runtime, value::cref right)
	{
		auto name = right.data<d_string, std::string>();
		if (!runtime.storage<sqf::operators::markers_storage>().exists(name))
		{
			runtime.__logmsg(err::MarkerNotExisting((*runtime.active_context().current_frame().current())->diag_info(), name));
			runtime.__logmsg(err::ReturningDefaultArray((*runtime.active_context().current_frame().current())->diag_info(), 2));
			return std::array<value, 2> { 0, 0 };
		}
		auto marker = runtime.storage<sqf::operators::markers_storage>().at(name);
		return marker.get_pos_sqf();
	}
	value markerbrush_string(runtime& runtime, value::cref right)
	{
		auto name = right.data<d_string, std::string>();
		if (!runtime.storage<sqf::operators::markers_storage>().exists(name))
		{
			runtime.__logmsg(err::MarkerNotExisting((*runtime.active_context().current_frame().current())->diag_info(), name));
			runtime.__logmsg(err::ReturningEmptyString((*runtime.active_context().current_frame().current())->diag_info()));
			return "";
		}
		auto marker = runtime.storage<sqf::operators::markers_storage>().at(name);
		return marker.get_brush_sqf();
	}
	value markertext_string(runtime& runtime, value::cref right)
	{
		auto name = right.data<d_string, std::string>();
		if (!runtime.storage<sqf::operators::markers_storage>().exists(name))
		{
			runtime.__logmsg(err::MarkerNotExisting((*runtime.active_context().current_frame().current())->diag_info(), name));
			runtime.__logmsg(err::ReturningEmptyString((*runtime.active_context().current_frame().current())->diag_info()));
			return "";
		}
		auto marker = runtime.storage<sqf::operators::markers_storage>().at(name);
		return marker.get_text_sqf();
	}
	value markerdir_string(runtime& runtime, value::cref right)
	{
		auto name = right.data<d_string, std::string>();
		if (!runtime.storage<sqf::operators::markers_storage>().exists(name))
		{
			runtime.__logmsg(err::MarkerNotExisting((*runtime.active_context().current_frame().current())->diag_info(), name));
			runtime.__logmsg(err::ReturningScalarZero((*runtime.active_context().current_frame().current())->diag_info()));
			return 0;
		}
		auto marker = runtime.storage<sqf::operators::markers_storage>().at(name);
		return marker.get_direction_sqf();
	}
	value markershape_string(runtime& runtime, value::cref right)
	{
		auto name = right.data<d_string, std::string>();
		if (!runtime.storage<sqf::operators::markers_storage>().exists(name))
		{
			runtime.__logmsg(err::MarkerNotExisting((*runtime.active_context().current_frame().current())->diag_info(), name));
			runtime.__logmsg(err::ReturningEmptyString((*runtime.active_context().current_frame().current())->diag_info()));
			return "";
		}
		auto marker = runtime.storage<sqf::operators::markers_storage>().at(name);
		return marker.get_shape_sqf();
	}
	value markeralpha_string(runtime& runtime, value::cref right)
	{
		auto name = right.data<d_string, std::string>();
		if (!runtime.storage<sqf::operators::markers_storage>().exists(name))
		{
			runtime.__logmsg(err::MarkerNotExisting((*runtime.active_context().current_frame().current())->diag_info(), name));
			runtime.__logmsg(err::ReturningEmptyString((*runtime.active_context().current_frame().current())->diag_info()));
			return "";
		}
		auto marker = runtime.storage<sqf::operators::markers_storage>().at(name);
		return marker.get_alpha_sqf();
	}
	value createmarker_array(runtime& runtime, value::cref right)
	{
		auto arr = right.data<d_array>();
		std::array<float, 3> pos {0, 0, 0};
		std::string name;
		if (arr->check_type(runtime, std::array<type, 2> { t_string(), t_object()}))
		{
			name = arr->at(0).data<d_string, std::string>();
			auto objdata = arr->at(1).data<d_object>();
			if (objdata->is_null())
			{
				runtime.__logmsg(err::ExpectedNonNullValueWeak((*runtime.active_context().current_frame().current())->diag_info()));
				runtime.__logmsg(err::ReturningNil((*runtime.active_context().current_frame().current())->diag_info()));
				return {};
			}
			auto obj = objdata->value();
			auto tmp = obj->position();
			pos = std::array<float, 3> { static_cast<float>(tmp.x), static_cast<float>(tmp.y), static_cast<float>(tmp.z)};
		}
		else if (arr->check_type(runtime, std::array<type, 2> { t_string(), t_array() }))
		{
			name = arr->at(0).data<d_string, std::string>();
			auto tmpArr = arr->at(1).data<d_array>();
			pos = std::array<float, 3>
			{
				tmpArr->at(0).data<d_scalar, float>(),
				tmpArr->at(1).data<d_scalar, float>(),
				tmpArr->size() > 2 ? tmpArr->at(2).data<d_scalar, float>() : 0
			};
			if (!arr->check_type(runtime, t_scalar(), 2, 3))
			{
				return {};
			}
		}
		else
		{
			return {};
		}
		if (vm->get_marker(name))
		{
			runtime.__logmsg(err::MarkerAlreadyExisting((*runtime.active_context().current_frame().current())->diag_info(), name));
			runtime.__logmsg(err::ReturningEmptyString((*runtime.active_context().current_frame().current())->diag_info()));
			return "";
		}
		auto m = marker();
		m.set_pos(pos);
		vm->set_marker(name, m);
		return name;
	}
	value deletemarker_string(runtime& runtime, value::cref right)
	{
		auto name = right.data<d_string, std::string>();
		if (vm->get_marker(name))
		{
			vm->remove_marker(name);
		}
		else
		{
			runtime.__logmsg(err::MarkerNotExisting((*runtime.active_context().current_frame().current())->diag_info(), name));
		}
		return {};
	}
	value setmarkershape_string_string(runtime& runtime, value::cref left, value::cref right)
	{
		auto mname = left.data<d_string, std::string>();
		auto m = vm->get_marker(mname);
		if (!m)
		{
			runtime.__logmsg(err::MarkerNotExisting((*runtime.active_context().current_frame().current())->diag_info(), mname));
			return {};
		}
		auto shapename = right.data<d_string, std::string>();
		if (str_cmpi(shapename.c_str(), -1, "ICON", -1) == 0)
		{
			m->set_shape(sqf::operators::markers_storage::marker::shape::Icon);
		}
		else if (str_cmpi(shapename.c_str(), -1, "RECTANGLE", -1) == 0)
		{
			m->set_shape(sqf::operators::markers_storage::marker::shape::Rectangle);
		}
		else if (str_cmpi(shapename.c_str(), -1, "ELLIPSE", -1) == 0)
		{
			m->set_shape(sqf::operators::markers_storage::marker::shape::Ellipse);
		}
		else if (str_cmpi(shapename.c_str(), -1, "POLYLINE", -1) == 0)
		{
			m->set_shape(sqf::operators::markers_storage::marker::shape::Polyline);
		}
		else
		{
			runtime.__logmsg(err::InvalidMarkershape((*runtime.active_context().current_frame().current())->diag_info(), shapename));
		}
		return {};
	}
	value setmarkerbrush_string_string(runtime& runtime, value::cref left, value::cref right)
	{
		auto mname = left.data<d_string, std::string>();
		auto m = vm->get_marker(mname);
		if (!m)
		{
			runtime.__logmsg(err::MarkerNotExisting((*runtime.active_context().current_frame().current())->diag_info(), mname));
			return {};
		}
		auto brushname = right.data<d_string, std::string>();
		if (vm->perform_classname_checks())
		{
			auto configBin = sqf::configdata::configFile().data<sqf::configdata>();
			auto cfgVehicles = configBin->navigate("CfgMarkerBrushes");
			auto brushConfig = cfgVehicles.data<sqf::configdata>()->navigate(brushname);
			if (brushConfig.data<configdata>()->is_null())
			{
				runtime.__logmsg(err::ConfigEntryNotFoundWeak((*runtime.active_context().current_frame().current())->diag_info(), std::array<std::string, 2> { "ConfigBin", "CfgMarkers" }, brushname));
				return {};
			}
		}
		m->set_brush(brushname);
		return {};
	}
	value setmarkerpos_string_array(runtime& runtime, value::cref left, value::cref right)
	{
		auto mname = left.data<d_string, std::string>();
		auto m = vm->get_marker(mname);
		if (!m)
		{
			runtime.__logmsg(err::MarkerNotExisting((*runtime.active_context().current_frame().current())->diag_info(), mname));
			return {};
		}
		auto arr = right.data<d_array>();
		if (!arr->check_type(vm, SCALAR, 2, 3))
		{
			return {};
		}
		m->set_pos(std::array<float, 3>{ arr->at(0).data<d_scalar, float>(), arr->at(1).data<d_scalar, float>(), arr->size() > 2 ? arr->at(2).data<d_scalar, float>() : 0 });
		return {};
	}
	value setmarkertype_string_string(runtime& runtime, value::cref left, value::cref right)
	{
		auto mname = left.data<d_string, std::string>();
		auto m = vm->get_marker(mname);
		if (!m)
		{
			runtime.__logmsg(err::MarkerNotExisting((*runtime.active_context().current_frame().current())->diag_info(), mname));
			return {};
		}
		auto tname = right.data<d_string, std::string>();
		if (vm->perform_classname_checks())
		{
			auto configBin = sqf::configdata::configFile().data<sqf::configdata>();
			auto cfgVehicles = configBin->navigate("CfgMarkers");
			auto typeConfig = cfgVehicles.data<sqf::configdata>()->navigate(tname);
			if (typeConfig.data<configdata>()->is_null())
			{
				runtime.__logmsg(err::ConfigEntryNotFoundWeak((*runtime.active_context().current_frame().current())->diag_info(), std::array<std::string, 2> { "ConfigBin", "CfgMarkers" }, tname));
				runtime.__logmsg(err::ReturningNil((*runtime.active_context().current_frame().current())->diag_info()));
				return {};
			}
		}
		m->set_type(tname);
		return {};
	}
	value setmarkertext_string_string(runtime& runtime, value::cref left, value::cref right)
	{
		auto mname = left.data<d_string, std::string>();
		auto m = vm->get_marker(mname);
		if (!m)
		{
			runtime.__logmsg(err::MarkerNotExisting((*runtime.active_context().current_frame().current())->diag_info(), mname));
			return {};
		}
		auto text = right.data<d_string, std::string>();
		m->set_text(text);
		return {};
	}
	value setmarkerdir_string_scalar(runtime& runtime, value::cref left, value::cref right)
	{
		auto mname = left.data<d_string, std::string>();
		auto m = vm->get_marker(mname);
		if (!m)
		{
			runtime.__logmsg(err::MarkerNotExisting((*runtime.active_context().current_frame().current())->diag_info(), mname));
			return {};
		}
		auto dir = right.data<d_scalar, float>();
		m->set_direction(dir);
		return {};
	}
	value setmarkercolor_string_string(runtime& runtime, value::cref left, value::cref right)
	{
		auto mname = left.data<d_string, std::string>();
		auto m = vm->get_marker(mname);
		if (!m)
		{
			runtime.__logmsg(err::MarkerNotExisting((*runtime.active_context().current_frame().current())->diag_info(), mname));
			return {};
		}
		auto colorname = right.data<d_string, std::string>();
		if (vm->perform_classname_checks())
		{
			auto configBin = sqf::configdata::configFile().data<sqf::configdata>();
			auto cfgVehicles = configBin->navigate("CfgMarkerColors");
			auto colorConfig = cfgVehicles.data<sqf::configdata>()->navigate(colorname);
			if (colorConfig.data<configdata>()->is_null())
			{
				runtime.__logmsg(err::ConfigEntryNotFoundWeak((*runtime.active_context().current_frame().current())->diag_info(), std::array<std::string, 2> { "ConfigBin", "CfgMarkers" }, colorname));
				runtime.__logmsg(err::ReturningNil((*runtime.active_context().current_frame().current())->diag_info()));
				return {};
			}
		}
		m->set_color(colorname);
		return {};
	}
	value setmarkersize_string_array(runtime& runtime, value::cref left, value::cref right)
	{
		auto mname = left.data<d_string, std::string>();
		auto m = vm->get_marker(mname);
		if (!m)
		{
			runtime.__logmsg(err::MarkerNotExisting((*runtime.active_context().current_frame().current())->diag_info(), mname));
			return {};
		}
		auto arr = right.data<d_array>();
		if (!arr->check_type(vm, SCALAR, 2))
		{
			return {};
		}
		m->set_size(std::array<float, 2>{ arr->at(0).data<d_scalar, float>(), arr->at(1).data<d_scalar, float>() });
		return {};
	}
	value setmarkeralpha_string_scalar(runtime& runtime, value::cref left, value::cref right)
	{
		auto mname = left.data<d_string, std::string>();
		auto m = vm->get_marker(mname);
		if (!m)
		{
			runtime.__logmsg(err::MarkerNotExisting((*runtime.active_context().current_frame().current())->diag_info(), mname));
			return {};
		}
		auto alpha = right.data<d_scalar, float>();
		m->set_alpha(alpha);
		return {};
	}
}

void sqf::operators::ops_markers(::sqf::runtime::runtime& runtime)
{
	using namespace sqf::runtime::sqfop;
	runtime.register_sqfop(nular("allMapMarkers", "Return all markers.", allmapmarkers_));
	runtime.register_sqfop(unary("getMarkerType", t_string(), "Returns the type of a given marker. See CfgMarkers.", getmarkertype_string));
	runtime.register_sqfop(unary("markerType", t_string(), "Returns the type of a given marker. See CfgMarkers.", getmarkertype_string));
	runtime.register_sqfop(unary("getMarkerSize", t_string(), "Returns the size of the given marker.", getmarkersize_string));
	runtime.register_sqfop(unary("markerSize", t_string(), "Returns the size of the given marker.", getmarkersize_string));
	runtime.register_sqfop(unary("getMarkerColor", t_string(), "Returns marker color for given marker. See CfgMarkerColors.", getmarkercolor_string));
	runtime.register_sqfop(unary("markerColor", t_string(), "Returns marker color for given marker. See CfgMarkerColors.", getmarkercolor_string));
	runtime.register_sqfop(unary("getMarkerPos", t_string(), "Returns the position of a given marker in format [x, y, 0].", getmarkerpos_string));
	runtime.register_sqfop(unary("markerPos", t_string(), "Returns the position of a given marker in format [x, y, 0].", getmarkerpos_string));
	runtime.register_sqfop(unary("markerBrush", t_string(), "Returns the fill texture for the marker. See CfgMarkerBrushes.", markerbrush_string));
	runtime.register_sqfop(unary("markerText", t_string(), "Get marker text.", markertext_string));
	runtime.register_sqfop(unary("markerDir", t_string(), "Get marker direction.", markerdir_string));
	runtime.register_sqfop(unary("markerShape", t_string(), "Gets the marker shape. Shape can be 'ICON', 'RECTANGLE', 'ELLIPSE' or 'POLYLINE'.", markershape_string));
	runtime.register_sqfop(unary("markerAlpha", t_string(), "Gets the marker alpha.", markeralpha_string));
	runtime.register_sqfop(unary("createMarker", t_array(), "Creates a new marker at the given position. The marker name has to be unique.", createmarker_array));
	runtime.register_sqfop(unary("createMarkerLocal", t_array(), "Creates a new local marker at the given position. The marker name has to be unique.", createmarker_array));
	runtime.register_sqfop(unary("deleteMarker", t_string(), "Destroys the given marker.", deletemarker_string));
	runtime.register_sqfop(unary("deletemarkerlocal", t_string(), "Destroys the given marker.", deletemarker_string));
	runtime.register_sqfop(binary(4, "setMarkerShape", t_string(), t_string(), "Selects the shape (type) of the marker. Shape can be 'ICON', 'RECTANGLE', 'ELLIPSE' or 'POLYLINE'.", setmarkershape_string_string));
	runtime.register_sqfop(binary(4, "setMarkerShapeLocal", t_string(), t_string(), "Selects the shape (type) of the local marker. Shape can be 'ICON', 'RECTANGLE', 'ELLIPSE' or 'POLYLINE'.", setmarkershape_string_string));
	runtime.register_sqfop(binary(4, "setMarkerBrush", t_string(), t_string(), "Selects the fill texture for the marker. Brush is the name of the subclass in CfgMarkerBrushes.", setmarkerbrush_string_string));
	runtime.register_sqfop(binary(4, "setMarkerBrushLocal", t_string(), t_string(), "Selects the fill texture for the marker. Brush is the name of the subclass in CfgMarkerBrushes.", setmarkerbrush_string_string));
	runtime.register_sqfop(binary(4, "setMarkerPos", t_string(), t_array(), "Moves the marker.", setmarkerpos_string_array));
	runtime.register_sqfop(binary(4, "setMarkerPosLocal", t_string(), t_array(), "Moves the local marker.", setmarkerpos_string_array));
	runtime.register_sqfop(binary(4, "setMarkerType", t_string(), t_string(), "Set marker type. Type is the name of the subclass in CfgMarkers.", setmarkertype_string_string));
	runtime.register_sqfop(binary(4, "setMarkerTypeLocal", t_string(), t_string(), "Set marker type. Type is the name of the subclass in CfgMarkers.", setmarkertype_string_string));
	runtime.register_sqfop(binary(4, "setMarkerText", t_string(), t_string(), "Sets the text label of an existing marker.", setmarkertext_string_string));
	runtime.register_sqfop(binary(4, "setMarkerTextLocal", t_string(), t_string(), "Sets the text label of an existing local marker.", setmarkertext_string_string));
	runtime.register_sqfop(binary(4, "setMarkerDir", t_string(), t_scalar(), "Sets the orientation of the marker. Angle is in degrees.", setmarkerdir_string_scalar));
	runtime.register_sqfop(binary(4, "setMarkerDirLocal", t_string(), t_scalar(), "Sets the orientation of the local marker. Angle is in degrees.", setmarkerdir_string_scalar));
	runtime.register_sqfop(binary(4, "setMarkerColor", t_string(), t_string(), "Sets the markers color. Color is the name of the subclass in CfgMarkers.", setmarkercolor_string_string));
	runtime.register_sqfop(binary(4, "setMarkerColorLocal", t_string(), t_string(), "Sets the local markers color. Color is the name of the subclass in CfgMarkers.", setmarkercolor_string_string));
	runtime.register_sqfop(binary(4, "setMarkerSize", t_string(), t_array(), "Sets markers size. Size is in format [a-axis, b-axis].", setmarkersize_string_array));
	runtime.register_sqfop(binary(4, "setMarkerSizeLocal", t_string(), t_array(), "Sets local markers size. Size is in format [a-axis, b-axis].", setmarkersize_string_array));
	runtime.register_sqfop(binary(4, "setMarkerAlpha", t_string(), t_scalar(), "Sets the markers alpha.", setmarkeralpha_string_scalar));
	runtime.register_sqfop(binary(4, "setMarkerAlphaLocal", t_string(), t_scalar(), "Sets the local markers alpha.", setmarkeralpha_string_scalar));

}