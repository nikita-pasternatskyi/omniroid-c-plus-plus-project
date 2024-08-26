public class SnakePart
{
  public PImage Image;
  public PImage CornerImage;
  public int X;
  public int Y;

  public boolean Corner = false;
  public float _angle = 0;
  public PImage _imageToDraw;
  public PVector _direction;
  public PVector _previousDirection;
  public boolean CanCorner = false;

  private PVector _rightVector = new PVector(1, 0);

  public SnakePart(int x, int y, PImage image, int xDirection, int yDirection)
  {
    _imageToDraw = Image = image;
    X = x;
    Y = y;
    _direction = new PVector(xDirection, yDirection);
  }

  public SnakePart(PImage image, int xDirection, int yDirection)
  {
    _imageToDraw = Image = image;
    _direction = new PVector(xDirection, yDirection);
    X = 0;
    Y = 0;
  }

  public SnakePart(int x, int y, PImage image, int xDirection, int yDirection, PImage cornerImage)
  {
    _imageToDraw = Image = image;
    _direction = new PVector(xDirection, yDirection);
    X = x;
    Y = y;
    CanCorner = true;
    CornerImage = cornerImage;
  }

  public SnakePart(SnakePart snakePart, PImage image, PImage cornerImage)
  {
    _imageToDraw = Image = image;
    CornerImage = cornerImage;
    _direction = _previousDirection = snakePart._direction;
    CopySnakePart(snakePart);
    X -= _direction.x * TILE_SIZE;
    Y -= _direction.y * TILE_SIZE;
  }

  public void CopySnakePart(SnakePart snakePart)
  {
    X = snakePart.X;
    Y = snakePart.Y;
    _previousDirection = _direction;
    _direction = snakePart._direction;
    _angle = snakePart._angle;
    Corner = snakePart.Corner;
  }

  public void Draw()
  {
    if (Corner && CornerImage != null)
      _imageToDraw = CornerImage;
    else
      _imageToDraw = Image;
    push();
    translate(X + 8, Y + 8);
    rotate(radians(_angle));
    translate(-X - 8, -Y - 8);
    image(_imageToDraw, X, Y);
    pop();
  }

  //really, these are for head of the snake only
  public boolean ChangeDirection(int x, int y)
  {
    if (x == 0 && y ==0)
      return false;
    _direction = new PVector(x, y);
    _angle = degrees(_direction.heading());
    return true;
  }
  
  public void ChangeAngle(float newAngle)
  {
    if(abs(newAngle - _angle) > 0)
    {
      _angle = 180+newAngle;
      println(_angle);
      Corner = true;
      return;
    }
  }

  public void Update()
  {
    X += _direction.x * TILE_SIZE;
    Y += _direction.y * TILE_SIZE;
  }
}
