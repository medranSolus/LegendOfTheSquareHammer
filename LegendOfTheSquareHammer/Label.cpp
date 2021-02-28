/*
Author: Marek Machliñski
Date:	20.04.2018
*/
#include "stdafx.h"
#include "Label.h"
#include <map>

namespace LOTSH
{
	extern std::map<std::string, void(*)()> MenuOptions;
}
namespace LOTSH::Renders
{
	Label::Label(const SDL_Rect& newPosition, const std::string& newPath, const std::string& callFunction) : path(newPath)
	{
		if (!texture.LoadFile(path))
			exit(0b1000000000);
		position = newPosition;
		if (position.w == 0 && position.h == 0)
		{
			position.w = texture.Width();
			position.h = texture.Height();
		}
		function = callFunction;
	}

	void Label::operator()()
	{
		std::map<std::string, void(*)()>::iterator iterator = MenuOptions.find(function);
		if (iterator != MenuOptions.end())
			iterator->second();
		else
		{
			std::ofstream fout("error.txt", std::ios_base::app);
			fout << "Cannot find and call function: " << function << std::endl;
			fout.close();
		}
	}

	std::ofstream& operator<<(std::ofstream& fout, const Label& label)
	{
		fout << label.position << std::endl;
		if (label.texture.IsText())
		{
			fout << 1 << ' '
				<< label.texture.TextColor().r << ' '
				<< label.texture.TextColor().g << ' '
				<< label.texture.TextColor().b << ' '
				<< label.texture.TextColor().a << std::endl
				<< label.texture.Text();
		}
		else
			fout << 0;
		fout << std::endl << label.path << std::endl
			<< label.function << std::endl;
		return fout;
	}

	std::ifstream& operator>>(std::ifstream& fin, Label& label)
	{
		fin >> label.position;
		fin.ignore();
		std::string text = "";
		SDL_Color textColor = { 0 };
		int isText = 0;
		fin >> isText;
		if (isText)
		{
			fin >> textColor.r
				>> textColor.g
				>> textColor.b
				>> textColor.a;
			fin.ignore();
			getline(fin, text);
		}
		if (fin.peek() == 10)
			fin.ignore();
		getline(fin, label.path);
		getline(fin, label.function);
		if (!label.texture.LoadFile(label.path))
		{
			std::ofstream fout("error.txt", std::ios_base::app);
			fout << "LoadObject error: cannot load file " << label.path << std::endl;
			fout.close();
			exit(0b10000000000);
		}
		if (text.size() && !label.texture.LoadText(text, textColor))
		{
			std::ofstream fout("error.txt", std::ios_base::app);
			fout << "LoadObject error: cannot load text " << text << std::endl;
			fout.close();
			exit(0b100000000000);
		}
		return fin;
	}
}