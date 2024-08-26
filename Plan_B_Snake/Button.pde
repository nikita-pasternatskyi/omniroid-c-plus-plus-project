import java.util.function.Consumer;

public class Button implements IUIElement, IDrawable
{
  private Rectangle _rectangle;
  private String _text;
  private int _textSize;

  private ArrayList<IButtonPressListener> _buttonListeners = new ArrayList<IButtonPressListener>();
  private ArrayList<Consumer> _onPressActions = new ArrayList<Consumer>();
  
  private boolean _selected;

  //--------------------cursor----------------------//
  private final int _cursorWidth = 10;

  //--------------------colors----------------------//
  private final color IDLE_COLOR = color(175);
  private final color PRESSED_COLOR = color(155);
  private final color HOVER_COLOR = color(135);
  
  private color _currentColor;

  public Button(int textSize, int x, int y, String text)
  {
    Init(textSize, x,y,text);
  }
  
  public Button(int textSize, int x, int y, String text, Consumer actionToRun)
  {
    Init(textSize, x,y,text);
    _onPressActions.add(actionToRun);
  }

  private void Init(int textSize, int x,int y, String text)
  {
    _textSize = textSize;
    var xWidth = textSize * (text.length()/1.75); //this is a magic number that worked
    _rectangle = new Rectangle(x - xWidth/2, y - textSize/2, xWidth, textSize);
    _text = text;
    _currentColor = IDLE_COLOR;
  }

  public void Draw()
  {
    pushStyle();
    rectMode(CENTER);
    textAlign(CENTER,CENTER);
    textSize(_textSize);
    fill(_currentColor);
    text(_text, _rectangle.GetCenteredX(), _rectangle.GetCenteredY());
    if(_selected)
      DrawCursor();
    
    popStyle();
  }
  
  public void RegisterButtonPressListener(IButtonPressListener listener)
  {
    _buttonListeners.add(listener);
  }

  private void DrawCursor()
  {
    //left triangle
    var rectX = _rectangle.X;
    var rectY = _rectangle.Y;
    var p1 = new PVector(rectX - _cursorWidth * 2, rectY);
    var p2 = new PVector(rectX - _cursorWidth, rectY + _rectangle.Height/2);
    var p3 = new PVector(rectX - _cursorWidth * 2, rectY + _rectangle.Height);
    triangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
    
    //right triangle
    rectX = _rectangle.X + _rectangle.Width;
    p1 = new PVector(rectX + _cursorWidth * 2, rectY);
    p2 = new PVector(rectX + _cursorWidth, rectY + _rectangle.Height/2);
    p3 = new PVector(rectX + _cursorWidth * 2, rectY + _rectangle.Height);
    triangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
  }

  public void Released()
  {
    _currentColor = IDLE_COLOR;
  }

  public void Pressed()
  {
    _currentColor = PRESSED_COLOR;
    for (var item : _buttonListeners) item.OnPressed();
    for(var item : _onPressActions) item.accept(null);
  }

  public void Hovered()
  {
    _currentColor = HOVER_COLOR;
    _selected = true;
  }

  public void StoppedHover()
  {
    _currentColor = IDLE_COLOR;
    _selected = false;
  }

  public boolean Contains(float x, float y)
  {
    return _rectangle.Contains((int)x,(int)y);
  }
}

public interface IButtonPressListener
{
  void OnPressed();
}
