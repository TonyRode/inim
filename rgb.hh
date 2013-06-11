#ifndef RGB_HH
#define RGB_HH

class Rgb {

public:
  Rgb();
  Rgb(int red, int green, int blue);
  ~Rgb();


  unsigned int get_red();
  unsigned int get_green();
  unsigned int get_blue();


  void set_rgb(unsigned int r, unsigned int g, unsigned int b);
  void set_red(unsigned int r);
  void set_green(unsigned int g);
  void set_blue(unsigned int b);


  Rgb getinstance() {
    return *this;
  }

private:
  unsigned int red_, green_, blue_;
};


#endif /* !RGB_HH */
