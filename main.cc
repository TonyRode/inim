#include "tab_bounds.hh" // the structure containing the relative coordinates for each tab
#include "rgb.hh"
#include <vector>
#include <map>
#include <iostream>
#include <sstream>


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
typedef std::map<Rgb, std::vector<std::vector<int> > > map_rgb_tab;
typedef std::map<Rgb, Tab_bounds> map_rgb_coo;



// Overloads (for the map)
/*bool operator<(const Tab_bounds& first, const Tab_bounds& second)
{
  return first.get_x() < second.get_x();
}
*/

bool operator<(Rgb first, Rgb second)
{
  /*
  if (first.get_red() == second.get_red())
  {
    if (first.get_green() == second.get_green())
    {
      if (first.get_blue() == second.get_blue())
	return true;
      else
	return first.get_blue() < second.get_blue();
    }
    else
      return first.get_green() < second.get_green();

  }
  else
    return first.get_red() < second.get_red();
  */


  return first.get_red() < second.get_red()
    || first.get_red() <= second.get_red() && first.get_green() < second.get_green()
    || first.get_red() <= second.get_red() && first.get_green() <= second.get_green()
                                           && first.get_blue() < second.get_blue();
}


bool operator<(int first, Tab_bounds second)
{
  return first < second.get_x();
}


bool operator==(Rgb first, Rgb second)
{
  return (first.get_red() == second.get_red() && first.get_green() == second.get_green() && first.get_blue() == second.get_blue());
}





// Forward declarations
bool is_known (std::vector<Rgb> colors, Rgb px);
bool is_black(Rgb px);
bool is_like_quarter_note(std::vector<std::vector<int> > obj, unsigned int radius);
void print_first_colored_obj(std::vector<Rgb> colors, std::map<Rgb, std::vector<std::vector<int> > > c_tab);


// à vérifier


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
  int x, y; // the x : col, the y : line

  /*
    ------------> x
    :
    :
    :
    y

  */


  std::vector<Rgb> colors; // vector of known colors
  std::vector<Rgb> colors_of_notes; // vector of recognized notes colors
  Rgb* current_color; // pointer to value of the current pixel
  current_color = new Rgb(0, 0, 0);
  //                      r  g  b


  // test part
  Rgb first_item_color = *(new Rgb(220, 220, 109));
  // end test part

  unsigned int radius = 5;   // accuracy of the detection, 5px by default for notes


  // 1 object <-> 1 color (represented by an int)
  /*
     this map does the link between colored objects and their own tab in which
     is stored the object
   */
  std::map<Rgb, std::vector<std::vector<int> > > c_tab;

  // link between colored objects and their relative position (coordonates)
  std::map<Rgb, Tab_bounds> coordonate_tab;

  // 1 color <-> 1 int (in order of discovering, begins at 1)
  std::map<int, Rgb> int_rgb;



  std::stringstream tmp;

  if (argc == 4)
  {
    tmp << argv[3];
    tmp >> radius;
  }



  // if ppm -> io::ppm::load, pbm -> io::pbm::load... etc

  io::pbm::load(img, argv[1]);

  labelize(img, c8(), argv[2]); // saves also the img


  std::cout << "Phase 1 terminated, the input image has been labelized in the output." << std::endl;

  // End of phase 1 (the detection of all objects)



  io::ppm::load(out, argv[2]);



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


	// adding the correspondance between the number of the object and its color
	//	int_rgb.insert(std::pair<int, Rgb> (nb_colors, *adding_rgb));


	new_tab.resize(300);

	for (int i = 0; i < new_tab.size(); ++i)
	  new_tab[i].resize(150);                      // + init to 0 values ?



	c_tab.insert(std::pair<Rgb, std::vector<std::vector<int> > >
		     (*adding_rgb, new_tab));
	// We create a tab which will contain the item to make it easier to manipulate (recognizing the shape of a quarter note)



	colors.push_back(*adding_rgb); // color (corresponding to a new item) added to the list




	Tab_bounds adding_it_bis = *(new Tab_bounds()); // on doit surement faire un new ! sinon la mémoire n'est pas allouée et ça fait de la merde

	adding_it_bis.set_y(row);
	//	adding_it_bis.set_x(1234567);


	coordonate_tab.insert(std::pair<Rgb, Tab_bounds>
			      (*adding_rgb, adding_it_bis));

      }

  }


  std::cout << std::endl << "3 (nb_colors = " << nb_colors << ")" << std::endl;

  map_rgb_coo::iterator it;

  it = coordonate_tab.find(colors[0]);

  std::cout << "y of the color 0 : " << (it->second).get_y() << std::endl;

  // Here we have our y and initialised all stuffs
  // Now we must define our x for each tab


  // So first we initialize it with the max values for the x (ie the width of our image)
  // (the y is directly set with the right value so we don't need to initialize y)

  for (std::vector<Rgb>::iterator it_rgb = colors.begin(); it_rgb != colors.end(); ++it_rgb)
    coordonate_tab.find(*it_rgb)->second.set_x(geom::max_col(out));



  x = 0; // col
  y = 0; // line
  int current_x = 0;



  // BEGIN Solution numéro 3
  /*
    On va tester de faire ça autrement, parcourir notre img colonne par colonne en partant de la gauche, premiere fois qu'on rencontre un pixel d'une couleur, on set le x et on passe le booléen "first_time" à faux pour ne pas le rechanger.
  */

  /*
  for (def::coord col = geom::min_col(out); col < geom::max_col(out); ++col)  // changement de sens
    for (def::coord row = geom::min_row(out); row < geom::max_row(out); ++row)
    {
      (*current_color).set_rgb(opt::at(out, row, col).red(), opt::at(out, row, col).green(), opt::at(out, row, col).blue());

      if (!is_black(*current_color) && coordonate_tab[*current_color].first_exchange())
      {
	coordonate_tab[*current_color].set_x(col);
	coordonate_tab[*current_color].turn_off_ex();
      }
    }
  */

  // END Solution numéro 3





  // BEGIN Solution numéro 2


  for (def::coord row = geom::min_row(out); row < geom::max_row(out); ++row)
  {
    for (def::coord col = geom::min_col(out); col < geom::max_col(out); ++col)  // changement de sens
    {
      // set the current_color to current pixel color
      (*current_color).set_rgb(opt::at(out, row, col).red(), opt::at(out, row, col).green(), opt::at(out, row, col).blue());


      // pour virer le test si on est sur du noir && que le x est le plus petit pour cette couleur
      if (!is_black(*current_color))// && is_known(colors, *current_color))
      {
	current_x = (coordonate_tab.find(*current_color)->second).get_x();

	/*
	  ptet qu'il y a une couleur qui se rapproche très fortement du noir,
	  sans l'être, donc on a l'impression que c'est du noir mais si ça se
	  trouve c'est un pixel à (0, 0, 1)  (r, g, b) par exemple.   --> NON, après avoir regardé un des pixels qui font bugger, il est à (0,0,0)
	*/


	if ((coordonate_tab.find(*current_color)->second).first_exchange())
	{
	  (coordonate_tab.find(*current_color)->second).set_x(col);
	  (coordonate_tab.find(*current_color)->second).turn_off_ex();
	}
	else
	{
	  if (col < current_x) //&& col + 30 > current_x) // 30 de différence max avec l'ancien x de l'objet
	    (coordonate_tab.find(*current_color)->second).set_x(col);  // set le x du tableau correspondant au minimum
	}


	/*
	if ((*current_color).get_red() == 220 && (*current_color).get_green() == 220 && (*current_color).get_blue() == 109) // si on est sur le 1er objet recontré
	{
	  std::cout << "Colonne actuelle (sur le 1er objet) : " << col << "             current get_x() : " << (coordonate_tab.find(*current_color)->second).get_x() << std::endl;
	  std::cout << "Ligne actuelle (sur le 1er objet) : " << row << std::endl;
	}
	*/

	/*
	if (coordonate_tab[first_item_color].get_x() <= 1160)
	std::cout << "x of the first item colored Bugged row " << row << ", col " << col << std::endl;
	*/
      }

    }
  }


  // END Solution numéro 2


  std::cout << "4 (got x now), width of the image : " << geom::max_col(out) << std::endl;


  std::cout << "Red proportion of the first obj (220 expected) " << colors[0].get_red() << std::endl;
  std::cout << "x of the first colored obj (should be 1161) " << (coordonate_tab.find(colors[0])->second).get_x() << std::endl;
  std::cout << "y of the first colored obj (should be 205) " << (coordonate_tab.find(colors[0])->second).get_y() << std::endl;


  // à moins que les rgb dans le vector colors soient triés (par valeur du red ?)


  /*
    --------> x
    -
    -
    -
    -
    -
    y



    ------------- first obj here (real supposed x in the img : 1161 , y: 205  -------------------
    ----------------------------------------------------------------------------------------
    ----------------------------------------------------------------------------------------
    ----------------------------------------------------------------------------------------
    ----------------------------------------------------------------------------------------
    ----------------------------------------------------------------------------------------
    ----------------------------------------------------------------------------------------
    ----------------------------------------------------------------------------------------
    ----------------------------------------------------------------------------------------

  */



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


  // Initialization of each tab (full of 0)
  for (std::vector<Rgb>::iterator a = colors.begin(); a != colors.end(); ++a)
    for (int i = 0; i < (c_tab.find(*a)->second).size(); i++)
      for (int j = 0; j < (c_tab.find(*a)->second)[i].size(); ++j)
	(c_tab.find(*a)->second)[i][j] = 0;







  Rgb bugged_color = *(new Rgb(129, 216, 143));




  // now we must put as many 1 as needed in each tab corresponding to right color at the right place and resize if necessary
  for (def::coord row = geom::min_row(out); /*row < 470 &&*/ row < geom::max_row(out); ++row)
    for (def::coord col = geom::min_col(out); col < geom::max_col(out); ++col) // $$$$  BE CAREFUL !! ENLEVER LE row < 470 (c'était pour éviter le segfault)
    {
      // set the current_color to current pixel color
      (*current_color).set_rgb(opt::at(out, row, col).red(), opt::at(out, row, col).green(), opt::at(out, row, col).blue());


      // pour virer le test si on est sur du noir && que le x est le plus petit pour cette couleur
      if (!is_black(*current_color)) // && is_known(colors, *current_color))
      {

	//	std::map<Rgb, std::vector<std::vector<int> > > c_tab;

	int a = (coordonate_tab.find(*current_color)->second).get_x();
	int b = (coordonate_tab.find(*current_color)->second).get_y();

	int vector_x_size = (c_tab.find(*current_color)->second).size();
	int vector_y_size = (c_tab.find(*current_color)->second)[0].size();

	if (col - a >= vector_x_size)
	  (c_tab.find(*current_color)->second).resize(vector_x_size + col - a + 1);


	// $$

	if (row - b >= vector_y_size)
	{
	  (c_tab.find(*current_color)->second)[0].resize((c_tab.find(*current_color)->second)[0].size() + row - b + 1); // init auto à 0
	}
	  for (int i = 1; i < (c_tab.find(*current_color)->second).size(); ++i)
	    (c_tab.find(*current_color)->second)[i].resize((c_tab.find(*current_color)->second)[0].size());
	  //}




	if (*current_color == bugged_color)
	  {
	    std::cout << "Col : " << col << "   Line : " << row << std::endl;
	    std::cout << "Trying to access a la case ([" << col - a << "][" << row - b << "] du tableau d'une couleur" << std::endl;
	  }
	(c_tab.find(*current_color)->second)[col - a][row - b] = 1;
      }
      // couleur du PIXEL qui segfault : 129, 216, 143 à l'endroit 571, 453
      // Re seg fault, même couleur, pixel 72 (relativement)

      // FIXME SEGFAULT bizarre

    }


  if (radius == 5)
    std::cout << "Ouiiiiiiiiiiiiiiiiiiiiiii   radius = 5 !" << std::endl;


  // $$$

  // Call of the function of recognizing objects likely a quarter note, on every tab

  map_rgb_tab::iterator tab_it;

  int nb_color2 = 0;

  for (tab_it = c_tab.begin(); tab_it != c_tab.end(); ++tab_it)
    if (is_like_quarter_note(tab_it->second, radius))
    {
      colors_of_notes.push_back(tab_it->first);
      ++nb_color2;
    }

  std::cout << "nb colors2 : " << nb_color2 << std::endl;


    // l'ajouter dans colors_of_notes


    // typedef std::map<Rgb, std::vector<std::vector<int> > > map_rgb_tab;



  // now we go through each pixel of the image to set it either back to white, either red
  for (def::coord row = geom::min_row(out); row < geom::max_row(out); ++row)
    for (def::coord col = geom::min_col(out); col < geom::max_col(out); ++col)
    {
      (*current_color).set_rgb(opt::at(out, row, col).red(), opt::at(out, row, col).green(), opt::at(out, row, col).blue());

      if (!is_black(*current_color))
      {
	if (is_known(colors_of_notes, *current_color))
	  opt::at(out, row, col) = literal::red;
	else
	  opt::at(out, row, col) = literal::white;
      }
    }




  io::ppm::save(out, argv[2]);


  // on peut sauvegarder les couleurs correspondantes aux notes reconnues, comme ça au moment de repasser sur l'output pour coloriser les notes blanc ou en rouge, dès qu'on tombe sur un pixel de couleur connue dans le vecteur de couleurs OK, on le passe en rouge, sinon blanc

  //  print_first_colored_obj(colors, c_tab);

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


bool is_like_quarter_note(std::vector<std::vector<int> > obj, unsigned int radius)
{
  unsigned int x, y, max_y, r_x, r_y;
  bool still_possible1 = true;

  max_y = 0;
  r_x = 0;
  r_y = 0;



  // pour des raisons d'opti, on va check que les pixels à la périphérie du "cercle" plutôt ovale

  for (y = 0; y < obj[0].size(); ++y)
    for (x = 0; x < obj.size(); ++x)
    {
      // Criteres : une sorte de bouboule


      // on vérifie qu'on est dans le cadre
      if (x > radius && (x + radius) < obj.size()
	  && y > radius && y + radius < obj[0].size())
      {
	// begins with top, 1ere moitié droite, sens inverse trigo
	for (r_x = x, r_y = y - radius; r_x <= x + radius / 2; ++r_x)
	  if (obj[r_x][r_y] != 1)
	    still_possible1 = false;

	for (; still_possible1 && r_x <= x + radius; ++r_x, ++r_y)
	  if (obj[r_x][r_y] != 1)
	    still_possible1 = false;

	for (; still_possible1 && r_y <= y + radius / 2; ++r_y)
	  if (obj[r_x][r_y] != 1)
	    still_possible1 = false;

	for (; still_possible1 && r_y <= y + radius; --r_x, ++r_y)
	  if (obj[r_x][r_y] != 1)
	    still_possible1 = false;


	// 1ere moitié de bouboule finie d'examiner, go continuer avec la 2eme
	for (; still_possible1 && r_x >= x - radius / 2; --r_x)
	  if (obj[r_x][r_y] != 1)
	    still_possible1 = false;


	for (; still_possible1 && r_x >= x - radius; --r_x, --r_y)
	  if (obj[r_x][r_y] != 1)
	    still_possible1 = false;


	for (; still_possible1 && r_y >= y - radius / 2; --r_y)
	  if (obj[r_x][r_y] != 1)
	    still_possible1 = false;


	for (; still_possible1 && r_x <= x - radius / 2; ++r_x, --r_y)
	  if (obj[r_x][r_y] != 1)
	    still_possible1 = false;


	for (; still_possible1 && r_x <= x; ++r_x)
	  if (obj[r_x][r_y] != 1)
	    still_possible1 = false;


	if (still_possible1)
	  return true;

      }



      if (obj[x][y] == 1 && y > max_y)
	max_y = y;

      // ET un objet en forme de batonnet a peu pres vertical
      /*      if (max_y >= 4 * radius)
	still_possible = true;
      */

      // à améliorer !
      if (still_possible1)
	{
	  //...

	}


      still_possible1 = true;

    }





  return false;
}


void print_first_colored_obj(std::vector<Rgb> colors, std::map<Rgb, std::vector<std::vector<int> > > c_tab)
{
  int i, j;

  for (i = 0; i < 90; ++i) //(c_tab.find(colors[0])->second).size(); ++i)
  {
    for (j = 0; j < 150; ++j)//(c_tab.find(colors[0])->second)[i].size(); ++j)
      if ((c_tab.find(colors[0])->second)[j][i] == 1)
	std::cout << "*";
      else
	std::cout << " ";
    std::cout << std::endl;
  }


  std::cout << "i:" << i << std::endl << "j:" << j << std::endl;

}
