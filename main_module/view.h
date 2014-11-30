class View {
  protected:
    int x;
    int y;
    int width;
    int height;
    
    bool is_visible;
    bool invalidate;
    
    static UTFT *lcd;

    #define SLCD (View::lcd)

  public:
    static void setLCD(UTFT *lcd) {
      SLCD = lcd;
    };

    View(int x, int y, int width, int height); 

    void show() { visible(true); }
    void hide() { visible(false); }

    void visible(bool visible);
    
    void update();
    
    // touch control
    bool isTouched(int x, int y);
    
    virtual void clear();
    virtual void draw();
};

class ImageView : public View {
  private:
    unsigned short *img;
    unsigned short *bg;
    
  public:
    ImageView(int x, int y, int width, int height, unsigned short *img, unsigned short *bg=0);

    virtual void clear();
    virtual void draw();
};

class TextView : public View {
  private:
    String text;
    unsigned char *font;
    
  public:
    TextView(int x, int y, int width, int height,String text, unsigned char *font); 
    void setText(String text);
    
    virtual void draw();
};
