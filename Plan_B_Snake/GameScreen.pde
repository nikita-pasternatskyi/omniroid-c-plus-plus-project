public class GameScreen extends Screen implements IDrawable
{
  public final int WIDTH = 16 * 15;
  public final int HEIGHT = 16 * 15;
  public final int TILE_SIZE = 16;
  private Player _player;
  private PImage _background;

  public GameScreen()
  {
    _background = loadImage("GardenBackground.png");
  }
  
  @Override public void Enter()
  {
    noiseSeed((int)random(10000000));
    RegisterDrawable(this);
    _player = new Player(this);
    RegisterDrawable(_player);
  }

  public int GetPlayerCell(float playerX)
  {
    return (int)(playerX/TILE_SIZE);
  }
 
  public void Draw()
  {
    background(0);
    image(_background,0,0);
  }
}
