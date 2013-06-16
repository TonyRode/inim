#ifndef TAB_BOUNDS_HH
#define TAB_BOUNDS_HH



class Tab_bounds {
public:
  Tab_bounds();
  ~Tab_bounds();

  int get_x();
  int get_y();
  bool first_exchange();
  void set_x(int x);
  void set_y(int y);
  void turn_off_ex();

private:
  int x_, y_;
  bool first_exchange_;
};


/*typedef struct tab_bounds {
  int x;
  int y;
} s_tab_bounds;
*/

/*
  -------> x
  -
  -
  -
  -
  -
  y


 */


#endif /* !TAB_BOUNDS_HH */
