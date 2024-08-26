public class Apple implements IDrawable
{
  private PImage _sprite;
  public int X;
  public int Y;
  
  public Apple(int x, int y)
  {
    _sprite = loadImage("apple.png");
    X = x;
    Y = y;
  }
  
  public void Draw()
  {
    image(_sprite,X,Y);
  }
}
