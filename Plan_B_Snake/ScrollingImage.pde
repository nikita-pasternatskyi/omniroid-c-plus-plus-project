public class ScrollingImage implements IDrawable
{
  private PImage _image;

  public float XScrollingSpeed;
  public float YScrollingSpeed;

  private float[] _positionX = new float[4];
  private float[] _positionY = new float[4];

  public ScrollingImage(PImage image)
  {
    _image = image;
    _positionX[0] = 0;
    _positionX[1] = image.width;
    _positionX[2] = 0;
    _positionX[3] = image.width;

    _positionY[0] = 0;
    _positionY[1] = 0;
    _positionY[2] = image.height;
    _positionY[3] = image.height;
  }

  public void Draw()
  {
    for (int i = 0; i < 4; i++)
    {
      _positionX[i] += XScrollingSpeed;
      _positionY[i] += YScrollingSpeed;
      if (abs(_positionX[i]) > _positionX[i] + _image.width * 2)
      {
        _positionX[i] += _image.width * 2;
      }

      if (abs(_positionY[i]) > _positionY[i] + _image.height * 2)
      {
        _positionY[i] += _image.height * 2;
      }

      image(_image, _positionX[i], _positionY[i]);
    }
  }
}
