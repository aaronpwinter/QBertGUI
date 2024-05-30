#include <string>
#include <vector>

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
	~AlgSaver();

	//Get by name
	static Alg get(const std::vector<Alg> &algs, std::wstring name);
	Alg get(std::wstring name) const;

	static std::vector<Alg> getCategory(const std::vector<Alg> &algs, std::wstring category);
	std::vector<Alg> getCategory(std::wstring category) const;

	static std::vector<Alg> getTagged(const std::vector<Alg> &algs, std::wstring tag);
	std::vector<Alg> getTagged(std::wstring tag) const;                          

	static bool hasTag(Alg a, std::wstring tag);

	//Cant add two alg with same name so yeah return if bad :(
	bool addAlg(Alg a) { m_Algs.push_back(a); }
	bool addAlg(std::wstring name, std::wstring alg, std::wstring category = L"", std::vector<std::wstring> tags = std::vector<std::wstring>());

private:
	std::vector<Alg> m_Algs;
};

