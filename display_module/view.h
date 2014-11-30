class View {
  private:
    int x;
    int y;
    int width;
    int height;

    unsigned short *img;

    static UTFT *lcd;

    #define SLCD (View::lcd)

  public:
    static void setLCD(UTFT *lcd) {
      SLCD = lcd;
    };

    View(int x, int y, int width, int height, unsigned short *img);

    void draw();
    void clear();
};
