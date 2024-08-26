public class InputButton
{
  public Boolean JustPressed = false;
  public Boolean JustReleased = false;
  public Boolean Pressed = false;
  public int KeyCode;

  public InputButton(int expectedKey)
  {
    KeyCode = expectedKey;
  }
}
