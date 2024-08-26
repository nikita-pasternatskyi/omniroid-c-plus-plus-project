//------------globals-------------------------//

public Screen CurrentScreen;
public MainMenuScreen MainMenu;
public PFont Font;
public final int TILE_SIZE = 16;
public final int RENDER_SIZE_X = 256;
public final int RENDER_SIZE_Y = 256;
public final int SCREEN_SCALE = 3;
public final int CENTER_SCREEN_X = RENDER_SIZE_X / 2;
public final int CENTER_SCREEN_Y = RENDER_SIZE_Y / 2;

//------------delta time----------------------//

public float DeltaTime;
public float TimeScale = 1;
private float _lastTimeStep;

//------------keyboard input------------------//

public HashMap<String, InputButton> InputButtons = new HashMap<String, InputButton>();

public final String GO_RIGHT = "Go Right";
public final String GO_LEFT = "Go Left";
public final String GO_UP = "Go Up";
public final String GO_DOWN = "Go Down";
public final String BACK = "Back";

public final int GO_RIGHT_KEY_CODE = 39; //right arrow
public final int GO_LEFT_KEY_CODE = 37; //left arrow
public final int GO_UP_KEY_CODE = 38; //right arrow
public final int GO_DOWN_KEY_CODE = 40; //left arrow
public final int BACK_KEY_CODE = 8; //Backspace

//----------processing methods----------------//

public void setup()
{
  Font = createFont("font.ttf", 12);
  textFont(Font);
  SetupInput();
  MainMenu = new MainMenuScreen();
  ChangeScreen(MainMenu);
}

public void settings()
{
  size(RENDER_SIZE_X * SCREEN_SCALE, RENDER_SIZE_Y * SCREEN_SCALE);
  noSmooth();
}

public void draw()
{
  scale(SCREEN_SCALE);
  UpdateTime();
  CurrentScreen.DrawObjects();
  FinishInputUpdate();
}

//-------------mouse methods------------------//

public void mousePressed()
{
  CurrentScreen.MousePressed();
}

public void mouseReleased()
{
  CurrentScreen.MouseReleased();
}

public void mouseMoved()
{
  CurrentScreen.MouseMoved();
}

//------------keyboard methods-----------------//

public void keyPressed()
{
  for (var button : InputButtons.values())
  {
    if (keyCode == button.KeyCode)
    {
      if (button.Pressed != true)
      {
        button.Pressed = true;
        button.JustPressed = true;
      }
    }
  }
}

public void keyReleased()
{
  for (var button : InputButtons.values())
  {
    if (keyCode == button.KeyCode)
    {
      if (button.Pressed == true)
      {
        button.JustReleased = true;
        button.Pressed = false;
      }
    }
  }
}

//-------------custom methods------------------//

public void ChangeScreen(Screen newScreen)
{
  if(CurrentScreen != null)
    CurrentScreen.Exit();
  CurrentScreen = newScreen;
  CurrentScreen.Enter();
}

private void UpdateTime()
{
  DeltaTime = (millis() - _lastTimeStep) / 1000.0 * TimeScale;
  _lastTimeStep = millis();
}

private void SetupInput()
{
  InputButtons.put(GO_RIGHT, new InputButton(GO_RIGHT_KEY_CODE));
  InputButtons.put(GO_LEFT, new InputButton(GO_LEFT_KEY_CODE));
  InputButtons.put(GO_UP, new InputButton(GO_UP_KEY_CODE));
  InputButtons.put(GO_DOWN, new InputButton(GO_DOWN_KEY_CODE));
  InputButtons.put(BACK, new InputButton(BACK_KEY_CODE));
}

private void FinishInputUpdate()
{
  for (var button : InputButtons.values())
  {
    if (button.Pressed == true)
    {
      button.JustPressed = false;
    } else if (button.Pressed == false)
    {
      button.JustReleased = false;
    }
  }
}

//--------------registration methods---------//

public void UnRegisterUIElement(IUIElement element)
{
  CurrentScreen.UnRegisterUIElement(element);
}

public void UnRegisterDrawable(IDrawable drawable)
{
  CurrentScreen.UnRegisterDrawable(drawable);
}

public void RegisterDrawable(IDrawable drawable)
{
  CurrentScreen.RegisterDrawable(drawable);
}

public void RegisterUIElement(IUIElement uiElement)
{
  CurrentScreen.RegisterUIElement(uiElement);
}
