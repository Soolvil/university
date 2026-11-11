using System.Text.RegularExpressions;

namespace TextParsing{
public class Word : Token{
   string _word;
   public Word(){
      _word = "";
   }
   public Word(String str){
      Value = str;
   }
   public override String Value{
      get {return _word;}
      set {
         if(!IsWord(value)){
            throw new ArgumentException("Word class value must be a string containing word");
         }
         _word = value;
      }
   }
   public bool StartsWithVowel{
      get{
         if(Value.Length == 0)return false;
         return Regex.IsMatch(char.ToLower(_word[0]).ToString(), @"[aeiouаяэеоёуюы]");
      }
   }
   public bool StartsWithConsonant{
      get{
         if(Value.Length == 0)return false;
         return !StartsWithVowel;
      }
   }
   public override Token Clone(){
      Word newWord = new Word(new String(this.Value));
      return newWord;
   }
   public static bool IsWord(String str){
      return Regex.Match(str, "^[a-zA-Z\\p{IsCyrillic}]+").Value.Equals(str);
   }
}
} // namespace TextParsing
