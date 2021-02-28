/*
Author: Marek Machliñski
Date:	20.04.2018
*/
#include "Info.h"
using LOTSH::Renders::Label;

namespace LOTSH
{
	void Info::ArmorInfo(Renders::Items::Armor& item, const Stats& statistics)
	{
		ProgramData::LabelsData& data = ProgramData::Data::GetLabels();
		switch (item.Part())
		{
		case BodyPart::Necklace:
		{
			data.Info.at(9).Texture().LoadText("Necklace", ProgramData::Data::NormalColor());
			break;
		}
		case BodyPart::Ring:
		{
			data.Info.at(9).Texture().LoadText("Ring", ProgramData::Data::NormalColor());
			break;
		}
		case BodyPart::Head:
		{
			data.Info.at(9).Texture().LoadText("Helmet", ProgramData::Data::NormalColor());
			break;
		}
		case BodyPart::Chest:
		{
			data.Info.at(9).Texture().LoadText("Armor", ProgramData::Data::NormalColor());
			break;
		}
		case BodyPart::Leg:
		{
			data.Info.at(9).Texture().LoadText("Greaves", ProgramData::Data::NormalColor());
			break;
		}
		case BodyPart::Hands:
		{
			data.Info.at(9).Texture().LoadText("Gauntlets", ProgramData::Data::NormalColor());
			break;
		}
		}
		for (auto& res : data.Resistance)
			res.SetPosition(res.Position().x, res.Position().y - 380);

		data.Armor.at(0).Texture().LoadText("Inteligence: " + std::to_string(item.Statistics().MainStatistics.Inteligence), ProgramData::Data::NormalColor());
		data.Armor.at(1).Texture().LoadText("Strength: " + std::to_string(item.Statistics().MainStatistics.Strenght), ProgramData::Data::NormalColor());
		data.Armor.at(2).Texture().LoadText("Dextermity: " + std::to_string(item.Statistics().MainStatistics.Dextermity), ProgramData::Data::NormalColor());
		data.Armor.at(3).Texture().LoadText("Endurance: " + std::to_string(item.Statistics().Endurance), ProgramData::Data::NormalColor());
		data.Armor.at(4).Texture().LoadText("Vitality: " + std::to_string(item.Statistics().Vitality), ProgramData::Data::NormalColor());
		data.Armor.at(5).Texture().LoadText("Armor: " + std::to_string(item.Statistics().Armor), ProgramData::Data::NormalColor());
		data.Resistance.at(1).Texture().LoadText("Fire: " + std::to_string(item.Statistics().FireResistance), { 255,0,0,255 });
		data.Resistance.at(2).Texture().LoadText("Ice: " + std::to_string(item.Statistics().IceResistance), { 0,197,255,255 });
		data.Resistance.at(3).Texture().LoadText("Magic: " + std::to_string(item.Statistics().MagicResistance), { 87,14,185,255 });
		data.Resistance.at(4).Texture().LoadText("Darkness: " + std::to_string(item.Statistics().DarknessResistance), { 100,100,100,255 });

		data.Armor.at(8).Texture().LoadText("Inteligence: " + std::to_string(item.MinimumStatistics().Inteligence),
			statistics.MainStatistics.Inteligence >= item.MinimumStatistics().Inteligence ? ProgramData::Data::NormalColor() : SDL_Color({ 255, 0, 0, 255 }));
		data.Armor.at(9).Texture().LoadText("Strenght: " + std::to_string(item.MinimumStatistics().Strenght),
			statistics.MainStatistics.Strenght >= item.MinimumStatistics().Strenght ? ProgramData::Data::NormalColor() : SDL_Color({ 255, 0, 0, 255 }));
		data.Armor.at(10).Texture().LoadText("Dextermity: " + std::to_string(item.MinimumStatistics().Dextermity),
			statistics.MainStatistics.Dextermity >= item.MinimumStatistics().Dextermity ? ProgramData::Data::NormalColor() : SDL_Color({ 255, 0, 0, 255 }));

		bool quit = false, resistance = false;
		while (!quit)
		{
			data.Mini.at(14).Render();
			item.Texture().Render(312, 193, 245, 245);
			for_each(data.Info.begin(), data.Info.begin() + 10, [](Label& x) { x.Render(); });
			for (auto& armor : data.Armor)
				armor.Render();
			if (resistance)
			{
				for (auto& res : data.Resistance)
					res.Render();
				data.Armor.at(6).Render();
			}
			SDL_RenderPresent(ProgramData::Data::Renderer());

			SDL_Event e;
			while (SDL_PollEvent(&e))
			{
				switch (e.type)
				{
				case SDL_MOUSEMOTION:
				{
					int x, y;
					SDL_GetMouseState(&x, &y);
					data.Info.at(7).Texture().SetColor(data.Info.at(7).IsMouseOn(x, y) ? ProgramData::Data::HOOVER_COLOR : ProgramData::Data::NormalColor());
					data.Armor.at(6).Texture().SetColor(data.Armor.at(6).IsMouseOn(x, y) ? ProgramData::Data::HOOVER_COLOR : ProgramData::Data::NormalColor());
					break;
				}
				case SDL_MOUSEBUTTONDOWN:
				{
					if (e.button.button == SDL_BUTTON_LEFT)
					{
						int x, y;
						SDL_GetMouseState(&x, &y);
						if (data.Info.at(7).IsMouseOn(x, y))
							quit = true;
						else
							resistance = data.Armor.at(6).IsMouseOn(x, y);
					}
					break;
				}
				}
			}
		}
		for (auto& res : data.Resistance)
			res.SetPosition(res.Position().x, res.Position().y + 380);
		data.Resistance.at(1).Texture().LoadText("Fire: " + std::to_string(statistics.FireResistance), { 255,0,0,255 });
		data.Resistance.at(2).Texture().LoadText("Ice: " + std::to_string(statistics.IceResistance), { 0,197,255,255 });
		data.Resistance.at(3).Texture().LoadText("Magic: " + std::to_string(statistics.MagicResistance), { 87,14,185,255 });
		data.Resistance.at(4).Texture().LoadText("Darkness: " + std::to_string(statistics.DarknessResistance), { 100,100,100,255 });
	}

	void Info::WeaponInfo(Renders::Items::Weapon& item, const Stats& statistics)
	{
		ProgramData::LabelsData& data = ProgramData::Data::GetLabels();
		switch (item.Type())
		{
		case WeaponType::Axe:
		{
			data.Info.at(9).Texture().LoadText("   Axe   ", ProgramData::Data::NormalColor());
			break;
		}
		case WeaponType::Hammer:
		{
			data.Info.at(9).Texture().LoadText("Hammer", ProgramData::Data::NormalColor());
			break;
		}
		case WeaponType::Sword:
		{
			data.Info.at(9).Texture().LoadText("Sword", ProgramData::Data::NormalColor());
			break;
		}
		case WeaponType::Dagger:
		{
			data.Info.at(9).Texture().LoadText("Dagger", ProgramData::Data::NormalColor());
			break;
		}
		}
		for (auto& res : data.Resistance)
			res.SetPosition(res.Position().x, res.Position().y - 255);

		data.Weapon.at(0).Texture().LoadText("Inteligence: " + std::to_string(item.Statistics().MainStatistics.Inteligence), ProgramData::Data::NormalColor());
		data.Weapon.at(1).Texture().LoadText("Strength: " + std::to_string(item.Statistics().MainStatistics.Strenght), ProgramData::Data::NormalColor());
		data.Weapon.at(2).Texture().LoadText("Dextermity: " + std::to_string(item.Statistics().MainStatistics.Dextermity), ProgramData::Data::NormalColor());
		data.Weapon.at(3).Texture().LoadText("Endurance: " + std::to_string(item.Statistics().Endurance), ProgramData::Data::NormalColor());
		data.Weapon.at(4).Texture().LoadText("Vitality: " + std::to_string(item.Statistics().Vitality), ProgramData::Data::NormalColor());
		data.Weapon.at(5).Texture().LoadText("Armor: " + std::to_string(item.Statistics().Armor), ProgramData::Data::NormalColor());
		data.Resistance.at(1).Texture().LoadText("Fire: " + std::to_string(item.Statistics().FireResistance), { 255,0,0,255 });
		data.Resistance.at(2).Texture().LoadText("Ice: " + std::to_string(item.Statistics().IceResistance), { 0,197,255,255 });
		data.Resistance.at(3).Texture().LoadText("Magic: " + std::to_string(item.Statistics().MagicResistance), { 87,14,185,255 });
		data.Resistance.at(4).Texture().LoadText("Darkness: " + std::to_string(item.Statistics().DarknessResistance), { 100,100,100,255 });
		data.Weapon.at(14).Texture().LoadText("Normal: " + std::to_string(item.Damage().Normal), ProgramData::Data::NormalColor());
		data.Weapon.at(15).Texture().LoadText("Fire: " + std::to_string(item.Damage().Fire), { 255,0,0,255 });
		data.Weapon.at(16).Texture().LoadText("Ice: " + std::to_string(item.Damage().Ice), { 0,197,255,255 });
		data.Weapon.at(17).Texture().LoadText("Magic: " + std::to_string(item.Damage().Magic), { 87,14,185,255 });
		data.Weapon.at(18).Texture().LoadText("Darkness: " + std::to_string(item.Damage().Darkness), { 100,100,100,255 });
		data.Weapon.at(19).Texture().LoadText("Critical: " + std::to_string(item.CriticalChance()), ProgramData::Data::NormalColor());
		data.Weapon.at(20).Texture().LoadText("Penetration: " + std::to_string(item.ArmorPenetration()), ProgramData::Data::NormalColor());

		data.Weapon.at(8).Texture().LoadText("Inteligence: " + std::to_string(item.MinimumStatistics().Inteligence),
			statistics.MainStatistics.Inteligence >= item.MinimumStatistics().Inteligence ? ProgramData::Data::NormalColor() : SDL_Color({ 255, 0, 0, 255 }));
		data.Weapon.at(9).Texture().LoadText("Strenght: " + std::to_string(item.MinimumStatistics().Strenght),
			statistics.MainStatistics.Strenght >= item.MinimumStatistics().Strenght ? ProgramData::Data::NormalColor() : SDL_Color({ 255,0,0,255 }));
		data.Weapon.at(10).Texture().LoadText("Dextermity: " + std::to_string(item.MinimumStatistics().Dextermity),
			statistics.MainStatistics.Dextermity >= item.MinimumStatistics().Dextermity ? ProgramData::Data::NormalColor() : SDL_Color({ 255,0,0,255 }));

		bool quit = false, resistance = false, damage = false;
		while (!quit)
		{
			data.Mini.at(14).Render();
			item.Texture().Render(312, 193, 245, 245);
			for_each(data.Info.begin(), data.Info.begin() + 10, [](Label& x) { x.Render(); });
			for_each(data.Weapon.begin(), data.Weapon.begin() + 13, [](Label& x) { x.Render(); });
			if (resistance)
			{
				for (auto& res : data.Resistance)
					res.Render();
				data.Weapon.at(6).Render();
			}
			if (damage)
			{
				for_each(data.Weapon.begin() + 13, data.Weapon.end(), [](Label& x) { x.Render(); });
				data.Weapon.at(12).Render();
			}
			SDL_RenderPresent(ProgramData::Data::Renderer());

			SDL_Event e;
			while (SDL_PollEvent(&e))
			{
				switch (e.type)
				{
				case SDL_MOUSEMOTION:
				{
					int x, y;
					SDL_GetMouseState(&x, &y);
					data.Info.at(7).Texture().SetColor(data.Info.at(7).IsMouseOn(x, y) ? ProgramData::Data::HOOVER_COLOR : ProgramData::Data::NormalColor());
					data.Weapon.at(6).Texture().SetColor(data.Weapon.at(6).IsMouseOn(x, y) ? ProgramData::Data::HOOVER_COLOR : ProgramData::Data::NormalColor());
					data.Weapon.at(12).Texture().SetColor(data.Weapon.at(12).IsMouseOn(x, y) ? ProgramData::Data::HOOVER_COLOR : ProgramData::Data::NormalColor());
					break;
				}
				case SDL_MOUSEBUTTONDOWN:
				{
					if (e.button.button == SDL_BUTTON_LEFT)
					{
						int x, y;
						SDL_GetMouseState(&x, &y);
						if (data.Info.at(7).IsMouseOn(x, y))
							quit = true;
						else
						{
							resistance = data.Weapon.at(6).IsMouseOn(x, y);
							damage = data.Weapon.at(12).IsMouseOn(x, y);
						}
					}
					break;
				}
				}
			}
		}
		for (auto& res : data.Resistance)
			res.SetPosition(res.Position().x, res.Position().y + 255);
		data.Resistance.at(1).Texture().LoadText("Fire: " + std::to_string(statistics.FireResistance), { 255,0,0,255 });
		data.Resistance.at(2).Texture().LoadText("Ice: " + std::to_string(statistics.IceResistance), { 0,197,255,255 });
		data.Resistance.at(3).Texture().LoadText("Magic: " + std::to_string(statistics.MagicResistance), { 87,14,185,255 });
		data.Resistance.at(4).Texture().LoadText("Darkness: " + std::to_string(statistics.DarknessResistance), { 100,100,100,255 });
	}

	void Info::PotionInfo(Renders::Items::SingleUse::Potion& item)
	{
		ProgramData::LabelsData& data = ProgramData::Data::GetLabels();
		if (item.Health() == 0)
			data.Potion.at(0).Texture().LoadText("No health changed", ProgramData::Data::NormalColor());
		else
			data.Potion.at(0).Texture().LoadText(std::string("Health ") + (item.Health() > 0 ? "restored" : "taken") + ": " + std::to_string(abs(item.Health())), ProgramData::Data::NormalColor());

		if (item.Mana() == 0)
			data.Potion.at(1).Texture().LoadText("No mana required", ProgramData::Data::NormalColor());
		else
			data.Potion.at(1).Texture().LoadText(std::string("Mana ") + (item.Mana() > 0 ? "restored" : "required") + ": " + std::to_string(abs(item.Mana())), ProgramData::Data::NormalColor());

		if (item.ItemType() == ItemType::StatsPotion)
			data.Info.at(9).Texture().LoadText("Upgrade Potion", ProgramData::Data::NormalColor());
		else
			data.Info.at(9).Texture().LoadText("Potion", ProgramData::Data::NormalColor());

		while (true)
		{
			data.Mini.at(14).Render();
			item.Texture().Render(312, 193, 245, 245);
			for_each(data.Info.begin(), data.Info.begin() + 10, [](Label& x) { x.Render(); });
			for (auto& pot : data.Potion)
				pot.Render();
			SDL_RenderPresent(ProgramData::Data::Renderer());

			SDL_Event e;
			while (SDL_PollEvent(&e))
			{
				switch (e.type)
				{
				case SDL_MOUSEMOTION:
				{
					int x, y;
					SDL_GetMouseState(&x, &y);
					data.Info.at(7).Texture().SetColor(data.Info.at(7).IsMouseOn(x, y) ? ProgramData::Data::HOOVER_COLOR : ProgramData::Data::NormalColor());
					break;
				}
				case SDL_MOUSEBUTTONDOWN:
				{
					if (e.button.button == SDL_BUTTON_LEFT)
					{
						int x, y;
						SDL_GetMouseState(&x, &y);
						if (data.Info.at(7).IsMouseOn(x, y))
							return;
					}
					break;
				}
				}
			}
		}
	}

	void Info::SetStatsLabels(const Stats& currentStatistics, const Damage& currentDamage)
	{
		ProgramData::LabelsData& data = ProgramData::Data::GetLabels();
		data.Stats.at(1).Texture().LoadText("Inteligence: " + std::to_string(currentStatistics.MainStatistics.Inteligence), ProgramData::Data::NormalColor());
		data.Stats.at(2).Texture().LoadText("Strength: " + std::to_string(currentStatistics.MainStatistics.Strenght), ProgramData::Data::NormalColor());
		data.Stats.at(3).Texture().LoadText("Dextermity: " + std::to_string(currentStatistics.MainStatistics.Dextermity), ProgramData::Data::NormalColor());
		data.Stats.at(4).Texture().LoadText("Endurance: " + std::to_string(currentStatistics.Endurance), ProgramData::Data::NormalColor());
		data.Stats.at(5).Texture().LoadText("Vitality: " + std::to_string(currentStatistics.Vitality), ProgramData::Data::NormalColor());
		data.Stats.at(8).Texture().LoadText("Armor: " + std::to_string(currentStatistics.Armor), ProgramData::Data::NormalColor());
		data.Resistance.at(1).Texture().LoadText("Fire: " + std::to_string(currentStatistics.FireResistance), { 255,0,0,255 });
		data.Resistance.at(2).Texture().LoadText("Ice: " + std::to_string(currentStatistics.IceResistance), { 0,197,255,255 });
		data.Resistance.at(3).Texture().LoadText("Magic: " + std::to_string(currentStatistics.MagicResistance), { 87,14,185,255 });
		data.Resistance.at(4).Texture().LoadText("Darkness: " + std::to_string(currentStatistics.DarknessResistance), { 100,100,100,255 });
		data.Damage.at(1).Texture().LoadText("Normal: " + std::to_string(currentDamage.Normal), ProgramData::Data::NormalColor());
		data.Damage.at(2).Texture().LoadText("Fire: " + std::to_string(currentDamage.Fire), { 255,0,0,255 });
		data.Damage.at(3).Texture().LoadText("Ice: " + std::to_string(currentDamage.Ice), { 0,197,255,255 });
		data.Damage.at(4).Texture().LoadText("Magic: " + std::to_string(currentDamage.Magic), { 87,14,185,255 });
		data.Damage.at(5).Texture().LoadText("Darkness: " + std::to_string(currentDamage.Darkness), { 100,100,100,255 });
	}

	void Info::ItemInfo(Renders::Items::Item& item, const Renders::Objects::Player& player)
	{
		auto& dataInfo = ProgramData::Data::GetLabels().Info;
		dataInfo.at(0).Texture().LoadText(item.Name(), ProgramData::Data::HOOVER_COLOR);
		dataInfo.at(8).Texture().LoadText("Level: " +
			std::to_string(item.MinimumLevel()), player.Level() < item.MinimumLevel() ? SDL_Color({ 255,0,0,255 }) : ProgramData::Data::NormalColor());

		std::string description = item.Description();
		if (description.size() > 125)
		{
			for (size_t i = 0, I = description.size(), j = 0; i < 5; ++i)
			{
				std::string line;
				for (size_t J = I / 5 + i * I / 5; j < J && j < I;)
				{
					if (j == J - 1 && description[j] == ' ')
						++j;
					else if (j == J - 1 && j < I - 1 && description[j + 1] != ' ' && description[j] != ' ')
					{
						for (; j < I && description[j] != ' '; ++j)
							line += description[j];
						break;
					}
					else if (j == I - 1)
					{
						line += description[j];
						++j;
						for (; j < J; ++j)
							line += " ";
						break;
					}
					else
					{
						line += description[j];
						++j;
					}
				}
				dataInfo.at(2 + i).Texture().LoadText(line, ProgramData::Data::NormalColor());
			}
		}
		else if (description.size() > 25)
		{
			for (size_t i = 0, I = description.size(), j = 0; i < 5; ++i)
			{
				std::string line;
				for (size_t J = 25 + i * 25; j < J && j < I;)
				{
					if (j == J - 1 && description[j] == ' ')
						++j;
					else if (j == J - 1 && j < I - 1 && description[j + 1] != ' ' && description[j] != ' ')
					{
						for (; j < I && description[j] != ' '; ++j)
							line += description[j];
						break;
					}
					else if (j == I - 1)
					{
						line += description[j];
						++j;
						for (; j < J; ++j)
							line += " ";
					}
					else
					{
						line += description[j];
						++j;
					}
				}
				dataInfo.at(2 + i).Texture().LoadText(line, ProgramData::Data::NormalColor());
			}
		}
		else
			dataInfo.at(2).Texture().LoadText(description, ProgramData::Data::NormalColor());

		switch (item.ItemType())
		{
		case ItemType::Armor:
		{
			ArmorInfo(static_cast<Renders::Items::Armor&>(item), player.Statistics());
			break;
		}
		case ItemType::Weapon:
		{
			WeaponInfo(static_cast<Renders::Items::Weapon&>(item), player.Statistics());
			break;
		}
		case ItemType::Potion:
		case ItemType::StatsPotion:
		{
			PotionInfo(static_cast<Renders::Items::SingleUse::Potion&>(item));
			break;
		}
		case ItemType::AttackScroll:
		{
			ProgramData::Data::GetLabels().Info.at(9).Texture().LoadText("Offensive Scroll", ProgramData::Data::NormalColor());
			break;
		}
		case ItemType::EffectScroll:
		{
			ProgramData::Data::GetLabels().Info.at(9).Texture().LoadText("Enchantment", ProgramData::Data::NormalColor());
			break;
		}
		case ItemType::SpellScroll:
		{
			ProgramData::Data::GetLabels().Info.at(9).Texture().LoadText("Spell", ProgramData::Data::NormalColor());
			break;
		}
		}
	}

	void Info::PlayerStatistics(Renders::Objects::Player& player)
	{
		size_t pointsLeft = 0;
		Stats newStatistics;
		bool resistance = false, damage = false, isStatisticsHoover = false, newLevel = false;
		ProgramData::LabelsData& data = ProgramData::Data::GetLabels();
		if (player.PointsLefToUse())
		{
			newLevel = true;
			pointsLeft = player.PointsLefToUse() * 5;
			data.Mini.at(13).Texture().LoadText("Points left: " + std::to_string(pointsLeft), ProgramData::Data::NormalColor());
		}

		data.Stats.at(0).Texture().LoadText("Level: " + std::to_string(player.Level()), ProgramData::Data::NormalColor());
		data.Stats.at(9).Texture().LoadText(player.Name(), ProgramData::Data::HOOVER_COLOR);
		data.Stats.at(10).Texture().LoadText("Health: " + std::to_string(player.Health()) + "/" + std::to_string(player.MaxHealth()), { 255,0,0,255 });
		data.Stats.at(11).Texture().LoadText("Mana: " + std::to_string(player.Mana()) + "/" + std::to_string(player.MaxMana()), { 0,197,255,255 });
		data.Stats.at(13).Texture().LoadText("Exp: " + std::to_string(player.Experience()) + "/" + std::to_string(player.NextLevelExperience()), ProgramData::Data::NormalColor());
		data.Stats.at(12).Texture().LoadText("Money: " + std::to_string(player.Money()), { 255,215,0,255 });
		SetStatsLabels(player.Statistics(), player.Damage());

		while (true)
		{
			data.Mini.at(12).Render();
			for_each(data.Stats.begin() + 9, data.Stats.begin() + 13, [](Label& x) { x.Render(); });
			if (!newLevel)
			{
				data.Stats.at(13).Render();
				for (auto& stat : data.Stats)
					stat.Render();
				if (damage)
				{
					for (auto& stat : data.Damage)
						stat.Render();
					data.Stats.at(6).Render();
				}
				if (resistance)
				{
					for (auto& stat : data.Resistance)
						stat.Render();
					data.Stats.at(7).Render();
				}
			}
			else
			{
				for_each(data.Stats.begin(), data.Stats.begin() + 6, [](Label& x) { x.Render(); });
				if (pointsLeft)
					for (auto& arrows : data.Arrows)
						arrows.second.Render();
				data.Mini.at(13).Render();
				if (newStatistics.MainStatistics.Inteligence)
					data.Arrows.at(0).first.Render();
				if (newStatistics.MainStatistics.Strenght)
					data.Arrows.at(1).first.Render();
				if (newStatistics.MainStatistics.Dextermity)
					data.Arrows.at(2).first.Render();
				if (newStatistics.Endurance)
					data.Arrows.at(3).first.Render();
				if (newStatistics.Vitality)
					data.Arrows.at(4).first.Render();
			}
			if (isStatisticsHoover)
			{
				data.Mini.at(10).Render();
				data.Mini.at(11).Render();
			}
			else
			{
				data.Mini.at(11).Render();
				data.Mini.at(10).Render();
			}
			SDL_RenderPresent(ProgramData::Data::Renderer());

			SDL_Event e;
			while (SDL_PollEvent(&e))
			{
				switch (e.type)
				{
				case SDL_KEYDOWN:
				case SDL_KEYUP:
				{
					Events::EventHandler::GetKeyboard().HandleKey(e);
					break;
				}
				case SDL_MOUSEMOTION:
				{
					int x, y;
					SDL_GetMouseState(&x, &y);
					isStatisticsHoover = data.Mini.at(11).IsMouseOn(x, y);
					data.Mini.at(11).Texture().SetColor(isStatisticsHoover ? ProgramData::Data::HOOVER_COLOR : ProgramData::Data::NormalColor());
					if (!newLevel)
					{
						data.Stats.at(6).Texture().SetColor(data.Stats.at(6).IsMouseOn(x, y) ? ProgramData::Data::HOOVER_COLOR : ProgramData::Data::NormalColor());
						data.Stats.at(7).Texture().SetColor(data.Stats.at(7).IsMouseOn(x, y) ? ProgramData::Data::HOOVER_COLOR : ProgramData::Data::NormalColor());
					}
					else
					{
						if (pointsLeft)
							for (auto& arrows : data.Arrows)
								arrows.second.Texture().SetColor(arrows.second.IsMouseOn(x, y) ? ProgramData::Data::HOOVER_COLOR : ProgramData::Data::NormalColor());
						else
							data.Mini.at(13).Texture().SetColor(data.Mini.at(13).IsMouseOn(x, y) ? ProgramData::Data::HOOVER_COLOR : ProgramData::Data::NormalColor());

						data.Arrows.at(0).first.Texture().SetColor(newStatistics.MainStatistics.Inteligence &&
							data.Arrows.at(0).first.IsMouseOn(x, y) ? ProgramData::Data::HOOVER_COLOR : ProgramData::Data::NormalColor());

						data.Arrows.at(1).first.Texture().SetColor(newStatistics.MainStatistics.Strenght &&
							data.Arrows.at(1).first.IsMouseOn(x, y) ? ProgramData::Data::HOOVER_COLOR : ProgramData::Data::NormalColor());

						data.Arrows.at(2).first.Texture().SetColor(newStatistics.MainStatistics.Dextermity &&
							data.Arrows.at(2).first.IsMouseOn(x, y) ? ProgramData::Data::HOOVER_COLOR : ProgramData::Data::NormalColor());

						data.Arrows.at(3).first.Texture().SetColor(newStatistics.Endurance &&
							data.Arrows.at(3).first.IsMouseOn(x, y) ? ProgramData::Data::HOOVER_COLOR : ProgramData::Data::NormalColor());

						data.Arrows.at(4).first.Texture().SetColor(newStatistics.Vitality &&
							data.Arrows.at(4).first.IsMouseOn(x, y) ? ProgramData::Data::HOOVER_COLOR : ProgramData::Data::NormalColor());
					}
					break;
				}
				case SDL_MOUSEBUTTONDOWN:
				{
					int x, y;
					SDL_GetMouseState(&x, &y);
					if (e.button.button == SDL_BUTTON_LEFT)
					{
						if (data.Mini.at(11).IsMouseOn(x, y))
						{
							data.Mini.at(11).Texture().SetColor(ProgramData::Data::NormalColor());
							return PlayerEquipment(player);
						}
						if (!newLevel)
						{
							damage = data.Stats.at(6).IsMouseOn(x, y);
							resistance = data.Stats.at(7).IsMouseOn(x, y);
						}
						else
						{
							if (pointsLeft)
							{
								if (data.Arrows.at(0).second.IsMouseOn(x, y))
								{
									--pointsLeft;
									data.Mini.at(13).Texture().LoadText("Points left: " + std::to_string(pointsLeft), ProgramData::Data::NormalColor());
									++newStatistics.MainStatistics.Inteligence;
									data.Stats.at(1).Texture().LoadText("Inteligence: " +
										std::to_string(player.Statistics().MainStatistics.Inteligence + newStatistics.MainStatistics.Inteligence), ProgramData::Data::NormalColor());
									data.Stats.at(11).Texture().LoadText("Mana: " + std::to_string(player.Mana()) + "/" +
										std::to_string((player.Statistics().MainStatistics.Inteligence + newStatistics.MainStatistics.Inteligence) * 7), { 0,197,255,255 });
								}
								if (data.Arrows.at(1).second.IsMouseOn(x, y))
								{
									--pointsLeft;
									data.Mini.at(13).Texture().LoadText("Points left: " + std::to_string(pointsLeft), ProgramData::Data::NormalColor());
									++newStatistics.MainStatistics.Strenght;
									data.Stats.at(2).Texture().LoadText("Strength: " +
										std::to_string(player.Statistics().MainStatistics.Strenght + newStatistics.MainStatistics.Strenght), ProgramData::Data::NormalColor());
								}
								if (data.Arrows.at(2).second.IsMouseOn(x, y))
								{
									--pointsLeft;
									data.Mini.at(13).Texture().LoadText("Points left: " + std::to_string(pointsLeft), ProgramData::Data::NormalColor());
									++newStatistics.MainStatistics.Dextermity;
									data.Stats.at(3).Texture().LoadText("Dextermity: " +
										std::to_string(player.Statistics().MainStatistics.Dextermity + newStatistics.MainStatistics.Dextermity), ProgramData::Data::NormalColor());
								}
								if (data.Arrows.at(3).second.IsMouseOn(x, y))
								{
									--pointsLeft;
									data.Mini.at(13).Texture().LoadText("Points left: " + std::to_string(pointsLeft), ProgramData::Data::NormalColor());
									++newStatistics.Endurance;
									data.Stats.at(4).Texture().LoadText("Endurance: " +
										std::to_string(player.Statistics().Endurance + newStatistics.Endurance), ProgramData::Data::NormalColor());
								}
								if (data.Arrows.at(4).second.IsMouseOn(x, y))
								{
									--pointsLeft;
									data.Mini.at(13).Texture().LoadText("Points left: " + std::to_string(pointsLeft), ProgramData::Data::NormalColor());
									++newStatistics.Vitality;
									data.Stats.at(5).Texture().LoadText("Vitality: " +
										std::to_string(player.Statistics().Vitality + newStatistics.Vitality), ProgramData::Data::NormalColor());
									data.Stats.at(10).Texture().LoadText("Health: " + std::to_string(player.Health()) + "/" +
										std::to_string((player.Statistics().Vitality + newStatistics.Vitality) * 12), { 255,0,0,255 });
								}
								if (!pointsLeft)
									data.Mini.at(13).Texture().LoadText("Accept", ProgramData::Data::NormalColor());
							}
							else if (data.Mini.at(13).IsMouseOn(x, y))
							{
								newLevel = false;
								player.LevelUp(newStatistics);
								SetStatsLabels(player.Statistics(), player.Damage());
								data.Stats.at(10).Texture().LoadText("Health: " + std::to_string(player.Health()) + "/" +
									std::to_string(player.MaxHealth()), { 255,0,0,255 });
								data.Stats.at(11).Texture().LoadText("Mana: " + std::to_string(player.Mana()) + "/" +
									std::to_string(player.MaxMana()), { 0,197,255,255 });
								break;
							}
							if (newStatistics.MainStatistics.Inteligence && data.Arrows.at(0).first.IsMouseOn(x, y))
							{
								++pointsLeft;
								data.Mini.at(13).Texture().LoadText("Points left: " + std::to_string(pointsLeft), ProgramData::Data::NormalColor());
								--newStatistics.MainStatistics.Inteligence;
								data.Stats.at(1).Texture().LoadText("Inteligence: " +
									std::to_string(player.Statistics().MainStatistics.Inteligence + newStatistics.MainStatistics.Inteligence), ProgramData::Data::NormalColor());
								data.Stats.at(11).Texture().LoadText("Mana: " + std::to_string(player.Mana()) + "/" +
									std::to_string((player.Statistics().MainStatistics.Inteligence + newStatistics.MainStatistics.Inteligence) * 7), { 0,197,255,255 });
							}
							if (newStatistics.MainStatistics.Strenght && data.Arrows.at(1).first.IsMouseOn(x, y))
							{
								++pointsLeft;
								data.Mini.at(13).Texture().LoadText("Points left: " + std::to_string(pointsLeft), ProgramData::Data::NormalColor());
								--newStatistics.MainStatistics.Strenght;
								data.Stats.at(2).Texture().LoadText("Strength: " +
									std::to_string(player.Statistics().MainStatistics.Strenght + newStatistics.MainStatistics.Strenght), ProgramData::Data::NormalColor());
							}
							if (newStatistics.MainStatistics.Dextermity && data.Arrows.at(2).first.IsMouseOn(x, y))
							{
								++pointsLeft;
								data.Mini.at(13).Texture().LoadText("Points left: " + std::to_string(pointsLeft), ProgramData::Data::NormalColor());
								--newStatistics.MainStatistics.Dextermity;
								data.Stats.at(3).Texture().LoadText("Dextermity: " +
									std::to_string(player.Statistics().MainStatistics.Dextermity + newStatistics.MainStatistics.Dextermity), ProgramData::Data::NormalColor());
							}
							if (newStatistics.Endurance && data.Arrows.at(3).first.IsMouseOn(x, y))
							{
								++pointsLeft;
								data.Mini.at(13).Texture().LoadText("Points left: " + std::to_string(pointsLeft), ProgramData::Data::NormalColor());
								--newStatistics.Endurance;
								data.Stats.at(4).Texture().LoadText("Endurance: " +
									std::to_string(player.Statistics().Endurance + newStatistics.Endurance), ProgramData::Data::NormalColor());
							}
							if (newStatistics.Vitality && data.Arrows.at(4).first.IsMouseOn(x, y))
							{
								++pointsLeft;
								data.Mini.at(13).Texture().LoadText("Points left: " + std::to_string(pointsLeft), ProgramData::Data::NormalColor());
								--newStatistics.Vitality;
								data.Stats.at(5).Texture().LoadText("Vitality: " +
									std::to_string(player.Statistics().Vitality + newStatistics.Vitality), ProgramData::Data::NormalColor());
								data.Stats.at(10).Texture().LoadText("Health: " + std::to_string(player.Health()) + "/" +
									std::to_string((player.Statistics().Vitality + newStatistics.Vitality) * 12), { 255,0,0,255 });
							}
						}
					}
					break;
				}
				}
			}
			if (Events::EventHandler::GetKeyboard().GetKeyState(SDLK_ESCAPE))
			{
				Events::EventHandler::GetKeyboard().SetKeyState(SDLK_ESCAPE, false);
				break;
			}
		}
	}

	void Info::PlayerEquipment(Renders::Objects::Player& player)
	{
		size_t itemCardNumber = 0;
		auto& itemList = player.Items();
		auto equipmentList = player.Equipment();
		bool rightClick = false, isRing = false, isStatisticsHoover = false;
		std::string itemName = "";
		size_t itemNumber = 0;
		int X = 0, Y = 0;
		ProgramData::LabelsData& data = ProgramData::Data::GetLabels();
		while (true)
		{
			data.Mini.at(3).Render();
			if (isStatisticsHoover)
			{
				data.Mini.at(11).Render();
				data.Mini.at(10).Render();
			}
			else
			{
				data.Mini.at(10).Render();
				data.Mini.at(11).Render();
			}
			auto it = itemList.begin();
			size_t current = itemCardNumber * 42;
			for (size_t Max = itemList.size(), i = 0; i < current && current < Max; ++i)
				++it;
			for (size_t i = 0, Max = itemList.size(); i < 6 && current < Max; ++i)
				for (size_t j = 0; j < 7 && current < Max; ++j, ++current, ++it)
					it->second->Texture().Render(877 + static_cast<int>(j) * 131, 266 + static_cast<int>(i) * 125, 70, 70);
			for (auto& item : data.Items)
				item.Render();

			if (equipmentList.second.at(0) != nullptr)
				equipmentList.second.at(0)->Texture().Render(292, 516, 70, 70);
			if (equipmentList.second.at(1) != nullptr)
				equipmentList.second.at(1)->Texture().Render(292, 641, 70, 70);
			if (equipmentList.second.at(3) != nullptr)
				equipmentList.second.at(3)->Texture().Render(292, 766, 70, 70);
			if (equipmentList.second.at(6) != nullptr)
				equipmentList.second.at(6)->Texture().Render(292, 891, 70, 70);
			if (equipmentList.first != nullptr)
				equipmentList.first->Texture().Render(506, 516, 70, 70);
			if (equipmentList.second.at(2) != nullptr)
				equipmentList.second.at(2)->Texture().Render(506, 641, 70, 70);
			if (equipmentList.second.at(4) != nullptr)
				equipmentList.second.at(4)->Texture().Render(506, 766, 70, 70);
			if (equipmentList.second.at(5) != nullptr)
				equipmentList.second.at(5)->Texture().Render(506, 891, 70, 70);

			if (rightClick)
			{
				short end = 8 + (isRing ? 2 : 0);
				for (short i = 4; i < end; ++i)
					data.Mini.at(i).Render();
			}
			SDL_RenderPresent(ProgramData::Data::Renderer());

			SDL_Event e;
			while (SDL_PollEvent(&e))
			{
				switch (e.type)
				{
				case SDL_KEYDOWN:
				case SDL_KEYUP:
				{
					Events::EventHandler::GetKeyboard().HandleKey(e);
					break;
				}
				case SDL_MOUSEMOTION:
				{
					int x, y;
					bool isHoover = false;
					SDL_GetMouseState(&x, &y);
					if (data.Mini.at(10).IsMouseOn(x, y))
					{
						data.Mini.at(10).Texture().SetColor(ProgramData::Data::HOOVER_COLOR);
						isStatisticsHoover = isHoover = true;
					}
					else
					{
						data.Mini.at(10).Texture().SetColor(ProgramData::Data::NormalColor());
						isStatisticsHoover = false;
					}

					if (!isHoover && equipmentList.second.at(0) != nullptr && data.Items.at(0).IsMouseOn(x, y))
					{
						data.Items.at(0).Texture().SetColor(255, 0, 0);
						isHoover = true;
					}
					else
						data.Items.at(0).Texture().SetColor(ProgramData::Data::NormalColor());
					if (!isHoover && equipmentList.second.at(1) != nullptr && data.Items.at(1).IsMouseOn(x, y))
					{
						data.Items.at(1).Texture().SetColor(255, 0, 0);
						isHoover = true;
					}
					else
						data.Items.at(1).Texture().SetColor(ProgramData::Data::NormalColor());
					if (!isHoover && equipmentList.second.at(3) != nullptr && data.Items.at(2).IsMouseOn(x, y))
					{
						data.Items.at(2).Texture().SetColor(255, 0, 0);
						isHoover = true;
					}
					else
						data.Items.at(2).Texture().SetColor(ProgramData::Data::NormalColor());
					if (!isHoover && equipmentList.second.at(6) != nullptr && data.Items.at(3).IsMouseOn(x, y))
					{
						data.Items.at(3).Texture().SetColor(255, 0, 0);
						isHoover = true;
					}
					else
						data.Items.at(3).Texture().SetColor(ProgramData::Data::NormalColor());
					if (!isHoover && equipmentList.first != nullptr && data.Items.at(4).IsMouseOn(x, y))
					{
						data.Items.at(4).Texture().SetColor(255, 0, 0);
						isHoover = true;
					}
					else
						data.Items.at(4).Texture().SetColor(ProgramData::Data::NormalColor());
					if (!isHoover && equipmentList.second.at(2) != nullptr && data.Items.at(5).IsMouseOn(x, y))
					{
						data.Items.at(5).Texture().SetColor(255, 0, 0);
						isHoover = true;
					}
					else
						data.Items.at(5).Texture().SetColor(ProgramData::Data::NormalColor());
					if (!isHoover && equipmentList.second.at(4) != nullptr && data.Items.at(6).IsMouseOn(x, y))
					{
						data.Items.at(6).Texture().SetColor(255, 0, 0);
						isHoover = true;
					}
					else
						data.Items.at(6).Texture().SetColor(ProgramData::Data::NormalColor());
					if (!isHoover && equipmentList.second.at(5) != nullptr && data.Items.at(7).IsMouseOn(x, y))
					{
						data.Items.at(7).Texture().SetColor(255, 0, 0);
						isHoover = true;
					}
					else
						data.Items.at(7).Texture().SetColor(ProgramData::Data::NormalColor());

					if (rightClick)
					{
						if (!isHoover && data.Mini.at(5).IsMouseOn(x, y))
						{
							data.Mini.at(5).Texture().SetColor(ProgramData::Data::HOOVER_COLOR);
							isHoover = true;
						}
						else
							data.Mini.at(5).Texture().SetColor(ProgramData::Data::NormalColor());
						if (!isHoover && data.Mini.at(6).IsMouseOn(x, y))
						{
							data.Mini.at(6).Texture().SetColor(ProgramData::Data::HOOVER_COLOR);
							isHoover = true;
						}
						else
							data.Mini.at(6).Texture().SetColor(ProgramData::Data::NormalColor());
						if (isRing)
						{
							if (!isHoover && data.Mini.at(8).IsMouseOn(x, y))
							{
								data.Mini.at(8).Texture().SetColor(ProgramData::Data::HOOVER_COLOR);
								isHoover = true;
							}
							else
								data.Mini.at(8).Texture().SetColor(ProgramData::Data::NormalColor());
							if (!isHoover && data.Mini.at(9).IsMouseOn(x, y))
							{
								data.Mini.at(9).Texture().SetColor(ProgramData::Data::HOOVER_COLOR);
								isHoover = true;
							}
							else
								data.Mini.at(9).Texture().SetColor(ProgramData::Data::NormalColor());
						}
						else
						{
							if (!isHoover && data.Mini.at(7).IsMouseOn(x, y))
							{
								data.Mini.at(7).Texture().SetColor(ProgramData::Data::HOOVER_COLOR);
								isHoover = true;
							}
							else
								data.Mini.at(7).Texture().SetColor(ProgramData::Data::NormalColor());
						}
					}
					auto it = itemList.begin();
					size_t current = itemCardNumber * 42;
					for (size_t Max = itemList.size(), i = 0; i < current && current < Max; ++i)
						++it;
					for (size_t i = 0, Max = itemList.size(); i < 6 && current < Max; ++i)
						for (size_t j = 0; j < 7 && current < Max; ++j, ++current, ++it)
						{
							if (!isHoover && x >= 852 + static_cast<int>(j) * 131 && x <= 971 + static_cast<int>(j) * 131
								&& y >= 248 + static_cast<int>(i) * 125 && y <= 354 + static_cast<int>(i) * 125)
							{
								it->second->Texture().SetColor(ProgramData::Data::HOOVER_COLOR);
								isHoover = true;
							}
							else
								it->second->Texture().SetColor(ProgramData::Data::NormalColor());
						}
					break;
				}
				case SDL_MOUSEBUTTONDOWN:
				{
					SDL_GetMouseState(&X, &Y);
					if (e.button.button == SDL_BUTTON_RIGHT)
					{
						rightClick = false;
						size_t current = itemCardNumber * 42;
						auto it = itemList.begin();
						for (size_t Max = itemList.size(), i = 0; i < current && current < Max; ++i)
							++it;
						for (size_t i = 0, Max = itemList.size(); i < 6 && current < Max && !rightClick; ++i)
							for (size_t j = 0; j < 7 && current < Max; ++j, ++current, ++it)
							{
								if (X >= 852 + static_cast<int>(j) * 131 && X <= 971 + static_cast<int>(j) * 131
									&& Y >= 248 + static_cast<int>(i) * 125 && Y <= 354 + static_cast<int>(i) * 125)
								{
									itemName = it->first;
									itemNumber = current;
									rightClick = true;
									data.Mini.at(4).SetPosition(X, Y); //back
									data.Mini.at(5).SetPosition(X, Y); //info
									data.Mini.at(6).SetPosition(X, Y + 64); // throw
									data.Mini.at(7).SetPosition(X, Y + 128); //use/wear/takeoff
									data.Mini.at(8).SetPosition(X, Y + 192); //left
									data.Mini.at(9).SetPosition(X, Y + 256); //right
									switch (it->second->ItemType())
									{
									case ItemType::Armor:
										isRing = static_cast<const Renders::Items::Armor&>(*it->second).Part() == BodyPart::Ring;
									case ItemType::Weapon:
									{
										data.Mini.at(7).Texture().LoadText("Wear", ProgramData::Data::NormalColor());
										break;
									}
									default:
									{
										data.Mini.at(7).Texture().LoadText("Use", ProgramData::Data::NormalColor());
										break;
									}
									}
									break;
								}
								else
								{
									itemName = "";
									rightClick = false;
								}
							}
					}
					if (e.button.button == SDL_BUTTON_LEFT)
					{
						if (rightClick)
						{
							auto it = itemList.begin();
							for (size_t i = 0; i < itemNumber; ++i)
								++it;
							if (data.Mini.at(5).IsMouseOn(X, Y))
								ItemInfo(*it->second, player);
							else if (data.Mini.at(6).IsMouseOn(X, Y))
								player.ThrowOut(itemName);
							else if (!isRing && data.Mini.at(7).IsMouseOn(X, Y))
							{
								if (it->second->ItemType() == ItemType::Armor || it->second->ItemType() == ItemType::Weapon)
								{
									player.Wear(itemName);
									equipmentList = player.Equipment();
								}
								else
									player.Use(itemName);
							}
							else if (isRing && data.Mini.at(8).IsMouseOn(X, Y))
							{
								player.Wear(itemName, 0);
								equipmentList = player.Equipment();
							}
							else if (isRing && data.Mini.at(9).IsMouseOn(X, Y))
							{
								player.Wear(itemName, 1);
								equipmentList = player.Equipment();
							}
							isRing = rightClick = false;
						}
						if (data.Mini.at(10).IsMouseOn(X, Y))
						{
							data.Mini.at(10).Texture().SetColor(ProgramData::Data::NormalColor());
							return PlayerStatistics(player);
						}
						if (equipmentList.second.at(0) != nullptr && data.Items.at(0).IsMouseOn(X, Y))
						{
							player.TakeOff(BodyPart::Necklace);
							equipmentList = player.Equipment();
						}
						if (equipmentList.second.at(1) != nullptr && data.Items.at(1).IsMouseOn(X, Y))
						{
							player.TakeOff(BodyPart::Ring, 0);
							equipmentList = player.Equipment();
						}
						if (equipmentList.second.at(3) != nullptr && data.Items.at(2).IsMouseOn(X, Y))
						{
							player.TakeOff(BodyPart::Head);
							equipmentList = player.Equipment();
						}
						if (equipmentList.second.at(6) != nullptr && data.Items.at(3).IsMouseOn(X, Y))
						{
							player.TakeOff(BodyPart::Hands);
							equipmentList = player.Equipment();
						}
						if (equipmentList.first != nullptr && data.Items.at(4).IsMouseOn(X, Y))
						{
							player.TakeOff(BodyPart::Weapon);
							equipmentList = player.Equipment();
						}
						if (equipmentList.second.at(2) != nullptr && data.Items.at(5).IsMouseOn(X, Y))
						{
							player.TakeOff(BodyPart::Ring, 1);
							equipmentList = player.Equipment();
						}
						if (equipmentList.second.at(4) != nullptr && data.Items.at(6).IsMouseOn(X, Y))
						{
							player.TakeOff(BodyPart::Chest);
							equipmentList = player.Equipment();
						}
						if (equipmentList.second.at(5) != nullptr && data.Items.at(7).IsMouseOn(X, Y))
						{
							player.TakeOff(BodyPart::Leg);
							equipmentList = player.Equipment();
						}
					}
					break;
				}
				}
			}
			if (Events::EventHandler::GetKeyboard().GetKeyState(SDLK_ESCAPE))
			{
				Events::EventHandler::GetKeyboard().SetKeyState(SDLK_ESCAPE, false);
				break;
			}
		}
	}
}