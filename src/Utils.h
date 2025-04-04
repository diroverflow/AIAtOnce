/*
 * Copyright (C) 2025 https://github.com/diroverflow
 *
 * This is free software. You can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 3 as published by the Free Software Foundation.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY.
 */

#pragma once

#include <string>

#define UNUSED(x) static_cast<void>(x)

std::string Sprintf(const char* fmt, ...);