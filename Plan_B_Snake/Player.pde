import java.util.LinkedList;

public class Player implements IDrawable
{
  //---------------properties-------------------//
  public final float UpdateRate = 4; //how many times per second should I update player

  //------------stats text----------------------//
  private final String SCORE_TEXT = "SCORE: ";

  //------------game over messages--------------//
  private final String GAMEOVER_MESSAGE = "Oh no!\n You bit yourself!\n Your score is: ";

  //------------winning conditions--------------//

  private boolean _gameStopped = false;
  private LinkedList<SnakePart> _snakeParts = new LinkedList<SnakePart>();

  //-----------------sprites----------------------//
  private PImage _headSprite = loadImage("head.png");
  private PImage _tailSprite = loadImage("tail.png");
  private PImage _bodySprite = loadImage("body.png");
  private PImage _cornerSprite = loadImage("corner.png");

  private int _lastAttachableSnakePartIndex = 0;
  private float _updateTimer;
  private GameScreen _level;

  public Player(GameScreen level)
  {
    _level = level;
    var head = new SnakePart(_headSprite, 1, 0);
    head.X = CENTER_SCREEN_X;
    head.Y = CENTER_SCREEN_Y;
    _snakeParts.add(head);
    AddSnakePart();

    var tail = new SnakePart(_snakeParts.get(1), _tailSprite, null);
    _snakeParts.add(tail);
  }

  private void AddSnakePart()
  {
    var part = _snakeParts.get(_lastAttachableSnakePartIndex);
    var snakePart = new SnakePart(part, _bodySprite, _cornerSprite);
    _snakeParts.add(_lastAttachableSnakePartIndex+1, snakePart);
    _lastAttachableSnakePartIndex++;
  }

  public void Draw()
  {
    Update();
    push();
    DrawPlayer();
    pop();
  }

  private void DrawPlayer()
  {
    for (int i = 0; i < _snakeParts.size(); i++)
    {
      _snakeParts.get(i).Draw();
    }
  }

  private void Update()
  {
    if (_gameStopped)
      return;

    int horizontal = CreateAxisFromButtons(GO_RIGHT, GO_LEFT);
    int vertical = CreateAxisFromButtons(GO_DOWN, GO_UP);
    if (horizontal == vertical)
      vertical = 0;

    _updateTimer += DeltaTime * UpdateRate;
    if (_updateTimer >= 1)
    {
      for (int i = _snakeParts.size()-1; i > 0; i--)
      {
        _snakeParts.get(i).Corner = false;
        _snakeParts.get(i).CopySnakePart(_snakeParts.get(i-1));
      }
      
      var head = _snakeParts.get(0);
      head.ChangeDirection(horizontal, vertical);
      head.Update();
      
      _snakeParts.get(1).ChangeAngle(head._angle);
      _updateTimer = 0;
    }
  }

  private void CheckLoseWinConditions()
  {
  }

  private void RegisterGameOverMessage(String message)
  {
    RegisterUIElement(new AnnotationRectangle(CENTER_SCREEN_X, CENTER_SCREEN_Y, CENTER_SCREEN_X, CENTER_SCREEN_Y,
      message
      ));
  }

  private void RegisterGameOverButtons()
  {
    RegisterUIElement(new Button(8, RENDER_SIZE_X/3 + 12, CENTER_SCREEN_Y + 28, "Return to Main Menu", Void -> {
      ChangeScreen(MainMenu);
    }
    ));
    RegisterUIElement(new Button(8, RENDER_SIZE_X - RENDER_SIZE_X/3 - 12, CENTER_SCREEN_Y + 28, "Restart", Void -> {
      ChangeScreen(new GameScreen());
    }
    ));
    _gameStopped = true;
  }

  private int WrapInt(int value, int min, int max)
  {
    if (value < min)
      value = max;
    if (value > max)
      value = min;
    return value;
  }

  private int GetButtonValue(String name)
  {
    if (InputButtons.get(name).Pressed == true)
      return 1;
    else return 0;
  }

  private int CreateAxisFromButtons(String positiveName, String negativeName)
  {
    return GetButtonValue(positiveName) - GetButtonValue(negativeName);
  }

  public CollisionCheckResult CheckCollision()
  {
    var result = new CollisionCheckResult();
    return result;
  }

  public class CollisionCheckResult
  {
    boolean CollidedWithSelf = false;
    boolean Collision = false;
  }
}
