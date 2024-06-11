#include <string>
#include <vector>


#include <algorithm> 
#include <cctype>
#include <cwctype>
#include <locale>

#include <sstream>

#include <random>


#pragma once
class AlgSaver
{
public:
	struct Alg
	{
		std::wstring name, alg, category;
		std::vector<std::wstring> tags = std::vector<std::wstring>();


	};

public:
	AlgSaver();
	AlgSaver(const std::vector<Alg>& algs) :
		m_Algs(algs)
	{

	}
	~AlgSaver();

	//Get by name
	//Can also get by Category + Name
	//	"[Category]-[Name]"
	static Alg get(const std::vector<Alg> &algs, std::wstring name);
	Alg get(std::wstring name) const;

	static AlgSaver getCategory(const std::vector<Alg> &algs, std::wstring category);
	AlgSaver getCategory(std::wstring category) const;

	static AlgSaver getTagged(const std::vector<Alg> &algs, std::wstring tag);
	AlgSaver getTagged(std::wstring tag) const;

	//Returns something with ANY of the tags
	static AlgSaver getCategoryTagged(const std::vector<Alg> &algs, std::wstring category, const std::vector<std::wstring>& tags);
	AlgSaver getCategoryTagged(std::wstring category, const std::vector<std::wstring>& tags) const;

	static bool hasTag(Alg a, std::wstring tag);

	//Cant add two alg with same category + name so yeah return if bad :(
	bool addAlg(Alg a);
	bool addAlg(std::wstring name, std::wstring alg, std::wstring category = L"", std::vector<std::wstring> tags = std::vector<std::wstring>());
	//Category/Tags found elsewhere - format is "[Alg Name],[Alg],[Opt Tag],[Opt Tag],[...]"
	bool addAlg(std::wstring line, std::wstring category = L"", std::vector<std::wstring> tags = std::vector<std::wstring>());
	void addAlgs(std::wstring allLines, std::wstring category = L"");

	//Random
	Alg getRandom() const;

	static std::wstring randomSideTurn(std::wstring side);


private:
	std::vector<Alg> m_Algs;



	//Helper functions
private:
	//https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
	// trim from start (in place)
	inline void ltrim(std::wstring &s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](wchar_t ch) {
			return !std::iswspace(ch);
		}));
	}

	// trim from end (in place)
	inline void rtrim(std::wstring &s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), [](wchar_t ch) {
			return !std::iswspace(ch);
		}).base(), s.end());
	}

	// trim from both ends (in place)
	inline void trim(std::wstring &s) {
		rtrim(s);
		ltrim(s);
	}

	inline void ltrim(std::string &s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](char ch) {
			return !std::isspace(ch);
		}));
	}

	// trim from end (in place)
	inline void rtrim(std::string &s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), [](char ch) {
			return !std::isspace(ch);
		}).base(), s.end());
	}

	// trim from both ends (in place)
	inline void trim(std::string &s) {
		rtrim(s);
		ltrim(s);
	}


	std::vector<std::wstring> split(const std::wstring &s, wchar_t delim) {
		std::vector<std::wstring> elems;
		std::wstringstream ss(s);
		std::wstring item;
		while (std::getline(ss, item, delim)) {
			if (!item.empty()) {
				trim(item);
				elems.push_back(item);
			}
		}
		return elems;
	}

	std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			if (!item.empty()) {
				trim(item);
				elems.push_back(item);
			}
		}
		return elems;
	}
};

