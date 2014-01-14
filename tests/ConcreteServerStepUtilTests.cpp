/*
 * PpM - Pong Plus More - A pong clone full of surprises written with C++11.
 * Copyright (C) 2013  Luke San Antonio
 *
 * You can contact me (Luke San Antonio) at lukesanantonio@gmail.com!
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "gtest/gtest.h"
#include "server/collision_util.h"

TEST(ConcreteServerStepUtilTest, isInTest)
{
  EXPECT_EQ(true, pong::isIn(5, 1362462, 1282522));
  EXPECT_EQ(true, pong::isIn(1362462, 5, 1282522));

  EXPECT_EQ(false, pong::isIn(2, 10, 1));
  EXPECT_EQ(false, pong::isIn(10, 2, 1));

  //isIn is less-than-or-equal-to, test that.
  EXPECT_EQ(true, pong::isIn(5, 18, 18));
  EXPECT_EQ(true, pong::isIn(5, 18, 5));
}
TEST(ConcreteServerStepUtilTest, cornerCollision)
{
  //TODO Move to the generic test file. Add more tests.
  pong::Paddle p{1, {0, 0}, 50, 50};
  pong::Ball b{1, {50, 50}, {0, 0}, 5};

  EXPECT_EQ(pong::CollisionSide::BottomRightCorner, paddleIsBesideBall(p, b));

  ++b.pos.x; ++b.pos.y;

  EXPECT_EQ(pong::CollisionSide::None, paddleIsBesideBall(p, b));

  p.pos.x = 50;
  p.pos.y = 50;
  b.pos.x = 45;
  b.pos.y = 45;

  EXPECT_EQ(pong::CollisionSide::TopLeftCorner, paddleIsBesideBall(p, b));
}
