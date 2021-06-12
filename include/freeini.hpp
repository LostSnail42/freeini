/*******************************************************************************
* The MIT License (MIT)
*
* Copyright (c) 2021 Garrett Tidd
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
*******************************************************************************/

#pragma once

#ifndef __INC_FREE_INI_HPP
#define __INC_FREE_INI_HPP


#include<iostream>
#include<fstream>

#include<map>
#include<string>

#include<regex>


template<class StringType>
class IniParser_base
{
	public:
		using SectionType = std::map<StringType, StringType>;
		using MapType = std::map<StringType, SectionType>;
		using Regex = std::basic_regex<typename StringType::value_type>;
		using RegexMatch = std::match_results<typename StringType::const_iterator>;



	private:
		MapType _map;

		StringType _defaultSecStr;
		StringType _newlineChar;

		const Regex _sectionRegex;
		const Regex _lineRegex;

	public:
		IniParser_base():
			_defaultSecStr("DEFAULT"),
			_newlineChar("\n"),
			_sectionRegex("^[\t ]*\\[([A-Za-z0-9_\\-]*)\\][\t ]*$"),
			_lineRegex("^[\t ]*([A-Za-z0-9_\\-]+)[\t ]*(\\=|\\:)[\t ]*([[:graph:]]*)[\t ]*$")
		{}
		~IniParser_base() = default;

		void clear()	{	_map.clear();	}

		bool load(std::fstream& file)
		{
			StringType lineBuffer;
			StringType curSection = _defaultSecStr;

			bool retVal = file.good();

			while(file.good())
			{
				std::getline(file, lineBuffer);

				RegexMatch match;
				if(std::regex_match(lineBuffer, match, _sectionRegex))
				{
					curSection = match[1];

					#ifndef NDEBUG
					std::cout << "New Section: " << curSection << std::endl;
					#endif//NDEBUG
				}
				else if(std::regex_match(lineBuffer, match, _lineRegex))
				{
					_map[curSection][match[1]] = match[3];

					#ifndef NDEBUG
					std::cout << "New option: " << match[0] << std::endl;
					#endif//NDEBUG
				}
				#ifndef NDEBUG
				else
				{
					std::cout << "No match: " << lineBuffer << std::endl;
				}
				#endif//NDEBUG
			}

			return retVal;
		}
		bool load(StringType filename)
		{
			std::fstream file(filename, std::ios_base::in | std::ios_base::binary);

			if(file.is_open())
			{
				return load(file);
			}

			return false;
		}

		bool save(std::fstream& file)
		{
			for(auto& section : _map)
			{
				if(!section.first.empty())
				{
					file << "[" << section.first << "]" << _newlineChar;

					#ifndef NDEBUG
					std::cout << "New Section: " << section.first << std::endl;
					#endif//NDEBUG
				}

				for(auto& option : section.second)
				{
					file << option.first << "=" << option.second << _newlineChar;

					#ifndef NDEBUG
					std::cout << "New option: " << option.first << " = " << option.second << std::endl;
					#endif//NDEBUG
				}
			}

			return true;
		}
		bool save(StringType filename)
		{
			std::fstream file(filename, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);

			if(file.is_open())
			{
				return save(file);
			}
		}

		SectionType& operator[](StringType section)
		{
			return _map[section];
		}

		// friend std::ostream& operator<<(std::ostream& out, std::ifstream& in)
		// {
		//
		// }
		// friend std::istream& operator>>(std::istream& in, std::ofstream& out)
		// {
		//
		// }
};

typedef IniParser_base<std::string> IniParser;
typedef IniParser_base<std::wstring> wIniParser;

#endif /* end of include guard: __INC_FREE_INI_HPP */
