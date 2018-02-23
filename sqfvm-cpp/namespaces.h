#pragma once
#include <memory>

namespace sqf
{
	class sqfnamespace;
	namespace commands
	{
		class namespaces
		{
		private:
			static std::shared_ptr<sqf::sqfnamespace> MissionNamespace;
			static std::shared_ptr<sqf::sqfnamespace> UiNamespace;
			static std::shared_ptr<sqf::sqfnamespace> ParsingNamespace;
			static std::shared_ptr<sqf::sqfnamespace> ProfileNamespace;
			namespaces() {}
		public:
			static std::shared_ptr<sqf::sqfnamespace> missionNamespace (void) { return MissionNamespace; }
			static std::shared_ptr<sqf::sqfnamespace> uiNamespace (void) { return UiNamespace; }
			static std::shared_ptr<sqf::sqfnamespace> parsingNamespace (void) { return ParsingNamespace; }
			static std::shared_ptr<sqf::sqfnamespace> profileNamespace (void) { return ProfileNamespace; }
		};
	}
}