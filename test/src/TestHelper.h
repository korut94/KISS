#pragma once

#include "TestHelper/EntitySet/CountMatcher.h"
#include "TestHelper/EntitySet/HasEntitiesMatcher.h"
#include "TestHelper/EntitySet/HasIntervalsMatcher.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

class KC_EntitySet;
void PrintTo(const KC_EntitySet& anEntitySet, std::ostream* os);
