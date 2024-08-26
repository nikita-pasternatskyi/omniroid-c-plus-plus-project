public class Rectangle
{
  public float X;
  public float Y;
  public float Width;
  public float Height;
  
  public Rectangle(float x, float y, float rectWidth, float rectHeight)
  {
    X = x;
    Y = y;
    Width = rectWidth;
    Height = rectHeight;
  }
  
  public float GetCenteredX()
  {
    return X + Width/2;
  }
  
  public float GetCenteredY()
  {
    return Y + Height/2;
  }
  
  public boolean Contains(int x, int y)
  {
    return (x > X && x < X + Width) && (y > Y && y < Y + Height);
  }
}
