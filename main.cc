#include <iostream>
#include <fstream>


#include "../lib_olena/olena-2.0/milena/mln/geom/all.hh"

bool DEBUG_ = false;


// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.


#include <mln/all.hh>
#include <mln/core/image/image2d.hh>
#include <mln/value/rgb8.hh>
#include <mln/data/fill.hh>
#include <mln/data/paste.hh>
#include <mln/core/routine/duplicate.hh>
#include <mln/literal/colors.hh>
#include <mln/opt/at.hh>


//#include <tests/data.hh>
//#include <doc/tools/sample_utils.hh>

int main(int argc, char** argv)
{
  using namespace mln;


  if (argc < 3)
    return 0;  // ou balancer sur "see help"

  // \{
  image2d<value::rgb8> ima(40, 40);
  // \}

  // \{
  data::fill(ima, literal::red);
  // \}

  // \{
  for (def::coord row = 20; row < 30; ++row)
    for (def::coord col = 20; col < 30; ++col)
      ima(point2d(row, col)) = literal::blue;
  // \}

  // \{
  for (def::coord row = 20; row < 30; ++row)
    for (def::coord col = 20; col < 30; ++col)
      opt::at(ima, row, col) = literal::blue;  // rajouter un if pour dessiner le rectangle :)
  // \}
  io::ppm::save(ima, argv[2]);   // path to img

  image2d<value::rgb8> ima2 = duplicate(ima);

  // \{
  image2d<value::rgb8> lena;
  io::ppm::load(lena, argv[1]); // remplacer par "/small.ppm" <-> argv[1] ...
  // \}

  // \{
  data::fill(ima, lena);
  // \}
  io::ppm::save(ima, argv[2]);

  ima = ima2;
  // \{
  data::paste(ima, lena);
  // \}
  io::ppm::save(lena, argv[2]);
}
































/*

int main (int argc, char** argv)
{
  if (argc >= 3)
  {

    std::ifstream src;
    std::ofstream dst;

    src.open(argv[1]);
    dst.open(argv[2]);


    if (!src.is_open() && !dst.is_open())
    {
      mln::image2d<value::rgb8> lena;
      io::ppm::load(lena, MLN_IMG_DIR "../Training_GT/GT_0001.png");

      box2d roi = make::box2d(20, 20, 40, 40);

      data::fill((lena | roi).rw(), literal::green);


    }
  }

}
*/
