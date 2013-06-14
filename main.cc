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



// Typedefs





// Overloads (for the map)
/*bool operator<(const Tab_bounds& first, const Tab_bounds& second)
{
  return first.get_x() < second.get_x();
}
*/

bool operator<(Rgb first, Rgb second)
{
  return first.get_red() < second.get_red();
}


bool operator<(int first, Tab_bounds second)
{
  return first < second.get_x();
}


// Forward declarations
bool is_known (std::vector<Rgb> colors, Rgb px);
bool is_black(Rgb px);


void print_first_colored_obj(std::vector<Rgb> colors, std::map<Rgb, std::vector<std::vector<int> > > c_tab)
{
  for (int i = 0; i < c_tab[colors[0]].size(); ++i)
  {
    for (int j = 0; j < c_tab[colors[0]][0].size(); ++j)
      if (c_tab[colors[0]][0][j] == 1)
	std::cout << "*";
    std::cout << std::endl;
  }
}


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
  int x,y; // the x : col, the y : line

  /*
    ------------> x
    :
    :
    :
    y

  */


  std::vector<Rgb> colors; // vector of known colors
  Rgb* current_color; // pointer to value of the current pixel


  current_color = new Rgb(0, 0, 0);
  //                      r  g  b



  // 1 object <-> 1 color (represented by an int)
  /*
     this map does the link between colored objects and their own tab in which
     is stored the object
   */
  std::map<Rgb, std::vector<std::vector<int> > > c_tab;


  // link between colored objects and their relative position (coordonates)
  std::map<const Rgb, Tab_bounds> coordonate_tab;



  // if ppm -> io::ppm::load, pbm -> io::pbm::load... etc

  io::pbm::load(img, argv[1]);

  labelize(img, c8(), argv[2]); // saves also the img


  std::cout << "1" << std::endl;

  // End of phase 1 (the detection of all objects)



  io::ppm::load(out, argv[2]);

  std::cout << "2" << std::endl;


  // Détecter les x et y de chaque tab, initialiser les maps

  std::cout << "Nouvelles couleurs détectées (on montre uniquement leur niveau de rouge)" << std::endl;

  int nb_colors = 0; // = nb objects recognized

  for (def::coord row = geom::min_row(out); row < geom::max_row(out); ++row)
    for (def::coord col = geom::min_col(out); col < geom::max_col(out); ++col)
    {
      // if couleur du px n'est pas présent dans le vecteur de couleurs
      //	if (opt::at(out, row, col).red() < 100 && opt::at(out, row, col).red() != 0)
      //  opt::at(out, row, col) = literal::white;

      (*current_color).set_rgb(opt::at(out, row, col).red(), opt::at(out, row, col).green(), opt::at(out, row, col).blue());

      if (!is_known(colors, *current_color) && !is_black(*current_color))
      {

	nb_colors++;
	std::cout << (*current_color).get_red() << "; ";

	Rgb* adding_rgb = new Rgb((*current_color).get_red(), (*current_color).get_green(), (*current_color).get_blue());
	std::vector<std::vector<int> > new_tab;


	new_tab.resize(80);

	for (int i = 0; i < new_tab.size(); ++i)
	  new_tab[i].resize(100);                      // + init to 0 values ?



	c_tab.insert(std::pair<Rgb, std::vector<std::vector<int> > >
		     (*adding_rgb, new_tab));
	// We create a tab which will contain the item to make it easier to manipulate (recognizing the shape of a quarter note)



	colors.push_back(*adding_rgb); // color (corresponding to a new item) added to the list




	Tab_bounds adding_it_bis;

	adding_it_bis.set_y(row);


	coordonate_tab.insert(std::pair<Rgb, Tab_bounds>
			      (*adding_rgb, adding_it_bis));

      }

  }

  std::cout << "3 (nb_colors = " << nb_colors << ")" << std::endl;

  // Here we have our y and initialised all stuffs
  // Now we must define our x for each tab

  x = 0; // col
  y = 0; // line

  for (def::coord col = geom::max_col(out); col < geom::min_col(out); --col)
  {
    for (def::coord row = geom::min_row(out); row < geom::max_row(out); ++row)
    {
      // set the current_color to current pixel color
      (*current_color).set_rgb(opt::at(out, row, col).red(), opt::at(out, row, col).green(), opt::at(out, row, col).blue());


      int a = coordonate_tab[*current_color].get_x();

      // pour virer le test si on est sur du noir && que le x est le plus petit pour cette couleur
      if (is_known(colors, *current_color) && col > a)
	coordonate_tab[*current_color].set_x(col);
    }
  }


  std::cout << "4 (got x now)" << std::endl;



      /*      Rgb* treating_it = new Rgb((*current_color).get_red(), (*current_color).get_green(), (*current_color).get_blue());


     // std::map<Rgb, Tab_bounds> coordonate_tab;



      if (is_known(colors, *treating_it))
      {
	const Rgb a = *treating_it;

	coordonate_tab[a] = col;

	Tab_bounds tmp = coordonate_tab.find(a);

	tmp.set_x(col);

	  //	(coordonate_tab[a]).set_x(col);

	//	coordonate_tab.at(a).set_x(col); //c++ only !
      }

    }
      */

  // Here x and y are well set for each tab


  // ok here we go


  // Initialization of each tab -> maybe


  for (def::coord row = geom::min_row(out); row < geom::max_row(out); ++row)
    for (def::coord col = geom::min_col(out); col < geom::max_col(out); ++col)
    {
      // set the current_color to current pixel color
      (*current_color).set_rgb(opt::at(out, row, col).red(), opt::at(out, row, col).green(), opt::at(out, row, col).blue());


      // pour virer le test si on est sur du noir && que le x est le plus petit pour cette couleur
      if (is_known(colors, *current_color) && !is_black(*current_color))
      {

	//	std::map<Rgb, std::vector<std::vector<int> > > c_tab;

	int a = coordonate_tab[*current_color].get_x();
	int b = coordonate_tab[*current_color].get_y();

	if (col - a > c_tab[*current_color].size())
	  c_tab[*current_color].resize(c_tab[*current_color].size() + col - a);

	if (row - b > c_tab[*current_color][0].size())
	  for (int i = 0; i < c_tab[*current_color].size(); ++i)
	    c_tab[*current_color][i].resize(c_tab[*current_color][i].size() + row - b);


	(c_tab[*current_color])[col - a][row - b] = 1;
	coordonate_tab[*current_color].set_x(col);
      }

      // FIXME SEGFAULT bizarre

    }


  std::cout << "5 Segfault ?" << std::endl;



  io::ppm::save(out, argv[2]);


  print_first_colored_obj(colors, c_tab);

  std::cout << "Segfault !" << std::endl;

}


bool is_known (std::vector<Rgb> colors, Rgb px)
{
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


bool is_black(Rgb px)
{
  if (px.get_red() == 0 && px.get_green() == 0 && px.get_blue() == 0)
    return true;
  return false;
}
