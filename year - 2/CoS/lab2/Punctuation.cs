using System.Text.RegularExpressions;

namespace TextParsing{
public class Punctuation : Token{
   private string _punctuation;
   public Punctuation(){
      _punctuation = "";
   }
   public Punctuation(String str){
      _punctuation = str;
   }
   public override String Value{
      get {return _punctuation;}
      set {
         if(!IsPunctuation(value)){
         throw new ArgumentException("Punctuation class value must be a string containing punctuation mark");
         }
         _punctuation = value;;
      }
   }
   public override Token Clone(){
      Punctuation newPunctuation = new Punctuation(new String(this.Value));
      return newPunctuation;
   }
   public static bool IsPunctuation(String str){ 
      return Regex.Match(str, "^[,.!?:;-\'\"]").Value.Equals(str);
   }
}
} // namespace TextParsing
