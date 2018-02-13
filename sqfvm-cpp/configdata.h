#ifndef _CONFIGDATA
#define _CONFIGDATA 1

#if !defined(_STRING) & !defined(_STRING_)
#error configdata requires <string> header
#endif // !_STRING
#if !defined(_MEMORY) & !defined(_MEMORY_)
#error configdata requires <memory> header
#endif // !_MEMORY_
#if !defined(_VECTOR) & !defined(_VECTOR_)
#error configdata requires <vector> header
#endif // !_VECTOR
#if !defined(_DATA)
#error configdata requires "data.h" header
#endif // !_DATA
#if !defined(_ARRAYDATA)
#error configdata requires "arraydata.h" header
#endif // !_ARRAYDATA
#if !defined(_WSTRING_OP_H_)
#error configdata requires "wstring_op.h" header
#endif // !_WSTRING_OP_H_

namespace sqf
{
	class configdata : public arraydata
	{
	private:
		std::wstring mname;
		std::wstring mparentname;
		std::weak_ptr<configdata> mlogicparent;
		value_s mvalue;
		bool misnull;

		value_s parent_unsafe(void)
		{
			std::weak_ptr<configdata> lparent = mlogicparent;
			while (!lparent.expired())
			{
				auto lockparent = lparent.lock();
				lparent = lockparent->mlogicparent;
				auto res = navigate_unsafe(mname);
				if (res.get())
				{
					return res;
				}
			}
			return value_s();
		}
		value_s navigate_unsafe(std::wstring nextnode)
		{
			for each (auto it in innervector())
			{
				if (it->dtype() != type::CONFIG)
					continue;
				auto cd = it->data<sqf::configdata>();
				if (wstr_cmpi(cd->mname.c_str(), -1, nextnode.c_str(), -1) == 0)
					return it;
			}
			return value_s();
		}
		value_s navigate_full_unsafe(std::wstring nextnode)
		{
			auto it = navigate_unsafe(nextnode);
			if (it.get())
			{
				return it;
			}
			else
			{
				value_s p;
				while ((p = parent_unsafe()).get())
				{
					auto it = navigate_full_unsafe(nextnode);
					if (it.get())
						return it;
				}
				return value_s();
			}
		}
	public:
		configdata(std::wstring name) : mname(name), misnull(true) {}
		configdata() : mname(L"bin\\config.bin"), misnull(false) {}
		configdata(std::weak_ptr<configdata> logicparent, std::wstring name) : mname(name), mlogicparent(logicparent), misnull(false) {}
		configdata(std::weak_ptr<configdata> logicparent, std::wstring name, std::wstring parentname) : mname(name), mparentname(parentname), mlogicparent(logicparent), misnull(false) {}

		inline value_s value(void) const { return mvalue; }
		inline void value(value_s val) { mvalue = val; }
		inline std::wstring name(void) const { return mname; }

		virtual std::wstring tosqf(void) const;
		virtual bool equals(std::shared_ptr<data> d) const { return d.get() == this; }
		size_t logicalparentcount(void) const { if (auto sptr = mlogicparent.lock()) { return sptr->logicalparentcount() + 1; } return 0; }
		value_s parent(void) { auto val = parent_unsafe(); return val.get() ? val : configNull(); }
		inline value_s navigate(std::wstring nextnode) { auto val = navigate_full_unsafe(nextnode); return val.get() ? val : configNull(); }

		inline bool haslogicparent(void) { return !mlogicparent.expired(); }
		inline value_s logicparent(void) { return mlogicparent.expired() ? configNull() : std::make_shared<sqf::value>(mlogicparent.lock(), type::CONFIG); }

		void mergeinto(std::shared_ptr<configdata>);

		static value_s configFile(void);
		static value_s configNull(void);
	};
}
#endif // !_CONFIGDATA