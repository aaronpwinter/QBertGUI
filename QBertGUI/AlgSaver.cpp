#include "stdafx.h"
#include "AlgSaver.h"


AlgSaver::AlgSaver()
{
}


AlgSaver::~AlgSaver()
{
}

AlgSaver::Alg AlgSaver::get(const std::vector<Alg>& algs, std::wstring name)
{
	for (Alg a : algs)
	{
		if (a.name == name) return a;
	}

	return Alg{ L"", L"", L"" };
}

AlgSaver::Alg AlgSaver::get(std::wstring name) const
{
	return get(m_Algs, name);
}

std::vector<AlgSaver::Alg> AlgSaver::getCategory(const std::vector<Alg>& algs, std::wstring category)
{
	std::vector<AlgSaver::Alg> algs2;

	for (Alg a : algs)
	{
		if (a.category == category) algs2.push_back(a);
	}

	return algs2;
}

std::vector<AlgSaver::Alg> AlgSaver::getCategory(std::wstring category) const
{
	return getCategory(m_Algs, category);
}

std::vector<AlgSaver::Alg> AlgSaver::getTagged(const std::vector<Alg>& algs, std::wstring tag)
{
	std::vector<AlgSaver::Alg> algs2;

	for (Alg a : algs)
	{
		if (hasTag(a, tag)) algs2.push_back(a);
	}

	return algs2;
}

std::vector<AlgSaver::Alg> AlgSaver::getTagged(std::wstring tag) const
{
	return getTagged(m_Algs, tag);
}

bool AlgSaver::hasTag(Alg a, std::wstring tag)
{
	for (std::wstring s : a.tags)
	{
		if (s == tag) return true;
	}

	return false;
}


bool AlgSaver::addAlg(std::wstring name, std::wstring alg, std::wstring category, std::vector<std::wstring> tags)
{
	//Check if already exists
	if (get(name).name == name) return false;

	m_Algs.push_back({ name, alg, category, tags });

	return true;
}
