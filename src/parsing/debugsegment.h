#pragma once
#include <string>
#include <string_view>
#include <algorithm>
namespace sqf
{
	namespace parse
	{
		std::string dbgsegment(std::string_view view, size_t off, size_t length)
		{
			size_t i = off < 15 ? 0 : off - 15;
			size_t len = 30 + length;
			if (i < 0)
			{
				len += i;
				i = 0;
			}
			for (size_t j = i; j < i + len; j++)
			{
				char wc = view[j];
				if (wc == '\0' || wc == '\n')
				{
					if (j < off)
					{
						i = j + 1;
					}
					else
					{
						len = j - i;
						break;
					}
				}
			}

			std::string spacing(off - i, ' ');
			std::string postfix(std::max<size_t>(1, length), '^');

			std::string txt;
			txt.reserve(len + 1 + spacing.length() + postfix.length() + 1);
			txt.append(view.substr(i, len));
			txt.append("\n");
			txt.append(spacing);
			txt.append(postfix);
			txt.append("\n");
			return txt;
		}
	}
}