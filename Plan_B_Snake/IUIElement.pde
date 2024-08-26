public interface IUIElement extends IDrawable
{
  void Pressed();
  void Released();
  void Hovered();
  void StoppedHover();

  boolean Contains(float x, float y);
}
