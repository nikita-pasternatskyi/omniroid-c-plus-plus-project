import java.util.LinkedList; //<>//

public class Screen
{
  //------------mouse event listeners-----------//
  private LinkedList<IUIElement> _uiElements = new LinkedList<IUIElement>();

  //------------drawables-----------------------//

  private LinkedList<IDrawable> _drawables = new LinkedList<IDrawable>();

  private IUIElement _selectedElement;

  //------------methods-------------------------//

  public void Exit() {
  }

  public void Enter() {
  }

  public void DrawObjects()
  {
    for (int i = 0; i < _drawables.size(); i++)
    {
      _drawables.get(i).Draw();
    }
  }

  //--------------mouse callbacks---------//

  public void MousePressed()
  {
    for (int i = 0; i < _uiElements.size(); i++)
    {
      var item = _uiElements.get(i);
      if (item.Contains(mouseX/SCREEN_SCALE, mouseY/SCREEN_SCALE))
      {
        if (mouseButton == LEFT)
        {
          item.Pressed();
        }
        break;
      }
    }
  }

  public void MouseReleased()
  {
    for (int i = 0; i < _uiElements.size(); i++)
    {
      var item = _uiElements.get(i);
      if (item.Contains(mouseX / SCREEN_SCALE, mouseY / SCREEN_SCALE))
      {
        if (mouseButton == LEFT)
        {
          item.Released();
        }
        break;
      }
    }
  }

  public void MouseMoved()
  {
    boolean selected = false;
    for (int i = 0; i < _uiElements.size(); i++)
    {
      var item = _uiElements.get(i);
      
      if (item.Contains(mouseX / SCREEN_SCALE, mouseY / SCREEN_SCALE))
      {
        if (_selectedElement != null)
        {
          _selectedElement.StoppedHover();
        }
        _selectedElement = item;
        _selectedElement.Hovered();
        selected = true;
        break;
      }
    }
    if(!selected && _selectedElement != null)
      _selectedElement.StoppedHover();
  }

  //--------------registration methods---------//

  public void UnRegisterUIElement(IUIElement element)
  {
    _uiElements.remove(element);
    _drawables.remove(element);
  }

  public void UnRegisterDrawable(IDrawable drawable)
  {
    _drawables.remove(drawable);
  }

  public void RegisterDrawable(IDrawable drawable)
  {
    _drawables.add(drawable);
  }

  public void RegisterUIElement(IUIElement element)
  {
    _uiElements.add(0, element);
    _drawables.add(element);
  }
}
