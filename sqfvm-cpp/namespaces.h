#ifndef _NAMESPACES
#define _NAMESPACES 1

namespace sqf
{
	namespace commands
	{
		class namespaces
		{
		private:
			static namespace_s MissionNamespace;
			static namespace_s UiNamespace;
			static namespace_s ParsingNamespace;
			static namespace_s ProfileNamespace;
			namespaces() {}
		public:
			static namespace_s missionNamespace (void) { return MissionNamespace; }
			static namespace_s uiNamespace (void) { return UiNamespace; }
			static namespace_s parsingNamespace (void) { return ParsingNamespace; }
			static namespace_s profileNamespace (void) { return ProfileNamespace; }
		};
	}
}

#endif // !_NAMESPACES
