/*  LOOT

    A load order optimisation tool for Oblivion, Skyrim, Fallout 3 and
    Fallout: New Vegas.

    Copyright (C) 2017 WrinklyNinja

    This file is part of the LOOT metadata validator.

    LOOT is free software: you can redistribute
    it and/or modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.

    LOOT is distributed in the hope that it will
    be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with LOOT.  If not, see
    <https://www.gnu.org/licenses/>.
    */

#include "gui/version.h"

namespace loot {
namespace gui {
const unsigned int Version::major = 0;
const unsigned int Version::minor = 16;
const unsigned int Version::patch = 0;
const std::string Version::revision = "b7ab704";

std::string Version::string() {
  return std::to_string(major) + '.' + std::to_string(minor) + '.' + std::to_string(patch);
}
}
}
