#ifndef TAB_BOUNDS_HH
#define TAB_BOUNDS_HH



class Tab_bounds {
public:
  int get_x();
  int get_y();
  void set_x(int x);
  void set_y(int y);

private:
  int x_, y_;
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
