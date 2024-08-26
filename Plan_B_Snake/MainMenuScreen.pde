public class MainMenuScreen extends Screen implements IDrawable
{
  private AnnotationRectangle _howToAnnotation;

  private ScrollingImage _scrollingImage2;
  private ScrollingImage _scrollingImage3;

  public MainMenuScreen()
  {
    _scrollingImage2 = new ScrollingImage(loadImage("BackgroundStars2.png"));
    _scrollingImage3 = new ScrollingImage(loadImage("BackgroundStars3.png"));
    _scrollingImage3.YScrollingSpeed = -0.1;
    _scrollingImage3.XScrollingSpeed = -0.1;
    _scrollingImage2.YScrollingSpeed = -0.25;
    _scrollingImage2.XScrollingSpeed = -0.25;
    
    RegisterDrawable(this);
    _howToAnnotation = new AnnotationRectangle(CENTER_SCREEN_X, CENTER_SCREEN_Y, 256, CENTER_SCREEN_Y,
      "Move the snake with arrow keys\n to get to it's food\n"
      + "and as a result become longer\n and gain score for your length\n"
      + "But be careful!\n Do not bite yourself, that hurts\n and resets the game"
      );
  }

  public void Enter()
  {
    RegisterUIElement(new Button(14, CENTER_SCREEN_X, CENTER_SCREEN_Y, "PLAY", (Void) -> {
      StartGame();
    }
    ));

    RegisterUIElement(new Button(14, CENTER_SCREEN_X, CENTER_SCREEN_Y + 30, "HOW TO PLAY", (Void) -> {
      OpenHowTo();
    }
    ));

    RegisterUIElement(new Button(14, CENTER_SCREEN_X, CENTER_SCREEN_Y + 60, "EXIT", (Void) -> {
      exit();
    }
    ));
  }

  public void Draw()
  {
    pushStyle();
    background(0);
    
    _scrollingImage3.Draw();
    _scrollingImage2.Draw();
    
    fill(255);
    textAlign(CENTER, CENTER);
    textSize(48);
    text("SNAKE", CENTER_SCREEN_X, CENTER_SCREEN_Y/2);
    
    textSize(12);
    text("BACKSPACE to go back", 72, RENDER_SIZE_Y - 12);

    if (InputButtons.get(BACK).JustPressed)
    {
      UnRegisterUIElement(_howToAnnotation);
    }
    popStyle();
  }

  private void StartGame()
  {
    ChangeScreen(new GameScreen());
  }

  private void OpenHowTo()
  {
    RegisterUIElement(_howToAnnotation);
  }
}
