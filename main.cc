#include "tab_bounds.hh" // the structure containing the relative coordinates for each tab
#include <vector>
#include <map>

#include <mln/core/image/image2d.hh>
#include <mln/labeling/blobs.hh>
#include <mln/labeling/colorize.hh>
#include <mln/value/rgb8.hh>
#include <mln/io/ppm/save.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/ppm/load.hh>
#include <mln/make/neighb2d.hh>
#include <mln/make/double_neighb2d.hh>
#include <mln/literal/colors.hh>

template <typename N>
void labelize(const mln::image2d<bool>& pic,
              const N& nbh,
              const std::string& filename)
{
  using namespace mln;
  using value::rgb8;
  unsigned n;
  image2d<unsigned> lab = labeling::blobs(pic, nbh, n);
  image2d<rgb8> out = labeling::colorize(rgb8(), lab, n);
  io::ppm::save(out, filename);
}



int main(int argc, char* argv[]) // works for pbm as input at the moment and ppm as output
{
  using namespace mln;

  if (argc < 3)
  {
    std::cout << "See the readme for the usage";
    exit(0);
  }

  image2d<bool> img;
  mln::image2d<value::rgb8> out;
  int color = 0;


  // 1 object <-> 1 color (represented by an int)
  /*
     this map does the link between colored objects and their own tab in which
     is stored the object
   */
  std::map<int, std::vector<std::vector<int> > > c_tab;


  // link between colored objects and their relative position (coordonates)
  std::map<int, std::vector<std::vector<s_tab_bounds> > > coordonate_tab;



  // if ppm -> io::ppm::load, pbm -> io::pbm::load... etc

  io::pbm::load(img, argv[1]);

  labelize(img, c8(), argv[2]);

  // End of phase 1 (the detection of all objects)



  io::ppm::load(out, argv[2]);


  // Détecter les x et y de chaque tab, initialiser les maps

  for (def::coord row = geom::min_row(out); row < geom::max_row(out); ++row)
    for (def::coord col = geom::min_row(out); col < geom::max_col(out); ++col)
      {
	// if nouvelle couleur ...

      }





  io::ppm::save(out, argv[2]);

}
