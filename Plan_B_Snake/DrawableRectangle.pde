public class DrawableRectangle extends Rectangle implements IDrawable
{
  private final color BORDER_COLOR = color(155);
  private final color FILL_COLOR = color(0);
  private final int STROKE_WEIGHT = 2;

  public DrawableRectangle(float x, float y, float rectWidth, float rectHeight)
  {
    super(x - rectWidth/2, y - rectHeight/2, rectWidth, rectHeight);
  }

  public void Draw()
  {
    pushStyle();
    strokeWeight(STROKE_WEIGHT);
    fill(FILL_COLOR);
    stroke(BORDER_COLOR);
    rect(X, Y, Width, Height);
    OnDraw();
    popStyle();
  }

  protected void OnDraw() {
  }
}

public class AnnotationRectangle extends DrawableRectangle implements IUIElement
{
  private final color TEXT_COLOR = color(155);
  private final int _textSize = 12;
  private String _text;

  public AnnotationRectangle(float x, float y, float rectWidth, float rectHeight, String text)
  {
    super(x, y, rectWidth, rectHeight);
    _text = text;
  }

  public void Pressed() {
  }
  public void Released() {
  }
  public void Hovered() {
  }
  public void StoppedHover() {
  }

  public boolean Contains(float x, float y)
  {
    return super.Contains((int)x, (int)y);
  }

  @Override protected void OnDraw()
  {
    fill(TEXT_COLOR);
    textSize(_textSize);
    textAlign(CENTER, CENTER);
    text(_text, X + Width/2, Y + Height/2);
  }
}
