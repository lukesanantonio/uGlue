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
#include "server/collision_util.h"
#include "gtest/gtest.h"

TEST(CollisionUtilTest, isIn)
{
  EXPECT_EQ(true, pong::isIn(0, 5, 5));
  EXPECT_EQ(true, pong::isIn(-7, 16, 6));
  EXPECT_EQ(true, pong::isIn(-100, 100, -100));
  EXPECT_EQ(false, pong::isIn(100, 560, 99));
  EXPECT_EQ(false, pong::isIn(100, 675, 691));
  EXPECT_EQ(false, pong::isIn(100, 200, -150));
}
TEST(CollisionUtilTest, isIntersecting)
{
  pong::Volume vol1, vol2;
  vol1.pos = {500, 510};
  vol1.width = 61;
  vol1.height = 55;

  vol2.pos = {560, 564};
  vol2.width = 50;
  vol2.height = 50;

  EXPECT_EQ(true, isIntersecting(vol1, vol2));

  vol1.width = 60;
  EXPECT_EQ(false, isIntersecting(vol1, vol2));
}
TEST(CollisionUtilTest, raytrace)
{
  namespace math = pong::math;
  math::vector<double> vec(3, 0);

  std::vector<math::vector<double> > points = pong::raytrace(vec);
  std::vector<math::vector<double> > expected = {{0,0}, {1,0}, {2,0}, {3,0}};
  EXPECT_EQ(expected, points);

  vec = {3, 3};
  points = pong::raytrace(vec);
  expected = {{0,0}, {1,1}, {2,2}, {3,3}};
  EXPECT_EQ(expected, points);

  points = pong::raytrace({500,620});
  math::vector<double> expected_point = {500,620};
  EXPECT_EQ(expected_point, points.back());
}
TEST(CollisionUtilTest, findClosestSide)
{
  using pong::Volume; using pong::VolumeSide; using pong::findClosestSide;
  Volume vol1, vol2;
  vol1.pos = {0, 0};
  vol1.width = 5; vol1.height = 5;

  vol2.pos = {4, 0};
  vol2.width = 5; vol2.height = 5;

  EXPECT_EQ(VolumeSide::Right, findClosestSide(vol1, vol2));

  vol1.pos = {500, 500};
  vol1.width = 10; vol1.height = 10;

  vol2.pos = {450, 500};
  vol2.width = 51; vol2.height = 51;

  EXPECT_EQ(VolumeSide::Left, findClosestSide(vol1, vol2));

  vol2.pos = {500, 450};
  EXPECT_EQ(VolumeSide::Top, findClosestSide(vol1, vol2));

  vol2.pos = {500, 509};
  EXPECT_EQ(VolumeSide::Bottom, findClosestSide(vol1, vol2));

  vol1.height = 9;
  EXPECT_EQ(VolumeSide::None, findClosestSide(vol1, vol2));
}
TEST(CollisionUtilTest, isInsideVolume)
{
  using pong::Volume; using pong::isInsideVolume;
  Volume bounds = {{0, 0}, 1000, 1000};
  Volume check = {{0, 0}, 1000, 1000};

  EXPECT_TRUE(isInsideVolume(bounds, check));

  check.pos = {500, 500};
  check.width = 500; check.height = 500;
  EXPECT_TRUE(isInsideVolume(bounds, check));

  check.pos = {-1, 0};
  EXPECT_FALSE(isInsideVolume(bounds, check));
}
TEST(CollisionUtilTest, closestSideFromInside)
{
  using pong::Volume; using pong::closestSideFromInside;
  using pong::VolumeSide;

  Volume bounds = {{0, 0}, 1000, 1000};
  Volume check = {{0, 500}, 20, 20};

  EXPECT_EQ(VolumeSide::Left, closestSideFromInside(bounds, check));

  //                                .- Next best choice.
  //##################################  .- Intersects, so the side isn't used.
  //#                        ###############
  //#                        #       #     #
  //#                        ###############
  //#                                #
  //##################################

  check.pos = {750, 200};
  check.width = 500;
  check.height = 20;
  EXPECT_EQ(VolumeSide::Top, closestSideFromInside(bounds, check));
}
TEST(CollisionUtilTest, snapVolumeToVolume)
{
  using pong::Volume; using pong::snapVolumeToVolume; using pong::VolumeSide;

  Volume v{{0, 0}, 5, 5};
  Volume anchor{{500, 500}, 50, 50};

  snapVolumeToVolume(v, VolumeSide::Top, anchor);
  Volume expected{{0, 495}, 5, 5};
  EXPECT_EQ(expected, v);

  snapVolumeToVolume(v, VolumeSide::Bottom, anchor);
  expected.pos.y = 550;
  EXPECT_EQ(expected, v);

  snapVolumeToVolume(v, VolumeSide::Left, anchor);
  expected.pos.x = 495;
  EXPECT_EQ(expected, v);

  snapVolumeToVolume(v, VolumeSide::Right, anchor);
  expected.pos.x = 550;
  EXPECT_EQ(expected, v);
}
