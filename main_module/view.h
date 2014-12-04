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

    virtual void postUpdate();
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
  protected:
    String text;
    unsigned char *font;

  public:
    TextView(int x, int y, int width, int height,String text, unsigned char *font);
    void setText(String text);
    String& getText();

    virtual void draw();
};

#define EDITVIEW_BLANK_UNTIL     50
#define EDITVIEW_SHOW_UNTIL      100

class EditView : public TextView {
  private:
    int editpos;
    int blink_counter;

  public:
    EditView(int x, int y, int width, int height,String text, unsigned char *font);
    void blinkPos(int pos);

    virtual void draw();
    virtual void postUpdate();
};
