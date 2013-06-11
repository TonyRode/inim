#include "tab_bounds.hh" // the structure containing the relative coordinates for each tab
#include "rgb.hh"
#include <vector>
#include <map>

#include <mln/core/image/image2d.hh>
#include <mln/labeling/blobs.hh>
#include <mln/labeling/colorize.hh>
#include <mln/value/rgb8.hh>
#include <mln/io/ppm/save.hh>

//#include <ntg/int.hh>
//#include <ntg/color/rgb.hh>


#include <mln/io/pbm/load.hh>
#include <mln/io/ppm/load.hh>
#include <mln/make/neighb2d.hh>
#include <mln/make/double_neighb2d.hh>
#include <mln/literal/colors.hh>



bool is_known (std::vector<Rgb> colors, Rgb px);

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
  mln::image2d<value::rgb8> out;  // value::rgb8

  std::vector<Rgb> colors; // vector of known colors
  Rgb* current_color; // value of the current pixel


  current_color = new Rgb();

  // 1 object <-> 1 color (represented by an int)
  /*
     this map does the link between colored objects and their own tab in which
     is stored the object
   */
  std::map<Rgb*, std::vector<std::vector<int> > > c_tab;


  // link between colored objects and their relative position (coordonates)
  std::map<Rgb*, s_tab_bounds> coordonate_tab;



  // if ppm -> io::ppm::load, pbm -> io::pbm::load... etc

  io::pbm::load(img, argv[1]);

  labelize(img, c8(), argv[2]);

  // End of phase 1 (the detection of all objects)



  io::ppm::load(out, argv[2]);

  // Détecter les x et y de chaque tab, initialiser les maps

  for (def::coord row = geom::min_row(out); row < geom::max_row(out); ++row)
    for (def::coord col = geom::min_col(out); col < geom::max_col(out); ++col)
    {
      // if couleur du px n'est pas présent dans le vecteur de couleurs
      //	if (opt::at(out, row, col).red() < 100 && opt::at(out, row, col).red() != 0)
      //  opt::at(out, row, col) = literal::white;

      (*current_color).set_rgb(opt::at(out, row, col).red(), opt::at(out, row, col).green(), opt::at(out, row, col).blue());

      if (!is_known(colors, *current_color))
      {
	Rgb* adding_it = new Rgb((*current_color).get_red(), (*current_color).get_green(), (*current_color).get_blue());
	std::vector<std::vector<int> > new_tab;


	new_tab.resize(80);

	for (int i = 0; i < new_tab.size(); ++i)
	  new_tab[i].resize(100);



	colors.push_back(*adding_it); // color (corresponding to a new item) added to the list

	const Rgb b = (*adding_it).getinstance();

	c_tab.insert(std::pair<Rgb, std::vector<std::vector<int> > >
		     (b, new_tab));
	// We create a tab which will contain the item to make it easier to manipulate (recognizing the shape of a quarter note)


	s_tab_bounds adding_it_bis;

	adding_it_bis.y = row;

	coordonate_tab.insert(new std::pair<Rgb*, s_tab_bounds>
			      ((*adding_it.getinstance()), adding_it_bis));

      }
  }


  // Here we have our y and initialised all stuffs


  for (def::coord col = geom::max_col(out); col > geom::min_col(out); --col)
    for (def::coord row = geom::min_row(out); row < geom::max_row(out); ++row)
    {
      (*current_color).set_rgb(opt::at(out, row, col).red(), opt::at(out, row, col).green(), opt::at(out, row, col).blue());

      const Rgb* treating_it = new Rgb((*current_color).get_red(), (*current_color).get_green(), (*current_color).get_blue());


      if (is_known(colors, *treating_it))
      {
	(*coordonate_tab.at(*treating_it)).x = col;
      }
    }

  // Here x and y are well set for each tab






  io::ppm::save(out, argv[2]);

}


bool is_known (std::vector<Rgb> colors, Rgb px)
{
  if (!px.get_red() && !px.get_green() && !px.get_blue())
    return false;

  //  for (auto a = colors.begin (); a != colors.end (); ++a)

  for (std::vector<Rgb>::iterator a = colors.begin(); a != colors.end(); ++a)
  {
    if ((*a).get_red() == px.get_red()
	&& (*a).get_green() == px.get_green()
	&& (*a).get_blue() == px.get_blue())
      return true;
  }

  return false;

}

