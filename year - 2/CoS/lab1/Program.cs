using System.Drawing;
using System.Drawing.Imaging;
using System.Text.RegularExpressions;

class Program{
   static Dictionary<string, Color> ColorMap = new(StringComparer.OrdinalIgnoreCase)
   {
      { "^(красн)", Color.Red },
      { "^(ал)", Color.Crimson }, 
      { "^(багр)", Color.DarkRed }, 
      { "^(зелен)", Color.Green },
      { "^(изумрудн)", Color.MediumSeaGreen },
      { "^(малахитов)", Color.MediumSeaGreen },
      { "^(син)", Color.Blue },
      { "^(голуб)", Color.LightBlue },
      { "^(лазурн)", Color.LightSkyBlue }, 
      { "^(ультрамаринов)", Color.Blue },
      { "^(желт)", Color.Yellow },
      { "^(золот)", Color.Gold },
      { "^(лимон)", Color.LemonChiffon },
      { "^(бел)", Color.White },
      { "^(черн)", Color.Black },
      { "^(сер)", Color.Gray },
      { "^(фиолетов)", Color.Purple },
      { "^(лилов)", Color.Purple },
      { "^(оранжев)", Color.Orange },
      { "^(коричнев)", Color.Brown },
      { "^(розов)", Color.Pink },
      { "^(бирюз)", Color.Turquoise },
   };
   
   static string readText(string fileName){
      StreamReader fstream = new StreamReader(fileName);
      return fstream.ReadToEnd();
   }

   static List<string> getWords(string text){
      const string pattern = "\\b[a-zA-Z\\p{IsCyrillic}]+\\b";
      List<string> result = new List<string>();
      foreach(Match word in Regex.Matches(text, pattern)){
	 result.Add(word.Value.ToLower());
      }
      return result;
   }

   static List<Color> getColors(List <string> words){
      List<Color> result = new List<Color>();
      foreach(string word in words){
	 foreach(var (pattern, color) in ColorMap){
	    if(Regex.IsMatch(word.ToLower(), pattern)){
	       Console.WriteLine(word + " - " + pattern);
	       result.Add(color);
	       continue;
	    }
	 }
      }
      return result;
   }

   static void createImage(string imageName, List<Color> pixels){
      int size = 0;
      while(size * size < pixels.Count){
	 size++;
      }
      Bitmap pixelMap = new Bitmap(size, size);
      int x = 0;
      int y = 0;
      foreach(Color color in pixels){
	 pixelMap.SetPixel(x, y, color);
	 x++;
	 if(x == size){
	    x = 0;
	    y++;
	 }
      }
      pixelMap.Save(imageName, ImageFormat.Png);
   }

   static void Main(){
      string? file = "Podarok";
      Console.WriteLine(Directory.GetCurrentDirectory() + "/" + file + ".txt");
      string imageName = file + ".png";
      string fileName = file + ".txt";
      string text = readText(fileName);
      createImage(imageName, getColors(getWords(text)));
   }
}
