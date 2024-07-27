#include "stdafx.h"
#include "AlgSaver.h"

#include <iostream>
#include <fstream>
	

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

	//Check category thing
	size_t dash = name.find(L'-');
	if (dash != std::wstring::npos)
	{
		std::wstring cat = name.substr(0, dash);
		std::wstring name2 = name.substr(dash + 1);

		for (Alg a : algs)
		{
			if (a.name == name2 && a.category == cat) return a;
		}
	}

	return Alg{ L"", L"", L"" };
}

AlgSaver::Alg AlgSaver::get(std::wstring name) const
{
	return get(m_Algs, name);
}

AlgSaver AlgSaver::getCategory(const std::vector<Alg>& algs, std::wstring category)
{
	std::vector<AlgSaver::Alg> algs2;

	for (Alg a : algs)
	{
		if (a.category == category) algs2.push_back(a);
	}

	return algs2;
}

AlgSaver AlgSaver::getCategory(std::wstring category) const
{
	return getCategory(m_Algs, category);
}

AlgSaver AlgSaver::getTagged(const std::vector<Alg>& algs, std::wstring tag)
{
	std::vector<AlgSaver::Alg> algs2;

	for (Alg a : algs)
	{
		if (hasTag(a, tag)) algs2.push_back(a);
	}

	return algs2;
}

AlgSaver AlgSaver::getTagged(const std::vector<Alg>& algs, const std::vector<std::wstring>& tags)
{
	std::vector<AlgSaver::Alg> algs2;

	for (Alg a : algs)
	{
		for (std::wstring t : tags)
		{
			if (hasTag(a, t))
			{
				algs2.push_back(a);
				break;
			}
		}
	}

	return algs2;
}

AlgSaver AlgSaver::getTagged(std::wstring tag) const
{
	return getTagged(m_Algs, tag);
}

AlgSaver AlgSaver::getTagged(const std::vector<std::wstring>& tags) const
{
	return getTagged(m_Algs, tags);
}

AlgSaver AlgSaver::getCategoryTagged(const std::vector<Alg>& algs, std::wstring category, const std::vector<std::wstring>& tags)
{
	std::vector<Alg> algs2;

	for (Alg a : algs)
	{
		if (a.category == category)
		{
			for (std::wstring t : tags)
			{
				if (hasTag(a, t))
				{
					algs2.push_back(a);
					break;
				}
			}
		}
	}

	return algs2;
}

AlgSaver AlgSaver::getCategoryTagged(std::wstring category, const std::vector<std::wstring>& tags) const
{
	return getCategoryTagged(m_Algs, category, tags);
}

bool AlgSaver::hasTag(Alg a, std::wstring tag)
{
	for (std::wstring s : a.tags)
	{
		if (s == tag) return true;
	}

	return false;
}


bool AlgSaver::addAlg(Alg a)
{
	if (a.name == L"") return false;

	//Check if already exists
	Alg existing = get(a.name);
	if (existing.name == a.name && existing.category == a.category) return false;

	a.index = int(m_Algs.size());
	m_Algs.push_back(a);

	return true;
}

bool AlgSaver::addAlg(std::wstring name, std::wstring alg, std::wstring category, std::vector<std::wstring> tags)
{
	return addAlg({ name, alg, category, tags });
}

bool AlgSaver::addAlg(std::wstring line, std::wstring category, std::vector<std::wstring> tags)
{
	std::vector<std::wstring> splitted = split(line, L',');
	if (splitted.size() >= 2)
	{
		std::vector<std::wstring> tagcopy = tags;

		//add optional tags to tags
		for (int i = 2; i < splitted.size(); ++i) 
			if (splitted[i] != L"") 
				tagcopy.push_back(splitted[i]);

		return addAlg(splitted[0], splitted[1], category, tagcopy);
	}
	
	return false;
}

void AlgSaver::addAlgs(std::wstring allLines, std::wstring category)
{
	std::wstringstream ss(allLines);
	std::wstring line;

	std::vector<std::wstring> curTags;

	while (std::getline(ss, line)) {
		if (!line.empty()) {
			//if starts with "*" is tag thing ("*[Tag],[Tag],[...]"
			if (line[0] == L'*')
			{
				line = line.substr(1);
				curTags = split(line, L',');
			}
			else
			{
				addAlg(line, category, curTags);
			}

		}
	}

}

void AlgSaver::addAlgsFromFile(const std::wstring& path, std::wstring category)
{
	std::wifstream file(path);
	std::wstring allLines = std::wstring((std::istreambuf_iterator<wchar_t>(file)), std::istreambuf_iterator<wchar_t>());
	addAlgs(allLines, category);
}

int AlgSaver::getRandTicketCount() const
{
	unsigned ct = 0;
	for (Alg a : m_Algs)
	{
		ct += a.randTickets;
	}
	return ct;
}

AlgSaver::Alg AlgSaver::getRandom() const
{
	unsigned tickets = getRandTicketCount();
	if(tickets <= 0) return Alg();

	unsigned ticketIndex = (rand()*RAND_MAX + rand()) % tickets;
	unsigned cur = 0;

	for (Alg a : m_Algs)
	{
		cur += a.randTickets;
		if (cur > ticketIndex) return a;
	}

	//idk mess up or something
	return m_Algs[m_Algs.size()-1];
}

void AlgSaver::updateRandTickets(const AlgSaver & algsToUpdate, Alg pickedAlg, int ticketIncChange, int minTicketInc)
{
	//Update all algs
	for (Alg a : algsToUpdate.m_Algs)
	{
		m_Algs[a.index].randTickets += m_Algs[a.index].randTicketInc;
	}

	//Update picked alg increment
	if (pickedAlg.index != -1)
	{
		if(int(m_Algs[pickedAlg.index].randTicketInc) + ticketIncChange < minTicketInc) m_Algs[pickedAlg.index].randTicketInc = minTicketInc;
		else m_Algs[pickedAlg.index].randTicketInc += ticketIncChange;

		m_Algs[pickedAlg.index].randTickets = 0;
	}
}

std::wstring AlgSaver::randomSideTurn(std::wstring side, bool halfSymmetric)
{
	//If its half symmetric you only do a U turn if turning at all (to make following steps same when practicing)
	int turning = rand() % (halfSymmetric? 2 : 4);

	switch (turning)
	{
	case 1:
		return side;
	case 2:
		return side + L"2";
	case 3:
		return side + L"'";
	}

	return L"";
}
