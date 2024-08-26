using SixLabors.ImageSharp;
using SixLabors.ImageSharp.PixelFormats;
using SixLabors.ImageSharp.Processing;
using System.Numerics;

public static class Program
{ 
    public static int Main(string[] args)
    {
        int getIntFromCommandLine(string name)
        {
            Console.Write("input your '" + name + "' : ");
            int value = 0;
            if(int.TryParse(Console.ReadLine(), out value) == false)
            {
                Console.Clear();
                Console.WriteLine("It has to be a number!");
                return getIntFromCommandLine(name);
            }
            return value;
        }
        bool getBoolFromCommandLine(string name)
        {
            Console.Write("input your '" + name + "' (true if yes, false if no): ");
            bool value = false;
            if (bool.TryParse(Console.ReadLine(), out value) == false)
            {
                Console.Clear();
                Console.WriteLine("It has to be a bool!");
                return getBoolFromCommandLine(name);
            }
            return value;
        }
        string getStringFromCommandLine(string name)
        {
            Console.Write("Input the " + name + ": ");
            string value = Console.ReadLine();
            if (value == string.Empty)
            {
                Console.Clear();
                return getStringFromCommandLine(name);
            }
            return value;
        }

        DirectoryInfo rootDirectory = new DirectoryInfo(Environment.CurrentDirectory);

        string spriteName = getStringFromCommandLine("Sprite Name");
        int sizeX = getIntFromCommandLine("cell size x");
        int sizeY = getIntFromCommandLine("cell size y");
        int maxSize = getIntFromCommandLine("Max Texture Size");
        int frameRate = getIntFromCommandLine("FrameRate");
        bool saveImagesIndividiually = getBoolFromCommandLine("Save Images Separately");

        List<Animation> animations = new List<Animation>();
        int endWidth = 0;
        int endHeight = 0;

        //Create all animation spriteSheets with a prefix -temp
        foreach (DirectoryInfo directory in rootDirectory.EnumerateDirectories())
        {
            var files = directory.GetFiles("*.png", SearchOption.TopDirectoryOnly);
            if (files.Length == 0)
                continue;
            int maxFrames = (int)MathF.Floor((float)maxSize / sizeX);
            int rows = (int)MathF.Floor(files.Length / (float)maxFrames) + 1;
            endHeight += rows;
            int width = int.Clamp(files.Length, 0, maxFrames);
            endWidth = int.Clamp(endWidth + width, 0, maxFrames);
            var spriteSheet = new Image<Rgba32>(sizeX * width, sizeY * rows);
            var spriteSheetName = rootDirectory.FullName + "\\" + "temp-" + directory.Name + ".png";

            int indexOfLoopFrameSymbol = directory.Name.LastIndexOf("-l");
            int loopFrame = 0;
            bool looped = true;
            if (indexOfLoopFrameSymbol != -1)
            {
                int.TryParse(directory.Name.Substring(indexOfLoopFrameSymbol), out loopFrame);
            }
            else 
            {
                looped = false;
            }

            
            for (int i = 0; i < files.Length; i++)
            {
                FileInfo file = files[i];

                int row = (int)MathF.Floor(i / (float)maxFrames);
                if (file.Extension == ".png")
                {
                    using (var image = Image.Load(file.FullName))
                    {
                        var movedIndex = i - row * maxFrames;
                        spriteSheet.Mutate(ctx => ctx.DrawImage(image, new Point(movedIndex * sizeX, row * sizeY), 1f));
                        spriteSheet.Save(spriteSheetName);
                    }
                }
            }
            animations.Add(new Animation(rows, files.Length, loopFrame, looped, spriteSheetName));
        }

        Image spriteSheetAtlas = new Image<Rgba32>(endWidth * sizeX, endHeight * sizeY);
        //and now, I stitch all of them into one spriteSheet
        int yPosition = 0;
        for (int i = 0; i < animations.Count;i++)
        {
            Animation animation = animations[i];
            using (Image img = Image.Load(animation.Path))
            {
                spriteSheetAtlas.Mutate(ctx => ctx.DrawImage(img, new Point(0, yPosition * sizeY), 1));
                spriteSheetAtlas.Save(rootDirectory.FullName + "\\spriteSheet.png");
                yPosition += animation.Rows;
            }
        }
        return 1;
    }

    struct Animation
    {
        public int Rows;
        public int FrameCount;
        public int LoopFrame = 0;
        public bool Looped = true;
        public string Path;
        public string Name;

        public Animation(int rows, int frameCount, int loopFrame, bool looped, string path)
        {
            Rows = rows;
            FrameCount = frameCount;
            LoopFrame = loopFrame;
            Looped = looped;
            Path = path;
            Name = Path.Substring(Path.LastIndexOf('-')+1);
        }
    }
}