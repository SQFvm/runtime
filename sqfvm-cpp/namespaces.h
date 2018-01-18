#ifndef _NAMESPACES
#define _NAMESPACES 1

namespace sqf
{
	namespace commands
	{
		class namespaces
		{
		private:
			static sqfnamespace MissionNamespace;
			static sqfnamespace UiNamespace;
			static sqfnamespace ParsingNamespace;
			static sqfnamespace ProfileNamespace;
			namespaces() {}
		public:
			static sqfnamespace& missionNamespace (void) { return MissionNamespace; }
			static sqfnamespace& uiNamespace (void) { return MissionNamespace; }
			static sqfnamespace& parsingNamespace (void) { return MissionNamespace; }
			static sqfnamespace& profileNamespace (void) { return MissionNamespace; }
		};
	}
}

#endif // !_NAMESPACES
